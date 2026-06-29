#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../../../clibs/duckdb.h"

#include "../../../clibs/mylogger.h"
#include "../../../ft_clibs/densebox_entry.h"

#include <unistd.h>
#include <getopt.h>

#define default_INPUT_FILENAME  "2d.txt"
#define default_OUTPUT_FILENAME "futhark_densebox.csv"

#define default_DATASET_SIZE 10

#define CHUNK_SIZE duckdb_vector_size()

#define default_DIM 2

#define default_WSIZE1 2048
#define default_WSIZE2 2048
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

		int64_t DIM = default_DIM;

		int64_t WSIZE1 = default_WSIZE1;
		int64_t WSIZE2 = default_WSIZE2;
		double EPS  = default_EPS;
		int64_t MIN_PTS = default_MIN_PTS;

		int64_t ITER = default_ITER;

		char LOGFILE[1000] = default_LOGFILE;

		static struct option long_options[] = {
			{"input", required_argument, 0, 'i'},
			{"output", required_argument, 0, 'o'},
			{"dataset_size", required_argument, 0, 's'},
			{"dim", required_argument, 0, 'd'},
			{"wsize_index", required_argument, 0, 'w'},
			{"wsize_clust", required_argument, 0, 'W'},
			{"eps", required_argument, 0, 'e'},
			{"min_pts", required_argument, 0, 'm'},
			{"iter", required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"i:o:s:d:w:W:e:m:I:L:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	        case 'i':
	        	memcpy(INPUT_FILENAME, optarg, strlen(optarg)+1); break; 
	        case 'o':
	        	memcpy(OUTPUT_FILENAME, optarg, strlen(optarg)+1); break; 
	        case 's':
	        	DATASET_SIZE = atol(optarg); break;
	        case 'd':
	        	DIM = atol(optarg); break;
	        case 'w':
	        	WSIZE1 = atol(optarg); break;
	        case 'W':
	        	WSIZE2 = atol(optarg); break;
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

		FILE* logfile = loginit(LOGFILE, "Starting futhark-densebox DBSCAN test program.");
	    if(LOGFILE && !logfile) {
	      perror("Failed to initialise logger.\n");
	      return -1;
	    }

	    switch(DIM) {
	    	case 2:
	    		mylog(logfile, "DBSCAN on 2-D data.");
	    		break;
	    	case 3:
	    		mylog(logfile, "DBSCAN on 3-D data.");
	    		break;
	    	case 4:
	    		mylog(logfile, "DBSCAN on 4-D data.");
	    		break;
	    	case 5:
	    		mylog(logfile, "DBSCAN on 5-D data.");
	    		break;
	    	case 7:
	    		mylog(logfile, "DBSCAN on 7-D data.");
	    		break;
	    	default:
	    		mylog(logfile, "Invalid dimensionality: only 2D, 3D, 4D, 5D, 7D are supported.");
	    		logclose(logfile);
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

			char query_str[1000 + strlen(INPUT_FILENAME)];

			int query_str_len = sprintf(query_str, "SELECT (#1)::DOUBLE");
			for (int64_t col=1; col<DIM; col++) {
				query_str_len += sprintf(query_str+query_str_len, ", (#%ld)::DOUBLE", col+1);
			}
			sprintf(query_str+query_str_len,
				" FROM read_csv('%s') LIMIT %ld;",
				INPUT_FILENAME, DATASET_SIZE
			);
			if(duckdb_query(con, query_str, &res) == DuckDBError) {
				perror("Failed to execute query to read data.\n");
				perror(query_str);
				return -1;
			}
			mylog(logfile, "Executed duckdb query to read input data.");

		// Init arrays

			double *xss[DIM];
			for(int64_t col=0; col<DIM; col++) {
				xss[col] = malloc(DATASET_SIZE*sizeof(double));
			}

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

				for(int64_t col=0; col<DIM; col++) {
					duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
					double *dat = (double *)duckdb_vector_get_data(vec);
					memcpy(xss[col] + num_rows, dat, this_rows*sizeof(double));
				}
				num_rows += this_rows;
			}
			duckdb_destroy_result(&res);

		// Wrap into futhark context

			struct futhark_f64_1d *ft_xss[DIM];
			for(int64_t col=0; col<DIM; col++) {
				ft_xss[col] = futhark_new_f64_1d(ctx, xss[col], num_rows);
			}
			mylog(logfile, "Wrapped data into futhark context.");
			for(int64_t col=0; col<DIM; col++) {
				free(xss[col]);
			}


		// Do dbscan

			struct futhark_opaque_dbscan_result *dbscan_res;

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "Performing Densebox dbscan ...");

			if(DIM==2) {
				struct futhark_opaque_indexed_data_2d_f64 *idx_dat;
				futhark_entry_densebox_index_dataset_2d_f64(ctx, &idx_dat, WSIZE1, EPS, ft_xss[0], ft_xss[1]);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished Indexing.");

				int64_t num_parts;
				futhark_project_opaque_indexed_data_2d_f64_parts_No(ctx, &num_parts, idx_dat);
				printf("\n#partitions: %ld\n\n", num_parts);

				futhark_entry_densebox_do_dbscan_2d_f64(ctx, &dbscan_res, WSIZE2, EPS, MIN_PTS, idx_dat);
				futhark_free_opaque_indexed_data_2d_f64(ctx, idx_dat);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished DBSCAN on 2D data.");

			}
			else if(DIM==3) {

				struct futhark_opaque_indexed_data_3d_f64 *idx_dat;
				futhark_entry_densebox_index_dataset_3d_f64(ctx, &idx_dat, WSIZE1, EPS,
					ft_xss[0], ft_xss[1], ft_xss[2]);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished Indexing.");

				int64_t num_parts;
				futhark_project_opaque_indexed_data_3d_f64_parts_No(ctx, &num_parts, idx_dat);
				printf("\n#partitions: %ld\n\n", num_parts);

				futhark_entry_densebox_do_dbscan_3d_f64(ctx, &dbscan_res, WSIZE2, EPS, MIN_PTS, idx_dat);
				futhark_free_opaque_indexed_data_3d_f64(ctx, idx_dat);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished DBSCAN on 3D data.");

			}
			else if(DIM==4) {

				struct futhark_opaque_indexed_data_4d_f64 *idx_dat;
				futhark_entry_densebox_index_dataset_4d_f64(ctx, &idx_dat, WSIZE1, EPS,
					ft_xss[0], ft_xss[1], ft_xss[2], ft_xss[3]);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished Indexing.");

				int64_t num_parts;
				futhark_project_opaque_indexed_data_4d_f64_parts_No(ctx, &num_parts, idx_dat);
				printf("\n#partitions: %ld\n\n", num_parts);

				futhark_entry_densebox_do_dbscan_4d_f64(ctx, &dbscan_res, WSIZE2, EPS, MIN_PTS, idx_dat);
				futhark_free_opaque_indexed_data_4d_f64(ctx, idx_dat);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished DBSCAN on 4D data.");

			}
			else if(DIM==5) {

				struct futhark_opaque_indexed_data_5d_f64 *idx_dat;
				futhark_entry_densebox_index_dataset_5d_f64(ctx, &idx_dat, WSIZE1, EPS,
					ft_xss[0], ft_xss[1], ft_xss[2], ft_xss[3], ft_xss[4]);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished Indexing.");

				int64_t num_parts;
				futhark_project_opaque_indexed_data_5d_f64_parts_No(ctx, &num_parts, idx_dat);
				printf("\n#partitions: %ld\n\n", num_parts);

				futhark_entry_densebox_do_dbscan_5d_f64(ctx, &dbscan_res, WSIZE2, EPS, MIN_PTS, idx_dat);
				futhark_free_opaque_indexed_data_5d_f64(ctx, idx_dat);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished DBSCAN on 5D data.");

			}
			else {

				struct futhark_opaque_indexed_data_7d_f64 *idx_dat;
				futhark_entry_densebox_index_dataset_7d_f64(ctx, &idx_dat, WSIZE1, EPS,
					ft_xss[0], ft_xss[1], ft_xss[2], ft_xss[3], ft_xss[4], ft_xss[5], ft_xss[6]);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished Indexing.");

				int64_t num_parts;
				futhark_project_opaque_indexed_data_7d_f64_parts_No(ctx, &num_parts, idx_dat);
				printf("\n#partitions: %ld\n\n", num_parts);

				futhark_entry_densebox_do_dbscan_7d_f64(ctx, &dbscan_res, WSIZE2, EPS, MIN_PTS, idx_dat);
				futhark_free_opaque_indexed_data_7d_f64(ctx, idx_dat);
				futhark_context_sync(ctx);
				mylog(logfile, "Finished DBSCAN on 7D data.");

			}

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			
			for(int64_t col=0; col<DIM; col++) {
				futhark_free_f64_1d(ctx, ft_xss[col]);
			}

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