// Make table S that is zipf-distributed.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"

#include "../../ft_clibs/ftSynthetic.h"

#include <unistd.h>
#include <getopt.h>

#define CHUNK_SIZE duckdb_vector_size()

#define S_name "S_tbl"

#define default_S_size 8192

#define default_S_vals 8192

#define default_SKEW (double)1.0

#define default_k_type "INTEGER"
#define default_pL_type "INTEGER"

#define k_name "k"
#define pL_name "pL"

#define default_num_pL 2

#define default_LOGFILE "stdout"
#define default_DBFILE "testdb.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations
		int64_t S_size = default_S_size;
		int64_t S_vals = default_S_vals;

		double SKEW = default_SKEW;

		char k_type[50]  = default_k_type;
		char pL_type[50] = default_pL_type;

		int64_t num_pL = default_num_pL;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"S_size", required_argument, 0, 'S'},
			{"S_vals", required_argument, 0, 's'},
			{"skew", required_argument, 0, 'z'},
			{"k_type", required_argument, 0, 'k'},
			{"pL_type", required_argument, 0, 'p'},
			{"pL_num", required_argument, 0, 'P'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"S:s:z:k:p:P:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	        case 'S':
	        	S_size = atol(optarg); break;
	        case 's':
	        	S_vals = atol(optarg); break;
	        case 'z':
	        	SKEW = atof(optarg); break;
	        case 'k':
	        	memcpy(k_type, optarg, strlen(optarg)+1); break; 
	        case 'p':
	        	memcpy(pL_type, optarg, strlen(optarg)+1); break; 
	        case 'P':
	        	num_pL = atol(optarg); break;
	        case 'L':
	        	memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
	        case 'f':
	        	memcpy(DBFILE, optarg, strlen(optarg)+1); break;
	      }
	    }

	// init logger

		FILE* logfile = loginit(LOGFILE, "Starting program to create zipf-skewed foreign key table.");
	    if(LOGFILE && !logfile) {
	      perror("Failed to initialise logger.\n");
	      return -1;
	    }

	// TODO log parameters

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

	  	printf("\n\n\n%ld\n\n\n\n", CHUNK_SIZE);

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

	// Allocate strings for the queries.

	  	char query_prepare_S[strlen(k_name)+strlen(S_name)+num_pL*(strlen(pL_type)+12)+500];

	  	char query_mk_S[strlen(k_name)+strlen(k_type)+num_pL*(strlen(pL_name)+strlen(pL_type)+5)+strlen(S_name)+500];

	  	char query_select_S[strlen(S_name) + 100];

	// -- -- -- Create uniformly random numbers together with payloads.

	  	int len_S1 = sprintf(query_prepare_S,
	  		"CREATE OR REPLACE TEMP TABLE %s_prep AS \n(SELECT random()::DOUBLE, ",
	  		S_name
	  	);
	  	for(int64_t i=0; i<num_pL; i++) {
	  		len_S1 += sprintf(query_prepare_S+len_S1,
	  			"(255*random())::%s, ",
	  			pL_type
	  		);
	  	}
	  	sprintf(query_prepare_S+len_S1," FROM range(%ld) t(i));", S_size);

	  	if(duckdb_query(con, query_prepare_S, NULL) == DuckDBError) {
	  		perror("Failed to prepare table S.\n");
	  		perror(query_prepare_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Created raw data to be converted to zipfian foreign keys.");

  	// -- -- -- Create empty destination table.

	  	len_S1 = sprintf(query_mk_S, "CREATE OR REPLACE TABLE %s (%s %s", S_name, k_name, k_type);
	  	for(int64_t i=0; i<num_pL; i++) {
	  		len_S1 += sprintf(query_mk_S+len_S1, ", %s_%ld %s", pL_name, i, pL_type);
	  	}
	  	sprintf(query_mk_S+len_S1, ");");

	  	if(duckdb_query(con, query_mk_S, NULL) == DuckDBError) {
	  		perror("Failed to create table S.\n");
	  		perror(query_mk_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Created tables in the database (currently empty).");

	// Scan prep table, convert uniform randoms into zipfian-distributed foreign keys, and append to dest table.

	  	duckdb_result res;
	  	sprintf(query_select_S, "FROM %s_prep;", S_name);
	  	if(duckdb_query(con, query_select_S, &res) == DuckDBError) {
	  		perror("Failed to scan raw data for table S.\n");
	  		perror(query_mk_S);
	  		return -1;
	  	}

	  	duckdb_appender appender;
	  	if (duckdb_appender_create(con,NULL,S_name,&appender) == DuckDBError) {
	  		perror("Failed to create appender.\n");
	  		return -1;
	  	}
		mylog(logfile, "Set up duckdb appender for output.");

		duckdb_logical_type k_ltype  = duckdb_create_logical_type(DUCKDB_TYPE_INTEGER);
		duckdb_logical_type pL_ltype = duckdb_create_logical_type(DUCKDB_TYPE_INTEGER);
		if(strcmp(k_type,  "BIGINT")==0) k_ltype  = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);
		if(strcmp(pL_type, "BIGINT")==0) pL_ltype = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);

	  	duckdb_logical_type ltypes[1+num_pL];
	  	ltypes[0]=k_ltype;
	  	for(int64_t col=1; col<1+num_pL; col++) {
	  		ltypes[col]=pL_ltype;
	  	}

	  	mylog(logfile, "Now scanning raw data to convert to zipfian-distributed keys...");
	  	for(int64_t i=0; true; i++) {
	  		duckdb_data_chunk cnk = duckdb_fetch_chunk(res);

			if(!cnk) {
				mylog(logfile, "Result is exhausted.");
				break;
			}
			int64_t this_sz = duckdb_data_chunk_get_size(cnk);

			duckdb_data_chunk append_cnk = duckdb_create_data_chunk(ltypes,1+num_pL);
			duckdb_data_chunk_set_size(append_cnk, this_sz);

			// Convert key-col
			duckdb_vector key_vec = duckdb_data_chunk_get_vector(cnk,0);
			double *key_dat = (double*) duckdb_vector_get_data(key_vec);

			struct futhark_f64_1d *ft_key_dat = futhark_new_f64_1d(ctx, key_dat, this_sz);
			
			if(strcmp(k_type,"BIGINT")==0) {
				struct futhark_i64_1d *zipf_keys;
				futhark_entry_zipf_skewed(ctx, &zipf_keys, SKEW, S_vals, ft_key_dat);
				futhark_context_sync(ctx);

				duckdb_vector key_dest_vec = duckdb_data_chunk_get_vector(append_cnk,0);
				void *key_dat = duckdb_vector_get_data(key_dest_vec);
				futhark_values_i64_1d(ctx, zipf_keys, key_dat);

				futhark_free_i64_1d(ctx, zipf_keys);
			} else {
				struct futhark_i32_1d *zipf_keys;
				futhark_entry_zipf_skewed_i32(ctx, &zipf_keys, SKEW, S_vals, ft_key_dat);
				futhark_context_sync(ctx);

				duckdb_vector key_dest_vec = duckdb_data_chunk_get_vector(append_cnk,0);
				void *key_dat = duckdb_vector_get_data(key_dest_vec);
				futhark_values_i32_1d(ctx, zipf_keys, key_dat);

				futhark_free_i32_1d(ctx,zipf_keys);
			}
			futhark_free_f64_1d(ctx, ft_key_dat);

			// Keep pL as are
			for(int64_t col=1; col<1+num_pL; col++) {
				duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
				void *dat = duckdb_vector_get_data(vec);

				duckdb_vector dest_vec = duckdb_data_chunk_get_vector(append_cnk,col);
				void *dest_dat = duckdb_vector_get_data(dest_vec);

				int type_sz = (strcmp(pL_type,"BIGINT")==0)? sizeof(int64_t): sizeof(int32_t);
				memcpy(dest_dat, dat, this_sz*type_sz);
			}

			duckdb_append_data_chunk(appender, append_cnk);
			duckdb_appender_flush(appender);
		  	duckdb_destroy_data_chunk(&cnk);
		  	duckdb_destroy_data_chunk(&append_cnk);
	  	}
	  	duckdb_appender_flush(appender);
		duckdb_appender_destroy(&appender);

	  	mylog(logfile, "Loaded zipfian keys & random payloads into the table.");

	// Cleanup
	  	duckdb_destroy_result(&res);

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