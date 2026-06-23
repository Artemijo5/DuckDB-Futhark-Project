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

#define CHUNK_SIZE duckdb_vector_size()

#define R_name "R_tbl"
#define S_name "S_tbl"

#define default_R_size 8192

#define k_name "k"

#define default_ITER 1

#define default_LOGFILE "stdout"
#define default_DBFILE "testdb.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

    	int64_t ITER = default_ITER;

    	int64_t R_size = default_R_size;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		bool skip_fetching = false;

		static struct option long_options[] = {
			{"R_size", required_argument, 0, 'R'},
			{"from_disk", no_argument, 0, 'F'},
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"I:R:FS:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	      	case 'I':
	      		ITER = atol(optarg); break;
	      	case 'R':
	      		R_size = atol(optarg); break;
	      	case 'F':
	      		skip_fetching = true; break;
	        case 'L':
	        	memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
	        case 'f':
	        	memcpy(DBFILE, optarg, strlen(optarg)+1); break;
	      }
	    }

	// init logger

		FILE* logfile = loginit(LOGFILE, "Starting program to evaluate runtimes for duckdb I/O.");
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

	// Fetch table into memory.

	  	char fetch_R[2*strlen(R_name) + 250];

	  	if(!skip_fetching) {
			sprintf(fetch_R,"CREATE OR REPLACE TEMP TABLE %s_tmp AS (FROM %s LIMIT %ld);", R_name, R_name, R_size);
		  	if(duckdb_query(con, fetch_R, NULL) == DuckDBError) {
		  		perror("Failed to fetch R into memory.\n");
		  		perror(fetch_R);
		  		return -1;
		  	}
	  	}
	  	mylog(logfile, "Fetched table into memory.");

	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####     Mode 1");
	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####------#####");
	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####------#####");
	for(int64_t cur_iter=0; cur_iter<ITER; cur_iter++) {
		if(ITER>1) {
			char iter_str[200];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		// Allocate strings for the queries.

		  	char select_R[strlen(R_name) + 250];

		  	char output_tbl[2*strlen(R_name)+2*strlen(S_name)+3*strlen(k_name)+250];

		// Scan R table

		  	duckdb_result res_R;
		  	if(!skip_fetching) 
		  		sprintf(select_R,"FROM %s_tmp;", R_name);
		  	else
		  		sprintf(select_R,"FROM %s LIMIT %ld;", R_name, R_size);
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

		  	struct futhark_i32_1d *R_keyCol;
		  	futhark_entry_mk_col_i32(ctx, &R_keyCol, R_size);
		  	struct futhark_i32_1d *R_pL_cols[num_pL];
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
		  		// TODO maybe try with raw
		  		struct futhark_i32_1d *ft_dat1 = futhark_new_i32_1d(ctx, dat1, this_sz);
		  		// Due to consumption, need to rearrange pointers.
				struct futhark_i32_1d *keyCol_tmp;
				futhark_entry_update_col_i32(ctx, &keyCol_tmp, cur_row_R, ft_dat1, R_keyCol);
				futhark_free_i32_1d(ctx, R_keyCol);
				futhark_free_i32_1d(ctx, ft_dat1);
				R_keyCol = keyCol_tmp;

				for(int64_t col=1; col<num_pL+1; col++) {
					duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
			  		int32_t *dat = (int32_t*)duckdb_vector_get_data(vec);
			  		// TODO maybe try with raw
			  		struct futhark_i32_1d *ft_dat = futhark_new_i32_1d(ctx, dat, this_sz);
			  		// Due to consumption, need to rearrange pointers.
					struct futhark_i32_1d *col_tmp;
					futhark_entry_update_col_i32(ctx, &col_tmp, cur_row_R, ft_dat, R_pL_cols[col-1]);
					futhark_free_i32_1d(ctx, R_pL_cols[col-1]);
					futhark_free_i32_1d(ctx, ft_dat);
					R_pL_cols[col-1] = col_tmp;
				}

		  		duckdb_destroy_data_chunk(&cnk);
		  		cur_row_R += this_sz;
		  	}
		  	duckdb_destroy_result(&res_R);

		// Sort table.

		  	struct futhark_opaque_sortInfo_i32 *R_sortRes;
		  	futhark_entry_radix_sort_i32_GFUR(ctx, &R_sortRes, R_keyCol);
		  	futhark_free_i32_1d(ctx, R_keyCol);
		  	struct futhark_i64_1d *sorted_R_is;
		  	struct futhark_i32_1d *sorted_R_ks;
		  	futhark_project_opaque_sortInfo_i32_is(ctx, &sorted_R_is, R_sortRes);
		  	futhark_project_opaque_sortInfo_i32_ks(ctx, &sorted_R_ks, R_sortRes);
		  	futhark_free_opaque_sortInfo_i32(ctx,R_sortRes);
		  	mylog(logfile, "Sorted R.");

		// Perform gather operations

		  	struct futhark_i32_1d *sorted_pL[num_pL];
		  	for (int64_t col=0; col<num_pL; col++) {
		  		futhark_entry_gather_i32(ctx, &(sorted_pL[col]), R_pL_cols[col],sorted_R_is);
		  		futhark_free_i32_1d(ctx, R_pL_cols[col]);
		  	}
		  	futhark_free_i64_1d(ctx, sorted_R_is);

		  	mylog(logfile, "Gathered R's payloads.");

		  	futhark_context_sync(ctx);
		  	mylog(logfile, "Synced futhark context.");

		// Create output table & apender
			
			int cursor_out = sprintf(output_tbl, "CREATE OR REPLACE TEMP TABLE R_out (k INTEGER");
			for(int64_t col=0; col<num_pL; col++) {
				cursor_out += sprintf(output_tbl+cursor_out, ", pL%ld INTEGER", col);
			}
			sprintf(output_tbl+cursor_out, ");");
			if(duckdb_query(con, output_tbl, NULL) == DuckDBError) {
				perror("Failed to create output table.");
				perror(output_tbl);
				return -1;
			}
			mylog(logfile, "Created output table.");

			duckdb_appender appender;
		  	if (duckdb_appender_create(con,NULL,"R_out",&appender) == DuckDBError) {
		  		perror("Failed to create appender.\n");
		  		return -1;
		  	}
		  	mylog(logfile, "Set up duckdb appender for output.");

		  	duckdb_logical_type ltypes[1+num_pL];
		  	ltypes[0] = k_ltype;
		  	for(int64_t col=0;col<num_pL;col++) {
		  		ltypes[col+1]=pL_ltype;
		  	}

		// Append chunk-by-chunk
		  	int64_t num_chunks = (cur_row_R + CHUNK_SIZE - 1) / CHUNK_SIZE;

		  	mylog(logfile, "Appending...");
		  	for(int64_t i=0; i<num_chunks; i++) {
		  		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, 1+num_pL);
		  		int64_t this_rows = (i<num_chunks-1)? CHUNK_SIZE: cur_row_R - i*CHUNK_SIZE;
		  		duckdb_data_chunk_set_size(cnk,this_rows);

		  		void *dat_k = duckdb_vector_get_data(duckdb_data_chunk_get_vector(cnk,0));
		  		void *dat_pL[num_pL];
		  		for(int64_t col=0; col<num_pL; col++) {
		  			dat_pL[col] = duckdb_vector_get_data(duckdb_data_chunk_get_vector(cnk,col+1));
		  		}

		  		struct futhark_i32_1d *ft_dat_k;
		  		struct futhark_i32_1d *ft_dat_pL[num_pL];
		  		futhark_entry_read_col_i32(ctx, &ft_dat_k, i*CHUNK_SIZE, this_rows, sorted_R_ks);
		  		for(int64_t col=0; col<num_pL; col++) {
		  			futhark_entry_read_col_i32(ctx, &(ft_dat_pL[col]), i*CHUNK_SIZE, this_rows, sorted_pL[col]);
		  		}

		  		futhark_context_sync(ctx);
		  		futhark_values_i32_1d(ctx, ft_dat_k, dat_k);
		  		futhark_free_i32_1d(ctx, ft_dat_k);
		  		for(int64_t col=0; col<num_pL; col++) {
		  			futhark_values_i32_1d(ctx, ft_dat_pL[col], dat_pL[col]);
		  			futhark_free_i32_1d(ctx, ft_dat_pL[col]);
		  		}

		  		duckdb_append_data_chunk(appender, cnk);
		  		if((i+1)%100 == 0) duckdb_appender_flush(appender);
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_appender_flush(appender);
		  	mylog(logfile, "Done appending.");
		  	duckdb_appender_destroy(&appender);

		  	futhark_free_i32_1d(ctx, sorted_R_ks);
		  	for(int64_t col=0; col<num_pL; col++) {
		  		futhark_free_i32_1d(ctx, sorted_pL[col]);
		  	}
		  	
	}


	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####     Mode 2");
	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####------#####");
	mylog(logfile, "#####------#####------#####------#####------#####------#####------#####------#####------#####------#####");
	for(int64_t cur_iter=0; cur_iter<ITER; cur_iter++) {
		if(ITER>1) {
			char iter_str[200];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		// Allocate strings for the queries.

		  	char select_R[strlen(R_name) + 250];

		  	char output_tbl[2*strlen(R_name)+2*strlen(S_name)+3*strlen(k_name)+250];

		// Scan R table

		  	duckdb_result res_R;
		  	if(!skip_fetching) 
		  		sprintf(select_R,"FROM %s_tmp;", R_name);
		  	else
		  		sprintf(select_R,"FROM %s LIMIT %ld;", R_name, R_size);
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

		  	struct futhark_i32_1d *R_keyCol;
		  	struct futhark_i32_1d *R_pL_cols[num_pL];
		  	
		  	int32_t *INPUT_BUFFS[1+num_pL];
		  	for(int64_t col=0; col<1+num_pL; col++) {
		  		INPUT_BUFFS[col] = malloc(R_size*sizeof(int32_t));
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

				for(int64_t col=0; col<num_pL+1; col++) {
					duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
			  		int32_t *dat = (int32_t*)duckdb_vector_get_data(vec);
			  		memcpy(INPUT_BUFFS[col] + cur_row_R, dat, this_sz*sizeof(int32_t));
				}

		  		duckdb_destroy_data_chunk(&cnk);
		  		cur_row_R += this_sz;
		  	}
		  	duckdb_destroy_result(&res_R);

		  	// TODO maybe try with raw
		  	R_keyCol = futhark_new_i32_1d(ctx, INPUT_BUFFS[0], cur_row_R);
		  	free(INPUT_BUFFS[0]);
		  	for(int64_t col=0; col<num_pL; col++) {
		  		R_pL_cols[col] = futhark_new_i32_1d(ctx, INPUT_BUFFS[col+1], cur_row_R);
		  		free(INPUT_BUFFS[col+1]);
		  	}
		  	mylog(logfile, "Wrapped data into futhark context.");

		// Sort table.

		  	struct futhark_opaque_sortInfo_i32 *R_sortRes;
		  	futhark_entry_radix_sort_i32_GFUR(ctx, &R_sortRes, R_keyCol);
		  	futhark_free_i32_1d(ctx, R_keyCol);
		  	struct futhark_i64_1d *sorted_R_is;
		  	struct futhark_i32_1d *sorted_R_ks;
		  	futhark_project_opaque_sortInfo_i32_is(ctx, &sorted_R_is, R_sortRes);
		  	futhark_project_opaque_sortInfo_i32_ks(ctx, &sorted_R_ks, R_sortRes);
		  	futhark_free_opaque_sortInfo_i32(ctx,R_sortRes);
		  	mylog(logfile, "Sorted R.");

		// Perform gather operations

		  	struct futhark_i32_1d *sorted_pL[num_pL];
		  	for (int64_t col=0; col<num_pL; col++) {
		  		futhark_entry_gather_i32(ctx, &(sorted_pL[col]), R_pL_cols[col],sorted_R_is);
		  		futhark_free_i32_1d(ctx, R_pL_cols[col]);
		  	}
		  	futhark_free_i64_1d(ctx, sorted_R_is);

		  	mylog(logfile, "Gathered R's payloads.");

		  	futhark_context_sync(ctx);
		  	mylog(logfile, "Synced futhark context.");

		// Create output table & apender
			
			int cursor_out = sprintf(output_tbl, "CREATE OR REPLACE TEMP TABLE R_out (k INTEGER");
			for(int64_t col=0; col<num_pL; col++) {
				cursor_out += sprintf(output_tbl+cursor_out, ", pL%ld INTEGER", col);
			}
			sprintf(output_tbl+cursor_out, ");");
			if(duckdb_query(con, output_tbl, NULL) == DuckDBError) {
				perror("Failed to create output table.");
				perror(output_tbl);
				return -1;
			}
			mylog(logfile, "Created output table.");

			duckdb_appender appender;
		  	if (duckdb_appender_create(con,NULL,"R_out",&appender) == DuckDBError) {
		  		perror("Failed to create appender.\n");
		  		return -1;
		  	}
		  	mylog(logfile, "Set up duckdb appender for output.");

		  	duckdb_logical_type ltypes[1+num_pL];
		  	ltypes[0] = k_ltype;
		  	for(int64_t col=0;col<num_pL;col++) {
		  		ltypes[col+1]=pL_ltype;
		  	}

		// Unwrap data

		  	int32_t *OUTPUT_BUFFS[1+num_pL];
		  	for(int64_t col=0; col<1+num_pL; col++) {
		  		OUTPUT_BUFFS[col] = malloc(cur_row_R*sizeof(int32_t));
		  		if(col==0) {
		  			futhark_values_i32_1d(ctx, sorted_R_ks, OUTPUT_BUFFS[col]);
		  			futhark_free_i32_1d(ctx, sorted_R_ks);
		  		}
		  		else {
		  			futhark_values_i32_1d(ctx, sorted_pL[col-1], OUTPUT_BUFFS[col]);
		  			futhark_free_i32_1d(ctx, sorted_pL[col-1]);
		  		}
		  	}

		// Append chunk-by-chunk
		  	int64_t num_chunks = (cur_row_R + CHUNK_SIZE - 1) / CHUNK_SIZE;


		  	mylog(logfile, "Appending...");
		  	for(int64_t i=0; i<num_chunks; i++) {
		  		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, 1+num_pL);
		  		int64_t this_rows = (i<num_chunks-1)? CHUNK_SIZE: cur_row_R - i*CHUNK_SIZE;
		  		duckdb_data_chunk_set_size(cnk,this_rows);

		  		for(int64_t col=0; col<1+num_pL; col++) {
		  			void *dat = duckdb_vector_get_data(duckdb_data_chunk_get_vector(cnk,col));
		  			memcpy(dat, OUTPUT_BUFFS[col] + i*CHUNK_SIZE, this_rows*sizeof(int32_t));
		  		}

		  		duckdb_append_data_chunk(appender, cnk);
		  		if((i+1)%100 == 0) duckdb_appender_flush(appender);
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_appender_flush(appender);
		  	mylog(logfile, "Done appending.");
		  	duckdb_appender_destroy(&appender);

		  	for(int64_t col=0; col<1+num_pL; col++) {
		  		free(OUTPUT_BUFFS[col]);
		  	}
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