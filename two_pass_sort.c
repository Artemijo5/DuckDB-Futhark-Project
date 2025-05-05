#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"
#include "ftsort.h"
#include "mylogger.h"
#include "myutil.h"

#define LOGFILE "two_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE CHUNK_SIZE*128
#define TABLE_SIZE 2*BUFFER_SIZE + 5*CHUNK_SIZE

#define DDB_MEMSIZE "2GB"
#define DDB_TEMPDIR "/tempdir/"

/* ------------------------------------------------------------------------------------------------------------------------------
  // TODO
  // save to chunks & new table "sortedTbl" (for now via appenders...)
  // -> 2-PASS
  // gpu runs...
  */

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Two-Pass Sort : Starting test program.");
  if(!logfile) {
    perror("Failed to initialise the logger.");
    return 1;
  }

  // DuckDB initialisation
  duckdb_database db;
	duckdb_connection con;
  duckdb_config config;

  // set config
  if (duckdb_create_config(&config) == DuckDBError) {
    perror("Failed to create config.");
    return -1;
  }
  duckdb_set_config(config, "max_memory", DDB_MEMSIZE);
  duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
	//duckdb_open(NULL, &db);
  if(duckdb_open_ext(NULL, &db, config, NULL) == DuckDBError) {
    perror("Failed to open database with configuration options.");
    return -1;
  }
  duckdb_destroy_config(&config);

	duckdb_connect(db, &con);

  // Create the table tbl on which the testing will be done.
  duckdb_result res;
	duckdb_query(con, "CREATE TABLE tbl (k DOUBLE, payload1 BIGINT, payload2 DOUBLE);", NULL);
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
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    mylog(logfile, "Obtained column's type.");
  }

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  mylog(logfile, "Beginning to process pages...");
  // STAGE 1 - SCAN TABLE, SAVE INTO TEMPORARY TABLES
  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    void *Buffers[col_count];
    mylog(logfile, "Initalising buffers for each column...");
    for(idx_t col=0; col<col_count; col++) {
      Buffers[col] = colType_malloc(type_ids[col], BUFFER_SIZE);
      if(Buffers[col] == NULL) {
        mylog(logfile, "ERROR -- failed to allocate memory.");
        logclose(logfile);
        return 1;
      }
    }
    mylog(logfile, "Successfully initialised buffers.");

    idx_t cur_rows = 0;

    // iterate until result is exhausted
  	while (BUFFER_SIZE - cur_rows >= CHUNK_SIZE) {
  		duckdb_data_chunk result = duckdb_fetch_chunk(res);
  		if (!result) {
  			mylog(logfile, "Table has been fully processed.");
        flag = false; // last iteration of the outer loop
  			break;
  		}
  		// get the number of rows & columns from the data chunk
  		idx_t row_count = duckdb_data_chunk_get_size(result);

      // obtain the column vectors
  		duckdb_vector res_col[col_count];
  		void *vector_data[col_count];
  		for (idx_t col = 0; col < col_count; col++) {
  			res_col[col] = duckdb_data_chunk_get_vector(result, col);
  			vector_data[col] = duckdb_vector_get_data(res_col[col]);
        memcpy(Buffers[col] + cur_rows*colType_bytes(type_ids[col]),
          vector_data[col],
          row_count * colType_bytes(type_ids[col]));
  		}
      
      cur_rows += row_count;

      duckdb_destroy_data_chunk(&result);
    }
    if(!flag && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      for(idx_t col=0; col<col_count; col++) {
        free(Buffers[col]); // free buffers
      }
      break;
    } // TODO flag is probably superfluous...
    char msgbuffer[50];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // sort key column
    struct futhark_i64_1d *sorted_idx_ft;
    void *sorted_cols[col_count];
    sorted_cols[0] = colType_malloc(type_ids[0], cur_rows);
    void *sorted_x = sorted_cols[0];
    mylog(logfile, "Passing key column for sorting...");
    sortKeyColumn(ctx, sorted_x, type_ids[0], incr_idx, &sorted_idx_ft, Buffers[0], cur_rows);
    mylog(logfile, "Sorted key column and obtained reordered indices.");
    //logarray_int(logfile, "Sorted x: ", sorted_x, cur_rows);
    // test that sorting & reordering was done correctly
    long sorted_idx[cur_rows];
    futhark_values_i64_1d(ctx, sorted_idx_ft, sorted_idx);
    futhark_context_sync(ctx);
    int isSorted = true;
    int indexIsCorrect = true;
    //size_t x_bytes = colType_bytes
    for(idx_t i=0; i<cur_rows-1; i++) {
      switch(type_ids[0]) {
        case DUCKDB_TYPE_SMALLINT:
          isSorted =  isSorted && (((short*)sorted_x)[i] <= ((short*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_INTEGER:
          isSorted =  isSorted && (((int*)sorted_x)[i] <= ((int*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_BIGINT:
          isSorted =  isSorted && (((long*)sorted_x)[i] <= ((long*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_FLOAT:
          isSorted =  isSorted && (((float*)sorted_x)[i] <= ((float*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_DOUBLE:
          isSorted =  isSorted && (((double*)sorted_x)[i] <= ((double*)sorted_x)[i+1]);
          break;
        default:
          perror("Invalid Type.");
          return 1;
      }
    }
    for(idx_t i=0; i<cur_rows; i++) {
      size_t x_bytes = colType_bytes(type_ids[0]);
      for(size_t b=0; b<x_bytes; b++) {
        indexIsCorrect = indexIsCorrect && (((char*)sorted_x)[i*x_bytes + b] == ((char*)Buffers[0])[sorted_idx[i]*x_bytes + b]);
      }
    }
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
    for(idx_t col=1; col<col_count; col++) {
      sorted_cols[col] = colType_malloc(type_ids[col], cur_rows);
      orderPayloadColumn(ctx, sorted_cols[col], type_ids[col], incr_idx, sorted_idx_ft, Buffers[col], cur_rows);
      mylog(logfile, "Reordered the next payload column.");
      // Test whether the reordering was done correctly...
      int yIsCorrect = true;
      for (idx_t i=0; i<cur_rows; i++) {
        size_t y_bytes = colType_bytes(type_ids[col]);
        for(size_t b=0; b<y_bytes; b++) {
          // Byte-wise comparison, rather than switch-case per type...
          yIsCorrect = yIsCorrect && (((char*)sorted_cols[col])[i*y_bytes + b] == ((char*)Buffers[col])[sorted_idx[i]*y_bytes + b]);
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
      free(sorted_cols[col]);
    }
    mylog(logfile, "Freed this page's buffers.");
    futhark_free_i64_1d(ctx, sorted_idx_ft);
    mylog(logfile, "Freed futhark objects for this page.");
  }


  // clean-up
  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");
	duckdb_destroy_result(&res);
	duckdb_disconnect(&con);
	duckdb_close(&db);

  logclose(logfile);
  
  return 0;
}