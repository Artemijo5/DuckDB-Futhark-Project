#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "db_util.h"

#define LOGFILE "group_by_aggregation.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 512*CHUNK_SIZE
#define TABLE_SIZE 4*BUFFER_SIZE
#define NUM_KEYS 32 // try various values here...
// TODO results for ID 0 are buggy, especially for NUM_KEYS > 5 (...)
// for 32+ keys it also (mostly) gets it right... even for 512 keys...
// for some strange reason, for some interval over 5 keys, results for 0 and sometimes other keys bug out...
// could it be a bug with the futhark C compiler? otherwise some stupid mistake somehow...

#define STG2_BUFFER_SIZE 5*NUM_KEYS

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
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

  // Create the table tbl on which the testing will be done.
  // TODO eventually make this into a miniature leveled star schema and perform joins...
  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl (k BIGINT, x1 DOUBLE, x2 DOUBLE);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (
    duckdb_prepare(
      con,
      //"INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
      "INSERT INTO tbl (SELECT ($1)*random(), 6 + 64*random(), 100*random() FROM range($2) t(i));",
      &init_stmt
    ) 
    == DuckDBError
  )
  {
    perror("Failed to initialise the table.");
    return 1;
  }
  duckdb_bind_int32(init_stmt, 1, NUM_KEYS-1);
  duckdb_bind_int32(init_stmt, 2, TABLE_SIZE);
  duckdb_execute_prepared(init_stmt, NULL);
  duckdb_destroy_prepare(&init_stmt);

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  void** Stg1_Buffer = malloc(sizeof(int64_t*) + 2*sizeof(double*));
  void** Stg2_Buffer = malloc(2*sizeof(int64_t*) + 3*sizeof(double*));

  Stg1_Buffer[0] = malloc(BUFFER_SIZE*sizeof(int64_t));
  Stg1_Buffer[1] = malloc(BUFFER_SIZE*sizeof(double));
  Stg1_Buffer[2] = malloc(BUFFER_SIZE*sizeof(double));

  Stg2_Buffer[0] = malloc(STG2_BUFFER_SIZE*sizeof(int64_t)); // k
  Stg2_Buffer[1] = malloc(STG2_BUFFER_SIZE*sizeof(int64_t)); // count k
  Stg2_Buffer[2] = malloc(STG2_BUFFER_SIZE*sizeof(double)); // sum x1
  Stg2_Buffer[3] = malloc(STG2_BUFFER_SIZE*sizeof(double)); // sum x2
  Stg2_Buffer[4] = malloc(STG2_BUFFER_SIZE*sizeof(double)); // sum x1*x2

  duckdb_type colTypes[3] = {DUCKDB_TYPE_BIGINT, DUCKDB_TYPE_DOUBLE, DUCKDB_TYPE_DOUBLE};

  int64_t knownKeys[NUM_KEYS];
  for(idx_t i=0; i<NUM_KEYS; i++) {
  	knownKeys[i] = i;
  }
  struct futhark_i64_1d *knownKeys_ft = futhark_new_i64_1d(ctx, knownKeys, NUM_KEYS);

  // Reset these right before each experiment!
  int isTblExhausted = false;
  idx_t curIdx_buff2 = 0;

  // 1 - test aggregation without sorting
  mylog(logfile, "TEST 1 ----- collecting aggregates without sorting (via parallel hist operator).");
  duckdb_result res;
  if (duckdb_query(con, "SELECT * FROM tbl;", &res) == DuckDBError) {
  	perror("Failed to obtain result from tbl.");
  	return -1;
  }
  mylog(logfile, "Performed duckdb query to read table.");
  isTblExhausted = false;
  curIdx_buff2 = 0;
  while(!isTblExhausted) {
  	idx_t curRows = bulk_load_chunks(
		CHUNK_SIZE,
		res,
		60,
		3,
		-1,
		colTypes,
		Stg1_Buffer,
		NULL,
		BUFFER_SIZE,
		&isTblExhausted
	);
  	mylog(logfile, "Read a buffer.");

  	if(curRows>0) {
	  	struct futhark_i64_1d *ks_ft = futhark_new_i64_1d(ctx, Stg1_Buffer[0], curRows);
	  	struct futhark_f64_1d *x1_ft = futhark_new_f64_1d(ctx, Stg1_Buffer[1], curRows);
	  	struct futhark_f64_1d *x2_ft = futhark_new_f64_1d(ctx, Stg1_Buffer[2], curRows);
	  	mylog(logfile, "Wrapped this buffer's values in the futhark core.");

	  	struct futhark_i64_1d *counts;
	  	struct futhark_f64_1d *sum_x1;
	  	struct futhark_f64_1d *sum_x2;
	  	struct futhark_f64_1d *sum_x1x2;

	  	futhark_entry_find_known_key_counts_long(ctx, &counts, NUM_KEYS, ks_ft);
	  	futhark_entry_double_sum(ctx, &sum_x1, NUM_KEYS, ks_ft, x1_ft);
	  	futhark_entry_double_sum(ctx, &sum_x2, NUM_KEYS, ks_ft, x2_ft);
	  	futhark_entry_double_sum_xy(ctx, &sum_x1x2, NUM_KEYS, ks_ft, x1_ft, x2_ft);
	  	mylog(logfile, "Computed this buffer's aggregates.");

	  	futhark_context_sync(ctx);
	  	mylog(logfile, "Synced futhark context.");

	  	futhark_free_i64_1d(ctx, ks_ft);
	  	futhark_free_f64_1d(ctx, x1_ft);
	  	futhark_free_f64_1d(ctx, x2_ft);

	  	memcpy(Stg2_Buffer[0] + curIdx_buff2*sizeof(int64_t), knownKeys, NUM_KEYS*sizeof(int64_t));
	  	futhark_values_i64_1d(ctx, counts, Stg2_Buffer[1] + curIdx_buff2*sizeof(int64_t));
	  	futhark_free_i64_1d(ctx, counts);
	  	futhark_values_f64_1d(ctx, sum_x1, Stg2_Buffer[2] + curIdx_buff2*sizeof(double));
	  	futhark_free_f64_1d(ctx, sum_x1);
	  	futhark_values_f64_1d(ctx, sum_x2, Stg2_Buffer[3] + curIdx_buff2*sizeof(double));
	  	futhark_free_f64_1d(ctx, sum_x2);
	  	futhark_values_f64_1d(ctx, sum_x1x2, Stg2_Buffer[4] + curIdx_buff2*sizeof(double));
	  	futhark_free_f64_1d(ctx, sum_x1x2);
	  	mylog(logfile, "Saved cur rows to stg2 buffer.");

	  	/*
	  	for(idx_t i=0; i<NUM_KEYS; i++) {
	  		printf("ID %5ld | COUNT %5ld | X1 %8.3f | X2 %8.3f | X1X2 %8.3f\n",
	  			((long*)Stg2_Buffer[0])[i+curIdx_buff2],
	  			((long*)Stg2_Buffer[1])[i+curIdx_buff2],
	  			((double*)Stg2_Buffer[2])[i+curIdx_buff2],
	  			((double*)Stg2_Buffer[3])[i+curIdx_buff2],
	  			((double*)Stg2_Buffer[4])[i+curIdx_buff2]
	  		);
	  	}
	  	*/
	}
	curIdx_buff2 += NUM_KEYS;

	if((STG2_BUFFER_SIZE - curIdx_buff2 < NUM_KEYS) || isTblExhausted) {
		mylog(logfile, "Stage 2 of aggregation...");
		struct futhark_i64_1d *s1_ks_ft = futhark_new_i64_1d(ctx, Stg2_Buffer[0], curIdx_buff2);
	  	struct futhark_i64_1d *s1_counts = futhark_new_i64_1d(ctx, Stg2_Buffer[1], curIdx_buff2);
	  	struct futhark_f64_1d *s1_sum_x1 = futhark_new_f64_1d(ctx, Stg2_Buffer[2], curIdx_buff2);
	  	struct futhark_f64_1d *s1_sum_x2 = futhark_new_f64_1d(ctx, Stg2_Buffer[3], curIdx_buff2);
	  	struct futhark_f64_1d *s1_sum_x1x2 = futhark_new_f64_1d(ctx, Stg2_Buffer[4], curIdx_buff2);
	  	mylog(logfile, "--- Wrapped this buffer's values in the futhark core.");

	  	struct futhark_i64_1d *s2_counts;
	  	struct futhark_f64_1d *s2_sum_x1;
	  	struct futhark_f64_1d *s2_sum_x2;
	  	struct futhark_f64_1d *s2_sum_x1x2;

	  	futhark_entry_long_sum(ctx, &s2_counts, NUM_KEYS, s1_ks_ft, s1_counts);
	  	futhark_entry_double_sum(ctx, &s2_sum_x1, NUM_KEYS, s1_ks_ft, s1_sum_x1);
	  	futhark_entry_double_sum(ctx, &s2_sum_x2, NUM_KEYS, s1_ks_ft, s1_sum_x2);
	  	futhark_entry_double_sum(ctx, &s2_sum_x1x2, NUM_KEYS, s1_ks_ft, s1_sum_x1x2);
	  	mylog(logfile, "--- Computed this buffer's aggregates.");

	  	futhark_context_sync(ctx);
	  	mylog(logfile, "--- Synced futhark context.");

	  	futhark_free_i64_1d(ctx, s1_ks_ft);
	  	futhark_free_i64_1d(ctx, s1_counts);
	  	futhark_free_f64_1d(ctx, s1_sum_x1);
	  	futhark_free_f64_1d(ctx, s1_sum_x2);
	  	futhark_free_f64_1d(ctx, s1_sum_x1x2);

	  	/*
	  	long count0, count1;
	  	futhark_index_i64_1d(ctx, &count0, s2_counts, 0);
	  	futhark_index_i64_1d(ctx, &count1, s2_counts, 1);
	  	printf("... %ld %ld\n", count0, count1);
	  	*/

	  	memcpy(Stg2_Buffer[0], knownKeys, NUM_KEYS*sizeof(int64_t));
	  	futhark_values_i64_1d(ctx, s2_counts, Stg2_Buffer[1]);
	  	futhark_free_i64_1d(ctx, s2_counts);
	  	futhark_values_f64_1d(ctx, s2_sum_x1, Stg2_Buffer[2]);
	  	futhark_free_f64_1d(ctx, s2_sum_x1);
	  	futhark_values_f64_1d(ctx, s2_sum_x2, Stg2_Buffer[3]);
	  	futhark_free_f64_1d(ctx, s2_sum_x2);
	  	futhark_values_f64_1d(ctx, s2_sum_x1x2, Stg2_Buffer[4]);
	  	futhark_free_f64_1d(ctx, s2_sum_x1x2);
	  	mylog(logfile, "--- Reduced stg2 buffer.");

	  	curIdx_buff2 = NUM_KEYS;

	  	/*
	  	for(idx_t i=0; i<NUM_KEYS; i++) {
	  		printf("ID %5ld | COUNT %5ld | X1 %8.3f | X2 %8.3f | X1X2 %8.3f\n",
	  			((long*)Stg2_Buffer[0])[i],
	  			((long*)Stg2_Buffer[1])[i],
	  			((double*)Stg2_Buffer[2])[i],
	  			((double*)Stg2_Buffer[3])[i],
	  			((double*)Stg2_Buffer[4])[i]
	  		);
	  	}
	  	*/
	}
  }
  duckdb_destroy_result(&res);
  mylog(logfile, "TEST 1 COMPLETE! ----------------------------------------------------");

  printf("Printing aggregate values...\n");
  for(idx_t j=0; j<NUM_KEYS; j++) {
  	long thisKey = ((long*)Stg2_Buffer[0])[j];
  	long thisCount = ((long*)Stg2_Buffer[1])[j];
  	thisCount = (thisCount==0)? 1: thisCount;
  	double thisX1 = ((double*)Stg2_Buffer[2])[j];
  	double thisX2 = ((double*)Stg2_Buffer[3])[j];
  	double thisX1X2 = ((double*)Stg2_Buffer[4])[j];
  	double thisCov = ( thisX1X2 - ((thisX1*thisX2)/((double)thisCount)) ) / ( (double)thisCount );
  	printf("ID %4ld | COUNT %10ld | COV %+8.3f\n", thisKey, thisCount, thisCov);
  }
  

  // Clean-up
  futhark_free_i64_1d(ctx, knownKeys_ft);
  for(idx_t i=0; i<3; i++) {
  	free(Stg1_Buffer[i]);
  }
  free(Stg1_Buffer);
  for(idx_t i=0; i<5; i++) {
  	free(Stg2_Buffer[i]);
  }
  free(Stg2_Buffer);

  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}
