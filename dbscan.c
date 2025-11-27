#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "duckdb.h"

#include "ftDBSCAN.h"
#include "mylogger.h"
#include "db_util.h"

#define LOGFILE "stdout"//"dbscan.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define CNK_TO_READ (long)64
#define BUFFER_CAP (long)4
#define TABLE_SIZE BUFFER_CAP*CHUNK_SIZE // assume entire dataset can be processed at once

#define DIM (long)2
#define MINVAL (float)0.0
#define MAXVAL (float)250.0

#define EPS (float)2.0
#define MIN_PTS 2 + DIM

#define GATHER_PSIZE 1*sizeof(long)*TABLE_SIZE
#define DBSCAN_MEMSIZE 100*TABLE_SIZE
#define OMIT_BORDER true

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
    FILE* logfile = loginit(LOGFILE, "DBSCAN : Starting test program.");
    if(LOGFILE && !logfile) {
      perror("Failed to initialise logger.\n");
      return -1;
    }

  // Log program parametres
  char log_param[10000];
  sprintf(log_param,
    "Logging program parametres:\n"
    "\tTABLE SIZE           %ld\n"
    "\tTABLE CHUNKS         %ld\n"
    "\tDATA DIMENSIONS      %ld\n"
    "\tepsilon              %f\n"
    "\tminPts               %ld\n"
    "\tGATHER PSIZE         %ld\n"
    "\tDBSCAN MEMSIZE       %ld\n"
    "\tOMIT BORDER POINTS   %d\n",
    TABLE_SIZE,BUFFER_CAP,DIM,EPS,MIN_PTS,GATHER_PSIZE,DBSCAN_MEMSIZE,OMIT_BORDER
  );
  mylog(logfile, log_param);

  // DuckDB initialisation
    duckdb_database db;
  	duckdb_connection con;
    duckdb_config config;

    // set config
    if (duckdb_create_config(&config) == DuckDBError) {
      perror("Failed to create config.\n");
      return -1;
    }
    duckdb_set_config(config, "max_memory", DDB_MEMSIZE);
    duckdb_set_config(config, "memory_limit", DDB_MEMSIZE);
    duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
  	//duckdb_open(NULL, &db);
    if(duckdb_open_ext(DBFILE, &db, config, NULL) == DuckDBError) {
      perror("Failed to open database with configuration options.\n");
      return -1;
    }
    duckdb_destroy_config(&config);

  	duckdb_connect(db, &con);
  	mylog(logfile, "Set up duckdb connection.");

  // Create the table tbl on which the testing will be done.
    char queryStr[150 + 20*DIM];
    int queryLen = sprintf(queryStr, "CREATE OR REPLACE TABLE dbcTbl (");
    for(idx_t i=0; i<DIM; i++) {
    	queryLen += sprintf(queryStr+queryLen, "x%ld FLOAT", i+1);
    	if(i<DIM-1) queryLen += sprintf(queryStr+queryLen, ", ");
    }
    queryLen += sprintf(queryStr+queryLen, ");");
    if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
    	perror("Failed to create initial table.\n");
    	perror(queryStr);
    	return -1;
    }

    duckdb_query(con, "setseed(0.42);", NULL);

    // Insert random data into the table
    char insertQueryStr[150 + 100*DIM];
    int insertQueryLen = sprintf(insertQueryStr, "INSERT INTO dbcTbl (SELECT ");
    for(idx_t i=0; i<DIM; i++) {
    	insertQueryLen += sprintf(insertQueryStr+insertQueryLen, "%f*random()-%f", MAXVAL-MINVAL, MINVAL);
    	if(i<DIM-1) insertQueryLen += sprintf(insertQueryStr+insertQueryLen, ", ");
    }
    insertQueryLen += sprintf(insertQueryStr+insertQueryLen, " FROM RANGE(%ld) t(i));", TABLE_SIZE);
    if( duckdb_query(con, insertQueryStr, NULL) == DuckDBError ) {
    	perror("Failed to insert data into the initial table.\n");
    	perror(insertQueryStr);
    	return -1;
    }

    mylog(logfile, "Created test table 'dbcTbl'.");

  // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    mylog(logfile, "Set up futhark context & config.");


  // Set some necessary parameters
    duckdb_type type_ids[DIM];
    int64_t byteSizes[DIM];
    int64_t prefixSizes[DIM];
    for(idx_t i=0; i<DIM; i++) {
    	type_ids[i] = DUCKDB_TYPE_FLOAT;
    	byteSizes[i] = sizeof(float);
    	prefixSizes[i] = i*sizeof(float);
    }

  // 1 - Read the dataset and perform DBSCAN
    duckdb_result res;
    duckdb_query(con, "SELECT * FROM dbcTbl;", &res);
    mylog(logfile, "Performed query to read dbcTbl.");

	idx_t chunks_read = 0;
	duckdb_data_chunk dbcChunks[BUFFER_CAP];
	for(idx_t i=0; i<BUFFER_CAP; i+=CNK_TO_READ) {
		idx_t cur_cnk_to_read = ((BUFFER_CAP-i) >= CNK_TO_READ)? CNK_TO_READ: (BUFFER_CAP-i);
	    idx_t cur_chunks_read = bulk_fetch_chunks(res, cur_cnk_to_read, dbcChunks + chunks_read);
	    chunks_read += cur_chunks_read;
	    if (cur_chunks_read==0) break;
	}
	logdbg(logfile, chunks_read>0, " - - Obtained dataset as chunks.", " - - Result is exhausted.");


	int64_t dbcResult_len = 0;
	struct futhark_f32_2d *dbcResult_pts;
	struct futhark_i64_1d *dbcResult_ids;
	if(chunks_read>0) {
	    idx_t cur_rows = 0;
	    char* dbcData = malloc(DIM*sizeof(float)*TABLE_SIZE);

    	for(idx_t j=0; j<chunks_read; j++) {
      		idx_t cnk_size = duckdb_data_chunk_get_size(dbcChunks[j]);
      		void *cnk_dat[DIM];
      		for(idx_t col=0; col<DIM; col++) {
      			duckdb_vector vec = duckdb_data_chunk_get_vector(dbcChunks[j], col);
      			cnk_dat[col] = duckdb_vector_get_data(vec);
	      	}
	      	payloadColumnsToByteArray_preallocated(
	    		  dbcData+cur_rows*DIM*sizeof(float),
	    		  DIM*sizeof(float),
	    		  byteSizes,
	    		  prefixSizes,
	    		  cnk_dat,
	    		  DIM,
	    		  cnk_size
	    	);
	    	cur_rows += cnk_size;
	    	duckdb_destroy_data_chunk(&(dbcChunks[j]));
    	}
    	mylog(logfile, " - - Buffered data from chunks.");

		struct futhark_f32_2d *dbcData_ft = futhark_new_f32_2d(ctx, (float*)dbcData, cur_rows, DIM);
	    mylog(logfile, " - - Wrapped data into futhark core.");
	    free(dbcData);

	    if(!OMIT_BORDER) {
	    	mylog(logfile, " - - Performing gpu-parallel DBSCAN algorithm...");
	    	dbcResult_len = cur_rows;
	    	dbcResult_pts = dbcData_ft;
	    	futhark_entry_ftDBSCAN_float(ctx, &dbcResult_ids, dbcData_ft, EPS, MIN_PTS, DBSCAN_MEMSIZE, GATHER_PSIZE);
	    	mylog(logfile, " - - DBSCAN has returned.");
	    }
	    else {
	    	mylog(logfile, " - - Performing gpu-parallel DBSCAN* algorithm...");
	    	struct futhark_opaque_core_cluster_float *DBSCAN_star_result;
	    	futhark_entry_ftDBSCAN_star_float(ctx, &DBSCAN_star_result, dbcData_ft, EPS, MIN_PTS, DBSCAN_MEMSIZE, GATHER_PSIZE);
	    	futhark_free_f32_2d(ctx, dbcData_ft);
	    	mylog(logfile, " - - DBSCAN* has returned.");
	    	futhark_context_sync(ctx);
	    	mylog(logfile, " - - Synced futhark context.");
	    	futhark_project_opaque_core_cluster_float_len(ctx, &dbcResult_len, DBSCAN_star_result);
	    	futhark_project_opaque_core_cluster_float_core_pts(ctx, &dbcResult_pts, DBSCAN_star_result);
	    	futhark_project_opaque_core_cluster_float_core_ids(ctx, &dbcResult_ids, DBSCAN_star_result);
	    	futhark_free_opaque_core_cluster_float(ctx, DBSCAN_star_result);
	    	mylog(logfile, " - - Projected fields of opaque core_cluster type.");
	    }
	}


  //char* ftErr = futhark_context_get_error(ctx);
  //printf("%s\n", ftErr);

  // 2 - Unwrap result
	float *result_pts = malloc(dbcResult_len*sizeof(float)*DIM);
	int64_t *result_ids = malloc(dbcResult_len*sizeof(int64_t));
	mylog(logfile, "Allocated memory for final result tuples.");

	futhark_context_sync(ctx);
	mylog(logfile, "Synced futhark context.");

	futhark_values_f32_2d(ctx, dbcResult_pts, result_pts);
	futhark_free_f32_2d(ctx, dbcResult_pts);
	futhark_values_i64_1d(ctx, dbcResult_ids, result_ids);
	futhark_free_i64_1d(ctx, dbcResult_ids);
	mylog(logfile, "Unwrapped from futhark context.");

	void *resultPts_cols[DIM];
	payloadColumnsFromByteArray(
      resultPts_cols,
      type_ids,
      (void*)result_pts,
      DIM,
      dbcResult_len
    );
    mylog(logfile, "Parsed byte array to separate float arrays for each column.");
    free(result_pts);

  // 3 - Store Results to Database
    const char *resTblName = (OMIT_BORDER)? "dbcStarResultTbl": "dbcResultTbl";

	mylog(logfile, "Now to store results in duckdb database...");
    char finalQueryStr[300 + 20*DIM];
    int finalQueryLen = sprintf(finalQueryStr, "CREATE OR REPLACE TABLE %s (", resTblName);
    for(idx_t i=0; i<DIM; i++) {
      finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "x%ld FLOAT, ", i+1);
    }
    finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "cluster_id BIGINT);");
    if( duckdb_query(con, finalQueryStr, NULL) == DuckDBError ) {
      perror("Failed to create final result table.\n");
      perror(finalQueryStr);
      return -1;
    }

    duckdb_appender dbc_appender;
    if( duckdb_appender_create(con, NULL, resTblName, &dbc_appender) == DuckDBError ) {
      perror("Failed to create appender.\n");
      return -1;
    }
    mylog(logfile, "Created appender.");

    // Store the results chunk-by-chunk
    duckdb_logical_type ltype_ids[DIM+1];
    for(idx_t i=0; i<DIM; i++) {
      ltype_ids[i] = duckdb_create_logical_type(DUCKDB_TYPE_FLOAT);
    }
    ltype_ids[DIM] = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);
    mylog(logfile, "Appending data chunk-by-chunk...");
    for(idx_t i=0; i<dbcResult_len; i+=CHUNK_SIZE) {
      duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype_ids, DIM+1);

      idx_t to_copy = (dbcResult_len-i >= CHUNK_SIZE)? CHUNK_SIZE: dbcResult_len-i;
      duckdb_data_chunk_set_size(cnk, to_copy);
      for(idx_t col=0; col<DIM; col++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
        void *dat = duckdb_vector_get_data(vec);
        memcpy(dat, resultPts_cols[col] + i*sizeof(float), to_copy*sizeof(float));
      }
      duckdb_vector idx_vec = duckdb_data_chunk_get_vector(cnk, DIM);
      void *idx_dat = duckdb_vector_get_data(idx_vec);
      memcpy(idx_dat, result_ids + i, to_copy*sizeof(int64_t));

      if(duckdb_append_data_chunk(dbc_appender, cnk) == DuckDBError) {
        perror("Failed to append data chunk.\n");
        return -1;
      }

      duckdb_destroy_data_chunk(&cnk);
      //if((i+CHUNK_SIZE)%(60*CHUNK_SIZE)==0) {
      if(true) {
        duckdb_appender_flush(dbc_appender);
      }
    }
    duckdb_appender_destroy(&dbc_appender);
    mylog(logfile, "Finished appending data to final result table.");

  // Cleanup
    for(idx_t i=0; i<DIM+1; i++) {
      duckdb_destroy_logical_type(&ltype_ids[i]);
    }
    free(result_ids);
    for(idx_t i=0; i<DIM; i++) {
      free(resultPts_cols[i]);
    }
    mylog(logfile, "Cleaned up result buffers.");

    futhark_context_free(ctx);
    futhark_context_config_free(cfg);
    mylog(logfile, "Freed futhark core.");

  	duckdb_disconnect(&con);
  	duckdb_close(&db);
    mylog(logfile, "Disconnected duckdb and freed its memory.");

    logclose(logfile);
  
  return 0;
}






