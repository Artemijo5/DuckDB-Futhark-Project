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
#define BUFFER_SIZE 5*CHUNK_SIZE//32*CHUNK_SIZE//128*CHUNK_SIZE
#define TABLE_SIZE 3*BUFFER_SIZE//16*BUFFER_SIZE//BUFFER_SIZE + CHUNK_SIZE + 4//64*BUFFER_SIZE

#define BLOCK_SIZE (int16_t)CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "2GB"
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
  duckdb_query(con, "CREATE OR REPLACE TABLE tbl (k BIGINT, payload1 BIGINT, payload2 DOUBLE);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (
    duckdb_prepare(
      con,
      //"INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
      "INSERT INTO tbl (SELECT 10000000*random(), 10000*random(), 10000*random() FROM range($1) t(i));",
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

  // where the sorting happens, concentrated into an one-liner
  two_pass_sort_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    logfile,
    ctx,
    con,
    "tbl",
    "tmp_interm",
    "TPSresult",
    false
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