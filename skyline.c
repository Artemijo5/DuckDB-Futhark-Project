#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "duckdb.h"

#include "ftSkyline.h"
#include "mylogger.h"
#include "db_util.h"

#define LOGFILE NULL//"stdout"//"skyline.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define CNK_TO_READ (long)2
#define BUFFER_CAP 1//(long)2*CNK_TO_READ
#define BUFFER_SIZE BUFFER_CAP*CHUNK_SIZE
#define TABLE_SIZE 1*BUFFER_SIZE//10*BUFFER_SIZE

#define DIM (long)2
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
      perror("Failed to initialise logger.\n");
      return -1;
    }

  // DuckDB initialisation
    duckdb_database db;
  	duckdb_connection con;
    duckdb_config config;

    // set config
    if (duckdb_create_config(&config) == DuckDBError) {
      perror("Failed to create config.\n");
      return -1;
    }
    duckdb_set_config(config, "max_memory", DDB_MEMSIZE);
    duckdb_set_config(config, "memory_limit", DDB_MEMSIZE);
    duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
  	//duckdb_open(NULL, &db);
    if(duckdb_open_ext(DBFILE, &db, config, NULL) == DuckDBError) {
      perror("Failed to open database with configuration options.\n");
      return -1;
    }
    duckdb_destroy_config(&config);

  	duckdb_connect(db, &con);
  	mylog(logfile, "Set up duckdb connection.");

  // Create the table tbl on which the testing will be done.
    char queryStr[150 + 20*DIM];
    int queryLen = sprintf(queryStr, "CREATE OR REPLACE TABLE skyTbl (");
    for(idx_t i=0; i<DIM; i++) {
    	queryLen += sprintf(queryStr+queryLen, "x%ld FLOAT", i+1);
    	if(i<DIM-1) queryLen += sprintf(queryStr+queryLen, ", ");
    }
    queryLen += sprintf(queryStr+queryLen, ");");
    if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
    	perror("Failed to create initial table.\n");
    	perror(queryStr);
    	return -1;
    }

    duckdb_query(con, "setseed(0.42);", NULL);

    // Insert random data into the table
    char insertQueryStr[150 + 100*DIM];
    int insertQueryLen = sprintf(insertQueryStr, "INSERT INTO skyTbl (SELECT ");
    for(idx_t i=0; i<DIM; i++) {
    	insertQueryLen += sprintf(insertQueryStr+insertQueryLen, "%f*random()-%f", MAXVAL-MINVAL, MINVAL);
    	if(i<DIM-1) insertQueryLen += sprintf(insertQueryStr+insertQueryLen, ", ");
    }
    insertQueryLen += sprintf(insertQueryStr+insertQueryLen, " FROM RANGE(%ld) t(i));", TABLE_SIZE);
    if( duckdb_query(con, insertQueryStr, NULL) == DuckDBError ) {
    	perror("Failed to insert data into the initial table.\n");
    	perror(insertQueryStr);
    	return -1;
    }

    mylog(logfile, "Created test table 'skyTbl'.");

  // Set up futhark core
    struct futhark_context_config *cfg = futhark_context_config_new();
    struct futhark_context *ctx = futhark_context_new(cfg);
    mylog(logfile, "Set up futhark context & config.");

  // Set some necessary parameters
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
	  futhark_entry_define_skyline_space_float(ctx, &skB, mins_ft, maxs_ft, grids_ft, angls_ft);
	  futhark_context_sync(ctx);
	  mylog(logfile, "Defined data space in the form of SkylineBase.");
	  futhark_free_f32_1d(ctx, mins_ft);
	  futhark_free_f32_1d(ctx, maxs_ft);
	  futhark_free_i64_1d(ctx, grids_ft);
	  futhark_free_i64_1d(ctx, angls_ft);

	struct futhark_opaque_skylineInfo_GFUR_float *skyWindows[5];
	idx_t currently_windowed = 0;

  // 1 - Calculate Intermediate Skylines per Window, Collapsing Windows
    duckdb_result res;
    duckdb_query(con, "SELECT * FROM skyTbl;", &res);
    mylog(logfile, "Performed query to read skyTbl.");
    int isTblExhausted = false;
    idx_t cur_idx=0;
    while(!isTblExhausted) {
      idx_t chunks_read = 0;
    	duckdb_data_chunk skyChunks[BUFFER_CAP];
      for(idx_t i=0; i<BUFFER_CAP; i+=CNK_TO_READ) {
        idx_t cur_cnk_to_read = ((BUFFER_CAP-i) >= CNK_TO_READ)? CNK_TO_READ: (BUFFER_CAP-i);
        idx_t cur_chunks_read = bulk_fetch_chunks(res, cur_cnk_to_read, skyChunks + chunks_read);
        chunks_read += cur_chunks_read;
        if (cur_chunks_read==0) break;
      }
    	isTblExhausted = (chunks_read < (BUFFER_SIZE/CHUNK_SIZE));
    	mylog(logfile, " - - Obtained chunks.");

      if(chunks_read>0) {
      	idx_t cur_rows = 0;
      	char* skyData = malloc(DIM*sizeof(float)*BUFFER_SIZE);

      	for(idx_t j=0; j<chunks_read; j++) {
      		idx_t cnk_size = duckdb_data_chunk_get_size(skyChunks[j]);
      		void *cnk_dat[DIM];
      		for(idx_t col=0; col<DIM; col++) {
      			duckdb_vector vec = duckdb_data_chunk_get_vector(skyChunks[j], col);
      			cnk_dat[col] = duckdb_vector_get_data(vec);
      		}
      		payloadColumnsToByteArray_preallocated(
    			  skyData+cur_rows*DIM*sizeof(float),
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

      	struct futhark_f32_2d *skyData_ft = futhark_new_f32_2d(ctx, (float*)skyData, cur_rows, DIM);
      	mylog(logfile, " - - Wrapped data into futhark core.");
      	free(skyData);

      	struct futhark_opaque_skylineInfo_GFUR_float *skyWindow;
      	struct futhark_opaque_skylineInfo_GFUR_float *local_skyWindow;
      	struct futhark_opaque_skylineInfo_GFUR_float *interm_skyWindow;

      	futhark_entry_pointwise_slice_and_dice_for_Skyline_GFUR_float(ctx, &skyWindow, skB, skyData_ft, cur_idx, false);
      	mylog(logfile, " - - Applied pointwise filtering & angular partitioning ('slice-and-dice').");
      	futhark_free_f32_2d(ctx, skyData_ft);

      	futhark_entry_calc_local_Skyline_GFUR_float(ctx, &local_skyWindow, skB, skyWindow);
      	futhark_free_opaque_skylineInfo_GFUR_float(ctx, skyWindow);
      	mylog(logfile, " - - Applied local filtering.");
      	futhark_entry_calc_intermSkyline_GFUR_float(ctx, &interm_skyWindow, skB, local_skyWindow, true, 1, 0, SIZE_THRESH);
      	futhark_free_opaque_skylineInfo_GFUR_float(ctx, local_skyWindow);
      	mylog(logfile, " - - Applied intermediate filtering steps.");

      	skyWindows[currently_windowed++] = interm_skyWindow;
      	mylog(logfile, " - - Cached this window's filtered results.");
        cur_idx += cur_rows;
      }

    	// Collapse windows
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
    					ctx, &collapsed_skyWindow, skB,
    					skyWindows[0], skyWindows[1]
    				);
    				break;
    			case 3:
    				futhark_entry_merge_Skylines_3_GFUR_float(
    					ctx, &collapsed_skyWindow, skB,
    					skyWindows[0], skyWindows[1], skyWindows[2]
    				);
    				break;
    			case 4:
    				futhark_entry_merge_Skylines_4_GFUR_float(
    					ctx, &collapsed_skyWindow, skB,
    					skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3]
    				);
    				break;
    			default:
    				futhark_entry_merge_Skylines_5_GFUR_float(
    					ctx, &collapsed_skyWindow, skB,
    					skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3], skyWindows[4]
    				);
    				break;
    		}
    		if(currently_windowed>1) {
    			for(idx_t w=0; w<currently_windowed; w++) {
    				futhark_free_opaque_skylineInfo_GFUR_float(ctx, skyWindows[w]);
    			}
    		}
    		mylog(logfile, " - - | - - Collapsed cached windows.");

    		if(currently_windowed>1) {
    			futhark_entry_calc_local_Skyline_GFUR_float(
  	  			ctx, &local_collapsed_skyWindow, skB, collapsed_skyWindow
  	  		);
  		  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, collapsed_skyWindow);
  		  	mylog(logfile, " - - | - - Applied local filtering.");
  		  	futhark_entry_calc_intermSkyline_GFUR_float(
  		  		ctx, &interm_collapsed_skyWindow, skB, local_collapsed_skyWindow, true, 1, 0, SIZE_THRESH
  		  	);
  		  	futhark_free_opaque_skylineInfo_GFUR_float(ctx, local_collapsed_skyWindow);
  		  	mylog(logfile, " - - | - - Applied intermediate filtering steps.");
    		}
    		else {
    			interm_collapsed_skyWindow = collapsed_skyWindow;
    			mylog(logfile, " - - | - - Don't need to apply filtering steps, as there is only one cached window.");
    		}
    		
  	  	skyWindows[0] = interm_collapsed_skyWindow;
  	  	currently_windowed = 1;
    	}
    }
    mylog(logfile, "Result is exhausted.");
    duckdb_destroy_result(&res);

  // 2 - Compute & Unwrap Global Skyline
    mylog(logfile, "Computing global skyline...");
    // At this stage, all data is in skyWindows[0]
    // Any needed intermediate filtering has already been applied
    // So, we only need to apply the final Global Skyline function
    struct futhark_opaque_skylineInfo_GFUR_float *GlobalSkylineInfo_ft;
    futhark_entry_calc_global_Skyline_GFUR_float(ctx, &GlobalSkylineInfo_ft, skB, skyWindows[0]);
    mylog(logfile, "Function to compute global skyline has returned.");
    futhark_free_opaque_skylineInfo_GFUR_float(ctx, skyWindows[0]);

    mylog(logfile, "Unwrapping data from futhark core...");
    struct futhark_opaque_skylineData_GFUR_float *GlobalSkylineData_ft;
    futhark_entry_crack_Skyline_float_GFUR(ctx, &GlobalSkylineData_ft, GlobalSkylineInfo_ft);
    mylog(logfile, "'Cracked' SkylineInfo.");
    futhark_free_opaque_skylineInfo_GFUR_float(ctx, GlobalSkylineInfo_ft);

    struct futhark_f32_2d *GS_data_ft;
    struct futhark_i64_1d *GS_idxs_ft;
    futhark_project_opaque_skylineData_GFUR_float_skyTups(ctx, &GS_data_ft, GlobalSkylineData_ft);
    futhark_project_opaque_skylineData_GFUR_float_pL(ctx, &GS_idxs_ft, GlobalSkylineData_ft);
    mylog(logfile, "Projected futhark primitive arrays.");

    futhark_context_sync(ctx);
    mylog(logfile, "Synced futhark context.");

    int64_t GlobalSkyline_len;
    futhark_project_opaque_skylineData_GFUR_float_len(ctx, &GlobalSkyline_len, GlobalSkylineData_ft);
    futhark_free_opaque_skylineData_GFUR_float(ctx, GlobalSkylineData_ft);
    // TODO testing
    printf("Skyline length: %ld\n", GlobalSkyline_len);

    float *GlobalSkyline_byteData = malloc(GlobalSkyline_len*DIM*sizeof(float));
    idx_t *GlobalSkyline_idxData = malloc(GlobalSkyline_len*sizeof(int64_t));
    mylog(logfile, "Allocated memory to unwrap results from futhark core.");

    futhark_values_f32_2d(ctx, GS_data_ft, GlobalSkyline_byteData);
    futhark_values_i64_1d(ctx, GS_idxs_ft, GlobalSkyline_idxData);
    mylog(logfile, "Unwrapped data into byte array.");
    futhark_free_f32_2d(ctx, GS_data_ft);
    futhark_free_i64_1d(ctx, GS_idxs_ft);
    // TODO testing
    /*
      printf("Skyline tuples (from bytes):\n");
      for(idx_t i=0; i<GlobalSkyline_len; i++) {
        for(idx_t d=0; d<DIM; d++) {
          float x = GlobalSkyline_byteData[i*DIM + d];
          printf("%4.3f", x);
          if(d<DIM-1) printf(", ");
          else printf("\n");
        }
      }
    */

    void *GlobalSkyline_numData[DIM];
    payloadColumnsFromByteArray(
      GlobalSkyline_numData,
      type_ids,
      (void*)GlobalSkyline_byteData,
      DIM,
      GlobalSkyline_len
    );
    mylog(logfile, "Parsed byte array to separate float arrays for each column.");
    free(GlobalSkyline_byteData);
    // TODO testing
    /*
      printf("Skyline tuples:\n");
      for(idx_t i=0; i<GlobalSkyline_len; i++) {
        for(idx_t d=0; d<DIM; d++) {
          printf("%4.3f", ((float*)GlobalSkyline_numData[d])[i]);
          if(d<DIM-1) printf(", ");
          else printf("\n");
        }
      }
    */
  
  // 3 - Store Results to Database
    mylog(logfile, "Now to store results in duckdb database...");
    char finalQueryStr[300 + 20*DIM];
    int finalQueryLen = sprintf(finalQueryStr, "CREATE OR REPLACE TABLE globalSkylineTbl (");
    for(idx_t i=0; i<DIM; i++) {
      finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "x%ld FLOAT, ", i+1);
    }
    finalQueryLen += sprintf(finalQueryStr+finalQueryLen, "rowidx BIGINT);");
    if( duckdb_query(con, finalQueryStr, NULL) == DuckDBError ) {
      perror("Failed to create final result table.\n");
      perror(finalQueryStr);
      return -1;
    }

    duckdb_appender gs_appender;
    if( duckdb_appender_create(con, NULL, "globalSkylineTbl", &gs_appender) == DuckDBError ) {
      perror("Failed to create appender.\n");
      return -1;
    }
    mylog(logfile, "Created appender.");

    // Store the results chunk-by-chunk
    duckdb_logical_type ltype_ids[DIM+1];
    for(idx_t i=0; i<DIM; i++) {
      ltype_ids[i] = duckdb_create_logical_type(DUCKDB_TYPE_FLOAT);
    }
    ltype_ids[DIM] = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);
    mylog(logfile, "Appending data chunk-by-chunk...");
    for(idx_t i=0; i<GlobalSkyline_len; i+=CHUNK_SIZE) {
      duckdb_data_chunk cnk = duckdb_create_data_chunk(ltype_ids, DIM+1);

      idx_t to_copy = (GlobalSkyline_len-i >= CHUNK_SIZE)? CHUNK_SIZE: GlobalSkyline_len-i;
      duckdb_data_chunk_set_size(cnk, to_copy);
      for(idx_t col=0; col<DIM; col++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
        void *dat = duckdb_vector_get_data(vec);
        memcpy(dat, GlobalSkyline_numData[col] + i*sizeof(float), to_copy*sizeof(float));
      }
      duckdb_vector idx_vec = duckdb_data_chunk_get_vector(cnk, DIM);
      void *idx_dat = duckdb_vector_get_data(idx_vec);
      memcpy(idx_dat, GlobalSkyline_idxData + i, to_copy*sizeof(int64_t));

      if(duckdb_append_data_chunk(gs_appender, cnk) == DuckDBError) {
        perror("Failed to append data chunk.\n");
        return -1;
      }

      duckdb_destroy_data_chunk(&cnk);
      //if((i+CHUNK_SIZE)%(60*CHUNK_SIZE)==0) {
      if(true) {
        duckdb_appender_flush(gs_appender);
      }
    }
    duckdb_appender_destroy(&gs_appender);
    mylog(logfile, "Finished appending data to final result table.");

  // Cleanup
    futhark_free_opaque_skylineBase_float(ctx, skB);
    for(idx_t i=0; i<DIM+1; i++) {
      duckdb_destroy_logical_type(&ltype_ids[i]);
    }
    free(GlobalSkyline_idxData);
    for(idx_t i=0; i<DIM; i++) {
      free(GlobalSkyline_numData[i]);
    }
    mylog(logfile, "Peformed cleanup for buffers and parameters.");

    futhark_context_free(ctx);
    futhark_context_config_free(cfg);
    mylog(logfile, "Freed futhark core.");

  	duckdb_disconnect(&con);
  	duckdb_close(&db);
    mylog(logfile, "Disconnected duckdb and freed its memory.");

    logclose(logfile);
  
  return 0;
}
