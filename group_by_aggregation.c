#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "db_util.h"

#define LOGFILE "stdout"//"group_by_aggregation.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 512*CHUNK_SIZE
#define TABLE_SIZE 4*BUFFER_SIZE
#define NUM_KEYS (int64_t)25

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
	  FILE* logfile = loginit(LOGFILE, "Group-By Aggregation : Starting test program.");
	  if(LOGFILE && !logfile) {
	    perror("Failed to initialise logger.");
	    return -1;
	  }

	// Log Initial Parametres
	  char log_param[10000];
	  sprintf(log_param,
	    "Logging program parametres:\n"
	    "\tTABLE SIZE         %ld\n"
	    "\tBUFFER SIZE        %ld\n"
	    "\tBUFFER CAPACITY    %ld\n"
	    "\tNUM_KEYS           %ld",
	    TABLE_SIZE,BUFFER_SIZE,BUFFER_SIZE/CHUNK_SIZE,NUM_KEYS
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
		mylog(logfile, "Set up duckdb connection.");

  // Create the table tbl on which the testing will be done.
  // TODO eventually make this into a miniature leveled star schema and perform joins...
	  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl (k BIGINT, x1 DOUBLE, x2 DOUBLE);", NULL);
	  duckdb_query(con, "setseed(0.42);", NULL);

	  duckdb_prepared_statement init_stmt;
	  if (
	    duckdb_prepare(
	      con,
	      //"INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
	      "INSERT INTO tbl (SELECT ($1)*random(), ((i%10)+(i%6)-2+5*random()), (1.2*(i%10)+0.6*(i%4)-5+7*random())"
	      "FROM range($2) t(i));",
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

	  mylog(logfile, "Created test table.");

	  duckdb_query(con, "CREATE OR REPLACE TEMP TABLE sorted_tbl (k BIGINT, x1 DOUBLE, x2 DOUBLE);", NULL);
	  duckdb_query(con, "INSERT INTO sorted_tbl (FROM tbl ORDER BY k);", NULL);
	  mylog(logfile, "Created sorted version of test table.");

  // Set up futhark core
	  struct futhark_context_config *cfg = futhark_context_config_new();
	  struct futhark_context *ctx = futhark_context_new(cfg);
	  mylog(logfile, "Set up futhark context & config.");

	// Allocate Buffers, initialise parametres
	  void** Stg1_Buffer = malloc(sizeof(int64_t*) + 2*sizeof(double*));
	  void** Stg2_Buffer = malloc(sizeof(int64_t*) + 3*sizeof(double*));
	  void** Res_Buffer = malloc(sizeof(int64_t*) + 3*sizeof(double*));

	  Stg1_Buffer[0] = malloc(BUFFER_SIZE*sizeof(int64_t));
	  Stg1_Buffer[1] = malloc(BUFFER_SIZE*sizeof(double));
	  Stg1_Buffer[2] = malloc(BUFFER_SIZE*sizeof(double));

	  Stg2_Buffer[0] = malloc(NUM_KEYS*sizeof(int64_t)); // count k
	  Stg2_Buffer[1] = malloc(NUM_KEYS*sizeof(double)); // sum x1
	  Stg2_Buffer[2] = malloc(NUM_KEYS*sizeof(double)); // sum x2
	  Stg2_Buffer[3] = malloc(NUM_KEYS*sizeof(double)); // sum x1*x2

	  Res_Buffer[0] = malloc(NUM_KEYS*sizeof(int64_t)); // count k
	  Res_Buffer[1] = malloc(NUM_KEYS*sizeof(double)); // sum x1
	  Res_Buffer[2] = malloc(NUM_KEYS*sizeof(double)); // sum x2
	  Res_Buffer[3] = malloc(NUM_KEYS*sizeof(double)); // sum x1*x2

	  int64_t *result_count = (int64_t*)Res_Buffer[0];
	  double *result_sum_x1 = (double*)Res_Buffer[1];
	  double *result_sum_x2 = (double*)Res_Buffer[2];
	  double *result_sum_x1x2 = (double*)Res_Buffer[3];

	  duckdb_type colTypes[3] = {DUCKDB_TYPE_BIGINT, DUCKDB_TYPE_DOUBLE, DUCKDB_TYPE_DOUBLE};

	  int64_t knownKeys[NUM_KEYS];
	  for(idx_t i=0; i<NUM_KEYS; i++) {
	  	knownKeys[i] = i;
	  }
	  struct futhark_i64_1d *knownKeys_ft = futhark_new_i64_1d(ctx, knownKeys, NUM_KEYS);

	  // Reset these right before each experiment!
	  int isTblExhausted = false;
	  /*
	  for(idx_t j=0; j<NUM_KEYS; j++) {
	  	result_count[j] = 0;
	  	result_sum_x1[j] = 0;
	  	result_sum_x2[j] = 0;
	  	result_sum_x1x2[j] = 0;
	  }
	  */

  // 1 - test aggregation without sorting
	  mylog(logfile, "TEST 1 ----- collecting aggregates without sorting (via parallel hist operator).");
		  duckdb_result res;
		  if (duckdb_query(con, "SELECT * FROM tbl;", &res) == DuckDBError) {
		  	perror("Failed to obtain result from tbl.");
		  	return -1;
		  }
		  mylog(logfile, "Performed duckdb query to read table.");
		  isTblExhausted = false;
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	result_count[j] = 0;
		  	result_sum_x1[j] = 0;
		  	result_sum_x2[j] = 0;
		  	result_sum_x1x2[j] = 0;
		  }
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

			  	futhark_values_i64_1d(ctx, counts, Stg2_Buffer[0]);
			  	futhark_free_i64_1d(ctx, counts);
			  	futhark_values_f64_1d(ctx, sum_x1, Stg2_Buffer[1]);
			  	futhark_free_f64_1d(ctx, sum_x1);
			  	futhark_values_f64_1d(ctx, sum_x2, Stg2_Buffer[2]);
			  	futhark_free_f64_1d(ctx, sum_x2);
			  	futhark_values_f64_1d(ctx, sum_x1x2, Stg2_Buffer[3]);
			  	futhark_free_f64_1d(ctx, sum_x1x2);
			  	mylog(logfile, "Unwrapped aggregates.");

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

			  	for(idx_t j=0; j<NUM_KEYS; j++) {
			  		result_count[j] += ((long*)Stg2_Buffer[0])[j];
			  		result_sum_x1[j] += ((double*)Stg2_Buffer[1])[j];
			  		result_sum_x2[j] += ((double*)Stg2_Buffer[2])[j];
			  		result_sum_x1x2[j] += ((double*)Stg2_Buffer[3])[j];
			  	}
			  	mylog(logfile, "Reduced final aggregates.");
				}
		  }
		  mylog(logfile, "TEST 1 COMPLETE! ----------------------------------------------------");
		  duckdb_destroy_result(&res);
		  mylog(logfile, "Destroyed duckdb result.");
		  
		  printf("Printing aggregate values...\n");
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	long thisKey = knownKeys[j];
		  	long thisCount = result_count[j];
		  	thisCount = (thisCount==0)? 1: thisCount;
		  	double thisX1 = result_sum_x1[j];
		  	double thisX2 = result_sum_x2[j];
		  	double thisX1X2 = result_sum_x1x2[j];
		  	double thisCov = ( thisX1X2 - ((thisX1*thisX2)/((double)thisCount)) ) / ( (double)thisCount );
		  	/*
		  	printf("ID %4ld | COUNT %8ld | SUM1 %+15.3f | SUM2 %+15.3f | SUM* %+15.3f | COV %+8.3f\n",
		  		thisKey, thisCount, thisX1, thisX2, thisX1X2, thisCov);
		  	*/
		  	printf("ID %4ld | COUNT %8ld | COV %+8.3f\n",
		  		thisKey, thisCount, thisCov);
		  }
		  printf("\n");

  // 2 - test aggregation with sorting
	  mylog(logfile, "TEST 2 ----- collecting aggregates after sorting (via parallel hist operator).");
		  duckdb_result sort_res;
		  if (duckdb_query(con, "SELECT * FROM sorted_tbl;", &sort_res) == DuckDBError) {
		  	perror("Failed to obtain result from tbl.");
		  	return -1;
		  }
		  mylog(logfile, "Performed duckdb query to sort and read table.");
		  isTblExhausted = false;
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	result_count[j] = 0;
		  	result_sum_x1[j] = 0;
		  	result_sum_x2[j] = 0;
		  	result_sum_x1x2[j] = 0;
		  }
		  while(!isTblExhausted) {
		  	idx_t curRows = bulk_load_chunks(
					CHUNK_SIZE,
					sort_res,
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

			  	struct futhark_opaque_sortgroupInfo *sg;
			  	struct futhark_i64_1d *counts;
			  	struct futhark_f64_1d *sum_x1;
			  	struct futhark_f64_1d *sum_x2;
			  	struct futhark_f64_1d *sum_x1x2;

			  	futhark_entry_sortgroup_find_known_key_counts_long(ctx, &sg, knownKeys_ft, ks_ft);
			  	futhark_project_opaque_sortgroupInfo_group_count(ctx, &counts, sg);
			  	futhark_entry_double_sum(ctx, &sum_x1, NUM_KEYS, ks_ft, x1_ft);
			  	futhark_entry_double_sum(ctx, &sum_x2, NUM_KEYS, ks_ft, x2_ft);
			  	futhark_entry_double_sum_xy(ctx, &sum_x1x2, NUM_KEYS, ks_ft, x1_ft, x2_ft);
			  	mylog(logfile, "Computed this buffer's aggregates.");

			  	futhark_context_sync(ctx);
			  	mylog(logfile, "Synced futhark context.");

			  	futhark_free_opaque_sortgroupInfo(ctx, sg);
			  	futhark_free_i64_1d(ctx, ks_ft);
			  	futhark_free_f64_1d(ctx, x1_ft);
			  	futhark_free_f64_1d(ctx, x2_ft);

			  	futhark_values_i64_1d(ctx, counts, Stg2_Buffer[0]);
			  	futhark_free_i64_1d(ctx, counts);
			  	futhark_values_f64_1d(ctx, sum_x1, Stg2_Buffer[1]);
			  	futhark_free_f64_1d(ctx, sum_x1);
			  	futhark_values_f64_1d(ctx, sum_x2, Stg2_Buffer[2]);
			  	futhark_free_f64_1d(ctx, sum_x2);
			  	futhark_values_f64_1d(ctx, sum_x1x2, Stg2_Buffer[3]);
			  	futhark_free_f64_1d(ctx, sum_x1x2);
			  	mylog(logfile, "Unwrapped aggregates.");

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

			  	for(idx_t j=0; j<NUM_KEYS; j++) {
			  		result_count[j] += ((long*)Stg2_Buffer[0])[j];
			  		result_sum_x1[j] += ((double*)Stg2_Buffer[1])[j];
			  		result_sum_x2[j] += ((double*)Stg2_Buffer[2])[j];
			  		result_sum_x1x2[j] += ((double*)Stg2_Buffer[3])[j];
			  	}
			  	mylog(logfile, "Reduced final aggregates.");
				}
		  }
		  mylog(logfile, "TEST 2 COMPLETE! ----------------------------------------------------");
		  duckdb_destroy_result(&sort_res);
		  mylog(logfile, "Destroyed duckdb result.");
		  
		  printf("Printing aggregate values...\n");
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	long thisKey = knownKeys[j];
		  	long thisCount = result_count[j];
		  	thisCount = (thisCount==0)? 1: thisCount;
		  	double thisX1 = result_sum_x1[j];
		  	double thisX2 = result_sum_x2[j];
		  	double thisX1X2 = result_sum_x1x2[j];
		  	double thisCov = ( thisX1X2 - ((thisX1*thisX2)/((double)thisCount)) ) / ( (double)thisCount );
		  	/*
		  	printf("ID %4ld | COUNT %8ld | SUM1 %+15.3f | SUM2 %+15.3f | SUM* %+15.3f | COV %+8.3f\n",
		  		thisKey, thisCount, thisX1, thisX2, thisX1X2, thisCov);
		  	*/
		  	printf("ID %4ld | COUNT %8ld | COV %+8.3f\n",
		  		thisKey, thisCount, thisCov);
		  }
		  printf("\n");
  
	// 3 - test alt aggregation with sorting
	  mylog(logfile, "TEST 3 ----- collecting aggregates after sorting (via nested map-reduce).");
		  duckdb_result alt_res;
		  if (duckdb_query(con, "SELECT * FROM sorted_tbl;", &alt_res) == DuckDBError) {
		  	perror("Failed to obtain result from tbl.");
		  	return -1;
		  }
		  mylog(logfile, "Performed duckdb query to sort and read table.");
		  isTblExhausted = false;
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	result_count[j] = 0;
		  	result_sum_x1[j] = 0;
		  	result_sum_x2[j] = 0;
		  	result_sum_x1x2[j] = 0;
		  }
		  while(!isTblExhausted) {
		  	idx_t curRows = bulk_load_chunks(
					CHUNK_SIZE,
					alt_res,
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

			  	struct futhark_opaque_sortgroupInfo *sg;
			  	struct futhark_i64_1d *counts;
			  	struct futhark_i64_1d *k_idxs;
			  	struct futhark_f64_1d *sum_x1;
			  	struct futhark_f64_1d *sum_x2;
			  	struct futhark_f64_1d *sum_x1x2;

			  	futhark_entry_sortgroup_find_unknown_key_counts_long(ctx, &sg, ks_ft);
			  	futhark_project_opaque_sortgroupInfo_group_count(ctx, &counts, sg);
			  	futhark_project_opaque_sortgroupInfo_group_idx(ctx, &k_idxs, sg);
			  	futhark_entry_alt_double_sum(ctx, &sum_x1, k_idxs, counts, x1_ft);
			  	futhark_entry_alt_double_sum(ctx, &sum_x2, k_idxs, counts, x2_ft);
			  	futhark_entry_alt_double_sum_xy(ctx, &sum_x1x2, k_idxs, counts, x1_ft, x2_ft);
			  	mylog(logfile, "Computed this buffer's aggregates (keys currently unknown).");

			  	futhark_context_sync(ctx);
			  	mylog(logfile, "Synced futhark context.");

			  	futhark_free_opaque_sortgroupInfo(ctx, sg);
			  	futhark_free_i64_1d(ctx, ks_ft);
			  	futhark_free_f64_1d(ctx, x1_ft);
			  	futhark_free_f64_1d(ctx, x2_ft);

			  	// Get the unknown keys to their respective slots
			  	int64_t uk_keysCount = (futhark_shape_i64_1d(ctx, k_idxs))[0];
			  	int64_t *uk_keys = malloc(uk_keysCount*sizeof(int64_t));

				  futhark_values_i64_1d(ctx, k_idxs, uk_keys);
				  futhark_free_i64_1d(ctx, k_idxs);
				  for(idx_t i=0; i<uk_keysCount; i++) {
				  	idx_t accIdx = uk_keys[i];
				  	uk_keys[i] = ((long*)Stg1_Buffer[0])[accIdx];
				  }
				  mylog(logfile, "Gathered key slots that current aggregates correspond to.");

			  	futhark_values_i64_1d(ctx, counts, Stg2_Buffer[0]);
			  	futhark_free_i64_1d(ctx, counts);
			  	futhark_values_f64_1d(ctx, sum_x1, Stg2_Buffer[1]);
			  	futhark_free_f64_1d(ctx, sum_x1);
			  	futhark_values_f64_1d(ctx, sum_x2, Stg2_Buffer[2]);
			  	futhark_free_f64_1d(ctx, sum_x2);
			  	futhark_values_f64_1d(ctx, sum_x1x2, Stg2_Buffer[3]);
			  	futhark_free_f64_1d(ctx, sum_x1x2);
			  	mylog(logfile, "Unwrapped aggregates.");

			  	for(idx_t j=0; j<uk_keysCount; j++) {
			  		idx_t accIdx = uk_keys[j];
			  		result_count[accIdx] += ((long*)Stg2_Buffer[0])[j];
			  		result_sum_x1[accIdx] += ((double*)Stg2_Buffer[1])[j];
			  		result_sum_x2[accIdx] += ((double*)Stg2_Buffer[2])[j];
			  		result_sum_x1x2[accIdx] += ((double*)Stg2_Buffer[3])[j];
			  	}
			  	free(uk_keys);
			  	mylog(logfile, "Reduced final aggregates.");
				}
		  }
		  mylog(logfile, "TEST 3 COMPLETE! ----------------------------------------------------");
		  duckdb_destroy_result(&alt_res);
		  mylog(logfile, "Destroyed duckdb result.");
		  
		  printf("Printing aggregate values...\n");
		  for(idx_t j=0; j<NUM_KEYS; j++) {
		  	long thisKey = knownKeys[j];
		  	long thisCount = result_count[j];
		  	thisCount = (thisCount==0)? 1: thisCount;
		  	double thisX1 = result_sum_x1[j];
		  	double thisX2 = result_sum_x2[j];
		  	double thisX1X2 = result_sum_x1x2[j];
		  	double thisCov = ( thisX1X2 - ((thisX1*thisX2)/((double)thisCount)) ) / ( (double)thisCount );
		  	/*
		  	printf("ID %4ld | COUNT %8ld | SUM1 %+15.3f | SUM2 %+15.3f | SUM* %+15.3f | COV %+8.3f\n",
		  		thisKey, thisCount, thisX1, thisX2, thisX1X2, thisCov);
		  	*/
		  	printf("ID %4ld | COUNT %8ld | COV %+8.3f\n",
		  		thisKey, thisCount, thisCov);
		  }
		  printf("\n");  

  // Clean-up
	  futhark_free_i64_1d(ctx, knownKeys_ft);
	  for(idx_t i=0; i<3; i++) {
	  	free(Stg1_Buffer[i]);
	  }
	  free(Stg1_Buffer);
	  for(idx_t i=0; i<4; i++) {
	  	free(Stg2_Buffer[i]);
	  	free(Res_Buffer[i]);
	  }
	  free(Res_Buffer);
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
