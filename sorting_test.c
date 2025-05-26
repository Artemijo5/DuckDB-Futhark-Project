#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftsort.h"

#define LOGFILE "sorting_test.log.txt"
#define TABLE_SIZE 4096

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "sorting_test.c: Starting test program.");
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
  // TODO placeholder
  // 2 columns (1 key & 1 payload) of integers
  duckdb_result res;
	duckdb_query(con, "CREATE TABLE tbl (k INTEGER, payload INTEGER);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);
  for(int i=0; i< TABLE_SIZE; i++) { // TODO do either in the query or with Appenders
  	duckdb_query(con, "INSERT INTO tbl VALUES (10000 * random(), 10000 * random());", NULL);
  }
  duckdb_query(con, "SELECT * FROM tbl;", &res);

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");

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
		idx_t col_count = duckdb_data_chunk_get_column_count(result);

    char msgbuffer[50];
    int msglen = sprintf(msgbuffer, "Read ");
    msglen += sprintf(msgbuffer + msglen, "%ld", row_count);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // obtain the column vectors
    mylog(logfile, "Obtaining column vectors.");
		duckdb_vector res_col[col_count];
		int32_t *vector_data[col_count];
		for (idx_t col = 0; col < col_count; col++) {
			res_col[col] = duckdb_data_chunk_get_vector(result, col);
			vector_data[col] = (int32_t *) duckdb_vector_get_data(res_col[col]);
		}
    mylog(logfile, "Getting data from column vectors for columns x&y.");
		int32_t* xp = (int32_t *) duckdb_vector_get_data(res_col[0]);
		int32_t* yp = (int32_t *) duckdb_vector_get_data(res_col[1]);
    mylog(logfile, "Columns x&y got.");
    // unwrap the vectors to C arrays
    int* x = (int*) xp;
		int* y = (int*) yp;

    // Log pre-sorted values of x & y
    //mylog(logfile, "Recording values prior to sorting:");
    //logarray_int(logfile, "Array x: ", x, row_count);
    //logarray_int(logfile, "Array y: ", y, row_count);
    
    mylog(logfile, "Beginning processing in the futhark core.");
    // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
  	struct futhark_context *ctx = futhark_context_new(cfg);
    mylog(logfile, "Set up futhark context & config.");
    
    struct futhark_i32_1d *x_ft = futhark_new_i32_1d(ctx, x, row_count);
    struct futhark_i32_1d *y_ft = futhark_new_i32_1d(ctx, y, row_count);
    mylog(logfile, "Wrapped x & y into a futhark arrays.");
    //futhark_context_sync(ctx);
    //mylog(logfile, "Synced futhark context.");
    
    struct futhark_opaque_sortInfo_int *sortInfo;
    futhark_entry_sortColumn_int(ctx, &sortInfo, (long)incr_idx, 256, x_ft);
    mylog(logfile, "Sorted x_arr.");
    //futhark_context_sync(ctx);
    //mylog(logfile, "Synced futhark context.");

    struct futhark_i64_1d *sorted_idx_ft;
    struct futhark_i32_1d *sorted_x_ft;
    futhark_project_opaque_sortInfo_int_is(ctx, &sorted_idx_ft, sortInfo);
    futhark_project_opaque_sortInfo_int_xs(ctx, &sorted_x_ft, sortInfo);
    mylog(logfile, "Decoupled sorted values & ordered indices.");
    //futhark_context_sync(ctx);
    //mylog(logfile, "Synced futhark context.");
    
    int sorted_x[row_count];
    long sorted_idx[row_count];
    futhark_values_i32_1d(ctx, sorted_x_ft, sorted_x);
    futhark_values_i64_1d(ctx, sorted_idx_ft, sorted_idx);
    futhark_context_sync(ctx);
    mylog(logfile, "Synced futhark context.");
    //logarray_int(logfile, "Sorted x: ", sorted_x, row_count);
    //logarray_long(logfile, "Ordered indices: ", sorted_idx, row_count);
    int isSorted = true;
    int indexIsCorrect = true;
    for(idx_t i=0; i<row_count-1; i++) {
      isSorted =  isSorted && (sorted_x[i] <= sorted_x[i+1]);
      indexIsCorrect = indexIsCorrect && (sorted_x[i] == x[sorted_idx[i] - incr_idx]);
    }
    indexIsCorrect = indexIsCorrect && (sorted_x[row_count-1] == x[sorted_idx[row_count-1] - incr_idx]);
    logdbg(logfile, isSorted, "x was sorted correctly.", "!!!!!!!!!!!!!!! Error: x was not sorted correctly. !!!!!!!!!!!!!!!");
    logdbg(logfile, indexIsCorrect, "Indices were ordered correctly.", "!!!!!!!!!!!!!!! Error: indices were not ordered correctly. !!!!!!!!!!!!!!!");

    struct futhark_i32_1d *sorted_y_ft;
    futhark_entry_orderByIndices_int(ctx, &sorted_y_ft, (long)incr_idx, sorted_idx_ft, y_ft);
    mylog(logfile, "Ordered y (wrapped) according to ordered indices.");
    //futhark_context_sync(ctx);
    //mylog(logfile, "Synced futhark context.");

    int sorted_y[row_count];
    futhark_values_i32_1d(ctx, sorted_y_ft, sorted_y);
    futhark_context_sync(ctx);
    mylog(logfile, "Synced futhark context.");
    //logarray_int(logfile, "Sorted y: ", sorted_y, row_count);
    int yIsCorrect = true;
    for (idx_t i=0; i<row_count; i++) {
      idx_t idx_i = sorted_idx[i] - incr_idx;
      yIsCorrect = yIsCorrect && (sorted_y[i] == y[idx_i]);
    }
    logdbg(logfile, yIsCorrect, "y was reordered correctly.", "!!!!!!!!!!!!!!! Error: y was not reordered correctly. !!!!!!!!!!!!!!!");

    incr_idx += row_count;
    mylog(logfile, "Updated index increment.");

    // clean-up
    futhark_free_i32_1d(ctx, sorted_y_ft);
    futhark_free_i32_1d(ctx, sorted_x_ft);
    futhark_free_i64_1d(ctx, sorted_idx_ft);
    futhark_free_opaque_sortInfo_int(ctx, sortInfo);
    futhark_free_i32_1d(ctx, y_ft);
    futhark_free_i32_1d(ctx, x_ft);
    mylog(logfile, "Freed futhark objects.");
    futhark_context_free(ctx);
    futhark_context_config_free(cfg);
    mylog(logfile, "Freed futhark core.");
    duckdb_destroy_data_chunk(&result);
    mylog(logfile, "Destroyed datachunk.");
  }
  // clean-up
	duckdb_destroy_result(&res);
	duckdb_disconnect(&con);
	duckdb_close(&db);

  logclose(logfile);
  
  return 0;
}
