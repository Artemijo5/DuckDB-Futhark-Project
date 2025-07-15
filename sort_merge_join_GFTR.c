#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "mylogger.h"
#include "myutil.h"
#include "sortstages.h"

#include "ftSMJ.h"
#include "smjutil.h"

#define LOGFILE "sort_merge_join_GFTR.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 512*CHUNK_SIZE

#define R_TABLE_SIZE 8*CHUNK_SIZE + 526
#define S_TABLE_SIZE 26*CHUNK_SIZE + 526

#define BLOCK_SIZE (int16_t)256 // used for multi-pass gather and scatter operations (and by extension blocked sorting)
#define EXT_PARALLELISM 1024 // decides the "upper bound" of external threads in some nested parallel operations (possibly redudant)
#define MERGE_PARTITION_SIZE 64 // average size of each partition in ONE array (half the size of co-partitions by Merge Path)
#define RESCALE_FACTOR 5 // (arbitrarily) used to set the number of windows vs the number of partitions

#define R_JOIN_BUFFER 3*CHUNK_SIZE
#define S_JOIN_BUFFER 3*R_JOIN_BUFFER
#define JOIN_TBL_NAME "R_S_joinTbl"

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "4GB"
#define DDB_TEMPDIR "tps_tempdir"

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Two-Pass Sort : Starting test program.");
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

  // Create tables R and S 
  duckdb_query(con, "setseed(0.42);", NULL);
  duckdb_query(con, "CREATE OR REPLACE TABLE R_tbl (k BIGINT, payload1 BIGINT, payload2 DOUBLE);", NULL);
  duckdb_query(con, "CREATE OR REPLACE TABLE S_tbl (k BIGINT, payload3 DOUBLE, payload4 DOUBLE);", NULL);

  // Create the test tables.
  char R_init_query[1000];
  char S_init_query[1000];
  sprintf(
  	R_init_query,
  	"INSERT INTO R_tbl (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
  	R_TABLE_SIZE
  );
  sprintf(
  	S_init_query,
  	"INSERT INTO S_tbl (SELECT 1000000*random(), 10000*random(), 10000*random() FROM range(%ld) t(i));",
  	S_TABLE_SIZE
  );
  duckdb_query(con, R_init_query, NULL);
  duckdb_query(con, S_init_query, NULL);
  mylog(logfile, "Created the tables R and S.");

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  mylog(logfile, "Now sorting the tables...");
  // R
  two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    "R_tbl",
    "R_tmp_interm",
    "R_tbl_sorted",
    false,
    false
  );
  mylog(logfile, "Sorted table R.");
  // S
  two_pass_sort_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    BLOCK_SIZE,
    NULL,
    ctx,
    con,
    "S_tbl",
    "S_tmp_interm",
    "S_tbl_sorted",
    false,
    false
  );
  mylog(logfile, "Sorted table S.");  

