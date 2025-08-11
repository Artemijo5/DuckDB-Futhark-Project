#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"
#include "sortstages.h"

#include "ftSMJ.h"
#include "smjutil.h"
#include "SMJstages.h"

#define LOGFILE "sort_merge_join_GFUR.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 10*512*CHUNK_SIZE // MUST BE CHUNK_SIZE MULTIPLE

#define R_TABLE_SIZE 10*50*CHUNK_SIZE
#define S_TABLE_SIZE 8*R_TABLE_SIZE + 4*CHUNK_SIZE

#define BLOCK_SIZE (int16_t)256 // used for multi-pass gather and scatter operations (and by extension blocked sorting)
#define EXT_PARALLELISM 1024 // decides the "upper bound" of external threads in some nested parallel operations (possibly redudant)
#define MERGE_PARTITION_SIZE 256 // average size of each partition in ONE array (half the size of co-partitions by Merge Path)
#define RESCALE_FACTOR 256 // (arbitrarily) used to set the number of windows vs the number of partitions

#define PAYLOAD_INDEX_BLOCK CHUNK_SIZE // Materialisation Phase -- MUST BE CHUNK_SIZE MULTIPLE
#define PAYLOAD_GATHER_BLOCK 10*CHUNK_SIZE // Materialisation Phase -- MUST BE CHUNK_SIZE MULTIPLE

#define R_TBL_NAME "R_tbl"
#define S_TBL_NAME "S_tbl"

#define R_interm "R_tbl_interm"
#define S_interm "S_tbl_interm"

#define R_SORTED_NAME "R_tbl_sorted"
#define S_SORTED_NAME "S_tbl_sorted"

#define R_JOIN_BUFFER R_TABLE_SIZE // MUST BE CHUNK_SIZE MULTIPLE
#define S_JOIN_BUFFER R_JOIN_BUFFER // MUST BE CHUNK_SIZE MULTIPLE
#define JOIN_TMP_TBL_NAME "R_S_joinTbl_GFUR_TMP"
#define JOIN_TBL_NAME "R_S_joinTbl_GFUR"

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "20GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "GFUR SMJ: Starting test program...");
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
  /*
  duckdb_query(con, "setseed(0.42);", NULL);
  char createRtbl[1000 + strlen(R_TBL_NAME)];
  char createStbl[1000 + strlen(S_TBL_NAME)];
  sprintf(createRtbl, "CREATE OR REPLACE TABLE %s (k BIGINT, payload1 SMALLINT, payload2 FLOAT);", R_TBL_NAME);
  duckdb_query(con, createRtbl, NULL);
  sprintf(createStbl, "CREATE OR REPLACE TABLE %s (k BIGINT, payload3 BIGINT, payload4 INTEGER);", S_TBL_NAME);
  duckdb_query(con, createStbl, NULL);

  // Create the test tables.
  char R_init_query[1000 + strlen(R_TBL_NAME)];
  char S_init_query[1000 + strlen(S_TBL_NAME)];
  sprintf(
  	R_init_query,
  	"INSERT INTO %s (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
    R_TBL_NAME,
  	R_TABLE_SIZE
  );
  sprintf(
  	S_init_query,
  	"INSERT INTO %s (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
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

  mylog(logfile, "Now sorting the tables...");
  // R
  two_pass_sort_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    R_TBL_NAME,
    R_interm,
    R_SORTED_NAME,
    false,
    false,
    true
  );
  mylog(logfile, "Sorted table R.");
  // S
  two_pass_sort_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    S_TBL_NAME,
    S_interm,
    S_SORTED_NAME,
    false,
    false,
    true
  );
  mylog(logfile, "Sorted table S.");

  Inner_MergeJoin_GFUR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    EXT_PARALLELISM,
    MERGE_PARTITION_SIZE,
    RESCALE_FACTOR,
    PAYLOAD_INDEX_BLOCK,
    PAYLOAD_GATHER_BLOCK,
    logfile,
    ctx,
    con,
    R_TBL_NAME,
    S_TBL_NAME,
    R_SORTED_NAME,
    S_SORTED_NAME,
    JOIN_TMP_TBL_NAME,
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
