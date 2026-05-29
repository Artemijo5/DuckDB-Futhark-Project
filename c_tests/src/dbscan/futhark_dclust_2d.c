// Make tables R_tbl, S_tbl, for narrow & wide joins tests.

// TODO make sure the time measurements correspong to actual query execution
// and not just time to launch the query or some other meaningless thing

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../../../clibs/duckdb.h"

#include "../../../clibs/mylogger.h"
#include "../../../ft_clibs/dclust_entry.h"

#include <unistd.h>
#include <getopt.h>

#define default_INPUT_FILENAME  "2d.txt"
#define default_OUTPUT_FILENAME "futhark_dclust_2d.csv"

#define default_DATASET_SIZE 10

#define CHUNK_SIZE duckdb_vector_size()
#define EXTPAR 2048
#define default_SEED_COUNT 1024
#define default_SUBDIV 100

#define DIM 2
#define default_EPS 0.5
#define default_MIN_PTS 3

#define default_ITER 1

#define default_LOGFILE "stdout"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations
    	char INPUT_FILENAME[2000] = default_INPUT_FILENAME;
    	char OUTPUT_FILENAME[2000] = default_OUTPUT_FILENAME;

		int64_t DATASET_SIZE = default_DATASET_SIZE;

		int64_t SEED_COUNT = default_SEED_COUNT;
		int64_t SUBDIV = default_SUBDIV;

		double EPS  = default_EPS;
		int64_t MIN_PTS = default_MIN_PTS;

		int64_t ITER = default_ITER;

		char LOGFILE[1000] = default_LOGFILE;

		static struct option long_options[] = {
			{"input", required_argument, 0, 'i'},
			{"output", required_argument, 0, 'o'},
			{"dataset_size", required_argument, 0, 's'},
			{"seed_count", required_argument, 0, 'c'},
			{"subdiv", required_argument, 0, 'd'},
			{"eps", required_argument, 0, 'e'},
			{"min_pts", required_argument, 0, 'm'},
			{"iter", required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"i:o:s:c:d:e:m:I:L:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	        case 'i':
	        	memcpy(INPUT_FILENAME, optarg, strlen(optarg)+1); break; 
	        case 'o':
	        	memcpy(OUTPUT_FILENAME, optarg, strlen(optarg)+1); break; 
	        case 's':
	        	DATASET_SIZE = atol(optarg); break;
	        case 'c':
	        	SEED_COUNT = atol(optarg); break;
	        case 'd':
	        	SUBDIV = atol(optarg); break;
	        case 'e':
	        	EPS = atof(optarg); break;
	        case 'm':
	        	MIN_PTS = atol(optarg); break;
	        case 'I':
	        	ITER = atol(optarg); break;
	        case 'L':
	        	memcpy(LOGFILE, optarg, strlen(optarg)+1); break;
	      }
	    }

	// init logger

		FILE* logfile = loginit(LOGFILE, "Starting futhark-DClust test program.");
	    if(LOGFILE && !logfile) {
	      perror("Failed to initialise logger.\n");
	      return -1;
	    }

	// TODO log parameters

    // make duckdb connection

	    duckdb_database db;
	  	duckdb_connection con;

	  	if (duckdb_open(NULL, &db) == DuckDBError) {
	  		perror("Failed to open in-memory database.\n");
	  		return -1;
	  	}
	  	if (duckdb_connect(db, &con) == DuckDBError) {
	  		perror("Failed to connect to in-memory database.\n");
	  		return -1;
	  	}

	  	mylog(logfile, "Connected to duckdb in-memory database.");

	// Initialize futhark context

	  	struct futhark_context_config *cfg = futhark_context_config_new();
	  	struct futhark_context *ctx = futhark_context_new(cfg);

	  	char *ctx_err = futhark_context_get_error(ctx);
	  	if(ctx_err) {
	  		perror(ctx_err);
	  		free(ctx_err);
	  		futhark_context_free(ctx);
	  		return -1;
	  	}
	  	free(ctx_err);

	  	mylog(logfile, "Set up futhark core.");

	for(int64_t iter_j=0; iter_j<ITER; iter_j++) {
		if(ITER>1) {
			char iter_str[100];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", iter_j);
			mylog(logfile, iter_str);
		}

		// Query to read data from input file.

			duckdb_result res;
			char query_str[300 + strlen(INPUT_FILENAME)];
			sprintf(query_str,
				"SELECT (#1)::DOUBLE, (#2)::DOUBLE FROM read_csv('%s') LIMIT %ld;",
				INPUT_FILENAME, DATASET_SIZE
			);
			if(duckdb_query(con, query_str, &res) == DuckDBError) {
				perror("Failed to execute query to read data.\n");
				perror(query_str);
				return -1;
			}
			mylog(logfile, "Executed duckdb query to read input data.");

		// Init futhark arrays

			struct futhark_f64_1d *col1;
			struct futhark_f64_1d *col2;

			futhark_entry_init_column_f64(ctx, &col1, DATASET_SIZE);
			futhark_entry_init_column_f64(ctx, &col2, DATASET_SIZE);

			mylog(logfile, "Initialized empty columns in the futhark core.");

		// Read duckdb chunks and copy into arrays

			mylog(logfile, "Obtaining query result into the futhark core...");
			int64_t num_rows = 0;
			while(true) {
				duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
				if(!cnk) {
					mylog(logfile, "Result is exhausted.");
					break;
				}

				int64_t this_rows = duckdb_data_chunk_get_size(cnk);

				duckdb_vector vec1 = duckdb_data_chunk_get_vector(cnk,0);
				duckdb_vector vec2 = duckdb_data_chunk_get_vector(cnk,1);

				double *dat1 = (double*)duckdb_vector_get_data(vec1);
				double *dat2 = (double*)duckdb_vector_get_data(vec2);

				struct futhark_f64_1d *ft_dat1 = futhark_new_f64_1d(ctx, dat1, this_rows);
				struct futhark_f64_1d *ft_dat2 = futhark_new_f64_1d(ctx, dat2, this_rows);

				// Due to consumption, need to rearrange pointers.
				struct futhark_f64_1d *col1_tmp;
				struct futhark_f64_1d *col2_tmp;

				futhark_entry_write_column_f64(ctx, &col1_tmp, num_rows, ft_dat1, col1);
				futhark_entry_write_column_f64(ctx, &col2_tmp, num_rows, ft_dat2, col2);

				futhark_free_f64_1d(ctx, col1);
				futhark_free_f64_1d(ctx, col2);

				col1 = col1_tmp;
				col2 = col2_tmp;

				num_rows += this_rows;
				duckdb_destroy_data_chunk(&cnk);
			}
			duckdb_destroy_result(&res);

		// Crop arrays if necessary

			if(num_rows < DATASET_SIZE) {
				struct futhark_f64_1d *col1_tmp;
				struct futhark_f64_1d *col2_tmp;

				futhark_entry_crop_column_f64(ctx, &col1_tmp, 0, num_rows, col1);
				futhark_entry_crop_column_f64(ctx, &col2_tmp, 0, num_rows, col2);

				futhark_free_f64_1d(ctx, col1);
				futhark_free_f64_1d(ctx, col2);

				col1 = col1_tmp;
				col2 = col2_tmp;

				mylog(logfile, "Cropped columns to actual dataset size.");
			}

		// Do dbscan

			struct futhark_opaque_dbscan_result *dbscan_res;

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "Performing D-Clust...");

			futhark_entry_do_dclust_2d_f64(ctx, &dbscan_res,
				SEED_COUNT, SUBDIV, EPS, MIN_PTS,
				col1, col2
			);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			
			futhark_free_f64_1d(ctx, col1);
			futhark_free_f64_1d(ctx, col2);

			mylog(logfile, "D-Clust completed.");

			// See if any errors occured.
			char *ft_error_msg = futhark_context_get_error(ctx);
			mylog(logfile, ft_error_msg);
			free(ft_error_msg);

		// Unwrap data from futhark core

			struct futhark_i64_1d *ft_cluster_id;
			struct futhark_bool_1d *ft_is_core;

			futhark_project_opaque_dbscan_result_cluster_id(ctx, &ft_cluster_id, dbscan_res);
			futhark_project_opaque_dbscan_result_is_core(ctx, &ft_is_core, dbscan_res);
			mylog(logfile, "Projected dbscan_res fields (cluster_id & is_core).");

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			int64_t *cluster_id = malloc(num_rows*sizeof(int64_t));
			bool *is_core = malloc(num_rows*sizeof(bool));

			futhark_values_i64_1d(ctx, ft_cluster_id, cluster_id);
			futhark_values_bool_1d(ctx, ft_is_core, is_core);
			mylog(logfile, "Unwrapped data from futhark core.");

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			futhark_free_opaque_dbscan_result(ctx, dbscan_res);

		// Output data via appenders

		  	if(duckdb_query(con,
		  		"CREATE OR REPLACE TEMP TABLE output_tbl (is_core BOOLEAN, cluster_id BIGINT);", NULL) == DuckDBError) {
		  		perror("Failed to create output table.\n");
		  		return -1;
		  	}
		  	duckdb_appender appender;
		  	if (duckdb_appender_create(con,NULL,"output_tbl",&appender) == DuckDBError) {
		  		perror("Failed to create appender.\n");
		  		return -1;
		  	}
		  	mylog(logfile, "Set up duckdb appender for output.");

		  	// Create logical types for appender datachunks.
		  	duckdb_logical_type ltypes[2] = {
		  		duckdb_create_logical_type(DUCKDB_TYPE_BOOLEAN),
		  		duckdb_create_logical_type(DUCKDB_TYPE_BIGINT)
		  	};

		  	int64_t num_chunks = (num_rows + CHUNK_SIZE - 1) / CHUNK_SIZE;

		  	for(int64_t j=0; j<num_chunks; j++) {
		  		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, 2);

		  		int64_t this_rows = (j==num_chunks-1)?
		  			(num_rows%CHUNK_SIZE):
		  			CHUNK_SIZE;
		  		duckdb_data_chunk_set_size(cnk, this_rows);

		  		bool *dat1 = (bool*)duckdb_vector_get_data(duckdb_data_chunk_get_vector(cnk,0));
		  		int64_t *dat2 = (int64_t*)duckdb_vector_get_data(duckdb_data_chunk_get_vector(cnk,1));

		  		// could be done faster if I used ft_keyCol read here
		  		// but would need to still have futhark context
		  		memcpy(dat1, is_core + j*CHUNK_SIZE, this_rows*sizeof(bool));
		  		memcpy(dat2, cluster_id + j*CHUNK_SIZE, this_rows*sizeof(int64_t));

		  		duckdb_append_data_chunk(appender, cnk);
		  		if((j+1)%100 == 0) duckdb_appender_flush(appender);
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_appender_flush(appender);
		  	duckdb_appender_destroy(&appender);

		  	free(is_core);
		  	free(cluster_id);

		  	mylog(logfile, "Finished appending chunks & freed arrays & appender.");

		  	char export_query[300 + strlen(OUTPUT_FILENAME)];
		  	sprintf(export_query, "COPY output_tbl TO '%s' (HEADER, DELIMITER ',');", OUTPUT_FILENAME);
		  	if(duckdb_query(con, export_query, NULL) == DuckDBError) {
		  		perror("Failed to export output as .csv.");
		  		return -1;
		  	}
		  	mylog(logfile, "Exported output as .csv.");

	}

	// Cleanup

		futhark_context_sync(ctx);
	  	futhark_context_free(ctx);
	  	futhark_context_config_free(cfg);
	  	mylog(logfile, "Freed futhark core.");
	  	
	  	duckdb_disconnect(&con);
		duckdb_close(&db);
  		mylog(logfile, "Disconnected duckdb and freed its memory.");

  		logclose(logfile);
  
  return 0;
}