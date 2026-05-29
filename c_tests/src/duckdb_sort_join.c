// Benchmark sort & join for tables R_tbl, S_tbl using duckdb.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../clibs/duckdb.h"

#include "../../clibs/mylogger.h"

// TODO use getopt to set parameters
#include <unistd.h>
#include <getopt.h>

#define R_name "R_tbl"
#define S_name "S_tbl"

#define k_name "k"

#define default_ITER 1

#define default_LOGFILE "stdout"
#define default_DBFILE "testdb.db"

int main(int argc, char *argv[]) {
	// Parse command line arguments
    // Initializations

    	int64_t ITER = default_ITER;

		char LOGFILE[1000] = default_LOGFILE;
		char DBFILE[1000]  = default_DBFILE;

		static struct option long_options[] = {
			{"iter",    required_argument, 0, 'I'},
			{"logfile", required_argument, 0, 'L'},
			{"db_file", required_argument, 0, 'f'},
			{0, 0, 0, 0}
		};

    	char ch;
	    while(
	    	(ch = getopt_long_only(argc,argv,"I:L:f:",long_options,NULL)) != -1
	    ) {
	      switch(ch) {
	      	case 'I':
	      		ITER = atol(optarg); break;
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

	for(int64_t cur_iter=0; cur_iter<ITER; cur_iter++) {
		if(ITER>1) {
			char iter_str[100];
			sprintf(iter_str, "#####------#####------#####------#####------#####------ Iteration #%ld", cur_iter);
			mylog(logfile, iter_str);
		}

		// Allocate strings for the queries.

		  	char fetch_R[2*strlen(R_name) + 250];
		  	char fetch_S[2*strlen(S_name) + 250];

		  	char joinQ[2*strlen(R_name)+2*strlen(S_name)+3*strlen(k_name)+250];

		  	char sort_R[2*strlen(R_name) + 250];
		  	char sort_S[2*strlen(S_name) + 250];

		  	char joinSQ[2*strlen(R_name)+2*strlen(S_name)+3*strlen(k_name)+250];

		// Fetch tables into memory.

		  	sprintf(fetch_R,"CREATE OR REPLACE TEMP TABLE %s_tmp AS (FROM %s);", R_name, R_name);
		  	if(duckdb_query(con, fetch_R, NULL) == DuckDBError) {
		  		perror("Failed to fetch R into memory.\n");
		  		perror(fetch_R);
		  		return -1;
		  	}

		  	sprintf(fetch_S,"CREATE OR REPLACE TEMP TABLE %s_tmp AS (FROM %s);", S_name, S_name);
		  	if(duckdb_query(con, fetch_S, NULL) == DuckDBError) {
		  		perror("Failed to fetch S into memory.\n");
		  		perror(fetch_S);
		  		return -1;
		  	}

		  	mylog(logfile, "Fetched tables into memory.");

		// Join unsorted tables

		  	mylog(logfile, "Performing Inner Equi-Join on unsorted tables...");

		  	sprintf(joinQ,
		  		"SELECT a.*, b.* EXCLUDE(%s) FROM %s_tmp a JOIN %s_tmp b ON a.%s = b.%s;"
		  		, k_name, R_name, S_name, k_name, k_name);
		  	if(duckdb_query(con, joinQ, NULL) == DuckDBError) {
		  		perror("Failed to perform Equi-Join on unsorted tables.\n");
		  		perror(joinQ);
		  		return -1;
		  	}

		  	mylog(logfile, "Completed Inner Equi-Join on unsorted tables.");


		// Sort tables.

		  	mylog(logfile, "Sorting table R...");

		  	sprintf(sort_R,"CREATE OR REPLACE TEMP TABLE %s_sorted AS (FROM %s_tmp ORDER BY %s);", R_name, R_name, k_name);
		  	if(duckdb_query(con, sort_R, NULL) == DuckDBError) {
		  		perror("Failed to sort R.\n");
		  		perror(sort_R);
		  		return -1;
		  	}

		  	mylog(logfile, "Sorted table R.");

		  	mylog(logfile, "Sorting table S...");

		  	sprintf(sort_S,"CREATE OR REPLACE TEMP TABLE %s_sorted AS (FROM %s_tmp ORDER BY %s);", S_name, S_name, k_name);
		  	if(duckdb_query(con, sort_S, NULL) == DuckDBError) {
		  		perror("Failed to sort S.\n");
		  		perror(sort_S);
		  		return -1;
		  	}

		  	mylog(logfile, "Sorted table S.");

		// Join sorted tables

		  	mylog(logfile, "Performing Inner Equi-Join on sorted tables...");

		  	sprintf(joinSQ,
		  		"SELECT a.*, b.* EXCLUDE(%s) FROM %s_sorted a JOIN %s_sorted b ON a.%s = b.%s;"
		  		, k_name, R_name, S_name, k_name, k_name);
		  	if(duckdb_query(con, joinSQ, NULL) == DuckDBError) {
		  		perror("Failed to perform Equi-Join on sorted tables.\n");
		  		perror(joinSQ);
		  		return -1;
		  	}

		  	mylog(logfile, "Completed Inner Equi-Join on sorted tables.");

	}

	// Cleanup

	  	duckdb_disconnect(&con);
		duckdb_close(&db);
  		mylog(logfile, "Disconnected duckdb and freed its memory.");

  		logclose(logfile);
  
  return 0;
}