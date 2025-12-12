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

#define R_TABLE_SIZE 16*CHUNK_SIZE
#define S_TABLE_SIZE 16*CHUNK_SIZE

#define BLOCK_SIZE (idx_t)128000 // TODO radix sort segfaults
#define MERGE_PARTITION_SIZE 128*1024*CHUNK_SIZE
#define GATHER_PSIZE (idx_t)256*CHUNK_SIZE*sizeof(long)
// TODO gathers seem to be ruined for some reason...

#define R_TBL_NAME "R_tbl"
#define S_TBL_NAME "S_tbl"

#define R_KEY "k"
#define S_KEY "k"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"
#define R_interm_without "R_tbl_interm_wo"
#define S_interm_without "S_tbl_interm_wo"

#define R_SORTED_NAME_GFTR "R_tbl_sorted"
#define R_SORTED_NAME_GFUR "R_tbl_sorted_wo"

#define R_SORT_BUFFER_GFTR 4*CHUNK_SIZE
#define S_SORT_BUFFER_GFTR 4*CHUNK_SIZE
#define R_SORT_BUFFER_GFUR 8*CHUNK_SIZE
#define S_SORT_BUFFER_GFUR 8*CHUNK_SIZE

#define R_JOIN_BUFFER 16*CHUNK_SIZE
#define S_JOIN_BUFFER 16*CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
#define DDB_TEMPDIR "tps_tempdir"

#define VERBOSE false
#define DO_CREATE_TABLES true
#define DO_TEST_CPU true
#define DO_TEST_UNSORTED true
#define DO_TEST_SEMISORTED true
#define DO_TEST_SORTED true

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
    "\tPARALLEL BYTES     %ld\n"
    "\tGATHER BYTES       %ld\n"
    "\tR_SORT_BUFFER GFTR %ld\n"
    "\tS_SORT_BUFFER GFTR %ld\n"
    "\tR_SORT_BUFFER GFUR %ld\n"
    "\tS_SORT_BUFFER GFUR %ld\n"
    "\tR_JOIN_BUFFER      %ld\n"
    "\tS_JOIN_BUFFER      %ld\n"
    "\t----------------------\n"
    "\tCREATE TABLES      %d\n"
    "\tTEST CPU JOIN      %d\n"
    "\tTEST UNSORTED      %d\n"
    "\tTEST SEMISORTED    %d\n"
    "\tTEST SORTED        %d\n"
    "\tVERBOSE            %d",
    R_TBL_NAME, S_TBL_NAME, R_TABLE_SIZE, S_TABLE_SIZE,
    MERGE_PARTITION_SIZE, GATHER_PSIZE,
    R_SORT_BUFFER_GFTR, S_SORT_BUFFER_GFTR, R_SORT_BUFFER_GFUR, S_SORT_BUFFER_GFUR,
    R_JOIN_BUFFER, S_JOIN_BUFFER,
    DO_CREATE_TABLES, DO_TEST_CPU, DO_TEST_UNSORTED, DO_TEST_SEMISORTED, DO_TEST_SORTED,
    VERBOSE
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

  // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    mylog(logfile, "Set up futhark context & config.");

  // Create tables R and S
    if(DO_CREATE_TABLES) {
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
        "INSERT INTO %s (SELECT 16000000*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
        R_TBL_NAME,
        R_TABLE_SIZE
      );
      sprintf(
        S_init_query,
        "INSERT INTO %s (SELECT 16000000*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
        S_TBL_NAME,
        S_TABLE_SIZE
      );
      duckdb_query(con, R_init_query, NULL);
      duckdb_query(con, S_init_query, NULL);
      mylog(logfile, "Created the tables R and S.");
    } else {mylog(logfile, "Skipped creating tables, using pre-existing versions...");}
  

  // Sorts & Semisorts
    idx_t R_tbl_num=0, S_tbl_num=0, R_tbl_num_without=0, S_tbl_num_without=0;

    if(DO_TEST_SEMISORTED) {
      mylog(logfile, "Semi-sorting table R...");
      R_tbl_num = semi_sort_with_payloads(
        CHUNK_SIZE,
        R_SORT_BUFFER_GFTR,
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
      R_tbl_num_without = semi_sort_without_payloads(
        CHUNK_SIZE,
        R_SORT_BUFFER_GFUR,
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
    }

    if(DO_TEST_SORTED) {
      mylog(logfile, "Sorting table R...");
      // Could just do stg2 of gpu sorting here...
      // for now do duckdb order by
      char sort_R_query[250 + strlen(R_SORTED_NAME_GFTR) + strlen(R_SORTED_NAME_GFUR) + strlen(R_TBL_NAME) + 2*strlen(R_KEY)];
      sprintf(sort_R_query, "CREATE OR REPLACE TEMP TABLE %s AS (FROM %s ORDER BY %s);", R_SORTED_NAME_GFTR, R_TBL_NAME, R_KEY);
      duckdb_query(con, sort_R_query, NULL);
      mylog(logfile, "Sorted R table (with payloads).");
      /*
      sprintf(sort_R_query, "CREATE OR REPLACE TEMP TABLE %s AS (SELECT %s FROM %s ORDER BY %s);", R_SORTED_NAME_GFUR, R_KEY R_TBL_NAME, R_KEY);
      duckdb_query(con, sort_R_query, NULL);
      mylog(logfile, "Sorted R table (without payloads).");
      */
    }
    
    if(DO_TEST_SORTED || DO_TEST_SEMISORTED) {
      mylog(logfile, "Semi-sorting table S...");
      S_tbl_num = semi_sort_with_payloads(
        CHUNK_SIZE,
        S_SORT_BUFFER_GFTR,
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
      if (DO_TEST_SEMISORTED) {
        S_tbl_num_without = semi_sort_without_payloads(
          CHUNK_SIZE,
          S_SORT_BUFFER_GFUR,
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
      }
    }

  
  
  
// ############################################################################################################
// JOIN PHASE
// ############################################################################################################

  if(DO_TEST_CPU) {
    mylog(logfile, "EXPERIMENT #1 -- CPU-base join.");
    char joinQ[1000];
    sprintf(joinQ, "CREATE OR REPLACE TABLE CPU_joinRes AS (SELECT r.*, s.* EXCLUDE s.%s FROM %s r JOIN %s s ON (r.%s == s.%s));",
      S_KEY, R_TBL_NAME, S_TBL_NAME, R_KEY, S_KEY);
    if(duckdb_query(con, joinQ, NULL) == DuckDBError) {
      perror("Failed to join with duckdb.");
    }
  } else {mylog(logfile, "Skipped CPU join test.\n");}

  if(DO_TEST_UNSORTED) {
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
  } else {mylog(logfile, "Skipped unsorted test.\n");}

  if(DO_TEST_SEMISORTED) {
    mylog(logfile, "EXPERIMENT #4 -- GPU-based (GFTR) SMJ (from semisorted)----------------------------------------------------");
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
    );    
  } else {mylog(logfile, "Skipped semi-sorted test.\n");}

  if(DO_TEST_SORTED) {
    mylog(logfile, "EXPERIMENT #6 -- GPU-based (GFTR) SMJ (with R sorted, S semisorted)----------------------------------------------------");
    SortMergeJoin_GFTR_with_S_semisorted(
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
      R_SORTED_NAME_GFTR,
      S_interm,
      true,
      S_tbl_num,
      R_KEY,
      S_KEY,
      "GPU_SMJ_GFTR_R_sorted_S_semisorted",
      false,
      false
    );
  } else {mylog(logfile, "Skipped sorted test.\n");}
  

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
