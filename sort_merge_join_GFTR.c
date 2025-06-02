#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftsort.h"
#include "mylogger.h"
#include "myutil.h"
#include "sortstages.h"

/*
TODO
start with assumption of primary keys (even if test data doesn't match)
then make buffer for tuple file
(make it an option whether to use it or not)
*/

#define LOGFILE "sort_merge_join_GFTR.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 5*CHUNK_SIZE //512*CHUNK_SIZE

#define R_TABLE_SIZE 3*CHUNK_SIZE
#define S_TABLE_SIZE 4*CHUNK_SIZE

#define BLOCK_SIZE (int16_t)2048
#define MERGE_PARTITION_SIZE CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "2GB"
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

  // Create tables R and S 
  duckdb_query(con, "setseed(0.42);", NULL);
  duckdb_query(con, "CREATE OR REPLACE TABLE R_tbl (k BIGINT, payload1 BIGINT, payload2 DOUBLE);", NULL);
  duckdb_query(con, "CREATE OR REPLACE TABLE S_tbl (k BIGINT, payload3 DOUBLE, payload4 DOUBLE);", NULL);

  // Create the test tables.
  char R_init_query[1000];
  char S_init_query[1000];
  sprintf(
  	R_init_query,
  	"INSERT INTO R_tbl (SELECT 10000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
  	R_TABLE_SIZE
  );
  sprintf(
  	S_init_query,
  	"INSERT INTO S_tbl (SELECT 10000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
  	S_TABLE_SIZE
  );
  duckdb_query(con, R_init_query, NULL);
  duckdb_query(con, S_init_query, NULL);
  mylog(logfile, "Created the tables R and S.");

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  mylog(logfile, "Now sorting the tables...");
  // R
  two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    "R_tbl",
    "R_tmp_interm",
    "R_tbl_sorted",
    false,
    false
  );
  mylog(logfile, "Sorted table R.");
  // S
  two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    "S_tbl",
    "S_tmp_interm",
    "S_tbl_sorted",
    false,
    false
  );
  mylog(logfile, "Sorted table S.");

  


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
