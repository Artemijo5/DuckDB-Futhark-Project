#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"
#include "sortstages.h"

#include "ftRelational.h"
#include "smjutil.h"
#include "SMJstages.h"

#define LOGFILE "sort_merge_join_GFTR.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 1024*CHUNK_SIZE

#define R_TABLE_SIZE 10*CHUNK_SIZE
#define S_TABLE_SIZE 10*R_TABLE_SIZE

#define BLOCK_SIZE (int16_t)2084 // used for multi-pass gather and scatter operations (and by extension blocked sorting)
#define MERGE_PARTITION_SIZE 2084 // average size of each partition in ONE array (half the size of co-partitions by Merge Path)

#define R_TBL_NAME "R_tbl"
#define S_TBL_NAME "S_tbl"

#define R_KEY "k"
#define S_KEY "k"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"

#define R_SORTED_NAME "R_tbl_sorted"
#define S_SORTED_NAME "S_tbl_sorted"

#define R_JOIN_BUFFER R_TABLE_SIZE
#define S_JOIN_BUFFER R_JOIN_BUFFER
#define JOIN_TBL_NAME "R_S_joinTbl_GFTR"

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "20GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "GFTR SMJ: Starting test program...");
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
  char createRtbl[1000 + strlen(R_TBL_NAME)];
  char createStbl[1000 + strlen(S_TBL_NAME)];
  sprintf(createRtbl, "CREATE OR REPLACE TABLE %s (k BIGINT, payload1 FLOAT, payload2 INTEGER, payload3 SMALLINT);", R_TBL_NAME);
  duckdb_query(con, createRtbl, NULL);
  sprintf(createStbl, "CREATE OR REPLACE TABLE %s (k BIGINT, payload4 INTEGER, payload5 SMALLINT, payload6 DOUBLE);", S_TBL_NAME);
  duckdb_query(con, createStbl, NULL);

  // Create the test tables.
  char R_init_query[1000 + strlen(R_TBL_NAME)];
  char S_init_query[1000 + strlen(S_TBL_NAME)];
  sprintf(
    R_init_query,
    "INSERT INTO %s (SELECT 100000000*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
    R_TABLE_SIZE
  );
  sprintf(
    S_init_query,
    "INSERT INTO %s (SELECT 100000000*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    S_TBL_NAME,
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
  /*two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    R_TBL_NAME,
    R_KEY,
    R_interm,
    R_SORTED_NAME,
    false,
    false,
    true
  );*/
  //Tmp
  /*
  if(  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE R_tbl_sorted AS (SELECT * FROM R_tbl ORDER BY k);", NULL) == DuckDBError) {
	perror("Failed to sort R_tbl.");
	return -1;
  }
  */
  mylog(logfile, "Sorted table R.");

  // S
  /*two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    S_TBL_NAME,
    S_KEY,
    S_interm,
    S_SORTED_NAME,
    false,
    false,
    true
  );*/
  idx_t S_tbl_num = semi_sort_with_payloads(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    S_TBL_NAME,
    S_KEY,
    S_interm,
    false,
    false
  );

  //Tmp
  /*
  if ( duckdb_query(con, "CREATE OR REPLACE TEMP TABLE S_tbl_sorted AS (SELECT * FROM S_tbl ORDER BY k);", NULL) == DuckDBError) {
	perror("Failed to sort S_tbl.");
	return -1;
  }
  */
  mylog(logfile, "Sorted table S.");  

// ############################################################################################################
// JOIN PHASE
// ############################################################################################################

  mylog(logfile, "EXPERIMENT $1 -- CPU-base join.");
  char joinQ[1000];
  sprintf(joinQ, "CREATE OR REPLACE TABLE CPU_joinRes AS (SELECT r.*, s.* EXCLUDE s.%s FROM %s r JOIN %s s ON (r.%s == s.%s));",
    S_KEY, R_TBL_NAME, S_TBL_NAME, R_KEY, S_KEY);
  if(duckdb_query(con, joinQ, NULL) == DuckDBError) {
    perror("Failed to join with duckdb.");
  }

  mylog(logfile, "EXPERIMENT #2 -- GPU-based (GFTR) join.");
  SortMergeJoin_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    R_KEY,
    S_KEY,
    JOIN_TBL_NAME,
    false,
    false
  );
  

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
