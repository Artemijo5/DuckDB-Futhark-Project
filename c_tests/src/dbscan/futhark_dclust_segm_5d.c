#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../../../clibs/duckdb.h"

#include "../../../clibs/mylogger.h"
#include "../../../ft_clibs/dclust_entry_segm_hd.h"

#include <unistd.h>
#include <getopt.h>

#define default_INPUT_FILENAME  "5d.txt"
#define default_OUTPUT_FILENAME "futhark_dclust_5d.csv"

#define default_DATASET_SIZE 10

#define CHUNK_SIZE duckdb_vector_size()
#define default_SEED_COUNT 1024
#define default_SUBDIV 1

#define DIM 5
#define default_EPS 0.5
#define default_MIN_PTS 6

#define default_ITER 1

#define default_LOGFILE "stdout"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations
    	char INPUT_FILENAME[2000] = default_INPUT_FILENAME;
    	char OUTPUT_FILENAME[2000] = default_OUTPUT_FILENAME;

		int64_t DATASET_SIZE = default_DATASET_SIZE;

		int64_t SEED_COUNT = default_SEED_COUNT;
		int64_t SUBDIV1 = default_SUBDIV;
		int64_t SUBDIV2 = default_SUBDIV;
		int64_t SUBDIV3 = default_SUBDIV;
		int64_t SUBDIV4 = default_SUBDIV;
		int64_t SUBDIV5 = default_SUBDIV;

		double EPS  = default_EPS;
		int64_t MIN_PTS = default_MIN_PTS;

		bool do_sampling = false;
		int64_t ITER = default_ITER;

		char LOGFILE[1000] = default_LOGFILE;

		static struct option long_options[] = {
			{"input", required_argument, 0, 'i'},
			{"output", required_argument, 0, 'o'},
			{"dataset_size", required_argument, 0, 's'},
			{"seed_count", required_argument, 0, 'c'},
			{"subdiv1", required_argument, 0, '1'},
			{"subdiv2", required_argument, 0, '2'},
			{"subdiv3", required_argument, 0, '3'},
			{"subdiv4", required_argument, 0, '4'},
			{"subdiv5", required_argument, 0, '5'},
			{"eps", required_argument, 0, 'e'},
			{"min_pts", required_argument, 0, 'm'},
			{"iter", required_argument, 0, 'I'},
			{"do_sampling", no_argument, 0, 'R'},
			{"logfile", required_argument, 0, 'L'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"i:o:s:c:1:2:3:4:5:e:m:I:RL:",long_options,NULL)) != -1
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
	        case '1':
	        	SUBDIV1 = atol(optarg); break;
	        case '2':
	        	SUBDIV2 = atol(optarg); break;
	        case '3':
	        	SUBDIV3 = atol(optarg); break;
	        case '4':
	        	SUBDIV4 = atol(optarg); break;
	        case '5':
	        	SUBDIV5 = atol(optarg); break;
	        case 'e':
	        	EPS = atof(optarg); break;
	        case 'm':
	        	MIN_PTS = atol(optarg); break;
	        case 'I':
	        	ITER = atol(optarg); break;
	        case 'R':
	        	do_sampling = true; break;
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

			if(!do_sampling)
				sprintf(query_str,
					"SELECT (#1)::DOUBLE,(#2)::DOUBLE,(#3)::DOUBLE,(#4)::DOUBLE,(#5)::DOUBLE \
					FROM read_csv('%s') LIMIT %ld;",
					INPUT_FILENAME, DATASET_SIZE
				);
			else
				sprintf(query_str,
					"SELECT (#1)::DOUBLE,(#2)::DOUBLE,(#3)::DOUBLE,(#4)::DOUBLE,(#5)::DOUBLE \
					FROM read_csv('%s') USING SAMPLE reservoir(%ld ROWS);",
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
			struct futhark_f64_1d *col3;
			struct futhark_f64_1d *col4;
			struct futhark_f64_1d *col5;

			futhark_entry_init_column_f64(ctx, &col1, DATASET_SIZE);
			futhark_entry_init_column_f64(ctx, &col2, DATASET_SIZE);
			futhark_entry_init_column_f64(ctx, &col3, DATASET_SIZE);
			futhark_entry_init_column_f64(ctx, &col4, DATASET_SIZE);
			futhark_entry_init_column_f64(ctx, &col5, DATASET_SIZE);

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
				duckdb_vector vec3 = duckdb_data_chunk_get_vector(cnk,2);
				duckdb_vector vec4 = duckdb_data_chunk_get_vector(cnk,3);
				duckdb_vector vec5 = duckdb_data_chunk_get_vector(cnk,4);

				double *dat1 = (double*)duckdb_vector_get_data(vec1);
				double *dat2 = (double*)duckdb_vector_get_data(vec2);
				double *dat3 = (double*)duckdb_vector_get_data(vec3);
				double *dat4 = (double*)duckdb_vector_get_data(vec4);
				double *dat5 = (double*)duckdb_vector_get_data(vec5);

				struct futhark_f64_1d *ft_dat1 = futhark_new_f64_1d(ctx, dat1, this_rows);
				struct futhark_f64_1d *ft_dat2 = futhark_new_f64_1d(ctx, dat2, this_rows);
				struct futhark_f64_1d *ft_dat3 = futhark_new_f64_1d(ctx, dat3, this_rows);
				struct futhark_f64_1d *ft_dat4 = futhark_new_f64_1d(ctx, dat4, this_rows);
				struct futhark_f64_1d *ft_dat5 = futhark_new_f64_1d(ctx, dat5, this_rows);

				// Due to consumption, need to rearrange pointers.
				struct futhark_f64_1d *col1_tmp;
				struct futhark_f64_1d *col2_tmp;
				struct futhark_f64_1d *col3_tmp;
				struct futhark_f64_1d *col4_tmp;
				struct futhark_f64_1d *col5_tmp;

				futhark_entry_write_column_f64(ctx, &col1_tmp, num_rows, ft_dat1, col1);
				futhark_entry_write_column_f64(ctx, &col2_tmp, num_rows, ft_dat2, col2);
				futhark_entry_write_column_f64(ctx, &col3_tmp, num_rows, ft_dat3, col3);
				futhark_entry_write_column_f64(ctx, &col4_tmp, num_rows, ft_dat4, col4);
				futhark_entry_write_column_f64(ctx, &col5_tmp, num_rows, ft_dat5, col5);

				futhark_free_f64_1d(ctx, col1);
				futhark_free_f64_1d(ctx, col2);
				futhark_free_f64_1d(ctx, col3);
				futhark_free_f64_1d(ctx, col4);
				futhark_free_f64_1d(ctx, col5);

				col1 = col1_tmp;
				col2 = col2_tmp;
				col3 = col3_tmp;
				col4 = col4_tmp;
				col5 = col5_tmp;

				num_rows += this_rows;
				duckdb_destroy_data_chunk(&cnk);
			}
			duckdb_destroy_result(&res);

		// Crop arrays if necessary

			if(num_rows < DATASET_SIZE) {
				struct futhark_f64_1d *col1_tmp;
				struct futhark_f64_1d *col2_tmp;
				struct futhark_f64_1d *col3_tmp;
				struct futhark_f64_1d *col4_tmp;
				struct futhark_f64_1d *col5_tmp;

				futhark_entry_crop_column_f64(ctx, &col1_tmp, 0, num_rows, col1);
				futhark_entry_crop_column_f64(ctx, &col2_tmp, 0, num_rows, col2);
				futhark_entry_crop_column_f64(ctx, &col3_tmp, 0, num_rows, col3);
				futhark_entry_crop_column_f64(ctx, &col4_tmp, 0, num_rows, col4);
				futhark_entry_crop_column_f64(ctx, &col5_tmp, 0, num_rows, col5);

				futhark_free_f64_1d(ctx, col1);
				futhark_free_f64_1d(ctx, col2);
				futhark_free_f64_1d(ctx, col3);
				futhark_free_f64_1d(ctx, col4);
				futhark_free_f64_1d(ctx, col5);

				col1 = col1_tmp;
				col2 = col2_tmp;
				col3 = col3_tmp;
				col4 = col4_tmp;
				col5 = col5_tmp;

				mylog(logfile, "Cropped columns to actual dataset size.");
			}

		// Do dbscan

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "Performing D-Clust...");

			mylog(logfile, "1. Index dataset and get partition info.");

			struct futhark_opaque_indexed_data_5d_f64 *indexed_dat;
			futhark_entry_index_dataset_5d_f64(
				ctx, &indexed_dat, EPS,
				SUBDIV1, SUBDIV2, SUBDIV3, SUBDIV4, SUBDIV5,
				col1, col2, col3, col4, col5
			);
			futhark_free_f64_1d(ctx, col1);
			futhark_free_f64_1d(ctx, col2);
			futhark_free_f64_1d(ctx, col3);
			futhark_free_f64_1d(ctx, col4);
			futhark_free_f64_1d(ctx, col5);

			struct futhark_opaque_partition_info_f64 *partInfo;
			futhark_entry_get_part_info_5d_f64(ctx, &partInfo, false, EPS, indexed_dat);

			struct futhark_opaque_partition_info_f64 *partInfo_bd;
			futhark_entry_get_part_info_5d_f64(ctx, &partInfo_bd, true, EPS, indexed_dat);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			// Record true subdiv and #cells.
				struct futhark_i64_1d *ft_subdiv;
				futhark_project_opaque_indexed_data_5d_f64_subdiv(ctx, &ft_subdiv, indexed_dat);

				struct futhark_i64_1d *ft_cell_ids;
				futhark_project_opaque_indexed_data_5d_f64_cell_ids(ctx, &ft_cell_ids, indexed_dat);

				futhark_context_sync(ctx);

				int64_t true_subdiv[DIM];
				futhark_values_i64_1d(ctx, ft_subdiv, true_subdiv);
				futhark_free_i64_1d(ctx, ft_subdiv);
				const int64_t *cells_shape_ptr = futhark_shape_i64_1d(ctx, ft_cell_ids);

				printf("\n~~\n");
				printf("True subdivisions per dim:\n");
				printf(
					"%ld\t%ld\t%ld\t%ld\t%ld\n",
					true_subdiv[0], true_subdiv[1], true_subdiv[2], true_subdiv[3], true_subdiv[4]
				);
				printf("True number of cells:\n");
				printf("%ld", *cells_shape_ptr);
				futhark_free_i64_1d(ctx, ft_cell_ids);
				printf("\n~~\n");

			mylog(logfile, "2. Find and isolate core points.");

			struct futhark_i64_1d *neigh_counts;
			futhark_entry_get_neighbour_counts_5d_f64(
				ctx, &neigh_counts, SEED_COUNT, EPS, MIN_PTS, indexed_dat, partInfo
			);

			struct futhark_bool_1d *ind_is_core;
			futhark_entry_get_is_core(ctx, &ind_is_core, MIN_PTS, neigh_counts);
			futhark_free_i64_1d(ctx, neigh_counts);

			struct futhark_opaque_isolated_core_pts_5d_f64 *only_core_pts;
			futhark_entry_isolate_core_pts_5d_f64(ctx, &only_core_pts, ind_is_core, indexed_dat, partInfo);

			struct futhark_opaque_part_core_info *coreInfo;
			futhark_entry_get_part_core_info_5d_f64(ctx, &coreInfo, only_core_pts, indexed_dat);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "3. Find clusters among core points.");

			struct futhark_i64_1d *core_cid;
			futhark_entry_mk_clusters_5d_f64(ctx, &core_cid, SEED_COUNT, EPS, partInfo, only_core_pts, coreInfo);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "4. Assign cluster id's to border points.");

			struct futhark_i64_1d *all_ids;
			futhark_entry_assign_cluster_ids_5d_f64(
				ctx, &all_ids,
				SEED_COUNT, EPS,
				indexed_dat, ind_is_core, partInfo_bd,
				only_core_pts, coreInfo, core_cid
			);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "5. De-index results.");

			struct futhark_opaque_dbscan_result *dbscan_res;
			futhark_entry_deindex_results_5d(ctx, &dbscan_res, indexed_dat, ind_is_core, all_ids);

			futhark_context_sync(ctx);
			mylog(logfile, "Synced futhark context.");

			mylog(logfile, "Cleanup...");

			futhark_free_opaque_indexed_data_5d_f64(ctx, indexed_dat);
			futhark_free_opaque_isolated_core_pts_5d_f64(ctx, only_core_pts);
			futhark_free_opaque_part_core_info(ctx, coreInfo);
			futhark_free_opaque_partition_info_f64(ctx, partInfo);
			futhark_free_opaque_partition_info_f64(ctx, partInfo_bd);
			futhark_free_i64_1d(ctx,core_cid);
			futhark_free_i64_1d(ctx,all_ids);
			futhark_free_bool_1d(ctx,ind_is_core);

			futhark_context_sync(ctx);
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