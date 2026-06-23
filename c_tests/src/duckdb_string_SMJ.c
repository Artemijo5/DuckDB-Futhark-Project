#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../clibs/duckdb.h"
#include "../../clibs/mylogger.h"
#include "../../ft_clibs/join_entry.h"

// TODO use getopt to set parameters
#include <unistd.h>
#include <getopt.h>

#define default_R_buff 1000000
#define default_S_buff 1000000

#define default_AVG_LEN 9

#define default_ITER 1

#define default_k_Name "Name"

#define default_LOGFILE "stdout"
#define default_DBFILE "datasets/US_Baby_Names.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

		int64_t R_buff = default_R_buff;
		int64_t S_buff = default_S_buff;

		int64_t AVG_LEN = default_AVG_LEN;

    	int64_t ITER = default_ITER;

    	char k_Name[1000] = default_k_Name;

    	bool outer = false;
    	bool from_disk = false;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"k_Name", required_argument, 0, 'k'},
			{"R_size", required_argument, 0, 'R'},
			{"S_size", required_argument, 0, 'S'},
			{"assume_strlen", required_argument, 0, 'l'},
			{"outer", no_argument, 0, 'o'},
			{"from_disk", no_argument, 0, 'F'},
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

		char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"k:R:S:l:oFI:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	      	case 'k':
	        	memcpy(k_Name, optarg, strlen(optarg)+1); break;
	      	case 'I':
	      		ITER = atol(optarg); break;
	      	case 'R':
	      		R_buff = atol(optarg); break;
	      	case 'S':
	      		S_buff = atol(optarg); break;
	      	case 'l':
	      		AVG_LEN = atol(optarg); break;
	      	case 'o':
	      		outer = true; break;
	      	case 'F':
	      		from_disk = true; break;
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


	// Fetch tables to memory

	  	if(!from_disk) {
		  	char fetch_R[1000];
			char fetch_S[1000];

			sprintf(fetch_R,"CREATE OR REPLACE TEMP TABLE R_tbl_tmp AS (FROM R_tbl LIMIT %ld);", R_buff);
		  	if(duckdb_query(con, fetch_R, NULL) == DuckDBError) {
		  		perror("Failed to fetch R into memory.\n");
		  		perror(fetch_R);
		  		return -1;
		  	}

		  	sprintf(fetch_S,"CREATE OR REPLACE TEMP TABLE S_tbl_tmp AS (FROM S_tbl LIMIT %ld);", S_buff);
		  	if(duckdb_query(con, fetch_S, NULL) == DuckDBError) {
		  		perror("Failed to fetch S into memory.\n");
		  		perror(fetch_S);
		  		return -1;
		  	}

		  	mylog(logfile, "Fetched tables into memory.");
	 	}


	// Make join query str.

	 	char tbl_suffix[100];
	 	if(from_disk)
	 		sprintf(tbl_suffix,"");
	 	else
	 		sprintf(tbl_suffix,"_tmp");

	 	char joinType[100];
	 	if(!outer)
	 		sprintf(joinType,"JOIN");
	 	else
	 		sprintf(joinType,"FULL OUTER JOIN");

	 	char joinQ[1000 + strlen(joinType) + 2*strlen(tbl_suffix) + 3*strlen(k_Name)];
	 	sprintf(joinQ, "SELECT a.*, b.* EXCLUDE %s FROM R_tbl%s a %s S_tbl%s b ON a.%s = b.%s;",
	 		k_Name, tbl_suffix, joinType, tbl_suffix, k_Name, k_Name);

	 	for(int64_t i=0; i<ITER; i++) {
	 		char iter_log[2000];
	 		sprintf(iter_log, "\n\n------\n-----\n-----\n\nIter #%ld\n\n------\n-----\n-----\n\n", i+1);
	 		mylog(logfile, iter_log);

	 		duckdb_result res;

	 		mylog(logfile, "Performing query...");
	 		if(duckdb_query(con, joinQ, &res) == DuckDBError) {
		  		perror("Failed to fetch S into memory.\n");
		  		perror(joinQ);
		  		return -1;
		  	}
		  	mylog(logfile, "Completed query.");

		  	int64_t NUM_PL = duckdb_column_count(&res) - 1;
		  	// TODO deprecated
		  	int64_t num_rows = R_buff+S_buff;

		  	char *contents = malloc(num_rows*AVG_LEN*sizeof(char));
		  	int32_t *pL[NUM_PL];
		  	for(int64_t col=0; col<NUM_PL; col++) {
		  		pL[col] = malloc(num_rows*sizeof(int32_t));
		  	}

		  	int64_t cur_row = 0;
		  	int64_t cur_len = 0;
		  	mylog(logfile, "Now scannning the join output...");
		  	while(true) {
		  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
		  		if(!cnk) {
		  			mylog(logfile, "Result is exhausted.");
		  			break;
		  		}
		  		int64_t this_rows = duckdb_data_chunk_get_size(cnk);

		  		// Scan string keys
		  		duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk,0);
		  		duckdb_string_t *kdat = (duckdb_string_t *)duckdb_vector_get_data(kvec);
		  		for(int64_t row=0; row<this_rows; row++) {
		  			duckdb_string_t str = kdat[row];
		  			if(duckdb_string_is_inlined(str)) {
		  				cur_len += sprintf(
	  						contents+cur_len, "%.*s",
	  						str.value.inlined.length, str.value.inlined.inlined
	  					);
	  					
		  			} else {
		  				cur_len += sprintf(
	  						contents+cur_len, "%.*s",
	  						str.value.pointer.length, str.value.pointer.ptr
	  					);
		  			}
		  		}
		  		cur_len += sprintf(contents+cur_len," ");

		  		for(int64_t col=0; col<NUM_PL; col++) {
		  			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,1+col);

		  			int32_t *dat = duckdb_vector_get_data(vec);
		  			uint64_t *validity = duckdb_vector_get_validity(vec);

		  			if (!outer)
		  				memcpy(pL[col] + cur_row, dat, this_rows*sizeof(int32_t));
		  			else {
		  				for (int64_t row=0; row<this_rows; row++) {
		  					if(duckdb_validity_row_is_valid(validity,row))
		  						pL[col][this_rows+cur_row] = dat[row];
		  					else
		  						pL[col][this_rows+cur_row] = -1;
		  				}
		  			}
		  		}

		  		cur_row += this_rows;
		  		duckdb_destroy_data_chunk(&cnk);
		  	}
		  	duckdb_destroy_result(&res);
		  	cur_len -= 1;

		  	printf("\n\n\nOutput size: %ld\n\n\n\n", cur_row);

		  	struct futhark_opaque_strInfo *superstring;
		  	struct futhark_u8_1d *delim = futhark_new_u8_1d(ctx, " ", 1);
		  	struct futhark_u8_1d *ft_contents= futhark_new_u8_1d(ctx, contents, cur_len);
		  	futhark_entry_str_split(ctx, &superstring, delim, ft_contents);
		  	futhark_free_u8_1d(ctx, ft_contents);
		  	futhark_context_sync(ctx);
		  	mylog(logfile, "Wrapped string key data into futhark context.");

		  	struct futhark_i32_1d *ft_pL[NUM_PL];
		  	for(int64_t col=0; col<NUM_PL; col++) {
		  		ft_pL[col] = futhark_new_i32_1d(ctx, pL[col], cur_row);
		  	}
		  	futhark_context_sync(ctx);
		  	mylog(logfile, "Wrapped payload data into futhark context.");

		  	// Cleanup
		  	for(int64_t col=0; col<NUM_PL; col++) {
		  		free(pL[col]);
		  		futhark_free_i32_1d(ctx,ft_pL[col]);
		  	}
		  	futhark_free_opaque_strInfo(ctx,superstring);
		  	futhark_free_u8_1d(ctx, delim);
		  	free(contents);
		  	mylog(logfile, "Did cleanup.");
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