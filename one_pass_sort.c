#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftsort.h"
#include <string.h>

#define LOGFILE "one_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define TABLE_SIZE 10000//CHUNK_SIZE*5
#define BUFFER_SIZE 65535

/* ------------------------------------------------------------------------------------------------------------------------------
  // TODO
  // save to chunks & new table "sortedTbl"
  // 2-PASS
  // (probably 2-PASS first ...)
  // gpu runs...
  */


/** A function like sizeof, but for the duckdb types used.
 * Params:
 * type : the logical type of the data, expressed in enum duckdb_type
 */
size_t colType_bytes(duckdb_type type);
/** A function to allocate memory for any type of column data.
 * Params:
 * type : the logical type of the data, expressed in enum duckdb_type
 * card : the cardinality of elements to store 
 */
void* colType_malloc(duckdb_type type, idx_t card);
/**
 * A function to sort the key column of a table, also provides the reordered indices.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the sorted column, which must have enough space allocated
 * type : the logical type of the data, expressed in enum duckdb_type
 * incr : the index of the first row in the current data chunk
 * outIdx : a double pointer for the output of the reordered indices (as a futhark array)
 * keys : the key column to be sorted
 * card : the cardinality of elements to process
 */
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d **outIdx, void* keys, idx_t card);
/**
 * A function to reorder a payload column, according to the reordered indices from sorting the key column.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the reordered column, which must have enough space allocated
 * type : the logical type of the data, expressed in enum duckdb_type
 * orderBy : the reordered indices (as a futhark array)
 * inCol : the payload column to be reordered
 * card : the cardinality of elements to process
 */
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d *orderBy, void* inCol, idx_t card);

