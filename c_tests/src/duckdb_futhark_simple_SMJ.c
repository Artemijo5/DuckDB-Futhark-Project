// Benchmark sort & join for tables R_tbl, S_tbl using duckdb & futhark.

// TODO currently assuming k and pL are i32
// Change to allow i64... or make separate script with i64...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"

#include "../../ft_clibs/join_entry.h"

#include <unistd.h>
#include <getopt.h>

#define R_name "R_tbl"
#define S_name "S_tbl"

#define default_R_size 8192
#define default_S_size 16384

#define k_name "k"

#define default_ITER 1

#define default_LOGFILE "stdout"
#define default_DBFILE "testdb.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

    	int64_t ITER = default_ITER;

    	int64_t R_size = default_R_size;
    	int64_t S_size = default_S_size;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"R_size", required_argument, 0, 'R'},
			{"S_size", required_argument, 0, 'S'},
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"I:R:S:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	      	case 'I':
	      		ITER = atol(optarg); break;
	      	case 'R':
	      		R_size = atol(optarg); break;
	      	case 'S':
	      		S_size = atol(optarg); break;
	        case 'L':
	        	memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
	        case 'f':
	        	memcpy(DBFILE, optarg, strlen(optarg)+1); break;
	      }
	    }

	// init logger

		FILE* logfile = loginit(LOGFILE, "Starting program to evaluate duckdb-futhark simple SMJ.");
	    if(LOGFILE && !logfile) {
	      perror("Failed to initialise logger.\n");
	      return -1;
	    }

	// make duckdb connection

	    duckdb_database db;
	  	duckdb_connection con;

	  	if (duckdb_open(DBFILE, &db) == DuckDBError) {
	  		perror("Failed to open database.\n");
	  		return -1;
	  	}
	  	if (duckdb_connect(db, &con) == DuckDBError) {
	  		perror("Failed to connect to database.\n");
	  		return -1;
	  	}

	  	mylog(logfile, "Connected to duckdb database.");

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

	  	char fetch_R[2*strlen(R_name) + 250];
		char fetch_S[2*strlen(S_name) + 250];

		sprintf(fetch_R,"CREATE OR REPLACE TEMP TABLE %s_tmp AS (FROM %s LIMIT %ld);", R_name, R_name, R_size);
	  	if(duckdb_query(con, fetch_R, NULL) == DuckDBError) {
	  		perror("Failed to fetch R into memory.\n");
	  		perror(fetch_R);
	  		return -1;
	  	}

	  	sprintf(fetch_S,"CREATE OR REPLACE TEMP TABLE %s_tmp AS (FROM %s LIMIT %ld);", S_name, S_name, S_size);
	  	if(duckdb_query(con, fetch_S, NULL) == DuckDBError) {
	  		perror("Failed to fetch S into memory.\n");
	  		perror(fetch_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Fetched tables into memory.");

	for(int64_t cur_iter=0; cur_iter<ITER; cur_iter++) {
		if(ITER>1) {
			char iter_str[100];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		// Allocate strings for the queries.

		  	char select_R[strlen(R_name) + 250];
		  	char select_S[strlen(S_name) + 250];

		  	char output_tbl[2*strlen(R_name)+2*strlen(S_name)+3*strlen(k_name)+250];

		// Scan R table

		  	duckdb_result res_R;
		  	sprintf(select_R,"FROM %s_tmp;", R_name);
		  	if(duckdb_query(con, select_R, &res_R) == DuckDBError) {
		  		perror("Failed to perform SELECT query on R.\n");
		  		perror(select_R);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on R.");

		  	int64_t num_pL = duckdb_column_count(&res_R)-1; // For these scripts, R & S have the same pL.

		  	duckdb_type k_type = DUCKDB_TYPE_INTEGER;
		  	duckdb_type pL_type = DUCKDB_TYPE_INTEGER;
		  	duckdb_logical_type k_ltype = duckdb_create_logical_type(k_type);
		  	duckdb_logical_type pL_ltype = duckdb_create_logical_type(pL_type);

		  	struct futhark_i32_2d *R_keyCol;
		  	futhark_entry_mk_col_i32(ctx, &R_keyCol, R_size);
		  	struct futhark_i32_2d *R_pL_cols[num_pL];
		  	for(int64_t col=0; col<num_pL; col++) {
		  		futhark_entry_mk_col_i32(ctx, &(R_pL_cols[col]), R_size);
		  	}

		  	mylog(logfile, "Now scanning R...");
		  	// Read elements from R, construct key column & payload columns.
		  	int64_t cur_row_R=0;
		  	while(true) {
		  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res_R);
		  		if(!cnk) {
		  			mylog(logfile, "Result is exhausted.");
		  			break;
		  		}
		  		int64_t this_sz = duckdb_data_chunk_get_size(cnk);

		  		duckdb_vector vec1 = duckdb_data_chunk_get_vector(cnk,0);
		  		int32_t *dat1 = (int32_t*)duckdb_vector_get_data(vec1);
		  		struct futhark_i32_1d *ft_dat1 = futhark_new_i32_1d(ctx, dat1, this_sz);
		  		// Due to consumption, need to rearrange pointers.
				struct futhark_f64_1d *keyCol_tmp;
				futhark_entry_update_col_i32(ctx, &keyCol_tmp, cur_row_R, ft_dat1, R_keyCol);
				futhark_free_i32_1d(ctx, R_keyCol);
				futhark_free_i32_1d(ctx, ft_dat1);
				R_keyCol = keyCol_tmp;

				for(int64_t col=1; col<num_pL+1; col++) {
					duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
			  		int32_t *dat = (int32_t*)duckdb_vector_get_data(vec);
			  		struct futhark_i32_1d *ft_dat = futhark_new_i32_1d(ctx, dat, this_sz);
			  		// Due to consumption, need to rearrange pointers.
					struct futhark_f64_1d *col_tmp;
					futhark_entry_update_col_i32(ctx, &col_tmp, cur_row_R, ft_dat, R_pL_cols[col+1]);
					futhark_free_i32_1d(ctx, R_pL_cols[col+1]);
					futhark_free_i32_1d(ctx, ft_dat);
					R_pL_cols[col+1] = col_tmp;
				}

		  		duckdb_destroy_data_chunk(&cnk);
		  		cur_row_R += this_sz;
		  	}
		  	duckdb_destroy_result(&res_R);

		// Scan S table

		  	duckdb_result res_S;
		  	sprintf(select_S,"FROM %s_tmp;", S_name);
		  	if(duckdb_query(con, select_S, &res_S) == DuckDBError) {
		  		perror("Failed to perform SELECT query on S.\n");
		  		perror(select_S);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on S.");

		  	struct futhark_i32_2d *S_keyCol;
		  	futhark_entry_mk_col_i32(ctx, &S_keyCol, S_size);
		  	struct futhark_i32_2d *S_pL_cols[num_pL];
		  	for(int64_t col=0; col<num_pL; col++) {
		  		futhark_entry_mk_col_i32(ctx, &(S_pL_cols[col]), S_size);
		  	}

		  	mylog(logfile, "Now scanning S...");
		  	// Read elements from S, construct key column & payload columns.
		  	int64_t cur_row_S=0;
		  	while(true) {
		  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res_S);
		  		if(!cnk) {
		  			mylog(logfile, "Result is exhausted.");
		  			break;
		  		}
		  		int64_t this_sz = duckdb_data_chunk_get_size(cnk);

		  		duckdb_vector vec1 = duckdb_data_chunk_get_vector(cnk,0);
		  		int32_t *dat1 = (int32_t*)duckdb_vector_get_data(vec1);
		  		struct futhark_i32_1d *ft_dat1 = futhark_new_i32_1d(ctx, dat1, this_sz);
		  		// Due to consumption, need to rearrange pointers.
				struct futhark_f64_1d *keyCol_tmp;
				futhark_entry_update_col_i32(ctx, &keyCol_tmp, cur_row_S, ft_dat1, S_keyCol);
				futhark_free_i32_1d(ctx, S_keyCol);
				futhark_free_i32_1d(ctx, ft_dat1);
				S_keyCol = keyCol_tmp;

				for(int64_t col=1; col<num_pL+1; col++) {
					duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
			  		int32_t *dat = (int32_t*)duckdb_vector_get_data(vec);
			  		struct futhark_i32_1d *ft_dat = futhark_new_i32_1d(ctx, dat, this_sz);
			  		// Due to consumption, need to rearrange pointers.
					struct futhark_f64_1d *col_tmp;
					futhark_entry_update_col_i32(ctx, &col_tmp, cur_row_S, ft_dat, S_pL_cols[col+1]);
					futhark_free_i32_1d(ctx, S_pL_cols[col+1]);
					futhark_free_i32_1d(ctx, ft_dat);
					S_pL_cols[col+1] = col_tmp;
				}

		  		duckdb_destroy_data_chunk(&cnk);
		  		cur_row_S += this_sz;
		  	}
		  	duckdb_destroy_result(&res_S);

		// Sort tables.

		  	struct futhark_opaque_sortInfo_i32 *R_sortRes;
		  	futhark_entry_radix_sort_i32_GFUR(ctx, &R_sortRes, R_keyCol);
		  	futhark_free_i32_1d(ctx, R_keyCol);
		  	struct futhark_i64_1d *sorted_R_is;
		  	struct futhark_i32_1d *sorted_R_ks;
		  	futhark_project_opaque_sortInfo_i32_is(ctx, &sorted_R_is, R_sortRes);
		  	futhark_project_opaque_sortInfo_i32_ks(ctx, &sorted_R_ks, R_sortRes);
		  	futhark_free_opaque_sortInfo_i32(ctx,R_sortRes);
		  	mylog(logfile, "Sorted R.");

		  	struct futhark_opaque_sortInfo_i32 *S_sortRes;
		  	futhark_entry_radix_sort_i32_GFUR(ctx, &S_sortRes, S_keyCol);
		  	futhark_free_i32_1d(ctx, S_keyCol);
		  	struct futhark_i64_1d *sorted_S_is;
		  	struct futhark_i32_1d *sorted_S_ks;
		  	futhark_project_opaque_sortInfo_i32_is(ctx, &sorted_S_is, S_sortRes);
		  	futhark_project_opaque_sortInfo_i32_ks(ctx, &sorted_S_ks, S_sortRes);
		  	futhark_free_opaque_sortInfo_i32(ctx,S_sortRes);
		  	mylog(logfile, "Sorted R.");

		// Join sorted tables

		  	struct futhark_opaque_joinPairs_i32 *joinRes;
		  	futhark_entry_innerSMJ_i32(ctx, &joinRes, sorted_R_ks, sorted_S_ks);
		  	futhark_free_i32_1d(ctx,sorted_R_ks);
		  	futhark_free_i32_1d(ctx,sorted_S_ks);
		  	mylog(logfile, "Join function returned.");

		  	// TODO continue from here...


		// Perform gather operations


		// TODO Create output table
		// and use appenders to output results
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