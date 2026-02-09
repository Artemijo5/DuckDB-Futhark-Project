#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../clibs/duckdb.h"

#include "../../ft_clibs/ftSkyline.h"
#include "../../clibs/mylogger.h"
#include "../../clibs/db_util.h"

#include <unistd.h>
#include <getopt.h>

#define default_LOGFILE "stdout"//"logs/skyline.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define CNK_TO_READ (long)4
#define default_BUFFER_CAP (long)1//(long)2*CNK_TO_READ
#define default_BUFFER_SIZE BUFFER_CAP*CHUNK_SIZE
#define default_TABLE_SIZE 16*BUFFER_SIZE//10*BUFFER_SIZE

#define default_DIM (long)4
#define default_ANGULAR_SUBDIV (long)2
#define default_MINVAL (float)0.0
#define default_MAXVAL (float)100.0

#define default_SKYLINE_MEMSIZE 256*1024*CHUNK_SIZE
#define default_USE_MANY_PTS false
#define default_USE_MANY_PTS_LOCAL false
#define default_SKIP_LOCAL_SKYLINE false
#define default_SKIP_MERGED_LOCAL_SKYLINE false

#define default_MAX_SUBDIV (int64_t)90
#define default_MIN_SUBDIV (int64_t)45
#define default_SUBDIV_STEP (int64_t)45
#define default_SIZE_THRESH default_BUFFER_SIZE/100

#define default_CREATE_TABLE false
#define default_TABLE_NAME "skyTbl"

#define default_DBFILE "testdb.db"
#define default_DDB_MEMSIZE "20GB"
#define default_DDB_TEMPDIR "/tps_tempdir"

#define default_VERBOSE false

