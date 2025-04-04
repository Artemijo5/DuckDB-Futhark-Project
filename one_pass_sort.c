#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftsort.h"
#include <string.h>

#define LOGFILE "one_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define TABLE_SIZE CHUNK_SIZE*2
#define BUFFER_SIZE 65535

// TODO parameterise 1-pass sort, then move on to 2-pass
// and do various gpu tests


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
 * outCol : a double pointer for the output of the sorted list
 * type : the logical type of the data, expressed in enum duckdb_type
 * incr : the index of the first row in the current data chunk
 * outIdx : a double pointer for the output of the reordered indices
 * keys : the key column to be sorted
 * size : the size of the memory space allocated to keys (number of rows * sizeof type)
 */
void sortKeyColumn(struct futhark_context *ctx, void **outCol, duckdb_type type, idx_t incr, long **outIdx, void* keys, idx_t size);
/**
 * A function to reorder a payload column, according to the reordered indices from sorting the key column.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a double pointer for the output of the sorted list
 * type : the logical type of the data, expressed in enum duckdb_type
 * orderBy : the reordered indices
 * inCol : the payload column to be reordered
 * size : the size of the memory space allocated to inCol (number of rows * sizeof type)
 */
void orderPayloadColumn(struct futhark_context *ctx, void **outCol, duckdb_type type, long* orderBy, void* inCol, idx_t size);

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

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

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

  char msgbuffer[50];
  int msglen = sprintf(msgbuffer, "Finished reading db table -- total of ");
  msglen += sprintf(msgbuffer + msglen, "%ld", incr_idx);
  msglen += sprintf(msgbuffer + msglen, " rows.");
  mylog(logfile, msgbuffer);

  // sort key column
  struct futhark_i32_1d *x_ft = futhark_new_i32_1d(ctx, (int*)Buffers[0], incr_idx);
  mylog(logfile, "Wrapped keys into a futhark array.");
  struct futhark_opaque_sortInfo_int *sortInfo;
  futhark_entry_sortColumn_int(ctx, &sortInfo, (long)0, x_ft);
  mylog(logfile, "Sorted keys.");
  struct futhark_i64_1d *sorted_idx_ft;
  struct futhark_i32_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_int_is(ctx, &sorted_idx_ft, sortInfo);
  futhark_project_opaque_sortInfo_int_xs(ctx, &sorted_x_ft, sortInfo);
  mylog(logfile, "Decoupled sorted keys & ordered indices.");
  int sorted_x[incr_idx];
  long sorted_idx[incr_idx];
  futhark_values_i32_1d(ctx, sorted_x_ft, sorted_x);
  futhark_values_i64_1d(ctx, sorted_idx_ft, sorted_idx);
  futhark_context_sync(ctx);
  mylog(logfile, "Unwrapped & synced futhark context.");
  int isSorted = true;
  int indexIsCorrect = true;
  for(idx_t i=0; i<incr_idx-1; i++) {
    isSorted =  isSorted && (sorted_x[i] <= sorted_x[i+1]);
    indexIsCorrect = indexIsCorrect && (sorted_x[i] == ((int*)Buffers[0])[sorted_idx[i]]);
  }
  indexIsCorrect = indexIsCorrect && (sorted_x[incr_idx-1] == ((int*)Buffers[0])[sorted_idx[incr_idx-1]]);
  logdbg(logfile, isSorted, "x was sorted correctly.", "!!!!!!!!!!!!!!! Error: x was not sorted correctly. !!!!!!!!!!!!!!!");
  logdbg(logfile, indexIsCorrect, "Indices were ordered correctly.", "!!!!!!!!!!!!!!! Error: indices were not ordered correctly. !!!!!!!!!!!!!!!");
  
  // Next do the payload columns
  struct futhark_i32_1d *y_ft = futhark_new_i32_1d(ctx, (int*)Buffers[1], incr_idx);
  struct futhark_f32_1d *z_ft = futhark_new_f32_1d(ctx, (float*)Buffers[2], incr_idx);
  mylog(logfile, "Wrapped payload columns y&z into futhark arrays.");
  struct futhark_i32_1d *sorted_y_ft;
  struct futhark_f32_1d *sorted_z_ft;
  futhark_entry_orderByIndices_int(ctx, &sorted_y_ft, (long)0, sorted_idx_ft, y_ft);
  mylog(logfile, "Ordered y (wrapped) according to ordered indices.");
  futhark_entry_orderByIndices_float(ctx, &sorted_z_ft, (long)0, sorted_idx_ft, z_ft);
  mylog(logfile, "Ordered z (wrapped) according to ordered indices.");
  // check that y was ordered correctly
  int sorted_y[incr_idx];
  futhark_values_i32_1d(ctx, sorted_y_ft, sorted_y);
  futhark_context_sync(ctx);
  mylog(logfile, "Synced futhark context.");
  int yIsCorrect = true;
  for (idx_t i=0; i<incr_idx; i++) {
    yIsCorrect = yIsCorrect && (sorted_y[i] == ((int*)Buffers[1])[sorted_idx[i]]);
  }
  logdbg(logfile, yIsCorrect, "y was reordered correctly.", "!!!!!!!!!!!!!!! Error: y was not reordered correctly. !!!!!!!!!!!!!!!");
  // check that z was ordered correctly
  float sorted_z[incr_idx];
  futhark_values_f32_1d(ctx, sorted_z_ft, sorted_z);
  futhark_context_sync(ctx);
  mylog(logfile, "Synced futhark context.");
  int zIsCorrect = true;
  for (idx_t i=0; i<incr_idx; i++) {
    zIsCorrect = zIsCorrect && (sorted_z[i] == ((float*)Buffers[2])[sorted_idx[i]]);
  }
  logdbg(logfile, zIsCorrect, "z was reordered correctly.", "!!!!!!!!!!!!!!! Error: z was not reordered correctly. !!!!!!!!!!!!!!!");
  
  /* ------------------------------------------------------------------------------------------------------------------------------
  // TODO
  // generalise
  // save to chunks & new table "sortedTbl"
  // 2-PASS
  // (probably 2-PASS first ...)
  */

  // clean-up
  futhark_free_f32_1d(ctx, sorted_z_ft);
  futhark_free_i32_1d(ctx, sorted_y_ft);
  futhark_free_f32_1d(ctx, z_ft);
  futhark_free_i32_1d(ctx, y_ft);
  futhark_free_i32_1d(ctx, sorted_x_ft);
  futhark_free_i64_1d(ctx, sorted_idx_ft);
  futhark_free_opaque_sortInfo_int(ctx, sortInfo);
  futhark_free_i32_1d(ctx, x_ft);
  mylog(logfile, "Freed futhark objects."); // TODO this'll have to happen right after sorting each - except for sorted_idx_ft ...
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
      return 0;
  }
}

void* colType_malloc(duckdb_type type, idx_t card) {
  size_t ms = colType_bytes(type);
  if (ms == 0) return NULL;
  return malloc(card * type);
}