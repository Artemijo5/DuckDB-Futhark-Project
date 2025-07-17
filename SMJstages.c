#include "SMJstages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftSMJ.h"
#include "mylogger.h"
#include "myutil.h"
#include "smjutil.h"

void Inner_MergeJoin_GFTR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *sorted_R_tbl_name,
	const char *sorted_S_tbl_name,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
) {
  // Read R's sorted keys
  mylog(logfile, "Preparing for join - obtain R's sorted keys...");
  duckdb_result res_Rk;
  char readRq[100 + strlen(sorted_R_tbl_name)];
  sprintf(readRq, "SELECT * FROM %s;", sorted_R_tbl_name);
  if( duckdb_query(con, readRq, &res_Rk) == DuckDBError) {
    perror("Failed to read R_tbl_sorted...\n");
    return;
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
  char dummySq[100 + strlen(sorted_S_tbl_name)];
  sprintf(dummySq, "SELECT * FROM %s LIMIT 1;", sorted_S_tbl_name);
  if( duckdb_query(con, dummySq, &S_dummyRes) == DuckDBError) {
    perror("Failed to obtain info for S table...\n");
    return;
  }
  idx_t S_col_count = duckdb_column_count(&S_dummyRes);
  duckdb_type S_type_ids[S_col_count];
  for(idx_t col=0; col<S_col_count; col++) {
    S_type_ids[col] = duckdb_column_type(&S_dummyRes, col);
  }
  mylog(logfile, "Obtained S's column & type info.");
  if(S_type_ids[0] != key_type) {
    perror("Key type mismatch!!!!!");
    return;
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
        return;
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
        return;
    }
  }

  // Create the Table
  char joinTbl_init_query[250 + strlen(Join_tbl_name) + 30 + (R_col_count-1)*30 + (S_col_count-1)*30];
  int joinTbl_strLen = (saveAsTempTable)?
	  sprintf(
	    joinTbl_init_query,
	    "CREATE OR REPLACE TEMP TABLE %s (k %s",
	    Join_tbl_name, R_type_strs[0]
	  ):
	  sprintf(
	    joinTbl_init_query,
	    "CREATE OR REPLACE TABLE %s (k %s",
	    Join_tbl_name, R_type_strs[0]
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
    return;
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
  if( duckdb_appender_create(con, NULL, Join_tbl_name, &join_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return;
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
    char S_sorted_query_str[250 + strlen(sorted_S_tbl_name)];
    sprintf(S_sorted_query_str, "SELECT * FROM %s OFFSET %ld;", sorted_S_tbl_name, S_minimum_relevant_idx);
    if( duckdb_query(con, S_sorted_query_str, &res_Sk) == DuckDBError) {
      perror("Failed to read S_tbl_sorted...\n");
      return;
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
        memcpy(join_dat, Spl[col-1], numPairs*colType_bytes(S_type_ids[col]));
      }
      mylog(logfile, "Prepared datachunk to load.");
      // Then APPEND
      if(duckdb_append_data_chunk(join_appender, joinCnk) == DuckDBError) {
        perror("Failed to append data chunk.\n");
        return;
      }
      if(quicksaves) duckdb_appender_flush(join_appender);
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
}

void Inner_MergeJoin_GFUR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	idx_t PAYLOAD_INDEX_BLOCK,
	idx_t PAYLOAD_GATHER_BLOCK,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *R_tbl_name,
	const char *S_tbl_name,
	const char *sorted_R_tbl_name,
	const char *sorted_S_tbl_name,
	const char *Join_temp_tbl_name,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
) {
	Inner_MergeJoin_GFTR(
		CHUNK_SIZE,
		R_JOIN_BUFFER,
		S_JOIN_BUFFER,
		BLOCK_SIZE,
		EXT_PARALLELISM,
		MERGE_PARTITION_SIZE,
		RESCALE_FACTOR,
		logfile,
		ctx,
		con,
		sorted_R_tbl_name,
		sorted_S_tbl_name,
		Join_temp_tbl_name,
		quicksaves,
		true
	);

  // 0. Get payload info from R_tbl, S_tbl
  duckdb_result R_plInfo, S_plInfo; // use these results to obtain info about original payloads
  char R_pLQ[150 + strlen(R_tbl_name)];
  char S_pLQ[150 + strlen(S_tbl_name)];
  sprintf(R_pLQ, "SELECT * FROM %s LIMIT 1;", R_tbl_name);
  sprintf(S_pLQ, "SELECT * EXCLUDE(k) FROM %s LIMIT 1;", S_tbl_name);
  if (
    duckdb_query(con, R_pLQ, &R_plInfo) == DuckDBError
    ||
    duckdb_query(con, S_pLQ, &S_plInfo) == DuckDBError
  ) {
    perror("Failed to obtain original payload info.");
    return;
  }
  idx_t R_payload_count = duckdb_column_count(&R_plInfo)-1;
  idx_t S_payload_count = duckdb_column_count(&S_plInfo);
  idx_t total_payload_count = R_payload_count + S_payload_count;

  duckdb_type key_type = duckdb_column_type(&R_plInfo, 0);
  char key_type_str[25];
  switch(key_type) {
  	case DUCKDB_TYPE_SMALLINT:
  		sprintf(key_type_str, "SMALLINT");
  		break;
  	case DUCKDB_TYPE_INTEGER:
  		sprintf(key_type_str, "INTEGER");
  		break;
  	case DUCKDB_TYPE_BIGINT:
  		sprintf(key_type_str, "BIGINT");
  		break;
  	case DUCKDB_TYPE_FLOAT:
  		sprintf(key_type_str, "FLOAT");
  		break;
  	case DUCKDB_TYPE_DOUBLE:
  		sprintf(key_type_str, "DOUBLE");
  		break;
  	default:
  		perror("Invalid type.");
  		return;
  }

  duckdb_type payload_types[total_payload_count];
  for(idx_t col=0; col<R_payload_count; col++) {
    payload_types[col] = duckdb_column_type(&R_plInfo, col+1);
  }
  for(idx_t col=R_payload_count; col<total_payload_count; col++) {
    payload_types[col] = duckdb_column_type(&S_plInfo, col-R_payload_count);
  }
  duckdb_destroy_result(&R_plInfo);
  duckdb_destroy_result(&S_plInfo);

  // String & logical type equivalents
  char payload_type_strs[total_payload_count][25];
  duckdb_logical_type payload_ltypes[1+total_payload_count];
  payload_ltypes[0] = duckdb_create_logical_type(key_type);
  for(idx_t col=0; col<total_payload_count; col++) {
    payload_ltypes[1+col] = duckdb_create_logical_type(payload_types[col]);
    switch(payload_types[col]){
      case DUCKDB_TYPE_SMALLINT:
        sprintf( payload_type_strs[col], "SMALLINT" );
        break;
      case DUCKDB_TYPE_INTEGER:
        sprintf( payload_type_strs[col], "INTEGER" );
        break;
      case DUCKDB_TYPE_BIGINT:
        sprintf( payload_type_strs[col], "BIGINT" );
        break;
      case DUCKDB_TYPE_FLOAT:
        sprintf( payload_type_strs[col], "FLOAT" );
        break;
      case DUCKDB_TYPE_DOUBLE:
        sprintf( payload_type_strs[col], "DOUBLE" );
        break;
      default:
        perror("Invalid type.");
        printf("Col %ld\n", col);
        return;
    }
  }

  // 1. Create FINAL (not tmp) result table & appender
  char finTbl_init_query[150 + strlen(Join_tbl_name) + 30 + (total_payload_count)*30];
  int finTbl_strLen = sprintf(
    finTbl_init_query,
    "CREATE OR REPLACE TABLE %s (k %s",
    Join_tbl_name, key_type_str
  );
  for(idx_t col=0; col<R_payload_count; col++) {
    finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ", R%ld %s", (col+1), payload_type_strs[col]);
  }
  for(idx_t col=R_payload_count; col<total_payload_count; col++) {
    finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ", S%ld %s", (col+1-R_payload_count), payload_type_strs[col]);
  }
  finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ");");
  // EXECUTE THE QUERY TO CREATE THE TABLE
  if (duckdb_query(con, finTbl_init_query, NULL) == DuckDBError) {
    perror("Failed to create Final Join Result Table.");
    return;
  }
  mylog(logfile, "Created final result table where join tuples will be stored.");

  // Create the Appender
  duckdb_appender final_join_appender;
  if( duckdb_appender_create(con, NULL, Join_tbl_name, &final_join_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return;
  }

  mylog(logfile, "Created FINAL result table and its appender.");

  // 2. Nested Loop (outer over Join Results, 2 inner loops over the original relations)
  // In ideal scenario, join results can fit entirely in memory, but figured this might be useful
  duckdb_result resIDs;
  char scan_resIDs[50 + strlen(Join_temp_tbl_name)];
  sprintf(scan_resIDs, "SELECT * FROM %s;", Join_temp_tbl_name);
  if(duckdb_query(con, scan_resIDs, &resIDs) == DuckDBError) {
    perror("Failed to obtain join ID's.");
    return;
  }
  void *buffer_keys = colType_malloc(key_type, PAYLOAD_INDEX_BLOCK);
  idx_t *buffer_R_is = colType_malloc(DUCKDB_TYPE_BIGINT, PAYLOAD_INDEX_BLOCK);
  idx_t *buffer_S_is = colType_malloc(DUCKDB_TYPE_BIGINT, PAYLOAD_INDEX_BLOCK);
  int finalFlag = true;
  while(finalFlag) {
    idx_t id_rows = 0;

    while(id_rows < PAYLOAD_INDEX_BLOCK) {
      duckdb_data_chunk cnk = duckdb_fetch_chunk(resIDs);
      if(!cnk) {
        mylog(logfile, "Join ID's have been exhausted.");
        finalFlag = false;
        break;
      }
      idx_t curRows = duckdb_data_chunk_get_size(cnk);

      duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk, 0);
      duckdb_vector Rvec = duckdb_data_chunk_get_vector(cnk, 1);
      duckdb_vector Svec = duckdb_data_chunk_get_vector(cnk, 2);

      void* kdat = duckdb_vector_get_data(kvec);
      void* Rdat = duckdb_vector_get_data(Rvec);
      void* Sdat = duckdb_vector_get_data(Svec);

      memcpy(buffer_keys + id_rows*colType_bytes(key_type), kdat, curRows*colType_bytes(key_type));
      memcpy(buffer_R_is + id_rows*sizeof(idx_t), Rdat, curRows*sizeof(idx_t));
      memcpy(buffer_S_is + id_rows*sizeof(idx_t), Sdat, curRows*sizeof(idx_t));

      id_rows += curRows;
      duckdb_destroy_data_chunk(&cnk);
    }
    if(id_rows == 0) { // nothing left
      break;
    }
    // Get index ranges
    idx_t min_R_idx, max_R_idx, min_S_idx, max_S_idx;
    indexRange(ctx, &min_R_idx, &max_R_idx, buffer_R_is, id_rows);
    indexRange(ctx, &min_S_idx, &max_S_idx, buffer_S_is, id_rows);

    printf("Ranges -----\n R: %ld - %ld \n S: %ld - %ld\n", min_R_idx, max_R_idx, min_S_idx, max_S_idx);

    // Prepare payload buffers
    void *PAYLOAD_BUFFERS[total_payload_count];
    void *MATERIALISED[total_payload_count];
    for(idx_t col=0; col<total_payload_count; col++) {
      PAYLOAD_BUFFERS[col] = colType_malloc(payload_types[col], PAYLOAD_GATHER_BLOCK);
      MATERIALISED[col] = colType_malloc(payload_types[col], id_rows);
    }
    // Prepare results.
    duckdb_result Rpl_res, Spl_res;
    char Rpl_query[250];
    sprintf(Rpl_query, "SELECT * EXCLUDE(k) FROM R_tbl LIMIT %ld OFFSET %ld;", (max_R_idx-min_R_idx+1), min_R_idx);
    if(duckdb_query(con, Rpl_query, &Rpl_res) == DuckDBError) {
      perror("Failed to obtain original R payloads.");
      return;
    }
    char Spl_query[250];
    sprintf(Spl_query, "SELECT * EXCLUDE(k) FROM S_tbl LIMIT %ld OFFSET %ld;", (max_S_idx-min_S_idx+1), min_S_idx);
    if(duckdb_query(con, Spl_query, &Spl_res) == DuckDBError) {
      perror("Failed to obtain original S payloads.");
      return;
    }
    mylog(logfile, "Obtained appropriate chunks for R & S.");

    // Gather R payloads.
    idx_t Rpl_incr = 0;
    int flag_Rpl = true;
    while(flag_Rpl) {
      idx_t scanned_Rpl_rows = 0;
      while(scanned_Rpl_rows < PAYLOAD_GATHER_BLOCK) {
        duckdb_data_chunk cnk = duckdb_fetch_chunk(Rpl_res);
        if(!cnk) {
          mylog(logfile, "Exhausted R payloads.");
          flag_Rpl = false;
          break;
        }
        idx_t curRows = duckdb_data_chunk_get_size(cnk);

        for(idx_t col=0; col<R_payload_count; col++) {
          duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
          void* dat = duckdb_vector_get_data(vec);
          memcpy(
            PAYLOAD_BUFFERS[col] + scanned_Rpl_rows*colType_bytes(payload_types[col]),
            dat,
            curRows*colType_bytes(payload_types[col])
          );
        }

        scanned_Rpl_rows += curRows;
        duckdb_destroy_data_chunk(&cnk);
      }
      if(scanned_Rpl_rows == 0) break; // Nothing left
      // Now do the gather
      for(idx_t col=0; col<R_payload_count; col++) {
        gatherPayloads_GFUR(ctx, MATERIALISED[col], payload_types[col], Rpl_incr+min_R_idx, BLOCK_SIZE,
          buffer_R_is, PAYLOAD_BUFFERS[col], scanned_Rpl_rows, id_rows
        );
      }
      Rpl_incr += scanned_Rpl_rows;
    }
    duckdb_destroy_result(&Rpl_res);
    mylog(logfile, "Did a gathering iteration over R.");

    // Gather S payloads
    idx_t Spl_incr = 0;
    int flag_Spl = true;
    while(flag_Spl) {
      idx_t scanned_Spl_rows = 0;
      while(scanned_Spl_rows < PAYLOAD_GATHER_BLOCK) {
        duckdb_data_chunk cnk = duckdb_fetch_chunk(Spl_res);
        if(!cnk) {
          mylog(logfile, "Exhausted S payloads.");
          flag_Spl = false;
          break;
        }
        idx_t curRows = duckdb_data_chunk_get_size(cnk);

        for(idx_t col=0; col<S_payload_count; col++) {
          duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, col);
          void* dat = duckdb_vector_get_data(vec);
          memcpy(
            PAYLOAD_BUFFERS[col+R_payload_count] + scanned_Spl_rows*colType_bytes(payload_types[col+R_payload_count]),
            dat,
            curRows*colType_bytes(payload_types[col+R_payload_count])
          );
        }

        scanned_Spl_rows += curRows;
        duckdb_destroy_data_chunk(&cnk);
      }
      if(scanned_Spl_rows == 0) break; // Nothing left
      // Now do the gather
      for(idx_t col=R_payload_count; col<total_payload_count; col++) {
        gatherPayloads_GFUR(ctx, MATERIALISED[col], payload_types[col], Spl_incr+min_S_idx, BLOCK_SIZE,
          buffer_S_is, PAYLOAD_BUFFERS[col], scanned_Spl_rows, id_rows
        );
      }
      Spl_incr += scanned_Spl_rows;
    }
    duckdb_destroy_result(&Spl_res);
    mylog(logfile, "Did a gathering iteration over S.");

    // Append to join result table
    duckdb_data_chunk finalCnk = duckdb_create_data_chunk(payload_ltypes, 1 + total_payload_count);
    duckdb_data_chunk_set_size(finalCnk, id_rows);
    // First take care of key column
    duckdb_vector f_kvec = duckdb_data_chunk_get_vector(finalCnk, 0);
    void* f_kdat = duckdb_vector_get_data(f_kvec);
    memcpy(f_kdat, buffer_keys, id_rows*colType_bytes(key_type));
    // Then payloads
    for(idx_t col=0; col<total_payload_count; col++) {
      duckdb_vector fvec = duckdb_data_chunk_get_vector(finalCnk, col+1);
      void* fdat = duckdb_vector_get_data(fvec);
      memcpy(fdat, MATERIALISED[col], id_rows*colType_bytes(payload_types[col]));
    }
    // Append, flush, destroy data chunk
    if (duckdb_append_data_chunk(final_join_appender, finalCnk) == DuckDBError) {
      perror("Failed to append data chunk to final table.");
      return;
    }
    duckdb_appender_flush(final_join_appender);
    duckdb_destroy_data_chunk(&finalCnk);

    // Cleanup
    for(idx_t col=0; col<total_payload_count; col++) {
      free(MATERIALISED[col]);
      free(PAYLOAD_BUFFERS[col]);
    }
  }
  duckdb_destroy_result(&resIDs);
  duckdb_appender_destroy(&final_join_appender);
  free(buffer_keys);
  free(buffer_R_is);
  free(buffer_S_is);
}