int main(int argc, char *argv[]) {
  // Parse command line arguments
    // Initializations
      char LOGFILE[250] = default_LOGFILE;
      int64_t BUFFER_CAP = default_BUFFER_CAP;
      int64_t BUFFER_SIZE = BUFFER_CAP*CHUNK_SIZE;
      int64_t TABLE_SIZE = default_TABLE_SIZE;
      int64_t DIM = default_DIM;
      int64_t ANGULAR_SUBDIV = default_ANGULAR_SUBDIV;
      float MINVAL = default_MINVAL;
      float MAXVAL = default_MAXVAL;
      int64_t SKYLINE_MEMSIZE = default_SKYLINE_MEMSIZE;
      bool USE_MANY_PTS = default_USE_MANY_PTS;
      bool USE_MANY_PTS_LOCAL = default_USE_MANY_PTS_LOCAL;
      bool SKIP_LOCAL_SKYLINE = default_SKIP_LOCAL_SKYLINE;
      bool SKIP_MERGED_LOCAL_SKYLINE = default_SKIP_MERGED_LOCAL_SKYLINE;
      int64_t MAX_SUBDIV = default_MAX_SUBDIV;
      int64_t MIN_SUBDIV = default_MIN_SUBDIV;
      int64_t SUBDIV_STEP = default_SUBDIV_STEP;
      int64_t SIZE_THRESH = default_SIZE_THRESH;
      bool CREATE_TABLE = default_CREATE_TABLE;
      char TABLE_NAME[250] = default_TABLE_NAME;
      char DBFILE[250] = default_DBFILE;
      char DDB_MEMSIZE[25] = default_DDB_MEMSIZE;
      char DDB_TEMPDIR[250] = default_DDB_TEMPDIR;
      bool VERBOSE = default_VERBOSE;

    static struct option long_options[] =
      {
          {"logfile", required_argument, 0, 'L'},
          {"buffer_cap", required_argument, 0, 'B'},
          {"table_size", required_argument, 0, 'T'},
          {"dim", required_argument, 0, 'D'},
          {"angular_subdiv", required_argument, 0, 'a'},
          {"minval", required_argument, 0, 'I'},
          {"maxval", required_argument, 0, 'S'},
          {"memsize", required_argument, 0, 'M'},
          {"use_many_pts", no_argument, 0, 'u'},
          {"use_many_pts_local", no_argument, 0, 'U'},
          {"skip_local", no_argument, 0, 'n'},
          {"skip_merged_local", no_argument, 0, 'N'},
          {"max_subdiv", required_argument, 0, '1'},
          {"min_subdiv", required_argument, 0, '2'},
          {"subdiv_step", required_argument, 0, '3'},
          {"size_thresh", required_argument, 0, 's'},
          {"create_table", no_argument, 0, 'C'},
          {"table_name", required_argument, 0, 't'},
          {"db_file", required_argument, 0, 'f'},
          {"db_memsize", required_argument, 0, 'm'},
          {"db_tempdir", required_argument, 0, 'd'},
          {"verbose", no_argument, 0, 'v'},
          {0, 0, 0, 0}
      };
    char ch;
    while(
      (ch = getopt_long_only(argc,argv,"L:B:T:D:a:I:S:M:uUnN1:2:3:s:Ct:f:m:d:v",long_options,NULL)) != -1
    ) {
      switch(ch) {
        case 'L':
          memcpy(LOGFILE, optarg, strlen(optarg)+1); break; 
        case 'B':
          BUFFER_CAP = atol(optarg); BUFFER_SIZE = BUFFER_CAP*CHUNK_SIZE; break;
        case 'T':
          TABLE_SIZE = atol(optarg); break;
        case 'D':
          DIM = atol(optarg); break;
        case 'a':
          ANGULAR_SUBDIV = atol(optarg); break;
        case 'I':
          MINVAL = (float)atof(optarg); break;
        case 'S':
          MAXVAL = (float)atof(optarg); break;
        case 'M':
          SKYLINE_MEMSIZE = atol(optarg); break;
        case 'u':
          USE_MANY_PTS = true; break;
        case 'U':
          USE_MANY_PTS_LOCAL = true; break;
        case 'n':
          SKIP_LOCAL_SKYLINE = true; break;
        case 'N':
          SKIP_MERGED_LOCAL_SKYLINE = true; break;
        case '1':
          MAX_SUBDIV = atol(optarg); break;
        case '2':
          MIN_SUBDIV = atol(optarg); break;
        case '3':
          SUBDIV_STEP = atol(optarg); break;
        case 's':
          SIZE_THRESH = atol(optarg); break;
        case 'C':
          CREATE_TABLE = true; break;
        case 't':
          memcpy(TABLE_NAME, optarg, strlen(optarg)+1); break;
        case 'f':
          memcpy(DBFILE, optarg, strlen(optarg)+1); break;
        case 'm':
          memcpy(DDB_MEMSIZE, optarg, strlen(optarg)+1); break;
        case 'd':
          memcpy(DDB_TEMPDIR, optarg, strlen(optarg)+1); break;
        case 'v':
          VERBOSE=true; break;
      }
    }

  // Initialise logger
    FILE* logfile = loginit(LOGFILE, "Skyline Computation : Starting test program.");
    if(LOGFILE && !logfile) {
      perror("Failed to initialise logger.\n");
      return -1;
    }
    FILE* func_logfile = (VERBOSE)? logfile: NULL;

  // Log program parametres
  char log_param[10000];
  sprintf(log_param,
    "Logging program parametres:\n"
    "\tTABLE SIZE           %ld\n"
    "\tBUFFER SIZE          %ld\n"
    "\tBUFFER CAPACITY      %ld\n"
    "\tDATA DIMENSIONS      %ld\n"
    "\tANGLE SUBDIVISIONS   %ld\n"
    "\tINTERM MAX_SUBDIV    %ld\n"
    "\tINTERM MIN_SUBDIV    %ld\n"
    "\tINTERM SUBDIV_STEP   %ld\n"
    "\tSIZE THRESHOLD       %ld\n"
    "\tSKYLINE MEMSIZE      %ld\n"
    "\tFILTER WITH MANY PTS %d\n"
    "\t-||- LOCALLY         %d\n"
    "\tSKIP LOCAL SKYLINE   %d\n"
    "\tVERBOSE            %d",
    TABLE_SIZE,BUFFER_SIZE,BUFFER_CAP,
    DIM,ANGULAR_SUBDIV,
    MAX_SUBDIV,MIN_SUBDIV,SUBDIV_STEP,SIZE_THRESH,
    SKYLINE_MEMSIZE,
    USE_MANY_PTS,USE_MANY_PTS_LOCAL,SKIP_LOCAL_SKYLINE,VERBOSE
  );
  mylog(logfile, log_param);

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
    if(CREATE_TABLE) {
      char queryStr[150 + strlen(TABLE_NAME) + 20*DIM];
      int queryLen = sprintf(queryStr, "CREATE OR REPLACE TABLE %s (", TABLE_NAME);
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
      int insertQueryLen = sprintf(insertQueryStr, "INSERT INTO %s (SELECT ", TABLE_NAME);
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

      mylog(logfile, "Created test table.");
    }
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
    	prefixSizes[i] = i*sizeof(float);
    	mins[i] = MINVAL;
    	maxs[i] = MAXVAL;
    	grid_per_dim[i] = 1;
    	if(i<DIM-1) {
    		int64_t this_angl_per_dim = (i>0)? ANGULAR_SUBDIV: 2;
        angl_per_dim[i] = (SKIP_LOCAL_SKYLINE)? 1: this_angl_per_dim;
    	}
    }

  // Define SkylineBase
	  struct futhark_opaque_skylineBase_float *skB;
	  struct futhark_f32_1d *mins_ft = futhark_new_f32_1d(ctx,mins,DIM);
	  struct futhark_f32_1d *maxs_ft = futhark_new_f32_1d(ctx,maxs,DIM);
	  struct futhark_i64_1d *grids_ft = futhark_new_i64_1d(ctx, grid_per_dim, DIM);
	  struct futhark_i64_1d *angls_ft = futhark_new_i64_1d(ctx, angl_per_dim, DIM-1);
	  futhark_entry_make_skylineBase_float(ctx, &skB, mins_ft, maxs_ft, grids_ft, angls_ft, SKYLINE_MEMSIZE);
	  futhark_context_sync(ctx);
	  mylog(logfile, "Defined data space in the form of SkylineBase.");
	  futhark_free_f32_1d(ctx, mins_ft);
	  futhark_free_f32_1d(ctx, maxs_ft);
	  futhark_free_i64_1d(ctx, grids_ft);
	  futhark_free_i64_1d(ctx, angls_ft);

	struct futhark_opaque_skylineInfo_float *skyWindows[5];
	idx_t currently_windowed = 0;

  // 1 - Calculate Intermediate Skylines per Window, Collapsing Windows
    duckdb_result res;
    char select_str[100 + strlen(TABLE_NAME)];
    sprintf(select_str, "SELECT * FROM %s;", TABLE_NAME);
    duckdb_query(con, select_str, &res);
    mylog(logfile, "Performed query to read skyTbl.");
    int isTblExhausted = false;
    idx_t cur_idx=0;
    bool self_filter_first = true; // for merge-filter func - true only for its first iteration
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
    	mylog(func_logfile, " - - Obtained chunks.");

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
      	mylog(func_logfile, " - - Buffered data from chunks.");

      	struct futhark_f32_2d *skyData_ft = futhark_new_f32_2d(ctx, (float*)skyData, cur_rows, DIM);
      	mylog(func_logfile, " - - Wrapped data into futhark core.");

        const char* ftErr = futhark_context_get_error(ctx);
        if(ftErr) perror(ftErr);


      	free(skyData);

      	struct futhark_opaque_skylineInfo_float *skyWindow;
      	struct futhark_opaque_skylineInfo_float *local_skyWindow;
      	struct futhark_opaque_skylineInfo_float *interm_skyWindow;

      	futhark_entry_skyline_slice_and_dice_float(ctx, &skyWindow, skB, skyData_ft, cur_idx, USE_MANY_PTS);
      	mylog(func_logfile, " - - Applied pointwise filtering & angular partitioning ('slice-and-dice').");
      	futhark_free_f32_2d(ctx, skyData_ft);

        if (!SKIP_LOCAL_SKYLINE) {
          futhark_entry_skyline_local_filter_float(
            ctx, &local_skyWindow, skB, skyWindow, USE_MANY_PTS_LOCAL
          );
        	futhark_free_opaque_skylineInfo_float(ctx, skyWindow);
        	mylog(func_logfile, " - - Applied local filtering.");
          //printf("\n\n%s\n\n", futhark_context_get_error(ctx));
          futhark_entry_skyline_intermediate_filter_float(
            ctx, &interm_skyWindow, skB, local_skyWindow, MAX_SUBDIV, MIN_SUBDIV, SUBDIV_STEP, SIZE_THRESH
          );
        	futhark_free_opaque_skylineInfo_float(ctx, local_skyWindow);
        	mylog(func_logfile, " - - Applied intermediate filtering steps.");
        }
        else {
          futhark_entry_calc_Global_Skyline_float(ctx, &interm_skyWindow, skB, skyWindow);
          futhark_free_opaque_skylineInfo_float(ctx, skyWindow);
          mylog(func_logfile, " - - Directly applied global filtering.");
        }

      	skyWindows[currently_windowed++] = interm_skyWindow;
      	mylog(func_logfile, " - - Cached this window's filtered results.");
        cur_idx += cur_rows;
      }

    	// Collapse windows
    	if(isTblExhausted || currently_windowed==5) {
    		logdbg(
    			func_logfile,
    			isTblExhausted,
    			" - - | - - Data exhausted - collapsing windows...",
    			" - - | - - Window cache filled - collapsing windows..."
    		);

    		struct futhark_opaque_skylineInfo_float *collapsed_skyWindow;
    		struct futhark_opaque_skylineInfo_float *local_collapsed_skyWindow;
    		struct futhark_opaque_skylineInfo_float *interm_collapsed_skyWindow;
    		
    		switch(currently_windowed) {
    			case 1:
            if (!self_filter_first || !(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE))
    				  collapsed_skyWindow = skyWindows[0];
            else
              futhark_entry_calc_Global_Skyline_float(
                ctx, &collapsed_skyWindow, skB, skyWindows[0]
              );
    				break;
    			case 2:
            if (!(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE))
      				futhark_entry_skyline_merge_2_float(
      					ctx, &collapsed_skyWindow,
      					skyWindows[0], skyWindows[1]
      				);
            else
              futhark_entry_skyline_mergeFilter_2_float(
                ctx, &collapsed_skyWindow, self_filter_first, skB,
                skyWindows[0], skyWindows[1]
              );
    				break;
    			case 3:
    				if (!(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE))
              futhark_entry_skyline_merge_3_float(
                ctx, &collapsed_skyWindow,
                skyWindows[0], skyWindows[1], skyWindows[2]
              );
            else
              futhark_entry_skyline_mergeFilter_3_float(
                ctx, &collapsed_skyWindow, self_filter_first, skB,
                skyWindows[0], skyWindows[1], skyWindows[2]
              );
    				break;
    			case 4:
    				if (!(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE))
              futhark_entry_skyline_merge_4_float(
                ctx, &collapsed_skyWindow,
                skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3]
              );
            else
              futhark_entry_skyline_mergeFilter_4_float(
                ctx, &collapsed_skyWindow, self_filter_first, skB,
                skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3]
              );
    				break;
    			default:
    				if (!(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE))
              futhark_entry_skyline_merge_5_float(
                ctx, &collapsed_skyWindow,
                skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3], skyWindows[4]
              );
            else
              futhark_entry_skyline_mergeFilter_5_float(
                ctx, &collapsed_skyWindow, self_filter_first, skB,
                skyWindows[0], skyWindows[1], skyWindows[2], skyWindows[3], skyWindows[4]
              );
    				break;
    		}
    		if(currently_windowed>1) {
    			for(idx_t w=0; w<currently_windowed; w++) {
    				futhark_free_opaque_skylineInfo_float(ctx, skyWindows[w]);
    			}
    		}
    		mylog(func_logfile, " - - | - - Collapsed cached windows.");

    		if(currently_windowed>1 && !(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE)) {
      		futhark_entry_skyline_local_filter_float(
    	  		ctx, &local_collapsed_skyWindow, skB, collapsed_skyWindow, USE_MANY_PTS_LOCAL
    	  	);
    			futhark_free_opaque_skylineInfo_float(ctx, collapsed_skyWindow);
    			mylog(func_logfile, " - - | - - Applied local filtering.");
    			futhark_entry_skyline_intermediate_filter_float(
    				ctx, &interm_collapsed_skyWindow, skB, local_collapsed_skyWindow, MAX_SUBDIV, MIN_SUBDIV, SUBDIV_STEP, SIZE_THRESH
    			);
    			futhark_free_opaque_skylineInfo_float(ctx, local_collapsed_skyWindow);
    			mylog(func_logfile, " - - | - - Applied intermediate filtering steps.");
    		}
    		else {
    			interm_collapsed_skyWindow = collapsed_skyWindow;
    			mylog(func_logfile, " - - | - - Don't need to apply additional filtering steps.");
    		}
    		
  	  	skyWindows[0] = interm_collapsed_skyWindow;
  	  	currently_windowed = 1;
        self_filter_first = false;
    	}
    }
    mylog(logfile, "Result is exhausted.");
    duckdb_destroy_result(&res);

  // 2 - Compute & Unwrap Global Skyline
    mylog(logfile, "Computing global skyline...");
    // At this stage, all data is in skyWindows[0]
    // Any needed intermediate filtering has already been applied
    // So, we only need to apply the final Global Skyline function
    struct futhark_opaque_skylineInfo_float *GlobalSkylineInfo_ft;
    if(!(SKIP_LOCAL_SKYLINE || SKIP_MERGED_LOCAL_SKYLINE)) {
      futhark_entry_calc_Global_Skyline_float(ctx, &GlobalSkylineInfo_ft, skB, skyWindows[0]);
      mylog(logfile, "Function to compute global skyline has returned.");
      futhark_free_opaque_skylineInfo_float(ctx, skyWindows[0]);
    }
    else {
      GlobalSkylineInfo_ft = skyWindows[0];
      mylog(logfile, "Global skyline retrieved from collapsed skyWindow.");
    }

    mylog(logfile, "Unwrapping data from futhark core...");
    struct futhark_opaque_skylineData_float *GlobalSkylineData_ft;
    futhark_entry_crack_skylineInfo_float(ctx, &GlobalSkylineData_ft, GlobalSkylineInfo_ft);
    mylog(logfile, "'Cracked' SkylineInfo.");
    futhark_free_opaque_skylineInfo_float(ctx, GlobalSkylineInfo_ft);

    struct futhark_f32_2d *GS_data_ft;
    struct futhark_i64_1d *GS_idxs_ft;
    futhark_project_opaque_skylineData_float_dat(ctx, &GS_data_ft, GlobalSkylineData_ft);
    futhark_project_opaque_skylineData_float_pL(ctx, &GS_idxs_ft, GlobalSkylineData_ft);
    mylog(logfile, "Projected futhark primitive arrays.");

    futhark_context_sync(ctx);
    mylog(logfile, "Synced futhark context.");

    int64_t GlobalSkyline_len;
    futhark_project_opaque_skylineData_float_len(ctx, &GlobalSkyline_len, GlobalSkylineData_ft);
    futhark_free_opaque_skylineData_float(ctx, GlobalSkylineData_ft);
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
