#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftsort.h"

#define LOGFILE "sorting_test.log.txt"
#define TABLE_SIZE 128

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "sorting_test.c: Starting test program.");
  if(!logfile) {
    perror("Failed to initialise the logger.")
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

  // iterate until result is exhausted
	while (true) {
    mylog(logfile, "Fetching next data chunk.");
		duckdb_data_chunk result = duckdb_fetch_chunk(res);
		if (!result) {
			mylog(logfile, "Table has been fully processed.");
			break;
		}
		// get the number of rows & columns from the data chunk
		idx_t row_count = duckdb_data_chunk_get_size(result);
		idx_t col_count = duckdb_data_chunk_get_column_count(result);

    // obtain the column vectors
		duckdb_vector res_col[col_count];
		int32_t *vector_data[col_count];
		for (idx_t col = 0; col < col_count; col++) {
			res_col[col] = duckdb_data_chunk_get_vector(result, col);
			vector_data[col] = (int32_t *) duckdb_vector_get_data(res_col[col]);
		}
		int32_t* xp = (int32_t *) duckdb_vector_get_data(res_col[0]);
		int32_t* yp = (int32_t *) duckdb_vector_get_data(res_col[1]);
    // unwrap the vectors to C arrays
    int* x = (int*) xp;
		int* y = (int*) yp;

    // Log pre-sorted values of x & y
    mylog(logfile, "Recording values prior to sorting:")
    char log_x[7*row_count + 10];
    char log_y[7*row_count + 10];
    int len_x = sprintf(log_x, "Array x ");
    int len_y = sprintf(log_y, "Array y ");
    for (idx_t row = 0; row < row_count; row++) {
      len_x += sprintf(log_x + len_x, "%d, ", x[row]);
      len_y += sprintf(log_y + len_y, "%d, ", y[row]);
    }
    mylog(logfile, log_x);
    mylog(logfile, log_y);
    
    // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
  	struct futhark_context *ctx = futhark_context_new(cfg);
    // Process arrays to futhark arrays
    struct futhark_i32_1d *x_arr = futhark_new_i32_1d(ctx, x, 4);
  	struct futhark_i32_1d *y_arr = futhark_new_i32_1d(ctx, y, 4);
    // Sort column x
    struct futhark_opaque_sortInfo sortpair_x;
    futhark_entry_sortColumn_int(ctx, &sortpair_x, x_arr);
    futhark_context_sync(ctx); // TODO needed?
    // Separate the ordered indices from sorted x
    struct futhark_i64_1d *sorted_i_arr;
    struct futhark_i32_1d *sorted_x_arr;
    futhark_project_opaque_sortInfo_is(ctx, &sorted_i_arr, &sortpair_x); // TODO &sortpair or sortpair?
    futhark_project_opaque_sortInfo_xs(ctx, &sorted_x_arr, &sortpair_x); // TODO =||=
    futhark_context_sync(ctx); // TODO needed?
    // unwrap from futhark
    long sortedIndices[row_count];
    int sorted_x[row_count];
    futhark_values_i64_1d(ctx, sorted_i_arr, sortedIndices);
    futhark_values_i32_1d(ctx, sorted_x_arr, sorted_x);
    futhark_context_sync(ctx);
    // TODO
    // sort y based on indices
    // free stuff
    // log
    // ...
    // at a time when I don't have a headache and my stomach is full
  }
  // clean-up
	duckdb_destroy_result(&res);
	duckdb_disconnect(&con);
	duckdb_close(&db);
  
  return 0;
}
