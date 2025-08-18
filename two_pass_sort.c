#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftSMJ.h"
#include "mylogger.h"
#include "myutil.h"
#include "sortstages.h"

#define LOGFILE "two_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 16*512*CHUNK_SIZE
#define TABLE_SIZE BUFFER_SIZE

#define BLOCK_SIZE (int16_t)256

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "20GB"
#define DDB_TEMPDIR "tps_tempdir"


int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Two-Pass Sort : Starting test program.");
  if(LOGFILE && !logfile) {
    perror("Failed to initialise logger.");
    return -1;
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
  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl (k BIGINT, payload1 SMALLINT, payload2 DOUBLE, payload3 FLOAT);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (
    duckdb_prepare(
      con,
      //"INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
      "INSERT INTO tbl (SELECT 10000000000*random(), 10000*random(), 10000*random(), (10000*random() + 10000) FROM range($1) t(i));",
      &init_stmt
    ) 
    == DuckDBError
  )
  {
    perror("Failed to initialise the table.");
    return 1;
  }
  duckdb_bind_int32(init_stmt, 1, TABLE_SIZE);
  duckdb_execute_prepared(init_stmt, NULL);
  duckdb_destroy_prepare(&init_stmt);

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");


  mylog(logfile, "EXPERIMENT #1 -- duckdb-native CPU sorting (without payloads).");
  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE CPU_withoutPL AS (SELECT k FROM tbl ORDER BY k);", NULL);

  mylog(logfile, "EXPERIMENT #2 -- duckdb-native CPU sorting (with payloads).");
  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE CPU_withPL AS (SELECT * FROM tbl ORDER BY k);", NULL);

  mylog(logfile, "EXPERIMENT #3 -- GPU sorting (without payloads).");
  two_pass_sort_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    logfile,
    ctx,
    con,
    "tbl",
    "k",
    "tmp_interm",
    "GPU_withoutPL",
    false,
    false,
    true
  );

  mylog(logfile, "EXPERIMENT #4 -- GPU sorting (with payloads).");
  two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    logfile,
    ctx,
    con,
    "tbl",
    "k",
    "tmp_interm",
    "GPU_withPL",
    false,
    false,
    true
  );

  // review result
  /*
  printf("\n\nResults\n");
  duckdb_result res;
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
  duckdb_destroy_result(&res);
  printf("\n\n");
  */

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
