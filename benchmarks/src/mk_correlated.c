#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../ft_clibs/ftSynthetic.h"
#include "../../clibs/mylogger.h"
#include "../../clibs/db_util.h"

#include <unistd.h>
#include <getopt.h>

#define default_LOGFILE "stdout"//"logs/synthetic.log.txt"
#define CHUNK_SIZE duckdb_vector_size()
#define default_BUFFER_CAP (long)16//(long)2*CNK_TO_READ
#define default_BUFFER_SIZE BUFFER_CAP*CHUNK_SIZE

#define default_TABLE_NAME "skyTbl"
#define default_TABLE_SIZE 3*BUFFER_SIZE//10*BUFFER_SIZE
#define default_DIM 2
#define default_MAGNITUDE (float)100.0
#define default_CORR_FACTOR (float)0.8
#define default_POSITIVE true

#define default_DBFILE "testdb.db"
#define default_DDB_MEMSIZE "20GB"
#define default_DDB_TEMPDIR "/tps_tempdir"

int main(int argc, char *argv[]) {
	// Parse command line arguments
		// Initializations
			char LOGFILE[250] = default_LOGFILE;
			int64_t BUFFER_CAP = default_BUFFER_CAP;
			int64_t BUFFER_SIZE = BUFFER_CAP*CHUNK_SIZE;
			char TABLE_NAME[250] = default_TABLE_NAME;
			int64_t TABLE_SIZE = default_TABLE_SIZE;
			int64_t DIM = default_DIM;
			float MAGNITUDE = default_MAGNITUDE;
			float CORR_FACTOR = default_CORR_FACTOR;
			bool POSITIVE = default_POSITIVE;
			char DBFILE[250] = default_DBFILE;
			char DDB_MEMSIZE[25] = default_DDB_MEMSIZE;
			char DDB_TEMPDIR[250] = default_DDB_TEMPDIR;

		static struct option long_options[] =
		  {
		      {"logfile", required_argument, 0, 'L'},
		      {"buffer_cap", required_argument, 0, 'B'},
		      {"table_name", required_argument, 0, 't'},
		      {"table_size", required_argument, 0, 'T'},
		      {"dim", required_argument, 0, 'D'},
		      {"magnitude", required_argument, 0, 'M'},
		      {"corr_factor", required_argument, 0, 'C'},
		      {"anticorrelated", no_argument, 0, 'A'},
		      {"db_file", required_argument, 0, 'f'},
		      {"db_memsize", required_argument, 0, 'm'},
		      {"db_tempdir", required_argument, 0, 'd'},
		      {0, 0, 0, 0}
		  };
		char ch;
		while(
		  (ch = getopt_long_only(argc,argv,"L:B:t:T:D:M:C:Af:m:d:",long_options,NULL)) != -1
		) {
		  switch(ch) {
		    case 'L':
		      memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
		    case 'B':
		      BUFFER_CAP = atol(optarg); BUFFER_SIZE = BUFFER_CAP*CHUNK_SIZE; break;
		    case 't':
		      memcpy(TABLE_NAME, optarg, strlen(optarg)+1); break;
		    case 'T':
		      TABLE_SIZE = atol(optarg); break;
		    case 'D':
		      DIM = atol(optarg); break;
		    case 'M':
		    	MAGNITUDE = (float)atof(optarg); break;
		    case 'C':
		    	CORR_FACTOR = (float)atof(optarg); break;
		    case 'A':
		    	POSITIVE = false; break;
		    case 'f':
		      memcpy(DBFILE, optarg, strlen(optarg)+1); break;
		    case 'm':
		      memcpy(DDB_MEMSIZE, optarg, strlen(optarg)+1); break;
		    case 'd':
		      memcpy(DDB_TEMPDIR, optarg, strlen(optarg)+1); break;
		  }
		}

	// Initialise logger
    FILE* logfile = loginit(LOGFILE, "mk_correlated : Generating test data.");
    if(LOGFILE && !logfile) {
      perror("Failed to initialise logger.\n");
      return -1;
    }

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

	// Create initial table (dim+1 random samples)
		char queryStr[250 + strlen(TABLE_NAME) + 20*(DIM+1)];
	    int queryLen = sprintf(queryStr, "CREATE OR REPLACE TEMP TABLE tmp_mk_correlated_buffer (");
	    for(idx_t i=0; i<DIM+1; i++) {
		    queryLen += sprintf(queryStr+queryLen, "r%ld FLOAT", i);
		    if(i<DIM) queryLen += sprintf(queryStr+queryLen, ", ");
	    }
	    queryLen += sprintf(queryStr+queryLen, ");");
	    if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
		    perror("Failed to create buffer table.\n");
		    perror(queryStr);
		    return -1;
	    }

	    duckdb_query(con, "setseed(0.42);", NULL);

	    // Insert random data into the table
	    char insertQueryStr[250 + 100*(DIM+1)];
	    int insertQueryLen = sprintf(insertQueryStr, "INSERT INTO tmp_mk_correlated_buffer (SELECT ");
	    for(idx_t i=0; i<DIM+1; i++) {
	    	insertQueryLen += sprintf(insertQueryStr+insertQueryLen, "random()");
	    	if(i<DIM) insertQueryLen += sprintf(insertQueryStr+insertQueryLen, ", ");
	    }
	    insertQueryLen += sprintf(insertQueryStr+insertQueryLen, " FROM RANGE(%ld) t(i));", TABLE_SIZE);
	    if( duckdb_query(con, insertQueryStr, NULL) == DuckDBError ) {
	    	perror("Failed to insert data into the buffer table.\n");
	    	perror(insertQueryStr);
	    	return -1;
	    }
		mylog(logfile, "Created test table.");

	// Set up futhark core
		struct futhark_context_config *cfg = futhark_context_config_new();
		struct futhark_context *ctx = futhark_context_new(cfg);
		mylog(logfile, "Set up futhark context & config.");

	// Create dest table & appender
		mylog(logfile, "Creating destination table...");
	    char finalQueryStr[300 + 20*DIM];
	    int finalQueryLen = sprintf(finalQueryStr, "CREATE OR REPLACE TABLE %s (", TABLE_NAME);
	    for(idx_t i=0; i<DIM-1; i++) {
	      finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "x%ld FLOAT, ", i+1);
	    }
	    finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "x%ld FLOAT);", DIM);
	    if( duckdb_query(con, finalQueryStr, NULL) == DuckDBError ) {
	      perror("Failed to create final result table.\n");
	      perror(finalQueryStr);
	      return -1;
	    }

	    duckdb_appender dest_appender;
	    if( duckdb_appender_create(con, NULL, TABLE_NAME, &dest_appender) == DuckDBError ) {
		    perror("Failed to create appender.\n");
		    return -1;
	    }
	    mylog(logfile, "Created appender.");

	// Transform and store data
	    duckdb_result res;
	    duckdb_query(con, "SELECT * FROM tmp_mk_correlated_buffer;", &res);
	    bool isTblExhausted = false;
	    mylog(logfile, "Processing buffer data...");
	    while(!isTblExhausted) {
	    	// Allocate buffers
		    	void **buffers  = malloc((DIM+1)*sizeof(float*));
		    	for(int64_t d=0; d<DIM+1; d++) {
		    		buffers[d] = malloc(BUFFER_SIZE*sizeof(float));
		    	}
		    	void *flatbuff = malloc((DIM+1)*BUFFER_SIZE*sizeof(float));
		    	// futhark output overwrites flatbuff

	    	// Obtain data from chunks
		    	int64_t row_count = 0;
		    	for(int64_t c=0; c<BUFFER_CAP; c++) {	
		    		duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
		    		if(!cnk) {
		    			isTblExhausted = true;
		    			mylog(logfile, "Result is exhausted.");
		    			break;
		    		}
		    		int64_t this_row_count = duckdb_data_chunk_get_size(cnk);		    		

		    		for(int64_t col=0; col<DIM+1; col++) {
		    			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
		    			void *dat = duckdb_vector_get_data(vec);

		    			memcpy(buffers[col] + row_count*sizeof(float), dat, this_row_count*sizeof(float));
		    		}
		    		
		    		duckdb_destroy_data_chunk(&cnk);
		    		row_count += this_row_count;
		    	}
		    	if(row_count==0) {
		    		free(flatbuff);
			    	for(int64_t d=0; d<DIM+1; d++) {
			    		free(buffers[d]);
			    	}
			    	free(buffers);
			    	break;
		    	}

		    	for(int64_t col=0;col<DIM+1;col++) {
		    		memcpy(
		    			(flatbuff) + col*row_count*sizeof(float),
		    			buffers[col],
		    			row_count*sizeof(float)
		    		);
		    	}
		    	mylog(logfile, "Buffered data for this iteration.");

	    	// Process data through futhark
		    	bool flags[DIM-1];
		    	for(int64_t i=0; i<DIM-1; i++) {
		    		flags[i] = !POSITIVE;
		    	}

		    	struct futhark_bool_1d *flags_ft = futhark_new_bool_1d(ctx, flags, DIM-1);
		    	struct futhark_f32_2d *buff_in_ft = futhark_new_f32_2d(ctx, flatbuff, DIM+1, row_count);

		    	struct futhark_f32_2d *buff_out_ft;
		    	futhark_entry_linear_correlated_float(ctx, &buff_out_ft, CORR_FACTOR, MAGNITUDE, flags_ft, buff_in_ft);
		    	futhark_context_sync(ctx);
		    	mylog(logfile, "Performed transformation in futhark & synced context.");
		    	futhark_free_f32_2d(ctx, buff_in_ft);
		    	futhark_free_bool_1d(ctx, flags_ft);

	    	// Unwrap data
		    	futhark_values_f32_2d(ctx, buff_out_ft, flatbuff);
		    	futhark_free_f32_2d(ctx, buff_out_ft);

	    	// Append data to dest table
		    	duckdb_logical_type ltype_ids[DIM];
			    for(idx_t i=0; i<DIM; i++) {
			    	ltype_ids[i] = duckdb_create_logical_type(DUCKDB_TYPE_FLOAT);
			    }

		    	int64_t rows_appended = 0;
		    	while(rows_appended < row_count) {
		    		int64_t this_rows = (row_count - CHUNK_SIZE > rows_appended)? CHUNK_SIZE: row_count - rows_appended;
		    		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype_ids, DIM);
		    		duckdb_data_chunk_set_size(cnk, this_rows);

		    		for(idx_t col=0; col<DIM; col++) {
					    duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
					    void *dat = duckdb_vector_get_data(vec);
					    memcpy(
					    	dat,
					    	(flatbuff) + (col*row_count + rows_appended)*sizeof(float),
					    	this_rows*sizeof(float)
					    );
				    }

				    if(duckdb_append_data_chunk(dest_appender, cnk) == DuckDBError) {
					    perror("Failed to append data chunk.\n");
					    return -1;
				    }

				    duckdb_appender_flush(dest_appender);
				    duckdb_destroy_data_chunk(&cnk);
		    		rows_appended += this_rows;
		    	}

		    	for(idx_t i=0; i<DIM; i++) {
			    	duckdb_destroy_logical_type(&ltype_ids[i]);
			    }
		    	mylog(logfile, "Finished this iteration of appending.");

	    	// Cleanup
		    	free(flatbuff);
		    	for(int64_t d=0; d<DIM+1; d++) {
		    		free(buffers[d]);
		    	}
		    	free(buffers);
	    }
	    mylog(logfile, "Data transformation completed.");

	// Cleanup
	    duckdb_destroy_result(&res);
	    duckdb_appender_destroy(&dest_appender);
	    mylog(logfile, "Destroyed result & appender.");

	    futhark_context_free(ctx);
	    futhark_context_config_free(cfg);
	    mylog(logfile, "Freed futhark core.");

	  	duckdb_disconnect(&con);
	  	duckdb_close(&db);
	    mylog(logfile, "Disconnected duckdb and freed its memory.");

		logclose(logfile);
	  
	return 0;
}