// TODO use this or not?
typedef struct _colInfo{
  void* col;
  idx_t rows;
  size_t sizeOf;
  duckdb_type type;
} colInfo;

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "One-Pass Sort : Starting test program.");
  if(!logfile) {
    perror("Failed to initialise the logger.");
    return 1;
  }

  // DuckDB initialisation & connection
  duckdb_database db;
	duckdb_connection con;
	duckdb_open(NULL, &db);
	duckdb_connect(db, &con);

  // Create the table tbl on which the testing will be done.
  duckdb_result res;
	duckdb_query(con, "CREATE TABLE tbl (k INTEGER, payload1 INTEGER, payload2 FLOAT);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (duckdb_prepare(con,
    "INSERT INTO tbl (SELECT 10000*random(), 10000*random(), 10000*random() FROM range($1));",
    &init_stmt) == DuckDBError)
  {
    perror("Failed to initialise the table.");
    return 1;
  }
  duckdb_bind_int32(init_stmt, 1, TABLE_SIZE);
  duckdb_execute_prepared(init_stmt, NULL);
  duckdb_destroy_prepare(&init_stmt);
  duckdb_query(con, "SELECT * FROM tbl;", &res);

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");
  idx_t col_count = duckdb_column_count(&res);
  // TODO current strat is to sort key column and reorder payload within the same pass, holding them all together
  // ultimate strat would be:
  // have a fixed buffer size (eg 64 bits x 65535 items -> 4 MB)
  // on each scan, process as many columns as can fit
  // the first scan sorts the key and produces the reordered indices that following scans use
  void *Buffers[col_count];
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising buffers for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    mylog(logfile, "Obtained column's type.");
    Buffers[col] = colType_malloc(type_ids[col], BUFFER_SIZE);
    if(Buffers[col] == NULL) {
      mylog(logfile, "ERROR -- failed to allocate memory.");
      logclose(logfile);
      return 1;
    }
    mylog(logfile, "Allocated memory for this column's buffer.");
  }

  // iterate until result is exhausted
	while (true) {
    mylog(logfile, "Fetching next data chunk...");
		duckdb_data_chunk result = duckdb_fetch_chunk(res);
		if (!result) {
			mylog(logfile, "Table has been fully processed.");
			break;
		}
    mylog(logfile, "Fetched data chunk.");
		// get the number of rows & columns from the data chunk
		idx_t row_count = duckdb_data_chunk_get_size(result);

    char msgbuffer[50];
    int msglen = sprintf(msgbuffer, "Read ");
    msglen += sprintf(msgbuffer + msglen, "%ld", row_count);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // obtain the column vectors
    mylog(logfile, "Obtaining column vectors...");
		duckdb_vector res_col[col_count];
		void *vector_data[col_count];
		for (idx_t col = 0; col < col_count; col++) {
      mylog(logfile, "Next column...");
			res_col[col] = duckdb_data_chunk_get_vector(result, col);
      mylog(logfile, "Column got.");
			vector_data[col] = duckdb_vector_get_data(res_col[col]);
      mylog(logfile, "Column data stored.");

      memcpy(Buffers[col] + incr_idx*colType_bytes(type_ids[col]),
        vector_data[col],
        row_count * colType_bytes(type_ids[col]));
      mylog(logfile, "Buffered column.");
		}
    
    incr_idx += row_count;
    mylog(logfile, "Updated index increment.");

    duckdb_destroy_data_chunk(&result);
    mylog(logfile, "Destroyed datachunk.");
  }

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  char msgbuffer[50];
  int msglen = sprintf(msgbuffer, "Finished reading db table -- total of ");
  msglen += sprintf(msgbuffer + msglen, "%ld", incr_idx);
  msglen += sprintf(msgbuffer + msglen, " rows.");
  mylog(logfile, msgbuffer);

  // sort key column
  struct futhark_i64_1d *sorted_idx_ft;
  int sorted_x[incr_idx];
  mylog(logfile, "Passing key column for sorting...");
  sortKeyColumn(ctx, sorted_x, type_ids[0], (idx_t)0, &sorted_idx_ft, Buffers[0], incr_idx);
  mylog(logfile, "Sorted key column and obtained reordered indices.");
  //logarray_int(logfile, "Sorted x: ", sorted_x, incr_idx);
  // test that sorting & reordering was done correctly
  long sorted_idx[incr_idx];
  futhark_values_i64_1d(ctx, sorted_idx_ft, sorted_idx);
  futhark_context_sync(ctx);
  int isSorted = true;
  int indexIsCorrect = true;
  for(idx_t i=0; i<incr_idx-1; i++) {
    isSorted =  isSorted && (sorted_x[i] <= sorted_x[i+1]);
    indexIsCorrect = indexIsCorrect && (sorted_x[i] == ((int*)Buffers[0])[sorted_idx[i]]);
  }
  indexIsCorrect = indexIsCorrect && (sorted_x[incr_idx-1] == ((int*)Buffers[0])[sorted_idx[incr_idx-1]]);
  logdbg(logfile,
    isSorted,
    "x was sorted correctly.",
    "!!!!!!!!!!!!!!! Error: x was not sorted correctly. !!!!!!!!!!!!!!!");
  logdbg(logfile,
    indexIsCorrect,
    "Indices were ordered correctly.",
    "!!!!!!!!!!!!!!! Error: indices were not ordered correctly. !!!!!!!!!!!!!!!");
  
  // Next do the payload columns
  mylog(logfile, "Reordering payload columns...");
  void *sorted_ys[col_count-1];
  for(idx_t col=1; col<col_count; col++) {
    sorted_ys[col-1] = colType_malloc(type_ids[col], incr_idx);
    orderPayloadColumn(ctx, sorted_ys[col-1], type_ids[col], (idx_t)0, sorted_idx_ft, Buffers[col], incr_idx);
    mylog(logfile, "Reordered the next payload column.");
    // Test whether the reordering was done correctly...
    int yIsCorrect = true;
    for (idx_t i=0; i<incr_idx; i++) {
      switch(type_ids[col]){
        case DUCKDB_TYPE_SMALLINT:
          yIsCorrect = yIsCorrect && (((short*)sorted_ys[col-1])[i] == ((short*)Buffers[col])[sorted_idx[i]]);
          break;
        case DUCKDB_TYPE_INTEGER:
          yIsCorrect = yIsCorrect && (((int*)sorted_ys[col-1])[i] == ((int*)Buffers[col])[sorted_idx[i]]);
          break;
        case DUCKDB_TYPE_BIGINT:
          yIsCorrect = yIsCorrect && (((long*)sorted_ys[col-1])[i] == ((long*)Buffers[col])[sorted_idx[i]]);
          break;
        case DUCKDB_TYPE_FLOAT:
          yIsCorrect = yIsCorrect && (((float*)sorted_ys[col-1])[i] == ((float*)Buffers[col])[sorted_idx[i]]);
          break;
        case DUCKDB_TYPE_DOUBLE:
          yIsCorrect = yIsCorrect && (((double*)sorted_ys[col-1])[i] == ((double*)Buffers[col])[sorted_idx[i]]);
          break;
        default:
          perror("Invalid type.");
          break;
      }
    }
    logdbg(logfile,
      yIsCorrect,
      "Payload column was reordered correctly.",
      "!!!!!!!!!!!!!!! Error: payload column was not reordered correctly. !!!!!!!!!!!!!!!");
  }
  
  // clean-up
  for(idx_t col=0; col<col_count; col++) {
    free(Buffers[col]);
    if(col>0) free(sorted_ys[col-1]);
  }
  mylog(logfile, "Freed buffers.");
  futhark_free_i64_1d(ctx, sorted_idx_ft);
  mylog(logfile, "Freed futhark objects.");
  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");
	duckdb_destroy_result(&res);
	duckdb_disconnect(&con);
	duckdb_close(&db);

  logclose(logfile);
  
  return 0;
}


