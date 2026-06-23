// Benchmark sort & join for tables R_tbl, S_tbl using duckdb & futhark.
// Assume that keys and payload data are the same type, both tables have same number of payload columns.
// Assume R can be entirely read in one input, S might require multiple.

// keys are string data
// hash and then do SMJ
// pL are i32

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

#define default_R_size 1000000
#define default_S_size 1000000
#define default_S_buff 1000000

#define default_AVG_LEN 9

#define k_name "k"

#define default_NUM_PL 2

#define default_ITER 1

#define default_HASH_BYTES 4

#define default_LOGFILE "stdout"
#define default_DBFILE "datasets/US_Baby_Names.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

    	int64_t ITER = default_ITER;

    	int64_t R_size = default_R_size;
    	int64_t S_size = default_S_size;
    	int64_t S_buff = default_S_buff;

    	int64_t AVG_LEN = default_AVG_LEN;

    	bool async = false;
    	bool outer = false;
    	bool skip_fetching = false;

    	int64_t NUM_PL = default_NUM_PL;

    	int64_t HASH_BYTES = default_HASH_BYTES;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"R_size", required_argument, 0, 'R'},
			{"S_size", required_argument, 0, 'S'},
			{"S_buff", required_argument, 0, 's'},
			{"num_pL", required_argument, 0, 'p'},
			{"assume_strlen", required_argument, 0, 'l'},
			{"hash_bytes", required_argument, 0, 'h'},
			{"async", no_argument, 0, 'a'},
			{"outer", no_argument, 0, 'o'},
			{"from_disk", no_argument, 0, 'F'},
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"I:R:S:s:p:h:l:aoFL:f:",long_options,NULL)) != -1
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
	      	case 'h':
	      		HASH_BYTES = atol(optarg); break;
	      	case 'l':
	      		AVG_LEN = atol(optarg); break;
	      	case 'a':
	      		async = true; break;
	      	case 'o':
	      		outer = true; break;
	      	case 'F':
	      		skip_fetching = true; break;
	        case 'L':
	        	memcpy(LOGFILE, optarg, strlen(optarg)+1); break;
	        case 'f':
	        	memcpy(DBFILE, optarg, strlen(optarg)+1); break;
	      }
	    }

	// init logger

		FILE* logfile = loginit(LOGFILE, "Starting program to evaluate duckdb-futhark SMJ (string keys).");
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

	  	if(!skip_fetching) {
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
	 	}

	for(int64_t cur_iter=0; cur_iter<ITER; cur_iter++) {
		if(ITER>1) {
			char iter_str[200];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		char *R_contents = malloc(R_size*(AVG_LEN+1)*sizeof(char));
		char *S_contents = malloc(S_buff*(AVG_LEN+1)*sizeof(char));

		int32_t *R_buffs[NUM_PL];
		int32_t *S_buffs[NUM_PL];
		for(int64_t col=0; col<NUM_PL; col++) {
			R_buffs[col] = malloc(R_size*sizeof(int32_t));
			S_buffs[col] = malloc(S_buff*sizeof(int32_t));
		}
		duckdb_result res_R, res_S;

		// 1. Perform queries to read tables.
			char query_read_R[250 + strlen(R_name)];
			if(!skip_fetching)
				sprintf(query_read_R, "SELECT * FROM %s_tmp;", R_name);
			else
				sprintf(query_read_R, "SELECT * FROM %s LIMIT %ld;", R_name, R_size);
			if(duckdb_query(con, query_read_R, &res_R) == DuckDBError) {
		  		perror("Failed to perform SELECT query on R.\n");
		  		perror(query_read_R);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on R.");

			char query_read_S[250 + strlen(S_name)];
			if(!skip_fetching)
				sprintf(query_read_S, "SELECT * FROM %s_tmp;", S_name);
			else
				sprintf(query_read_S, "SELECT * FROM %s LIMIT %ld;", S_name, S_size);
			if(duckdb_query(con, query_read_S, &res_S) == DuckDBError) {
		  		perror("Failed to perform SELECT query on S.\n");
		  		perror(query_read_S);
		  		return -1;
		  	}
		  	mylog(logfile, "Performed SELECT query on S.");

	  	// 2. Read and sort R
		  	int64_t cur_row_R = 0;
		  	int64_t cur_R_len = 0;
		  	mylog(logfile, "Now scannning R");
		  	while(true) {
		  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res_R);
		  		if(!cnk) {
		  			mylog(logfile, "Result is exhausted (R).");
		  			break;
		  		}
		  		int64_t this_rows = duckdb_data_chunk_get_size(cnk);

		  		// Scan string keys
		  		duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk,0);
		  		duckdb_string_t *kdat = (duckdb_string_t *)duckdb_vector_get_data(kvec);
		  		for(int64_t row=0; row<this_rows; row++) {
		  			duckdb_string_t str = kdat[row];
		  			if(duckdb_string_is_inlined(str)) {
		  				cur_R_len += sprintf(
	  						R_contents+cur_R_len, "%.*s",
	  						str.value.inlined.length, str.value.inlined.inlined
	  					);
	  					cur_R_len += sprintf(R_contents+cur_R_len," ");
		  			} else {
		  				cur_R_len += sprintf(
	  						R_contents+cur_R_len, "%.*s",
	  						str.value.pointer.length, str.value.pointer.ptr
	  					);
	  					cur_R_len += sprintf(R_contents+cur_R_len," ");
		  			}
		  		}

		  		for(int64_t col=0; col<NUM_PL; col++) {
		  			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,1+col);
		  			int32_t *dat = duckdb_vector_get_data(vec);
		  			memcpy(R_buffs[col] + cur_row_R, dat, this_rows*sizeof(int32_t));
		  		}

		  		cur_row_R += this_rows;
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_destroy_result(&res_R);
		  	cur_R_len -= 1;

		  	struct futhark_opaque_strInfo *R_superstring;
		  	struct futhark_u8_1d *delim = futhark_new_u8_1d(ctx, " ", 1);
		  	struct futhark_u8_1d *ft_R_contents= futhark_new_u8_1d(ctx, R_contents, cur_R_len);
		  	futhark_entry_str_split(ctx, &R_superstring, delim, ft_R_contents);
		  	futhark_free_u8_1d(ctx, ft_R_contents);
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Wrapped R's string key data into futhark context.");

		  	struct futhark_i32_1d *ft_R_buffs[NUM_PL];
		  	for(int64_t col=0; col<NUM_PL; col++) {
		  		ft_R_buffs[col] = futhark_new_i32_1d(ctx, R_buffs[col], cur_row_R);
		  	}
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Wrapped R's payload data into futhark context.");

		  	struct futhark_u8_2d *R_hashed_ks;
		  	futhark_entry_str_hash(ctx, &R_hashed_ks, false, true, 1, 0, 1, 0, HASH_BYTES, R_superstring);
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Hashed R's strings.");

		  	struct futhark_opaque_sortInfo_bsq *R_hash_sortInfo;
		  	struct futhark_u8_2d *R_sorted_ks;
		  	struct futhark_i64_1d *R_sorted_is;
		  	futhark_entry_sort_hashes(ctx, &R_hash_sortInfo, R_hashed_ks);
		  	futhark_project_opaque_sortInfo_bsq_is(ctx, &R_sorted_is, R_hash_sortInfo);
		  	futhark_project_opaque_sortInfo_bsq_ks(ctx, &R_sorted_ks, R_hash_sortInfo);
		  	futhark_free_opaque_sortInfo_bsq(ctx, R_hash_sortInfo);
		  	if(!async) futhark_context_sync(ctx);
		  	mylog(logfile, "Sorted R's hashed strings.");

		// 3. Iterate over S
			bool is_S_exhausted = false;
			mylog(logfile, "Now scannning S and performing the join...");
			while(!is_S_exhausted) {

				// 3.1 Read S until it fills the buffer & Sort S buffer
					int64_t cur_row_S = 0;
					int64_t cur_S_len = 0;
					mylog(logfile, "Starting new scan cycle...");
					while(cur_row_S<S_buff && !is_S_exhausted) {
						duckdb_data_chunk cnk = duckdb_fetch_chunk(res_S);
				  		if(!cnk) {
				  			mylog(logfile, "Result is exhausted (S).");
				  			is_S_exhausted = true;
				  			break;
				  		}
				  		int64_t this_rows = duckdb_data_chunk_get_size(cnk);

				  		// Scan string keys
				  		duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk,0);
				  		duckdb_string_t *kdat = (duckdb_string_t *)duckdb_vector_get_data(kvec);
				  		for(int64_t row=0; row<this_rows; row++) {
				  			duckdb_string_t str = kdat[row];
				  			if(duckdb_string_is_inlined(str)) {
				  				cur_S_len += sprintf(
			  						S_contents+cur_S_len, "%.*s",
			  						str.value.inlined.length, str.value.inlined.inlined
			  					);
				  			} else {
				  				cur_S_len += sprintf(
			  						S_contents+cur_S_len, "%.*s",
			  						str.value.pointer.length, str.value.pointer.ptr
			  					);
				  			}
				  			cur_S_len += sprintf(S_contents+cur_S_len," ");
				  		}

				  		for(int64_t col=0; col<NUM_PL; col++) {
				  			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,1+col);
				  			int32_t *dat = duckdb_vector_get_data(vec);
				  			memcpy(S_buffs[col] + cur_row_S, dat, this_rows*sizeof(int32_t));
				  		}

				  		cur_row_S += this_rows;
				  		duckdb_destroy_data_chunk(&cnk);
					}
					cur_S_len -= 1;
					if(is_S_exhausted) duckdb_destroy_result(&res_S);
					mylog(logfile, "Current scan cycle finished.");

					struct futhark_opaque_strInfo *S_superstring;
				  	struct futhark_u8_1d *ft_S_contents= futhark_new_u8_1d(ctx, S_contents, cur_S_len);
				  	futhark_entry_str_split(ctx, &S_superstring, delim, ft_S_contents);
				  	futhark_free_u8_1d(ctx, ft_S_contents);
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Wrapped S buffer's string key data into futhark context.");

				  	struct futhark_i32_1d *ft_S_buffs[NUM_PL];
				  	for(int64_t col=0; col<NUM_PL; col++) {
				  		ft_S_buffs[col] = futhark_new_i32_1d(ctx, S_buffs[col], cur_row_S);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Wrapped S buffer's payload data into futhark context.");

				  	struct futhark_u8_2d *S_hashed_ks;
				  	futhark_entry_str_hash(ctx, &S_hashed_ks, false, true, 1, 0, 1, 0, HASH_BYTES, S_superstring);
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Hashed S buffer's strings.");

				  	struct futhark_opaque_sortInfo_bsq *S_hash_sortInfo;
				  	struct futhark_u8_2d *S_sorted_ks;
				  	struct futhark_i64_1d *S_sorted_is;
				  	futhark_entry_sort_hashes(ctx, &S_hash_sortInfo, S_hashed_ks);
				  	futhark_project_opaque_sortInfo_bsq_is(ctx, &S_sorted_is, S_hash_sortInfo);
				  	futhark_project_opaque_sortInfo_bsq_ks(ctx, &S_sorted_ks, S_hash_sortInfo);
				  	futhark_free_opaque_sortInfo_bsq(ctx, S_hash_sortInfo);
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Sorted S buffer's hashed strings.");		  	


				// 3.2 Perform Join

				  	mylog(logfile, "Performing Inner Equi-Join (SMJ) on key columns...");
				  	struct futhark_opaque_joinPairs_str *joinRes;
					futhark_entry_innerHSMJ_str(
						ctx, &joinRes,
						false, (cur_row_R+cur_row_S+8191)/8192,
						R_superstring, S_superstring,
						R_sorted_ks, S_sorted_ks,
						R_sorted_is, S_sorted_is
					);

					if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Completed Inner Join.");

				  	if(outer) {
				  		mylog(logfile, "Expanding to Full Outer Join...");
						struct futhark_i64_1d *ix;
						struct futhark_i64_1d *iy;

						futhark_project_opaque_joinPairs_str_ix(ctx, &ix, joinRes);
						futhark_project_opaque_joinPairs_str_iy(ctx, &iy, joinRes);
						futhark_free_opaque_joinPairs_str(ctx,joinRes);

						// Un-sort joinRes
						struct futhark_opaque_strInfo *vs2;
						struct futhark_i64_1d *ix2;
				  		struct futhark_i64_1d *iy2;
					  	futhark_entry_gather_i64(ctx, &ix2, R_sorted_is, ix);
					  	futhark_entry_gather_i64(ctx, &iy2, S_sorted_is, iy);
					  	futhark_free_i64_1d(ctx,ix);
						futhark_free_i64_1d(ctx,iy);
						futhark_entry_str_gather(ctx, &vs2, R_superstring, ix2);
						
						struct futhark_opaque_joinPairs_str *joinRes2;
						futhark_new_opaque_joinPairs_str(ctx, &joinRes2, ix2, iy2, vs2);
						futhark_free_i64_1d(ctx,ix2);
						futhark_free_i64_1d(ctx,iy2);
						futhark_free_opaque_strInfo(ctx,vs2);

					  	struct futhark_opaque_joinPairs_str *outer_joinRes;
						futhark_entry_inner_to_full_outer_str(ctx, &outer_joinRes, R_superstring, S_superstring, joinRes2);

						futhark_free_opaque_joinPairs_str(ctx,joinRes2);
						joinRes = outer_joinRes;

						if(!async) futhark_context_sync(ctx);
				  		mylog(logfile, "Completed Full Outer Join.");
				  	}

					struct futhark_opaque_strInfo *vs;
					struct futhark_i64_1d *ix;
					struct futhark_i64_1d *iy;

					futhark_project_opaque_joinPairs_str_strs(ctx, &vs, joinRes);
					futhark_project_opaque_joinPairs_str_ix(ctx, &ix, joinRes);
					futhark_project_opaque_joinPairs_str_iy(ctx, &iy, joinRes);

					futhark_free_opaque_joinPairs_str(ctx,joinRes);

				  	// Print shape of ix to confirm correctness.
				  	printf("\n\nOutput size: %ld\n\n\n", *(futhark_shape_i64_1d(ctx, ix)));

				// 3.3 Gather Payloads
				  	mylog(logfile, "Materializing payload data...");

				  	struct futhark_i64_1d *R_pL_is;
				  	struct futhark_i64_1d *S_pL_is;

				  	struct futhark_i32_1d *R_pL[NUM_PL];
				  	struct futhark_i32_1d *S_pL[NUM_PL];

				  	futhark_entry_gather_i64(ctx, &R_pL_is, R_sorted_is, ix);
				  	futhark_entry_gather_i64(ctx, &S_pL_is, S_sorted_is, iy);
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Gathered intermediate indices.");

				  	for(int64_t col=0; col<NUM_PL; col++) {
				  		futhark_entry_gather_i32(ctx, &R_pL[col], ft_R_buffs[col], R_pL_is);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Gathered R's payloads.");
				  	
				  	
				  	for(int64_t col=0; col<NUM_PL; col++) {
				  		futhark_entry_gather_i32(ctx, &S_pL[col], ft_S_buffs[col], S_pL_is);
				  		futhark_free_i32_1d(ctx, ft_S_buffs[col]);
				  	}
				  	if(!async) futhark_context_sync(ctx);
				  	mylog(logfile, "Gathered S's payloads.");

				// 3.4 Cleanup
				futhark_free_opaque_strInfo(ctx,vs);
				futhark_free_i64_1d(ctx,ix);
				futhark_free_i64_1d(ctx,iy);
				
				futhark_free_u8_2d(ctx,S_sorted_ks);
				futhark_free_opaque_strInfo(ctx,S_superstring);
				futhark_free_i64_1d(ctx, S_sorted_is);

				futhark_free_i64_1d(ctx, R_pL_is);
				futhark_free_i64_1d(ctx, S_pL_is);
				for(int64_t col=0; col<NUM_PL; col++) {
					futhark_free_i32_1d(ctx, R_pL[col]);
					futhark_free_i32_1d(ctx, S_pL[col]);
				}
			}

		// 4. Cleanup

			futhark_free_u8_1d(ctx, delim);
			futhark_free_u8_2d(ctx,R_sorted_ks);
			futhark_free_opaque_strInfo(ctx,R_superstring);
			futhark_free_i64_1d(ctx, R_sorted_is);
			for(int64_t col=0; col<NUM_PL; col++) {
		  		futhark_free_i32_1d(ctx, ft_R_buffs[col]);
		  	}
		  	free(R_contents);
		  	free(S_contents);
		  	for(int64_t col=0; col<NUM_PL; col++) {
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