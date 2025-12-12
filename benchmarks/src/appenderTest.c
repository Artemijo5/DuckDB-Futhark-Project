#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"

#define LOGFILE "appenderTest.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define TABLE_SIZE 32*512*CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "6GB"
#define DDB_TEMPDIR "tps_tempdir"

// TODO appenders flush every <100 chunks

int main() {
	// Initialise logger
	FILE* logfile = loginit(LOGFILE, "appenderTest.c : Starting test program.");
	if(LOGFILE && !logfile) {
	  perror("Failed to initialise logger.");
	  return -1;
	}

	// DuckDB initialisation
	duckdb_database db;
	duckdb_connection con;
	duckdb_config config;
	// set config
	if (duckdb_create_config(&config) == DuckDBError) {
	  perror("Failed to create config.");
	  return -1;
	}
	duckdb_set_config(config, "max_memory", DDB_MEMSIZE);
	duckdb_set_config(config, "memory_limit", DDB_MEMSIZE);
	duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
	//duckdb_open(NULL, &db);
	if(duckdb_open_ext(DBFILE, &db, config, NULL) == DuckDBError) {
	  perror("Failed to open database with configuration options.");
	  return -1;
	}
	duckdb_destroy_config(&config);
	duckdb_connect(db, &con);

	mylog(logfile, "Creating example data...");
	/*
	// Create buffer to store
	int64_t *buff;
	buff = malloc(TABLE_SIZE * sizeof(int64_t));
	for(idx_t i=0; i<TABLE_SIZE; i++) {
		buff[i] = i;
	}
	*/
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE init_data (k BIGINT);", NULL);
	mylog(logfile, "Created initial table (empty).");
	char queryStr[1000];
	sprintf(queryStr, "INSERT INTO init_data (SELECT 1000000000*random() FROM range(%ld));", TABLE_SIZE);
	duckdb_query(con, queryStr, NULL);
	mylog(logfile, "Filled initial table with example data.");

	mylog(logfile, "Sorting example data...");
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE sorted_data AS (SELECT * FROM init_data ORDER BY k);", NULL);
	mylog(logfile, "Sorted example data.");
	duckdb_query(con, "DROP TABLE init_data;", NULL);
	mylog(logfile, "Dropped init table.");

	// SCANNING TESTS
	// 1. Read & buffer each chunk.
	// 2. Read chunks & buffer per 60.
	// 3. Read chunks & buffer per 600.

	mylog(logfile, "Allocating buffer.");
	int64_t *buff = malloc(TABLE_SIZE*sizeof(long));
	mylog(logfile, "Allocated buffer.");
	duckdb_result res1, res2, res3;
	idx_t row_count;
	idx_t cnk_count;
	int flag;

	mylog(logfile, "Scanning Experiment #1 ------------------------------------------");
	duckdb_query(con, "SELECT * FROM sorted_data;", &res1);
	mylog(logfile, "Performed selection query.");
	row_count = 0;
	while(true) {
		duckdb_data_chunk cnk = duckdb_fetch_chunk(res1);
		if(!cnk) break;
		idx_t cnk_size = duckdb_data_chunk_get_size(cnk);
		duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
		int64_t *dat = duckdb_vector_get_data(vec);
		memcpy((char*)buff + sizeof(int64_t)*row_count, (char*)dat, sizeof(int64_t)*cnk_size);
		duckdb_destroy_data_chunk(&cnk);
		row_count += cnk_size;
	}
	mylog(logfile, "Buffered all data.");
	duckdb_destroy_result(&res1);
	mylog(logfile, "Destroyed result.");

	mylog(logfile, "Scanning Experiment #2 ------------------------------------------");
	duckdb_query(con, "SELECT * FROM sorted_data;", &res2);
	mylog(logfile, "Performed selection query.");
	row_count = 0;
	cnk_count = 0;
	duckdb_data_chunk cnks2[60];
	flag = true;
	while(flag) {
		duckdb_data_chunk cnk = duckdb_fetch_chunk(res2);
		if(!cnk) flag = false;
		else {
			cnks2[cnk_count++] = cnk;
		}
		if(cnk_count == 60 || (!flag && cnk_count > 0)) {
			idx_t cur_rows = 0;
			for(idx_t c=0; c<cnk_count; c++) {
				idx_t cnk_size = duckdb_data_chunk_get_size(cnks2[c]);
				duckdb_vector vec = duckdb_data_chunk_get_vector(cnks2[c], 0);
				int64_t *dat = duckdb_vector_get_data(vec);
				memcpy((char*)buff + sizeof(int64_t)*(row_count+cur_rows), (char*)dat, sizeof(int64_t)*cnk_size);
				duckdb_destroy_data_chunk(&(cnks2[c]));
				cur_rows += cnk_size;
			}
			cnk_count = 0;
			row_count += cur_rows;
		}
	}
	mylog(logfile, "Buffered all data.");
	duckdb_destroy_result(&res2);
	mylog(logfile, "Destroyed result.");

	mylog(logfile, "Scanning Experiment #3 ------------------------------------------");
	duckdb_query(con, "SELECT * FROM sorted_data;", &res3);
	mylog(logfile, "Performed selection query.");
	row_count = 0;
	cnk_count = 0;
	duckdb_data_chunk cnks3[600];
	flag = true;
	while(flag) {
		duckdb_data_chunk cnk = duckdb_fetch_chunk(res3);
		if(!cnk) flag = false;
		else {
			cnks3[cnk_count++] = cnk;
		}
		if(cnk_count == 600 || (!flag && cnk_count > 0)) {
			idx_t cur_rows = 0;
			for(idx_t c=0; c<cnk_count; c++) {
				idx_t cnk_size = duckdb_data_chunk_get_size(cnks3[c]);
				duckdb_vector vec = duckdb_data_chunk_get_vector(cnks3[c], 0);
				int64_t *dat = duckdb_vector_get_data(vec);
				memcpy((char*)buff + sizeof(int64_t)*(row_count+cur_rows), (char*)dat, sizeof(int64_t)*cnk_size);
				duckdb_destroy_data_chunk(&(cnks3[c]));
				cur_rows += cnk_size;
			}
			cnk_count = 0;
			row_count += cur_rows;
		}
	}
	mylog(logfile, "Buffered all data.");
	duckdb_destroy_result(&res3);
	mylog(logfile, "Destroyed result.");


	// APPENDING TESTS
	// 1. row-wise appends flushing every chunk
	// 2. row-wise appends flushing every 60 chunks
	// 3. row-wise appends flushing every 3000 chunks
	// 4. chunk-wise appends
	// 5. chunk-wise appends flushing every 60 chunks
	// 6. chunks-wise appends flushing every 3000 chunks
	// More: chunk-wise appends with buffering? probably doesn't achieve anything...

	// i. CREATE TABLES
	mylog(logfile, "Creating destination tables...");
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl1 (k BIGINT);", NULL);
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl2 (k BIGINT);", NULL);
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl3 (k BIGINT);", NULL);
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl4 (k BIGINT);", NULL);
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl5 (k BIGINT);", NULL);
	duckdb_query(con, "CREATE OR REPLACE TEMP TABLE tbl6 (k BIGINT);", NULL);
	mylog(logfile, "Created destination tables.");

	// ii. CREATE APPENDERS
	mylog(logfile, "Creating appenders...");
	duckdb_appender append[6];
	for(idx_t i=0; i<6; i++) {
		char tblName[10];
		sprintf(tblName, "tbl%ld", i);
		duckdb_appender_create(con, NULL, tblName, append + i);
	}
	mylog(logfile, "Created appenders.");

	// iii. DO THE EXPERIMENTS
	// Free appender & drop table immediately after each experiment

	// 1.
	mylog(logfile, "Appending Experiment #1 ------------------------------------------");
	/*
	for(idx_t i=0; i<TABLE_SIZE; i++) {
		duckdb_append_int64(append[0], buff[i]);
		duckdb_appender_end_row(append[0]);
		if(((i+1))%(CHUNK_SIZE) == 0) duckdb_appender_flush(append[1]);
	}
	*/
	mylog(logfile, "Skipped due to being too slow.");
	duckdb_appender_destroy(append + 0);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl1;", NULL);
	mylog(logfile, "Dropped table -- finished with exp1.");

	// 2.
	mylog(logfile, "Appending Experiment #2 ------------------------------------------");
	for(idx_t i=0; i<TABLE_SIZE; i++) {
		duckdb_append_int64(append[1], buff[i]);
		duckdb_appender_end_row(append[1]);
		if(((i+1))%(60*CHUNK_SIZE) == 0) duckdb_appender_flush(append[1]);
	}
	mylog(logfile, "Done appending.");
	duckdb_appender_destroy(append + 1);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl2;", NULL);
	mylog(logfile, "Dropped table -- finished with exp2.");

	// 3.
	mylog(logfile, "Appending Experiment #3 ------------------------------------------");
	for(idx_t i=0; i<TABLE_SIZE; i++) {
		duckdb_append_int64(append[2], buff[i]);
		duckdb_appender_end_row(append[2]);
		if(((i+1))%(3000*CHUNK_SIZE) == 0) duckdb_appender_flush(append[2]);
	}
	mylog(logfile, "Done appending.");
	duckdb_appender_destroy(append + 2);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl3;", NULL);
	mylog(logfile, "Dropped table -- finished with exp3.");

	// 4.
	mylog(logfile, "Appending Experiment #4 ------------------------------------------");
	for(idx_t i=0; i<TABLE_SIZE; i+=CHUNK_SIZE) {
		duckdb_logical_type ltype[1] = {duckdb_create_logical_type(DUCKDB_TYPE_BIGINT)};
		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype, 1);
		idx_t cnk_size = (i+CHUNK_SIZE < TABLE_SIZE)? CHUNK_SIZE: TABLE_SIZE-i;
		duckdb_data_chunk_set_size(cnk, cnk_size);
		duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
		int64_t *dat = duckdb_vector_get_data(vec);
		memcpy(dat, buff + i, cnk_size);
		duckdb_append_data_chunk(append[3], cnk);
		duckdb_destroy_data_chunk(&cnk);
	}
	mylog(logfile, "Done appending.");
	duckdb_appender_destroy(append + 3);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl4;", NULL);
	mylog(logfile, "Dropped table -- finished with exp4.");

	// 5.
	mylog(logfile, "Appending Experiment #5 ------------------------------------------");
	for(idx_t i=0; i<TABLE_SIZE; i+=CHUNK_SIZE) {
		duckdb_logical_type ltype[1] = {duckdb_create_logical_type(DUCKDB_TYPE_BIGINT)};
		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype, 1);
		idx_t cnk_size = (i+CHUNK_SIZE < TABLE_SIZE)? CHUNK_SIZE: TABLE_SIZE-i;
		duckdb_data_chunk_set_size(cnk, cnk_size);
		duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
		int64_t *dat = duckdb_vector_get_data(vec);
		memcpy(dat, buff + i, cnk_size);
		duckdb_append_data_chunk(append[4], cnk);
		duckdb_destroy_data_chunk(&cnk);
		if(((i+1))%(60*CHUNK_SIZE) == 0) duckdb_appender_flush(append[4]);
	}
	mylog(logfile, "Done appending.");
	duckdb_appender_destroy(append + 4);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl5;", NULL);
	mylog(logfile, "Dropped table -- finished with exp5.");

	// 6.
	mylog(logfile, "Appending Experiment #6 ------------------------------------------");
	for(idx_t i=0; i<TABLE_SIZE; i+=CHUNK_SIZE) {
		duckdb_logical_type ltype[1] = {duckdb_create_logical_type(DUCKDB_TYPE_BIGINT)};
		duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype, 1);
		idx_t cnk_size = (i+CHUNK_SIZE < TABLE_SIZE)? CHUNK_SIZE: TABLE_SIZE-i;
		duckdb_data_chunk_set_size(cnk, cnk_size);
		duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
		int64_t *dat = duckdb_vector_get_data(vec);
		memcpy(dat, buff + i, cnk_size);
		duckdb_append_data_chunk(append[5], cnk);
		duckdb_destroy_data_chunk(&cnk);
		if(((i+1))%(3000*CHUNK_SIZE) == 0) duckdb_appender_flush(append[5]);
	}
	mylog(logfile, "Done appending.");
	duckdb_appender_destroy(append + 5);
	mylog(logfile, "Destroyed appender.");
	duckdb_query(con, "DROP TABLE tbl6;", NULL);
	mylog(logfile, "Dropped table -- finished with exp6.");


	free(buff);
	duckdb_disconnect(&con);
	duckdb_close(&db);
	mylog(logfile, "Disconnected duckdb and freed its memory.");

	logclose(logfile);

	return 0;
}