#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"
#include "../../algo_utils/sort/sortstages.h"

#include "../../ft_clibs/ftRelational.h"
#include "../../algo_utils/join/rhj/radixJoin_util.h"
#include "../../algo_utils/join/rhj/RadixJoinStages.h"

#include <unistd.h>
#include <getopt.h>

// TODO (bug)
// results mysteriously disappear when allowed to repartition up until the final byte (...)

#define default_LOGFILE "stdout"//"logs/radix_hash_join.log.txt"

#define CHUNK_SIZE duckdb_vector_size()

#define default_R_TABLE_SIZE 1*CHUNK_SIZE//1*CHUNK_SIZE
#define default_S_TABLE_SIZE 1*CHUNK_SIZE//4*CHUNK_SIZE

#define default_BLOCK_SIZE (int16_t)2084 // used for multi-pass gather and scatter operations (and by extension blocked sorting)
#define default_MAX_PARTITION_SIZE (idx_t)1
#define default_SCATTER_PSIZE (idx_t)320000000

#define default_RADIX_BITS 16
#define default_MAX_DEPTH 3

#define default_R_TBL_NAME "R2_tbl"
#define default_S_TBL_NAME "S2_tbl"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"
#define R_interm_without "R_tbl_interm_wo"
#define S_interm_without "S_tbl_interm_wo"

#define default_R_KEY "k"
#define default_S_KEY "k"

#define default_R_JOIN_BUFFER 512*CHUNK_SIZE
#define default_S_JOIN_BUFFER 512*CHUNK_SIZE

#define default_DBFILE "testdb.db"
#define default_DDB_MEMSIZE "20GB"
#define default_DDB_TEMPDIR "tps_tempdir"

#define default_VERBOSE false

int main(int argc, char *argv[]) {
  // Parse command line arguments
    // Initializations
      char LOGFILE[250] = default_LOGFILE;
      char R_TBL_NAME[250] = default_R_TBL_NAME;
      char S_TBL_NAME[250] = default_S_TBL_NAME;
      char R_KEY[250] = default_R_KEY;
      char S_KEY[250] = default_S_KEY;
      int64_t R_TABLE_SIZE = default_R_TABLE_SIZE;
      int64_t S_TABLE_SIZE = default_S_TABLE_SIZE;
      int16_t BLOCK_SIZE = default_BLOCK_SIZE;
      int32_t RADIX_BITS = default_RADIX_BITS;
      int32_t MAX_DEPTH = default_MAX_DEPTH;
      int64_t MAX_PARTITION_SIZE = default_MAX_PARTITION_SIZE;
      int64_t SCATTER_PSIZE = default_SCATTER_PSIZE;
      int64_t R_JOIN_BUFFER = default_R_JOIN_BUFFER;
      int64_t S_JOIN_BUFFER = default_S_JOIN_BUFFER;
      char DBFILE[250] = default_DBFILE;
      char DDB_MEMSIZE[25] = default_DDB_MEMSIZE;
      char DDB_TEMPDIR[250] = default_DDB_TEMPDIR;
      int32_t VERBOSE = default_VERBOSE;

    static struct option long_options[] =
      {
          {"logfile", required_argument, 0, 'L'},
          {"R_tbl", required_argument, 0, 'r'},
          {"S_tbl", required_argument, 0, 's'},
          {"R_key", required_argument, 0, 'k'},
          {"S_key", required_argument, 0, 'K'},
          {"R_size", required_argument, 0, 'R'},
          {"S_size", required_argument, 0, 'S'},
          {"block_size", required_argument, 0, 'B'},
          {"radix_bits", required_argument, 0, 'b'},
          {"max_depth", required_argument, 0, 'D'},
          {"partition_limit", required_argument, 0, 'P'},
          {"scatter_bytes", required_argument, 0, 'G'},
          {"R_join_buffer", required_argument, 0, 'j'},
          {"S_join_buffer", required_argument, 0, 'J'},
          {"db_file", required_argument, 0, 'f'},
          {"db_memsize", required_argument, 0, 'm'},
          {"db_tempdir", required_argument, 0, 'd'},
          {"verbose", no_argument, 0, 'v'},
          {0, 0, 0, 0}
      };
    char ch;
    while(
      (ch = getopt_long_only(argc,argv,"L:r:s:k:K:R:S:B:b:D:P:G:j:J:f:m:d:v",long_options,NULL)) != -1
    ) {
      switch(ch) {
        case 'L':
          memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
        case 'r':
          memcpy(R_TBL_NAME, optarg, strlen(optarg)+1); break;
        case 's':
          memcpy(S_TBL_NAME, optarg, strlen(optarg)+1); break;
        case 'k':
          memcpy(R_KEY, optarg, strlen(optarg)+1); break;
        case 'K':
          memcpy(S_KEY, optarg, strlen(optarg)+1); break;
        case 'R':
          R_TABLE_SIZE = atol(optarg); break;
        case 'S':
          S_TABLE_SIZE = atol(optarg); break;
        case 'B':
          BLOCK_SIZE = (int16_t)atoi(optarg); break;
        case 'b':
          RADIX_BITS = atoi(optarg); break;
        case 'D':
          MAX_DEPTH = atoi(optarg); break;
        case 'P':
          MAX_PARTITION_SIZE = atol(optarg); break;
        case 'G':
          SCATTER_PSIZE = atol(optarg); break;
        case 'j':
          R_JOIN_BUFFER = atol(optarg); break;
        case 'J':
          S_JOIN_BUFFER = atol(optarg); break;
        case 'f':
          memcpy(DBFILE, optarg, strlen(optarg)+1); break;
        case 'm':
          memcpy(DDB_MEMSIZE, optarg, strlen(optarg)+1); break;
        case 'd':
          memcpy(DDB_TEMPDIR, optarg, strlen(optarg)+1); break;
        case 'v':
          VERBOSE=true; break;
      }
    }

  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Radix Hash Join: Starting test program...");
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
    "\tMAX PARTITION SIZE %ld\n"
    "\tRADIX BITS         %d\n"
    "\tMAX PART DEPTH     %d\n"
    "\tSCATTER BYTES      %ld\n"
    "\tR_JOIN_BUFFER      %ld\n"
    "\tS_JOIN_BUFFER      %ld\n"
    "\tVERBOSE            %d",
    R_TBL_NAME, S_TBL_NAME, R_TABLE_SIZE, S_TABLE_SIZE,
    MAX_PARTITION_SIZE, RADIX_BITS, MAX_DEPTH,
    SCATTER_PSIZE, R_JOIN_BUFFER, S_JOIN_BUFFER, VERBOSE
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
    "INSERT INTO %s (SELECT 4096*random(), 10000*random(), 1000000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
    R_TABLE_SIZE
  );
  sprintf(
    S_init_query,
    "INSERT INTO %s (SELECT 4096*random(), 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    S_TBL_NAME,
    S_TABLE_SIZE
  );
  duckdb_query(con, R_init_query, NULL);
  duckdb_query(con, S_init_query, NULL);
  mylog(logfile, "Created the tables R and S.");
  

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
  char joinQ[1000 + strlen(R_TBL_NAME) + strlen(S_TBL_NAME) + strlen(R_KEY) + 2*strlen(S_KEY)];
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
