#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"
#include "sortstages.h"

#include "ftRelational.h"
#include "radixJoin_util.h"
#include "RadixJoinStages.h"

#define LOGFILE "radix_hash_join.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 1024*CHUNK_SIZE

#define R_TABLE_SIZE 2*CHUNK_SIZE
#define S_TABLE_SIZE 4*CHUNK_SIZE

#define BLOCK_SIZE (int16_t)2084 // used for multi-pass gather and scatter operations (and by extension blocked sorting)
#define MAX_PARTITION_SIZE 1024
#define SCATTER_PSIZE 32000

#define RADIX_BITS 16
// TODO for some reason repartitioning past a threshold (different per try) omits result tuples...
// possibly the useful threshold?
#define MAX_DEPTH 2

#define R_TBL_NAME "R2_tbl"
#define S_TBL_NAME "S2_tbl"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"
#define R_interm_without "R_tbl_interm_wo"
#define S_interm_without "S_tbl_interm_wo"

#define R_KEY "k"
#define S_KEY "k"

#define R_JOIN_BUFFER 1*CHUNK_SIZE
#define S_JOIN_BUFFER 2*CHUNK_SIZE
#define JOIN_TBL_NAME "R_S_HashJoinTbl_GFTR"

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "20GB"
#define DDB_TEMPDIR "tps_tempdir"

#define VERBOSE true

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Radix Hash Join: Starting test program...");
  if(LOGFILE && !logfile) {
    perror("Failed to initialise logger.");
    return -1;
  }
  FILE* func_logfile = (VERBOSE)? logfile: NULL;

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

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  // Create tables R and S
  ///*
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
    "INSERT INTO %s (SELECT 3000000*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
    R_TABLE_SIZE
  );
  sprintf(
    S_init_query,
    "INSERT INTO %s (SELECT 3000000*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    S_TBL_NAME,
    S_TABLE_SIZE
  );
  duckdb_query(con, R_init_query, NULL);
  duckdb_query(con, S_init_query, NULL);
  mylog(logfile, "Created the tables R and S.");
  //*/

  /*
  duckdb_query(con, "setseed(0.42);", NULL);
  char createRtbl[1000 + strlen(R_TBL_NAME)];
  char createStbl[1000 + strlen(S_TBL_NAME)];
  sprintf(createRtbl, "CREATE OR REPLACE TABLE %s (k BIGINT UNIQUE, payload1 FLOAT, payload2 INTEGER, payload3 SMALLINT);", R_TBL_NAME);
  duckdb_query(con, createRtbl, NULL);
  sprintf(createStbl, "CREATE OR REPLACE TABLE %s (k BIGINT UNIQUE, payload4 INTEGER, payload5 SMALLINT, payload6 DOUBLE);", S_TBL_NAME);
  duckdb_query(con, createStbl, NULL);

  // Create the test tables.
  char R_init_query[1000 + strlen(R_TBL_NAME)];
  char S_init_query[1000 + strlen(S_TBL_NAME)];
  sprintf(
    R_init_query,
    "INSERT INTO %s (SELECT 10000000*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
    (long)250
  );
  sprintf(
    S_init_query,
    "INSERT INTO %s (SELECT 10000000*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    S_TBL_NAME,
    (long)250
  );
  for(idx_t i=0; i<(R_TABLE_SIZE+249)/250;i++) {
    duckdb_query(con, R_init_query, NULL);
  }
  for(idx_t i=0; i<(S_TABLE_SIZE+249)/250;i++) {
    duckdb_query(con, S_init_query, NULL);
  }
  mylog(logfile, "Created the tables R and S.");
  */

  mylog(logfile, "Semi-partitioning tables...");
  idx_t R_num_tbl = radix_semiPartition_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    NULL,
    RADIX_BITS,
    2,
    ctx,
    con,
    R_TBL_NAME,
    R_KEY,
    R_interm,
    false
  );
  mylog(logfile, "Semi-partitioned R (with payloads).");
  idx_t S_num_tbl = radix_semiPartition_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    NULL,
    RADIX_BITS,
    2,
    ctx,
    con,
    S_TBL_NAME,
    S_KEY,
    S_interm,
    false
  );
  mylog(logfile, "Semi-partitioned S (with payloads).");
  idx_t R_num_tbl_wo = radix_semiPartition_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    NULL,
    RADIX_BITS,
    2,
    ctx,
    con,
    R_TBL_NAME,
    R_KEY,
    R_interm_without,
    false
  );
  mylog(logfile, "Semi-partitioned R (without payloads).");
  idx_t S_num_tbl_wo = radix_semiPartition_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    NULL,
    RADIX_BITS,
    2,
    ctx,
    con,
    S_TBL_NAME,
    S_KEY,
    S_interm_without,
    false
  );
  mylog(logfile, "Semi-partitioned S (without payloads).");

// ############################################################################################################
// JOIN PHASE
// ############################################################################################################

  mylog(logfile, "EXPERIMENT $1 -- CPU-base join.");
  char joinQ[1000];
  sprintf(joinQ, "CREATE OR REPLACE TABLE CPU_joinRes_RHJ AS (SELECT r.*, s.* EXCLUDE s.%s FROM %s r JOIN %s s ON (r.%s == s.%s));",
    S_KEY, R_TBL_NAME, S_TBL_NAME, R_KEY, S_KEY);
  if(duckdb_query(con, joinQ, NULL) == DuckDBError) {
    perror("Failed to join with duckdb.");
  }

  mylog(logfile, "EXPERIMENT #2 -- GPU-based (GFTR) radix-hash join. ----------------------------------------");
  RadixHashJoin_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    func_logfile,
    ctx,
    con,
    RADIX_BITS,
    2, // bit step
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_RHJ_GFTR",
    false,
    false
  );
  mylog(logfile, "EXPERIMENT #3 -- GPU-based (GFUR) radix-hash join. ----------------------------------------");
  RadixHashJoin_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    func_logfile,
    ctx,
    con,
    RADIX_BITS,
    2, // bit step
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_RHJ_GFUR",
    false,
    false
  );
  mylog(logfile, "EXPERIMENT #4 -- GPU-based (GFTR) radix-hash join (from Semi-partitioned). ---------------------");
  RadixHashJoin_semisorted_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    func_logfile,
    ctx,
    con,
    RADIX_BITS,
    2,
    R_interm,
    S_interm,
    R_num_tbl,
    S_num_tbl,
    R_KEY,
    S_KEY,
    "GPU_RHJ_GFTR_semipart",
    false,
    false
  );
  mylog(logfile, "EXPERIMENT #5 -- GPU-based (GFUR) radix-hash join (from Semi-partitioned). ---------------------");
  RadixHashJoin_semisorted_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    SCATTER_PSIZE,
    MAX_PARTITION_SIZE,
    MAX_DEPTH,
    func_logfile,
    ctx,
    con,
    RADIX_BITS,
    2,
    R_TBL_NAME,
    S_TBL_NAME,
    R_interm_without,
    S_interm_without,
    R_num_tbl_wo,
    S_num_tbl_wo,
    R_KEY,
    S_KEY,
    "GPU_RHJ_GFUR_semipart_TMP",
    "GPU_RHJ_GFUR_semipart",
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
