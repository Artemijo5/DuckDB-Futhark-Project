#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "duckdb.h"

#include "ftSkyline.h"
#include "mylogger.h"
#include "db_util.h"

#define LOGFILE "skyline.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 2*CHUNK_SIZE
#define TABLE_SIZE 10*BUFFER_SIZE
#define CNK_TO_READ (long)60

#define DIM (long)5
#define ANGULAR_SUBDIV (long)4
#define MINVAL (float)0.0
#define MAXVAL (float)100.0
#define SIZE_THRESH BUFFER_SIZE/10

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Skyline Computation : Starting test program.");
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
	mylog(logfile, "Set up duckdb connection.");

  // Create the table tbl on which the testing will be done.
  char queryStr[150 + 20*DIM];
  int queryLen = sprintf(queryStr, "CREATE OR REPLACE TEMP TABLE skyTbl (");
  for(idx_t i=0; i<DIM; i++) {
  	queryLen += sprintf(queryStr+queryLen, "x%ld FLOAT", i+1);
  	if(i<DIM-1) queryLen += sprintf(queryStr+queryLen, ", ");
  }
  queryLen += sprintf(queryStr+queryLen, ");");
  if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
  	perror("Failed to create initial table.");
  	perror(queryStr);
  	return -1;
  }

  duckdb_query(con, "setseed(0.42);", NULL);

  // Insert random data into the table
  char insertQueryStr[150 + 100*DIM];
  int insertQueryLen = sprintf(insertQueryStr, "INSERT INTO skyTbl (SELECT ");
  for(idx_t i=0; i<DIM; i++) {
  	insertQueryLen += sprintf(insertQueryStr+insertQueryLen, "%ld*random()-%ld", MAXVAL-MINVAL, MINVAL);
  	if(i<DIM-1) insertQueryLen += sprintf(insertQueryStr+insertQueryLen, ", ");
  }
  insertQueryLen += sprintf(insertQueryStr+insertQueryLen, " FROM RANGE(%ld) t(i));", TABLE_SIZE);
  if( duckdb_query(con, insertQueryStr, NULL) == DuckDBError ) {
  	perror("Failed to insert data into the initial table.");
  	perror(insertQueryStr);
  	return -1;
  }

  mylog(logfile, "Created test table 'skyTbl'.");

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  duckdb_type type_ids[DIM];
  int64_t byteSizes[DIM];
  int64_t prefixSizes[DIM];
  float mins[DIM];
  float maxs[DIM];
  int64_t grid_per_dim[DIM];
  int64_t angl_per_dim[DIM-1];
  for(idx_t i=0; i<DIM; i++) {
  	type_ids[i] = DUCKDB_TYPE_FLOAT;
  	byteSizes[i] = sizeof(float);
  	prefixSizes[i] = (i>0)? sizeof(float): 0;
  	mins[i] = MINVAL;
  	maxs[i] = MAXVAL;
  	grid_per_dim[i] = 1;
  	if(i<DIM-1) {
  		angl_per_dim[i] = (i>0)? ANGULAR_SUBDIV: 2;
  	}
  }

  // Define SkylineBase
	  struct futhark_opaque_skylineBase_float *skB;
	  struct futhark_f32_1d *mins_ft = futhark_new_f32_1d(ctx,mins,DIM);
	  struct futhark_f32_1d *maxs_ft = futhark_new_f32_1d(ctx,maxs,DIM);
	  struct futhark_i64_1d *grids_ft = futhark_new_i64_1d(ctx, grid_per_dim, DIM);
	  struct futhark_i64_1d *angls_ft = futhark_new_i64_1d(ctx, angl_per_dim, DIM-1);
	  futhark_entry_define_skyline_space_float(ctx, &skB, mins, maxs, grids_ft, angs_ft);
	  futhark_context_sync(ctx);
	  mylog(logfile, "Defined data space in the form of SkylineBase.");
	  futhark_free_f32_1d(ctx, mins_ft);
	  futhark_free_f32_1d(ctx, maxs_ft);
	  futhark_free_i64_1d(ctx, grids_ft);
	  futhark_free_i64_1d(ctx, angls_ft);

	struct futhark_opaque_skylineInfo_GFUR_double *skyWindows[5];
	idx_t currently_windowed = 0;

  duckdb_result res;
  duckdb_query(con, "SELECT * FROM skyTbl;");
  mylog(logfile, "Performed query to read skyTbl.");
  int isTblExhausted = false;
  while(!isTblExhausted) {
  	duckdb_data_chunk skyChunks[CNK_TO_READ];
  	idx_t chunks_read = bulk_fetch_chunks(res, CNK_TO_READ, skyChunks);
  	isTblExhausted = (chunks_read < CNK_TO_READ);
  	if(chunks_read==0) break;
  	mylog(logfile, " - - Obtained chunks.");

  	idx_t cur_rows = 0;
  	char* skyData = malloc(DIM*sizeof(float)*chunks_read*CHUNK_SIZE);

  	for(idx_t j=0; j<chunks_read; j++) {
  		cnk_size = duckdb_data_chunk_get_size(skyChunks[j]);
  		void *cnk_dat[DIM];
  		for(idx_t col=0; col<DIM; col++) {
  			duckdb_vector vec = duckdb_data_chunk_get_vector(skyChunks[j], col);
  			cnk_dat[col] = duckdb_vector_get_data(vec);
  		}
  		payloadColumnsToByteArray_preallocated(
			  skyData+cur_rows,
			  DIM*sizeof(float),
			  byteSizes,
			  prefixSizes,
			  cnk_dat,
			  DIM,
			  cnk_size
			);
			cur_rows += cnk_size;
			duckdb_destroy_data_chunk(&(skyChunks[j]));
  	}
  	mylog(logfile, " - - Buffered data from chunks.");

  	struct futhark_f32_2d *skyData_ft = futhark_new_f32_2d(ctx, skyData, cur_rows, DIM);
  	mylog(logfile, " - - Wrapped data into futhark core.");
  	free(skyData);

  	struct futhark_opaque_skylineInfo_GFUR_float *skyWindow;
  	struct futhark_opaque_skylineInfo_GFUR_float *local_skyWindow;
  	struct futhark_opaque_skylineInfo_GFUR_float *interm_skyWindow;

  	futhark_entry_pointwise_slice_and_dice_for_Skyline_GFUR_double(ctx, &skyWindow, skB, skyData_ft, 0, false);
  	mylog(logfile, " - - Applied pointwise filtering & angular partitioning ('slice-and-dice').");
  	futhark_free_f32_1d(ctx, skyData_ft);

  	futhark_entry_calc_local_Skyline_GFUR_float(ctx, local_skyWindow, skB, skyWindow);
  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, skyWindow);
  	mylog(logfile, " - - Applied local filtering.");
  	futhark_entry_calc_intermSkyline_GFUR_float(ctx, interm_skyWindow, skB, local_skyWindow, true, 1, 0, SIZE_THRESH);
  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, local_skyWindow);
  	mylog(logfile, " - - Applied intermediate filtering steps.");

  	skyWindows[currently_windowed++] = interm_skyWindow;
  	if(isTblExhausted || currently_windowed==5) {
  		logdbg(
  			logfile,
  			isTblExhausted,
  			" - - | - - Data exhausted - collapsing windows...",
  			" - - | - - Window cache filled - collapsing windows..."
  		);

  		struct futhark_opaque_skylineInfo_GFUR_float *collapsed_skyWindow;
  		struct futhark_opaque_skylineInfo_GFUR_float *local_collapsed_skyWindow;
  		struct futhark_opaque_skylineInfo_GFUR_float *interm_collapsed_skyWindow;
  		switch(currently_windowed) {
  			case 1:
  				collapsed_skyWindow = skyWindows[0];
  				break;
  			case 2:
  				futhark_entry_merge_Skylines_2_GFUR_float(
  					ctx, collapsed_skyWindow, skB,
  					skyWindows[0], skyWindows[1]
  				);
  				break;
  			case 3:
  				futhark_entry_merge_Skylines_3_GFUR_float(
  					ctx, collapsed_skyWindow, skB,
  					skyWindows[0], skyWindows[1], skyWindows[2]
  				);
  				break;
  			case 4:
  				futhark_entry_merge_Skylines_4_GFUR_float(
  					ctx, collapsed_skyWindow, skB,
  					skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3]
  				);
  				break
  			default:
  				futhark_entry_merge_Skylines_5_GFUR_float(
  					ctx, collapsed_skyWindow, skB,
  					skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3], skyWindows[4]
  				);
  				break;
  		}
  		if(currently_windowed>1) {
  			for(idx_t w=0; w<currently_windowed; w++) {
  				futhark_free_opaque_skylineInfo_GFUR_float(ctx, skyWindows[w]);
  			}
  		}
  		currently_windowed = 1;
  		mylog(logfile, " - - | - - Collapsed cached windows.");

  		futhark_entry_calc_local_Skyline_GFUR_float(
  			ctx, local_collapsed_skyWindow, skB, collapsed_skyWindow
  		);
	  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, collapsed_skyWindow);
	  	mylog(logfile, " - - | - - Applied local filtering.");
	  	futhark_entry_calc_intermSkyline_GFUR_float(
	  		ctx, interm_collapsed_skyWindow, skB, local_collapsed_skyWindow, true, 1, 0, SIZE_THRESH
	  	);
	  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, local_collapsed_skyWindow);
	  	mylog(logfile, " - - | - - Applied intermediate filtering steps.");

	  	skyWindows[0] = interm_collapsed_skyWindow;
  	}
  }
  mylog(logfile, "Result is exhausted.");
  duckdb_destroy_result(&res);

  // TODO compute global skyline, save results to table...
  
  // Cleanup
  futhark_free_opaque_skylineBase_float(ctx, skB);

  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}