// ############################################################################################################
// ############################################################################################################
// ############################################################################################################
// JOIN PHASE
// ############################################################################################################
// ############################################################################################################
// ############################################################################################################

  // Read R's sorted keys
  mylog(logfile, "Preparing for join - obtain R's sorted keys...");
  duckdb_result res_Rk;
  if( duckdb_query(con, "SELECT * FROM R_tbl_sorted;", &res_Rk) == DuckDBError) {
    perror("Failed to read R_tbl_sorted...\n");
    return -1;
  }
  mylog(logfile, "Obtained sorted R result.");
  // result info
  idx_t R_col_count = duckdb_column_count(&res_Rk);
  duckdb_type R_type_ids[R_col_count];
  for(idx_t col=0; col<R_col_count; col++) {
    R_type_ids[col] = duckdb_column_type(&res_Rk, col);
  }
  mylog(logfile, "Obtained R's column & type info.");
  // Key type
  duckdb_type key_type = R_type_ids[0];
  // ALSO OBTAIN S INFO
  duckdb_result S_dummyRes;
  if( duckdb_query(con, "SELECT * FROM S_tbl_sorted LIMIT 1;", &S_dummyRes) == DuckDBError) {
    perror("Failed to obtain info for S table...\n");
    return -1;
  }
  idx_t S_col_count = duckdb_column_count(&S_dummyRes);
  duckdb_type S_type_ids[S_col_count];
  for(idx_t col=0; col<S_col_count; col++) {
    S_type_ids[col] = duckdb_column_type(&S_dummyRes, col);
  }
  mylog(logfile, "Obtained S's column & type info.");
  if(S_type_ids[0] != key_type) {
    perror("Key type mismatch!!!!!");
    return -1;
  }
  duckdb_destroy_result(&S_dummyRes);

  idx_t R_curIdx = 0;
  idx_t S_minimum_relevant_idx = 0; // minimum index of the first relevant chunk of S
  idx_t S_curIdx = 0;

  // ##### 0 --- Create Join Table & Appender

  // 0.0 create strings for types...
  char R_type_strs[R_col_count][25];
  for(idx_t col=0; col<R_col_count; col++) {
    switch(R_type_ids[col]){
      case DUCKDB_TYPE_SMALLINT:
        sprintf( R_type_strs[col], "SMALLINT" );
        break;
      case DUCKDB_TYPE_INTEGER:
        sprintf( R_type_strs[col], "INTEGER" );
        break;
      case DUCKDB_TYPE_BIGINT:
        sprintf( R_type_strs[col], "BIGINT" );
        break;
      case DUCKDB_TYPE_FLOAT:
        sprintf( R_type_strs[col], "FLOAT" );
        break;
      case DUCKDB_TYPE_DOUBLE:
        sprintf( R_type_strs[col], "DOUBLE" );
        break;
      default:
        perror("Invalid type.");
        return -1;
    }
  }
  char S_type_strs[S_col_count][25];
  for(idx_t col=0; col<S_col_count; col++) {
    switch(S_type_ids[col]){
      case DUCKDB_TYPE_SMALLINT:
        sprintf( S_type_strs[col], "SMALLINT" );
        break;
      case DUCKDB_TYPE_INTEGER:
        sprintf( S_type_strs[col], "INTEGER" );
        break;
      case DUCKDB_TYPE_BIGINT:
        sprintf( S_type_strs[col], "BIGINT" );
        break;
      case DUCKDB_TYPE_FLOAT:
        sprintf( S_type_strs[col], "FLOAT" );
        break;
      case DUCKDB_TYPE_DOUBLE:
        sprintf( S_type_strs[col], "DOUBLE" );
        break;
      default:
        perror("Invalid type.");
        return -1;
    }
  }

  // Create the Table
  char joinTbl_init_query[150 + strlen(JOIN_TBL_NAME) + 30 + (R_col_count-1)*30 + (S_col_count-1)*30];
  int joinTbl_strLen = sprintf(
    joinTbl_init_query,
    "CREATE OR REPLACE TABLE %s (k %s",
    JOIN_TBL_NAME, R_type_strs[0]
  );
  for(idx_t col=1; col<R_col_count; col++) {
    joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ", R%ld %s", col, R_type_strs[col]);
  }
  for(idx_t col=1; col<S_col_count; col++) {
    joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ", S%ld %s", col, S_type_strs[col]);
  }
  joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ");");
  // EXECUTE THE QUERY TO CREATE THE TABLE
  if (duckdb_query(con, joinTbl_init_query, NULL) == DuckDBError) {
    perror("Failed to create Join Result Table.");
    return -1;
  }
  mylog(logfile, "Created result table where join pairs will be stored.");

  // Create composite logical_type id info
  duckdb_logical_type join_type_ids[R_col_count + S_col_count - 1];
  join_type_ids[0] = duckdb_create_logical_type(key_type);
  for(idx_t col=1; col<R_col_count; col++) {
    join_type_ids[col] = duckdb_create_logical_type(R_type_ids[col]);
  }
  for(idx_t col=1; col<S_col_count; col++) {
    join_type_ids[col + R_col_count - 1] = duckdb_create_logical_type(S_type_ids[col]);
  }

  // Create the Appender
  duckdb_appender join_appender;
  if( duckdb_appender_create(con, NULL, JOIN_TBL_NAME, &join_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return -1;
  }

  mylog(logfile, "Created result table and its appender.");

  // TODO ##### Loop over R (left table) -- for each chunk of R we will loop over S
  mylog(logfile, "Iterating over R...");
  while(true) {
    idx_t R_rowCount = 0;
    void* Rbuff[R_col_count];
    for(idx_t col=0; col<R_col_count; col++) {
      Rbuff[col] = colType_malloc(R_type_ids[col], R_JOIN_BUFFER);
    }

    // READ R DATA INTO BUFFER
    while(R_rowCount < R_JOIN_BUFFER) {
      duckdb_data_chunk cnk = duckdb_fetch_chunk(res_Rk);
      if(!cnk) {
        mylog(logfile, "Exhausted sorted R result.");
        break;
      }
      idx_t curRows = duckdb_data_chunk_get_size(cnk);
      // Read all column data
      for(idx_t col=0; col<R_col_count; col++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk,col);
        void* dat = (void*)duckdb_vector_get_data(vec);
        // Copy to buffer
        memcpy(
          Rbuff[col] + R_rowCount*colType_bytes(R_type_ids[col]),
          dat,
          curRows*colType_bytes(R_type_ids[col])
        );
      }
      duckdb_destroy_data_chunk(&cnk);
      R_rowCount += curRows;
    }
    if(R_rowCount == 0) break; // Nothing left to join.
    mylog(logfile, "Buffered this partition of R.");

    mylog(logfile, "Obtain S's sorted keys...");
    duckdb_result res_Sk;
    // TODO construct S query to read from minimum relevant index
    char S_sorted_query_str[250];
    sprintf(S_sorted_query_str, "SELECT * FROM S_tbl_sorted OFFSET %ld;", S_minimum_relevant_idx);
    if( duckdb_query(con, S_sorted_query_str, &res_Sk) == DuckDBError) {
      perror("Failed to read S_tbl_sorted...\n");
      return -1;
    }
    mylog(logfile, "Obtained S's sorted keys.");
    mylog(logfile, "Now iterating over S for the join...");
    S_curIdx = S_minimum_relevant_idx;
    int flag_continueWithThisR_partition = true; // when Sbuff.max > Rbuff.max, stop reading S chunks for this R partition
    // (saves an input to determine this afterwards)
    while(flag_continueWithThisR_partition) {
      idx_t S_rowCount = 0;
      void* Sbuff[S_col_count];
      for(idx_t col=0; col<S_col_count; col++) {
        Sbuff[col] = colType_malloc(S_type_ids[col], S_JOIN_BUFFER);
      }

      while(S_rowCount < S_JOIN_BUFFER) {
        duckdb_data_chunk cnk = duckdb_fetch_chunk(res_Sk);
        if(!cnk) {
          mylog(logfile, "Exhausted S result.");
          break;
        }
        idx_t curRows = duckdb_data_chunk_get_size(cnk);

        // FIRST BUFFER KEY COLUMN TO HANDLE IRRELEVANT CHUNKS
        duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk,0);
        void* kdat = (void*)duckdb_vector_get_data(kvec);
        // ----- Deal with irrelevant chunks
        // a. If cnk.max < Rbuffer.min -> continue to next buffer
        if(compare_max_to_min(key_type, kdat, Rbuff[0], curRows, R_rowCount) < 0) {
          mylog(logfile, "Skipping this S chunk (not reached relevant ones yet)...");
          S_curIdx += curRows;
          S_minimum_relevant_idx = S_curIdx;
          duckdb_destroy_data_chunk(&cnk);
          continue;
        }
        // b. If Rbuffer.max < cnk.min -> break
        if(compare_max_to_min(key_type, Rbuff[0], kdat, R_rowCount, curRows) < 0) {
          mylog(logfile, "S chunks no longer relevant, break...");
          duckdb_destroy_data_chunk(&cnk);
          flag_continueWithThisR_partition = false;
          break;
        }
        // c. Chunk is relevant, proceed with buffering...
        mylog(logfile, "Buffering this (relevant) S chunk...");
        // d. CHECK IF IT'LL BE RELEVANT FOR NEXT R PARTITION
        int Smax_vs_Rmax = compare_maxima(key_type, kdat, Rbuff[0], curRows, R_rowCount);
        if(Smax_vs_Rmax < 0) {
          mylog(logfile, "(This chunk will be irrelevant in the next iteration over R.)");
          S_minimum_relevant_idx += curRows;
        }
        // Copy to buffer
        memcpy(
          Sbuff[0] + S_rowCount*colType_bytes(key_type),
          kdat,
          curRows*colType_bytes(key_type)
        );
        // NOW BUFFER PAYLOAD COLUMNS
        for(idx_t col=1; col<S_col_count; col++) {
          duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
          void* dat = (void*)duckdb_vector_get_data(vec);
          memcpy(
            Sbuff[col] + S_rowCount*colType_bytes(S_type_ids[col]),
            dat,
            curRows*colType_bytes(S_type_ids[col])
          );
        }
        duckdb_destroy_data_chunk(&cnk);
        S_rowCount += curRows;

        // e. If cnk.max > Rbuff.max, stop buffering S chunks
        if (Smax_vs_Rmax > 0) {
          mylog(logfile, "(This is the last relevant chunk for this partition of R)");
          flag_continueWithThisR_partition = false;
          break;
        }
      }
      if(S_rowCount == 0) break; // Nothing left to join.
      mylog(logfile, "Buffered this partition of S.");

      // Perform join
      idx_t numPairs = 0;
      void* joinedKeys;
      struct futhark_i64_1d *idxR_ft;
      struct futhark_i64_1d *idxS_ft;
      // Calculate partition & window numbers for this size...
      idx_t avgSize = (R_rowCount+S_rowCount + 1)/2;
      idx_t numPartitions = (avgSize + MERGE_PARTITION_SIZE - 1) / MERGE_PARTITION_SIZE;
      idx_t numWindows = 1;
      // arbitrary rescaling...
      if(numPartitions >= RESCALE_FACTOR*2) {
        int rsFactor = RESCALE_FACTOR;
        idx_t rescale = (numPartitions + rsFactor - 1) / rsFactor;
        numWindows *= rescale;
        numPartitions /= rescale;
      }
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      // PERFORM THE JOIN
      // Internally, left side is the smaller one
      // (if implementing a left outer join, I'd have to subpartition the external left side instead...)
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      if (R_rowCount <= S_rowCount) {
        InnerJoin_joinKeyColumns(
          ctx,
          &numPairs,
          &joinedKeys, // vs
          &idxR_ft, // R indices
          &idxS_ft, // S indices
          key_type,
          R_curIdx, // R_idx
          S_curIdx, // S_idx
          Rbuff[0], // R keys
          Sbuff[0], // S keys
          R_rowCount, // card1
          S_rowCount, // card2
          numWindows,
          numPartitions,
          EXT_PARALLELISM,
          BLOCK_SIZE // for multi-pass scatter
        );
        mylog(logfile, "Join has been performed (R first).");
      }
      else {
        InnerJoin_joinKeyColumns(
          ctx,
          &numPairs,
          &joinedKeys, // vs
          &idxS_ft, // S indices
          &idxR_ft, // R indices
          key_type,
          S_curIdx, // S_idx
          R_curIdx, // R_idx
          Sbuff[0], // S keys
          Rbuff[0], // R keys
          S_rowCount, // card2
          R_rowCount, // card1
          numWindows,
          numPartitions,
          EXT_PARALLELISM,
          BLOCK_SIZE // for multi-pass scatter
        );
        mylog(logfile, "Join has been performed (S first).");
      }
      // Gather R's payloads
      void* Rpl[R_col_count-1];
      for(idx_t col=1; col<R_col_count; col++) {
        Rpl[col-1] = colType_malloc(R_type_ids[col], numPairs);
        gatherPayloads(ctx, Rpl[col-1], R_type_ids[col], R_curIdx, BLOCK_SIZE, idxR_ft, Rbuff[col], R_rowCount, numPairs);
      }
      mylog(logfile, "Gathered R payloads.");
      // Gather S's payloads
      void* Spl[S_col_count-1];
      for(idx_t col=1; col<S_col_count; col++) {
        Spl[col-1] = colType_malloc(S_type_ids[col], numPairs);
        gatherPayloads(ctx, Spl[col-1], S_type_ids[col], S_curIdx, BLOCK_SIZE, idxS_ft, Sbuff[col], S_rowCount, numPairs);
      }
      mylog(logfile, "Gathered S payloads.");

      // CREATE DATA CHUNK AND APPEND
      duckdb_data_chunk joinCnk = duckdb_create_data_chunk(join_type_ids, (R_col_count+S_col_count-1));
      duckdb_data_chunk_set_size(joinCnk, numPairs);
      // Load keys
      duckdb_vector join_kvec = duckdb_data_chunk_get_vector(joinCnk, 0);
      void* join_kdat = duckdb_vector_get_data(join_kvec);
      memcpy(join_kdat, joinedKeys, numPairs*colType_bytes(key_type));
      // Proceed with payload columns - first for R
      for(idx_t col=1; col<R_col_count; col++) {
        duckdb_vector join_vec = duckdb_data_chunk_get_vector(joinCnk, col);
        void* join_dat = duckdb_vector_get_data(join_vec);
        memcpy(join_dat, Rpl[col-1], numPairs*colType_bytes(R_type_ids[col]));
      }
      // Then for S
      for(idx_t col=1; col<S_col_count; col++) {
        duckdb_vector join_vec = duckdb_data_chunk_get_vector(joinCnk, col + R_col_count - 1);
        void* join_dat = duckdb_vector_get_data(join_vec);
        memcpy(join_dat, Spl[col-1], numPairs*colType_bytes(R_type_ids[col]));
      }
      mylog(logfile, "Prepared datachunk to load.");
      // Then APPEND
      if(duckdb_append_data_chunk(join_appender, joinCnk) == DuckDBError) {
        perror("Failed to append data chunk.\n");
        return -1;
      }
      mylog(logfile, "Appended datachunk to result table.");

      // CLEANUP
      duckdb_destroy_data_chunk(&joinCnk);
      for(idx_t col=0; col<S_col_count; col++) {
        free(Sbuff[col]);
      }
      for(idx_t col=1; col<R_col_count; col++) {
        free(Rpl[col-1]);
      }
      for(idx_t col=1; col<S_col_count; col++) {
        free(Spl[col-1]);
      }
      free(joinedKeys);
      futhark_free_i64_1d(ctx, idxR_ft);
      futhark_free_i64_1d(ctx, idxS_ft);

      S_curIdx += S_rowCount;
    }

    duckdb_appender_flush(join_appender);

    for(idx_t col=0; col<R_col_count; col++) {
      free(Rbuff[col]);
    }
    duckdb_destroy_result(&res_Sk);

    R_curIdx += R_rowCount;
  }
  duckdb_destroy_result(&res_Rk);
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);

  // Clean-up  
  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}