size_t colType_bytes(duckdb_type type) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      return sizeof(short);
    case DUCKDB_TYPE_INTEGER:
      return sizeof(int);
    case DUCKDB_TYPE_BIGINT:
      return sizeof(long);
    case DUCKDB_TYPE_FLOAT:
      return sizeof(float);
    case DUCKDB_TYPE_DOUBLE:
      return sizeof(double);
    default:
      perror("Invalid type.");
      return 0;
  }
}

void* colType_malloc(duckdb_type type, idx_t card) {
  size_t ms = colType_bytes(type);
  if (ms == 0) return NULL;
  return malloc(card * type);
}

void sortKeyColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, struct futhark_i64_1d **outIdx, short* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i16_1d *x_ft = futhark_new_i16_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_short *sortInfo;
  futhark_entry_sortColumn_short(ctx, &sortInfo, (long)incr, x_ft);
  // Decouple indices from keys
  struct futhark_i16_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_short_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_short_xs(ctx, &sorted_x_ft, sortInfo);
  // Unwrap sorted keys into outCol
  futhark_values_i16_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i16_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_short(ctx, sortInfo);
  futhark_free_i16_1d(ctx, x_ft);
}
void sortKeyColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, struct futhark_i64_1d **outIdx, int* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i32_1d *x_ft = futhark_new_i32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_int *sortInfo;
  futhark_entry_sortColumn_int(ctx, &sortInfo, (long)incr, x_ft);
  // Decouple indices from keys
  struct futhark_i32_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_int_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_int_xs(ctx, &sorted_x_ft, sortInfo);
  // Unwrap sorted keys into outCol
  futhark_values_i32_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i32_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_int(ctx, sortInfo);
  futhark_free_i32_1d(ctx, x_ft);
}
void sortKeyColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, struct futhark_i64_1d **outIdx, long* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i64_1d *x_ft = futhark_new_i64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_long *sortInfo;
  futhark_entry_sortColumn_long(ctx, &sortInfo, (long)incr, x_ft);
  // Decouple indices from keys
  struct futhark_i64_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_long_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_long_xs(ctx, &sorted_x_ft, sortInfo);
  // Unwrap sorted keys into outCol
  futhark_values_i64_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i64_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_long(ctx, sortInfo);
  futhark_free_i64_1d(ctx, x_ft);
}
void sortKeyColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, struct futhark_i64_1d **outIdx, float* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f32_1d *x_ft = futhark_new_f32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_float *sortInfo;
  futhark_entry_sortColumn_float(ctx, &sortInfo, (long)incr, x_ft);
  // Decouple indices from keys
  struct futhark_f32_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_float_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_float_xs(ctx, &sorted_x_ft, sortInfo);
  // Unwrap sorted keys into outCol
  futhark_values_f32_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_f32_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_float(ctx, sortInfo);
  futhark_free_f32_1d(ctx, x_ft);
}
void sortKeyColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, struct futhark_i64_1d **outIdx, double* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f64_1d *x_ft = futhark_new_f64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_double *sortInfo;
  futhark_entry_sortColumn_double(ctx, &sortInfo, (long)incr, x_ft);
  // Decouple indices from keys
  struct futhark_f64_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_double_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_double_xs(ctx, &sorted_x_ft, sortInfo);
  // Unwrap sorted keys into outCol
  futhark_values_f64_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_f64_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_double(ctx, sortInfo);
  futhark_free_f64_1d(ctx, x_ft);
}
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d **outIdx, void* keys, idx_t card) {
  switch (type){
  case DUCKDB_TYPE_SMALLINT:
      sortKeyColumn_short(ctx, (short*)outCol, incr, outIdx, (short*)keys, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortKeyColumn_int(ctx, (int*)outCol, incr, outIdx, (int*)keys, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortKeyColumn_long(ctx, (long*)outCol, incr, outIdx, (long*)keys, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortKeyColumn_float(ctx, (float*)outCol, incr, outIdx, (float*)keys, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortKeyColumn_double(ctx, (double*)outCol, incr, outIdx, (double*)keys, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}

void orderPayloadColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, struct futhark_i64_1d *orderBy, short* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i16_1d *y_ft = futhark_new_i16_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i16_1d *sorted_y_ft;
  futhark_entry_orderByIndices_short(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
  // Unwrap reordered payload into outCol
  futhark_values_i16_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i16_1d(ctx, sorted_y_ft);
  futhark_free_i16_1d(ctx, y_ft);
}
void orderPayloadColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, struct futhark_i64_1d *orderBy, int* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i32_1d *y_ft = futhark_new_i32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_int(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
  // Unwrap reordered payload into outCol
  futhark_values_i32_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i32_1d(ctx, sorted_y_ft);
  futhark_free_i32_1d(ctx, y_ft);
}
void orderPayloadColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, struct futhark_i64_1d *orderBy, long* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i64_1d *y_ft = futhark_new_i64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_long(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
  // Unwrap reordered payload into outCol
  futhark_values_i64_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i64_1d(ctx, sorted_y_ft);
  futhark_free_i64_1d(ctx, y_ft);
}
void orderPayloadColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, struct futhark_i64_1d *orderBy, float* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f32_1d *y_ft = futhark_new_f32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_float(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
  // Unwrap reordered payload into outCol
  futhark_values_f32_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f32_1d(ctx, sorted_y_ft);
  futhark_free_f32_1d(ctx, y_ft);
}
void orderPayloadColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, struct futhark_i64_1d *orderBy, double* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f64_1d *y_ft = futhark_new_f64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_double(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
  // Unwrap reordered payload into outCol
  futhark_values_f64_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f64_1d(ctx, sorted_y_ft);
  futhark_free_f64_1d(ctx, y_ft);
}
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d *orderBy, void* inCol, idx_t card) {
  switch (type){
  case DUCKDB_TYPE_SMALLINT:
      orderPayloadColumn_short(ctx, (short*)outCol, incr, orderBy, (short*)inCol, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      orderPayloadColumn_int(ctx, (int*)outCol, incr, orderBy, (int*)inCol, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      orderPayloadColumn_long(ctx, (long*)outCol, incr, orderBy, (long*)inCol, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      orderPayloadColumn_float(ctx, (float*)outCol, incr, orderBy, (float*)inCol, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      orderPayloadColumn_double(ctx, (double*)outCol, incr, orderBy, (double*)inCol, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}