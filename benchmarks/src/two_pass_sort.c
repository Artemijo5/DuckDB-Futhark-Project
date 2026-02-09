#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../ft_clibs/ftRelational.h"
#include "../../clibs/mylogger.h"
#include "../../algo_utils/sort/sort_util.h"
#include "../../algo_utils/sort/sortstages.h"

#include <unistd.h>
#include <getopt.h>

// TODO parameterisable input & output tables...

#define default_LOGFILE "stdout"//"logs/two_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define default_BUFFER_SIZE 1*CHUNK_SIZE
#define default_TABLE_SIZE default_BUFFER_SIZE - 12

#define default_BLOCK_SIZE (idx_t)2048

#define default_CREATE_TABLE false

#define default_DBFILE "testdb.db"
#define default_DDB_MEMSIZE "20GB"
#define default_DDB_TEMPDIR "tps_tempdir"

int main(int argc, char *argv[]) {
  // Parse command line arguments
    char LOGFILE[250] = default_LOGFILE;
    int64_t BUFFER_SIZE = default_BUFFER_SIZE;
    int64_t TABLE_SIZE = default_TABLE_SIZE;
    int64_t BLOCK_SIZE = default_BLOCK_SIZE;
    int32_t CREATE_TABLE = default_CREATE_TABLE;
    char DBFILE[250] = default_DBFILE;
    char DDB_MEMSIZE[25] = default_DDB_MEMSIZE;
    char DDB_TEMPDIR[250] = default_DDB_TEMPDIR;

    static struct option long_options[] =
      {
          {"logfile", required_argument, 0, 'L'},
          {"buffer_size", required_argument, 0, 's'},
          {"table_size", required_argument, 0, 't'},
          {"block_size", required_argument, 0, 'b'},
          {"create_table", no_argument, 0, 'c'},
          {"db_file", required_argument, 0, 'f'},
          {"db_memsize", required_argument, 0, 'm'},
          {"db_tempdir", required_argument, 0, 'd'},
          {0, 0, 0, 0}
      };
    char ch;
    while(
      (ch = getopt_long_only(argc,argv,"L:s:t:b:cf:m:d:",long_options,NULL)) != -1
    ) {
      switch(ch) {
        case 'L':
          memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
        case 's':
          BUFFER_SIZE = atol(optarg); break;
        case 't':
          TABLE_SIZE = atol(optarg); break;
        case 'b':
          BLOCK_SIZE = atol(optarg); break;
        case 'c':
          CREATE_TABLE=true; break;
        case 'f':
          memcpy(DBFILE, optarg, strlen(optarg)+1); break;
        case 'm':
          memcpy(DDB_MEMSIZE, optarg, strlen(optarg)+1); break;
        case 'd':
          memcpy(DDB_TEMPDIR, optarg, strlen(optarg)+1); break;
      }
    }

  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Two-Pass Sort : Starting test program.");
  if(LOGFILE && !logfile) {
    perror("Failed to initialise logger.");
    return -1;
  }

  char log_param[10000];
  sprintf(log_param,
    "Logging program parametres:\n"
    "\tTABLE SIZE         %ld\n"
    "\tBUFFER SIZE        %ld\n"
    "\tBUFFER CAPACITY    %ld\n"
    "\tBLOCK_SIZE         %ld\n"
    "\tCREATE_TABLE       %d",
    TABLE_SIZE,BUFFER_SIZE,BUFFER_SIZE/CHUNK_SIZE,BLOCK_SIZE,CREATE_TABLE
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

  // Create the table tbl on which the testing will be done.
    if(CREATE_TABLE) {
      duckdb_query(con, "CREATE OR REPLACE TABLE tbl (k BIGINT, payload1 SMALLINT, payload2 DOUBLE, payload3 FLOAT);", NULL);
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
      mylog(logfile, "Created test table.");
      duckdb_destroy_prepare(&init_stmt);
    } else {mylog(logfile, "Skipped table creation.");}

  // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    mylog(logfile, "Set up futhark context & config.");


  mylog(logfile, "EXPERIMENT #1 -- duckdb-native CPU sorting (without payloads).");
  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE CPU_withoutPL AS (SELECT k FROM tbl ORDER BY k);", NULL);

  mylog(logfile, "EXPERIMENT #2 -- duckdb-native CPU sorting (with payloads).");
  duckdb_query(con, "CREATE OR REPLACE TABLE CPU_withPL AS (SELECT * FROM tbl ORDER BY k);", NULL);

  mylog(logfile, "EXPERIMENT #3.a -- GPU merge-sorting (without payloads).");
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
    "GPUmerge_withoutPL",
    false,
    false,
    true
  );

  /*mylog(logfile, "EXPERIMENT #3.b -- GPU radix-sorting (without payloads).");
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
    "GPUradix_withoutPL",
    true,
    false,
    true
  );*/

  mylog(logfile, "EXPERIMENT #4.a -- GPU merge-sorting (with payloads).");
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
    "GPUmerge_withPL",
    false,
    false,
    false
  );

  /*mylog(logfile, "EXPERIMENT #4.b -- GPU radix-sorting (with payloads).");
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
    "GPUmerge_withPL",
    true,
    false,
    true
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
