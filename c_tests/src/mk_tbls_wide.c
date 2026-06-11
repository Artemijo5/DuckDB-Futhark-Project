// Make tables R_tbl, S_tbl, for narrow & wide joins tests.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"

#include <unistd.h>
#include <getopt.h>

#define R_name "R_tbl"
#define S_name "S_tbl"

#define default_R_size 8192
#define default_S_size 16384

#define default_R_vals 8192
#define default_S_vals 8192

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
		int64_t R_size = default_R_size;
		int64_t S_size = default_S_size;

		int64_t R_vals = default_R_vals;
		int64_t S_vals = default_S_vals;

		char k_type[50]  = default_k_type;
		char pL_type[50] = default_pL_type;

		int64_t num_pL = default_num_pL;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"R_size", required_argument, 0, 'R'},
			{"S_size", required_argument, 0, 'S'},
			{"R_vals", required_argument, 0, 'r'},
			{"S_vals", required_argument, 0, 's'},
			{"k_type", required_argument, 0, 'k'},
			{"pL_type", required_argument, 0, 'p'},
			{"pL_num", required_argument, 0, 'P'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"R:S:r:s:k:p:P:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	        case 'R':
	        	R_size = atol(optarg); break;
	        case 'S':
	        	S_size = atol(optarg); break;
	        case 'r':
	        	R_vals = atol(optarg); break;
	        case 's':
	        	S_vals = atol(optarg); break;
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

		FILE* logfile = loginit(LOGFILE, "Starting program to create tables for join.");
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

	// Allocate strings for the queries.

	  	char R_primary[50] = "";
	  	if(R_vals >= R_size) {
	  		sprintf(R_primary, "PRIMARY KEY");
	  	}

	  	char query_prepare_R[strlen(k_name)+strlen(R_name)+strlen(k_type)+500];
	  	char query_prepare_S[strlen(k_name)+strlen(R_name)+strlen(k_type)+500];

	  	char query_mk_R[strlen(k_name)+strlen(R_primary)+strlen(k_type)+num_pL*(strlen(pL_name)+strlen(pL_type)+5)+strlen(R_name)+500];
	  	char query_mk_S[strlen(k_name)+strlen(k_type)+num_pL*(strlen(pL_name)+strlen(pL_type)+5)+strlen(S_name)+500];

	  	char query_load_R[2*strlen(R_name)+500+25*num_pL];
	  	char query_load_S[2*strlen(R_name)+500+25*num_pL];

  	// -- -- -- Create pre-shuffled keys.

	  	sprintf(query_prepare_R,
	  		"CREATE OR REPLACE TEMP TABLE %s_prep AS \n(SELECT i %% %ld, (random()::BIGINT)*(2*%ld) FROM range(%ld) t(i));"
	  		, R_name, R_vals, R_size, R_size);
	  	sprintf(query_prepare_S,
	  		"CREATE OR REPLACE TEMP TABLE %s_prep AS \n(SELECT i %% %ld, (random()::BIGINT)*(2*%ld) FROM range(%ld) t(i));"
	  		, S_name, S_vals, S_size, S_size);

	  	if(duckdb_query(con, query_prepare_R, NULL) == DuckDBError) {
	  		perror("Failed to prepare table R.\n");
	  		perror(query_prepare_R);
	  		return -1;
	  	}
	  	if(duckdb_query(con, query_prepare_S, NULL) == DuckDBError) {
	  		perror("Failed to prepare table S.\n");
	  		perror(query_prepare_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Created unshuffled data.");

  	// -- -- -- Create tables where keys will be shuffled.

	  	int len_R1 = sprintf(query_mk_R, "CREATE OR REPLACE TABLE %s (%s %s %s", R_name, k_name, k_type, R_primary);
	  	for(int64_t i=0; i<num_pL; i++) {
	  		len_R1 += sprintf(query_mk_R+len_R1, ", %s_%ld %s", pL_name, i, pL_type);
	  	}
	  	sprintf(query_mk_R+len_R1, ");");

	  	int len_S1 = sprintf(query_mk_S, "CREATE OR REPLACE TABLE %s (%s %s", S_name, k_name, k_type);
	  	for(int64_t i=0; i<num_pL; i++) {
	  		len_S1 += sprintf(query_mk_S+len_S1, ", %s_%ld %s", pL_name, i, pL_type);
	  	}
	  	sprintf(query_mk_S+len_S1, ");");

	  	if(duckdb_query(con, query_mk_R, NULL) == DuckDBError) {
	  		perror("Failed to create table R.\n");
	  		perror(query_mk_R);
	  		return -1;
	  	}
	  	if(duckdb_query(con, query_mk_S, NULL) == DuckDBError) {
	  		perror("Failed to create table S.\n");
	  		perror(query_mk_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Created tables in the database (currently empty).");

  	// -- -- -- Shuffle keys & append random payloads.

  		int len_R2 = sprintf(query_load_R, "INSERT INTO %s \nSELECT #1", R_name);
  		for(int64_t i=0; i<num_pL; i++) {
  			len_R2 += sprintf(query_load_R+len_R2,", 255*random()");
  		}
  		sprintf(query_load_R+len_R2," FROM (FROM %s_prep ORDER BY #2);", R_name);

  		int len_S2 = sprintf(query_load_S, "INSERT INTO %s \nSELECT #1", S_name);
  		for(int64_t i=0; i<num_pL; i++) {
  			len_S2 += sprintf(query_load_S+len_S2,", 255*random()");
  		}
  		sprintf(query_load_S+len_S2," FROM (FROM %s_prep ORDER BY #2);", S_name);

  		if(duckdb_query(con, query_load_R, NULL) == DuckDBError) {
	  		perror("Failed to load data into R.\n");
	  		perror(query_load_R);
	  		return -1;
	  	}
	  	if(duckdb_query(con, query_load_S, NULL) == DuckDBError) {
	  		perror("Failed to load data into S.\n");
	  		perror(query_load_S);
	  		return -1;
	  	}

	  	mylog(logfile, "Loaded shuffled keys & random payloads into the tables.");

	// Cleanup

	  	duckdb_disconnect(&con);
		duckdb_close(&db);
  		mylog(logfile, "Disconnected duckdb and freed its memory.");

  		logclose(logfile);
  
  return 0;
}