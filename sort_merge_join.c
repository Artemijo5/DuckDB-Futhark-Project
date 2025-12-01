#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"
#include "sortstages.h"

#include "ftRelational.h"
#include "smjutil.h"
#include "SMJstages.h"

#define LOGFILE "stdout"//"sort_merge_join.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 64*128*CHUNK_SIZE

#define R_TABLE_SIZE 128*1024*CHUNK_SIZE
#define S_TABLE_SIZE 128*1024*CHUNK_SIZE

#define BLOCK_SIZE (idx_t)128000 // TODO radix sort segfaults
#define MERGE_PARTITION_SIZE 128*1024*CHUNK_SIZE
#define GATHER_PSIZE (idx_t)128*1024*CHUNK_SIZE*sizeof(long)

#define R_TBL_NAME "R_tbl"
#define S_TBL_NAME "S_tbl"

#define R_KEY "k"
#define S_KEY "k"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"
#define R_interm_without "R_tbl_interm_wo"
#define S_interm_without "S_tbl_interm_wo"

#define R_SORTED_NAME "R_tbl_sorted"
#define S_SORTED_NAME "S_tbl_sorted"

#define R_JOIN_BUFFER 128*1024*CHUNK_SIZE
#define S_JOIN_BUFFER 128*1024*CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "20GB"
#define DDB_TEMPDIR "tps_tempdir"

#define VERBOSE true

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Sort Merge Join: Starting test program...");
  if(LOGFILE && !logfile) {
    perror("Failed to initialise logger.");
    return -1;
  }
  FILE* func_logfile = (VERBOSE)? logfile: NULL;

  char log_param[10000];
  sprintf(log_param,
    "Logging program parametres:\n"
    "\tR TABLE NAME       %s\n"
    "\tS TABLE NAME       %s\n"
    "\tR TABLE SIZE       %ld\n"
    "\tS TABLE SIZE       %ld\n"
    "\tBUFFER SIZE        %ld\n"
    "\tBUFFER CAPACITY    %ld\n"
    "\tPARALLEL BYTES     %ld\n"
    "\tGATHER BYTES       %ld\n"
    "\tR_JOIN_BUFFER      %ld\n"
    "\tS_JOIN_BUFFER      %ld\n"
    "\tVERBOSE            %d",
    R_TBL_NAME, S_TBL_NAME, R_TABLE_SIZE, S_TABLE_SIZE,
    BUFFER_SIZE, BUFFER_SIZE/CHUNK_SIZE, MERGE_PARTITION_SIZE, GATHER_PSIZE,
    R_JOIN_BUFFER, S_JOIN_BUFFER, VERBOSE
  );
  mylog(logfile, log_param);

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

/*
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
    "INSERT INTO %s (SELECT 2000000000000000000*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
    R_TABLE_SIZE
  );
  sprintf(
    S_init_query,
    "INSERT INTO %s (SELECT 2000000000000000000*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    S_TBL_NAME,
    S_TABLE_SIZE
  );
  duckdb_query(con, R_init_query, NULL);
  duckdb_query(con, S_init_query, NULL);
  mylog(logfile, "Created the tables R and S.");
*/
  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  // Semisorts
  /*
  mylog(logfile, "Semi-sorting tables...");
  idx_t R_tbl_num = semi_sort_with_payloads(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    R_TBL_NAME,
    R_KEY,
    R_interm,
    false,
    false
  );
  mylog(logfile, "Semi-sorted R (with payloads).");
  idx_t S_tbl_num = semi_sort_with_payloads(
    CHUNK_SIZE,
    R_JOIN_BUFFER+S_JOIN_BUFFER,
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
  mylog(logfile, "Semi-sorted S (with payloads).");
  idx_t R_tbl_num_without = semi_sort_without_payloads(
    CHUNK_SIZE,
    2*(R_JOIN_BUFFER+S_JOIN_BUFFER),
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    R_TBL_NAME,
    R_KEY,
    R_interm_without,
    false,
    false
  );
  mylog(logfile, "Semi-sorted R (without payloads).");
  idx_t S_tbl_num_without = semi_sort_without_payloads(
    CHUNK_SIZE,
    2*(R_JOIN_BUFFER+S_JOIN_BUFFER),
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    S_TBL_NAME,
    S_KEY,
    S_interm_without,
    false,
    false
  );
  mylog(logfile, "Semi-sorted S (without payloads).");
  */
// ############################################################################################################
// JOIN PHASE
// ############################################################################################################

  mylog(logfile, "EXPERIMENT #1 -- CPU-base join.");
  char joinQ[1000];
  sprintf(joinQ, "CREATE OR REPLACE TABLE CPU_joinRes AS (SELECT r.*, s.* EXCLUDE s.%s FROM %s r JOIN %s s ON (r.%s == s.%s));",
    S_KEY, R_TBL_NAME, S_TBL_NAME, R_KEY, S_KEY);
  if(duckdb_query(con, joinQ, NULL) == DuckDBError) {
    perror("Failed to join with duckdb.");
  }

  mylog(logfile, "EXPERIMENT #2.a -- GPU-based (GFTR) SMJ (with merge sort)----------------------------------------------------");
  SortMergeJoin_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    false,
    func_logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFTR_mergeSort",
    false,
    false
  );
  /*mylog(logfile, "EXPERIMENT #2.b -- GPU-based (GFTR) SMJ (with radix sort)----------------------------------------------------");
  SortMergeJoin_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    true,
    func_logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFTR_radixSort",
    false,
    false
  );*/
  mylog(logfile, "EXPERIMENT #3.a -- GPU-based (GFUR) SMJ (with merge sort)----------------------------------------------------");
  SortMergeJoin_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    false,
    func_logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFUR_mergeSort",
    false,
    false
  );
  /*mylog(logfile, "EXPERIMENT #3.b -- GPU-based (GFUR) SMJ (with radix sort)----------------------------------------------------");
  SortMergeJoin_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    true,
    func_logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    false,
    false,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFUR_radixSort",
    false,
    false
  );*/
 /* mylog(logfile, "EXPERIMENT #4 -- GPU-based (GFTR) SMJ (from semisorted)----------------------------------------------------");
  MergeJoin_GFTR_semisorted(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    func_logfile,
    ctx,
    con,
    R_interm,
    S_interm,
    R_tbl_num,
    S_tbl_num,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFTR_semisorted",
    false,
    false
  );
  mylog(logfile, "EXPERIMENT #5 -- GPU-based (GFUR) SMJ (from semisorted)----------------------------------------------------");
  MergeJoin_GFUR_semisorted(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    func_logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    R_interm_without,
    S_interm_without,
    R_tbl_num_without,
    S_tbl_num_without,
    R_KEY,
    S_KEY,
    "GPU_SMJ_GFUR_semisorted_TMP",
    "GPU_SMJ_GFUR_semisorted",
    false,
    false
  );*/

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
