// Benchmark sort & join for tables R_tbl, S_tbl using duckdb & futhark.
// Assume that keys and payload data are the same type, both tables have same number of payload columns.
// Assume R can be entirely read in one input, S might require multiple.

// k and pL are i64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"

#include "../../ft_clibs/join_entry.h"

#include <unistd.h>
#include <getopt.h>

#define R_name "R_tbl"
#define S_name "S_tbl"

#define default_R_size 8192
#define default_S_size 32768
#define default_S_buff 16384

#define k_name "k"

#define default_NUM_PL 2

#define default_ITER 1

#define default_LOGFILE "stdout"
#define default_DBFILE "testdb.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

    	int64_t ITER = default_ITER;

    	int64_t R_size = default_R_size;
    	int64_t S_size = default_S_size;
    	int64_t S_buff = default_S_buff;

    	bool async = false;
    	bool outer = false;

    	int64_t NUM_PL = default_NUM_PL;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"R_size", required_argument, 0, 'R'},
			{"S_size", required_argument, 0, 'S'},
			{"S_buff", required_argument, 0, 's'},
			{"num_pL", required_argument, 0, 'p'},
			{"async", no_argument, 0, 'a'},
			{"outer", no_argument, 0, 'o'},
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"I:R:S:s:p:aoL:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	      	case 'I':
	      		ITER = atol(optarg); break;
	      	case 'R':
	      		R_size = atol(optarg); break;
	      	case 'S':
	      		S_size = atol(optarg); break;
	      	case 's':
	      		S_buff = atol(optarg); break;
	      	case 'p':
	      		NUM_PL = atol(optarg); break;
	      	case 'a':
	      		async = true; break;
	      	case 'o':
	      		outer = true; break;
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

	   	if(async) mylog(logfile, "-- # ----- Asynchronous execution.");
	   	else mylog(logfile, "-- # ----- Synchronized exection (context is blocked between routines).");

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

	// Fetch tables into memory;

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
			char iter_str[200];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		int64_t *R_buffs[1+NUM_PL];
		int64_t *S_buffs[1+NUM_PL];
		for(int64_t col=0; col<=NUM_PL; col++) {
			R_buffs[col] = malloc(R_size*sizeof(int64_t));
			S_buffs[col] = malloc(S_buff*sizeof(int64_t));
		}
		duckdb_result res_R, res_S;

		// 1. Perform queries to read tables.
			char query_read_R[250 + strlen(R_name)];
			sprintf(query_read_R, "SELECT * FROM %s_tmp;", R_name);
			if(duckdb_query(con, query_read_R, &res_R) == DuckDBError) {
		  		perror("Failed to perform SELECT query on R.\n");
		  		perror(query_read_R);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on R.");

			char query_read_S[250 + strlen(S_name)];
			sprintf(query_read_S, "SELECT * FROM %s_tmp;", S_name);
			if(duckdb_query(con, query_read_S, &res_S) == DuckDBError) {
		  		perror("Failed to perform SELECT query on S.\n");
		  		perror(query_read_S);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on S.");

	  	// 2. Read and sort R
		  	int64_t cur_row_R = 0;
		  	mylog(logfile, "Now scannning R");
		  	while(true) {
		  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res_R);
		  		if(!cnk) {
		  			mylog(logfile, "Result is exhausted (R).");
		  			break;
		  		}
		  		int64_t this_rows = duckdb_data_chunk_get_size(cnk);

		  		for(int64_t col=0; col<=NUM_PL; col++) {
		  			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
		  			int64_t *dat = duckdb_vector_get_data(vec);
		  			memcpy(R_buffs[col] + cur_row_R, dat, this_rows*sizeof(int64_t));
		  		}

		  		cur_row_R += this_rows;
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_destroy_result(&res_R);

		  	struct futhark_i64_1d *ft_R_buffs[1+NUM_PL];
		  	for(int64_t col=0; col<=NUM_PL; col++) {
		  		ft_R_buffs[col] = futhark_new_i64_1d(ctx, R_buffs[col], cur_row_R);
		  	}
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Wrapped R's data into futhark context.");

		  	struct futhark_opaque_sortInfo_i64 *R_sortInfo;
		  	struct futhark_i64_1d *R_sorted_ks;
		  	struct futhark_i64_1d *R_sorted_is;
		  	futhark_entry_radix_sort_i64_GFUR(ctx, &R_sortInfo, ft_R_buffs[0]);
		  	futhark_free_i64_1d(ctx, ft_R_buffs[0]);
		  	futhark_project_opaque_sortInfo_i64_ks(ctx, &R_sorted_ks, R_sortInfo);
		  	futhark_project_opaque_sortInfo_i64_is(ctx, &R_sorted_is, R_sortInfo);
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Sorted R's keys and projected fields.");

		// 3. Iterate over S
			bool is_S_exhausted = false;
			mylog(logfile, "Now scannning S and performing the join...");
			while(!is_S_exhausted) {
				// 3.1 Read S until it fills the buffer & Sort S buffer
					int64_t cur_row_S = 0;
					mylog(logfile, "Starting new scan cycle...");
					while(cur_row_S<S_buff && !is_S_exhausted) {
						duckdb_data_chunk cnk = duckdb_fetch_chunk(res_S);
				  		if(!cnk) {
				  			mylog(logfile, "Result is exhausted (S).");
				  			is_S_exhausted = true;
				  			break;
				  		}
				  		int64_t this_rows = duckdb_data_chunk_get_size(cnk);

				  		for(int64_t col=0; col<=NUM_PL; col++) {
				  			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
				  			int64_t *dat = duckdb_vector_get_data(vec);
				  			memcpy(S_buffs[col] + cur_row_S, dat, this_rows*sizeof(int64_t));
				  		}

				  		cur_row_S += this_rows;
				  		duckdb_destroy_data_chunk(&cnk);
					}
					if(is_S_exhausted) duckdb_destroy_result(&res_S);
					mylog(logfile, "Current scan cycle finished.");

					struct futhark_i64_1d *ft_S_buffs[1+NUM_PL];
				  	for(int64_t col=0; col<=NUM_PL; col++) {
				  		ft_S_buffs[col] = futhark_new_i64_1d(ctx, S_buffs[col], cur_row_S);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Wrapped S buffer's data into futhark context.");

				  	struct futhark_opaque_sortInfo_i64 *S_sortInfo;
				  	struct futhark_i64_1d *S_sorted_ks;
				  	struct futhark_i64_1d *S_sorted_is;
				  	futhark_entry_radix_sort_i64_GFUR(ctx, &S_sortInfo, ft_S_buffs[0]);
				  	futhark_free_i64_1d(ctx, ft_S_buffs[0]);
				  	futhark_project_opaque_sortInfo_i64_ks(ctx, &S_sorted_ks, S_sortInfo);
				  	futhark_project_opaque_sortInfo_i64_is(ctx, &S_sorted_is, S_sortInfo);
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Sorted S's keys and projected fields.");


				// 3.2 Perform Join

				  	mylog(logfile, "Performing Inner Equi-Join (SMJ) on key columns...");
				  	struct futhark_opaque_joinPairs_i64 *joinRes;
					futhark_entry_innerSMJ_i64(ctx, &joinRes, R_sorted_ks, S_sorted_ks);

					struct futhark_i64_1d *vs;
					struct futhark_i64_1d *ix;
					struct futhark_i64_1d *iy;

					if(outer) {
						if(!async) futhark_context_sync(ctx);
						mylog(logfile, "Expanding to Full Outer Join...");

						struct futhark_opaque_joinPairs_i64 *outer_joinRes;
						futhark_entry_fullOuterJoin_i64(ctx, &outer_joinRes, R_sorted_ks, S_sorted_ks, joinRes);

						futhark_free_opaque_joinPairs_i64(ctx,joinRes);
						joinRes = outer_joinRes;
					}

					futhark_project_opaque_joinPairs_i64_vs(ctx, &vs, joinRes);
					futhark_project_opaque_joinPairs_i64_ix(ctx, &ix, joinRes);
					futhark_project_opaque_joinPairs_i64_iy(ctx, &iy, joinRes);

					futhark_free_i64_1d(ctx,R_sorted_ks);
					futhark_free_i64_1d(ctx,S_sorted_ks);
					futhark_free_opaque_joinPairs_i64(ctx,joinRes);

					if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Completed current Join cycle and projected fields.");


				// 3.3 Gather Payloads
				  	mylog(logfile, "Materializing payload data...");

				  	struct futhark_i64_1d *R_pL_is;
				  	struct futhark_i64_1d *S_pL_is;

				  	struct futhark_i64_1d *R_pL[NUM_PL];
				  	struct futhark_i64_1d *S_pL[NUM_PL];

				  	futhark_entry_gather_i64(ctx, &R_pL_is, R_sorted_is, ix);
				  	for(int64_t col=0; col<NUM_PL; col++) {
				  		futhark_entry_gather_i64(ctx, &R_pL[col], ft_R_buffs[col+1], R_pL_is);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Gathered R's payloads.");
				  	
				  	futhark_entry_gather_i64(ctx, &S_pL_is, S_sorted_is, iy);
				  	for(int64_t col=0; col<NUM_PL; col++) {
				  		futhark_entry_gather_i64(ctx, &S_pL[col], ft_S_buffs[col+1], S_pL_is);
				  		futhark_free_i64_1d(ctx, ft_S_buffs[col+1]);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Gathered S's payloads.");

				// 3.4 Cleanup
				futhark_free_i64_1d(ctx,vs);
				futhark_free_i64_1d(ctx,ix);
				futhark_free_i64_1d(ctx,iy);
				
				futhark_free_i64_1d(ctx, S_sorted_ks);
				futhark_free_i64_1d(ctx, S_sorted_is);

				futhark_free_i64_1d(ctx, R_pL_is);
				futhark_free_i64_1d(ctx, S_pL_is);
				for(int64_t col=0; col<NUM_PL; col++) {
					futhark_free_i64_1d(ctx, R_pL[col]);
					futhark_free_i64_1d(ctx, S_pL[col]);
				}
			}

		// 4. Cleanup

			futhark_free_i64_1d(ctx, R_sorted_ks);
			futhark_free_i64_1d(ctx, R_sorted_is);
			for(int64_t col=0; col<NUM_PL; col++) {
		  		futhark_free_i64_1d(ctx, ft_R_buffs[col+1]);
		  	}
		  	for(int64_t col=0; col<=NUM_PL; col++) {
		  		free(R_buffs[col]);
		  		free(S_buffs[col]);
		  	}

		  	mylog(logfile, "Finished iteration and performed cleanup...");
	}
	mylog(logfile, "Completed duckdb-futhark SMJ.");

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