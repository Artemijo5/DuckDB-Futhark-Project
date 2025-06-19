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
#define BUFFER_SIZE 512*CHUNK_SIZE

#define R_TABLE_SIZE 9*CHUNK_SIZE + 12
#define S_TABLE_SIZE 32*CHUNK_SIZE

#define BLOCK_SIZE (int16_t)256
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
  	"INSERT INTO R_tbl (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
  	R_TABLE_SIZE
  );
  sprintf(
  	S_init_query,
  	"INSERT INTO S_tbl (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
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

  struct futhark_i8_1d *relInfo_ft;
  struct futhark_i64_1d *idx_ft;

  void *Rbuff = colType_malloc(DUCKDB_TYPE_BIGINT, R_TABLE_SIZE);
  void *Sbuff = colType_malloc(DUCKDB_TYPE_BIGINT, S_TABLE_SIZE);
  void *Merger = colType_malloc(DUCKDB_TYPE_BIGINT, R_TABLE_SIZE + S_TABLE_SIZE);

  // TODO why does it segfault ?!?!?!?!?!?!?!?

  mylog(logfile, "Buffering sorted R...");
  duckdb_result res_R;
  if( duckdb_query(con, "SELECT #1 FROM R_tbl_sorted;", &res_R) == DuckDBError) {
    perror("Failed to read R_tbl_sorted...\n");
    return -1;
  }
  mylog(logfile, "Obtained result...");
  idx_t R_cnk_counter = 0;
  while(true) {
    duckdb_data_chunk cnk = duckdb_fetch_chunk(res_R);
    if(!cnk) {
      mylog(logfile, "Exhausted result.");
      break;
    }
    idx_t vecsize = duckdb_data_chunk_get_size(cnk);

    duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk, 0);
    long *kdata = (long*)duckdb_vector_get_data(kvec);
    memcpy(
      Rbuff + R_cnk_counter*CHUNK_SIZE*colType_bytes(DUCKDB_TYPE_BIGINT),
      kdata,
      vecsize*colType_bytes(DUCKDB_TYPE_BIGINT)
    );
    R_cnk_counter+=1;

    duckdb_destroy_data_chunk(&cnk);
  }
  duckdb_destroy_result(&res_R);

  mylog(logfile, "Buffering sorted S...");
  duckdb_result res_S;
  if(duckdb_query(con, "SELECT #1 FROM S_tbl_sorted;", &res_S) == DuckDBError) {
    perror("Failed to read S_tbl_sorted...\n");
    return -1;
  }
  mylog(logfile, "Obtained result...");
  idx_t S_cnk_counter = 0;
  while(true) {
    duckdb_data_chunk cnk = duckdb_fetch_chunk(res_S);
    if(!cnk) {
      mylog(logfile, "Exhausted result.");
      break;
    }
    idx_t vecsize = duckdb_data_chunk_get_size(cnk);

    duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk, 0);
    long *kdata = (long*)duckdb_vector_get_data(kvec);
    memcpy(
      Sbuff + S_cnk_counter*CHUNK_SIZE*colType_bytes(DUCKDB_TYPE_BIGINT),
      kdata,
      vecsize*colType_bytes(DUCKDB_TYPE_BIGINT)
    );
    S_cnk_counter+=1;

    duckdb_destroy_data_chunk(&cnk);
  }
  duckdb_destroy_result(&res_S);

  mylog(logfile, "Beginning to merge keys of R & S...");
  mergeSortedKeys(ctx, &relInfo_ft, &idx_ft, Merger, DUCKDB_TYPE_BIGINT, 20, 100, Rbuff, Sbuff, R_TABLE_SIZE, S_TABLE_SIZE, true);
  mylog(logfile, "Finished merging sorted keys of tables R & S.");
/*
  logarray_long(logfile, "Merged key list:", Rbuff, R_TABLE_SIZE);
  logarray_long(logfile, "Merged key list:", Sbuff, S_TABLE_SIZE);
  logarray_long(logfile, "Merged key list:", (long*)Merger, R_TABLE_SIZE + S_TABLE_SIZE);
*/
  idx_t i_r = 0, i_s = 0;
  int is_right = true;
  for(idx_t j=0; j<(R_TABLE_SIZE+S_TABLE_SIZE); j++) {
    long cur = ((long*)Merger)[j];
    if(i_r < R_TABLE_SIZE && cur == ((long*)Rbuff)[i_r]) i_r ++;
    else if(i_s < S_TABLE_SIZE && cur == ((long*)Sbuff)[i_s]) i_s++;
    else {
      is_right = false;
      printf("Failure at iter: %ld\n", j);
      break;
    }
  }
  logdbg(logfile, is_right, "Merging was done correctly.", "!!!!!!! Incorrect merging!");


  // Clean-up
  free(Rbuff);
  free(Sbuff);
  free(Merger);

  futhark_free_i8_1d(ctx, relInfo_ft);
  futhark_free_i64_1d(ctx, idx_ft);
  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}
