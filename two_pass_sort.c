#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftsort.h"
#include "mylogger.h"
#include "myutil.h"
#include "sortstages.c"

#define LOGFILE "two_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 10*CHUNK_SIZE//128*CHUNK_SIZE
#define TABLE_SIZE 4*BUFFER_SIZE + 134//64*BUFFER_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "10MB"
#define DDB_TEMPDIR "tps_tempdir"

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
  duckdb_set_config(config, "memory_limit", DDB_MEMSIZE);
  duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
	//duckdb_open(NULL, &db);
  if(duckdb_open_ext(DBFILE, &db, config, NULL) == DuckDBError) {
    perror("Failed to open database with configuration options.");
    return -1;
  }
  duckdb_destroy_config(&config);

	duckdb_connect(db, &con);

  // Create the table tbl on which the testing will be done.
  duckdb_result res;
	
  duckdb_query(con, "CREATE OR REPLACE TABLE tbl (k BIGINT, payload1 BIGINT, payload2 DOUBLE);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (duckdb_prepare(con,
    "INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
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
  

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  idx_t col_count = duckdb_column_count(&res);
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
  type_ids[col] = duckdb_column_type(&res, col);
    mylog(logfile, "Obtained column's type.");
  }


  mylog(logfile, "Beginning to process pages...");
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  // STAGE 1 - SCAN TABLE, SAVE INTO LOCALLY SORTED TEMPORARY TABLES
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  idx_t numIntermediate = sort_Stage1_with_payloads(CHUNK_SIZE,BUFFER_SIZE,
    logfile,
    ctx,
    con,
    res,
    col_count,
    type_ids,
    &incr_idx
  );

  duckdb_destroy_result(&res);
  mylog(logfile, "Freed initial duckdb result from the 1st stage.");

  // TODO currently Stg2 handles only key column - need to also reorder & store the payloads...
  // Will probably need to put each of those into their own buffers & then reorder, like in Stg1...

  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  // STAGE 2 - RETRIEVE DATA, SORTING THE FIRST BLOCK OF EACH FILE EACH TIME
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  sort_Stage2(
    CHUNK_SIZE,
    BUFFER_SIZE,
    logfile,
    ctx,
    con,
    col_count,
    type_ids,
    numIntermediate
  );

  // review result
  printf("\n\nResults\n");
  duckdb_result finalRes;
  if( duckdb_query(con, "SELECT * FROM TPSResult;", &res) == DuckDBError ) {
    perror("Failed final query...\n");
    return -1;
  }
  idx_t cc = 0; // chunk counter
  while(true) {
    duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
    if(!cnk) break;

    duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
    long* dat = (long*)duckdb_vector_get_data(vec);

    idx_t r = duckdb_data_chunk_get_size(cnk);
    //for(idx_t j=0; j<r; j++) {
    //  printf("%ld, ", dat[j]);
    //}
      printf(
        "Chunk %ld: first element %ld, last element %ld, number of elements %ld.\n",
        cc,
        dat[0],
        dat[r-1],
        r
      );
    cc++;

    duckdb_destroy_data_chunk(&cnk);
  }
  duckdb_destroy_result(&finalRes);
  printf("\n\n");

  // Clean-up
  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}