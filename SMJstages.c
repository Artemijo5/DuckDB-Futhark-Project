#include "SMJstages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftSMJ.h"
#include "mylogger.h"
#include "smjutil.h"
#include "myutil.h"

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
  const char *R_keyName,
  const char *S_keyName,
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
  idx_t R_keyCol_Idx = -1;
  idx_t R_col_count = duckdb_column_count(&res_Rk);
  duckdb_type R_type_ids[R_col_count];
  for(idx_t col=0; col<R_col_count; col++) {
    R_type_ids[col] = duckdb_column_type(&res_Rk, col);
    if(strcmp(R_keyName, duckdb_column_name(&res_Rk, col)) == 0) R_keyCol_Idx = col;
  }
  mylog(logfile, "Obtained R's column & type info.");
  // Key type
  if(R_keyCol_Idx<0) {
    mylog(logfile, "Invalid column for left-side table.");
    duckdb_destroy_result(&res_Rk);
    return;
  }
  duckdb_type key_type = R_type_ids[R_keyCol_Idx];
  // ALSO OBTAIN S INFO
  duckdb_result S_dummyRes;
  char dummySq[100 + strlen(sorted_S_tbl_name)];
  sprintf(dummySq, "SELECT * FROM %s LIMIT 0;", sorted_S_tbl_name);
  if( duckdb_query(con, dummySq, &S_dummyRes) == DuckDBError) {
    perror("Failed to obtain info for S table...\n");
    duckdb_destroy_result(&res_Rk);
    return;
  }
  idx_t S_keyCol_Idx = -1;
  idx_t S_col_count = duckdb_column_count(&S_dummyRes);
  duckdb_type S_type_ids[S_col_count];
  for(idx_t col=0; col<S_col_count; col++) {
    S_type_ids[col] = duckdb_column_type(&S_dummyRes, col);
    if(strcmp(S_keyName, duckdb_column_name(&S_dummyRes, col)) == 0) S_keyCol_Idx = col;
  }
  mylog(logfile, "Obtained S's column & type info.");
  if(S_keyCol_Idx<0) {
    mylog(logfile, "Invalid column for right-side table.");
    duckdb_destroy_result(&res_Rk);
    duckdb_destroy_result(&S_dummyRes);
    return;
  }
  if(S_type_ids[S_keyCol_Idx] != key_type) {
    mylog(logfile, "Key type mismatch!!!!!");
    duckdb_destroy_result(&res_Rk);
    duckdb_destroy_result(&S_dummyRes);
    return;
  }

  idx_t R_curIdx = 0;
  idx_t S_minimum_relevant_idx = 0; // minimum index of the first relevant chunk of S
  idx_t S_curIdx = 0;

  // ##### 0 --- Create Join Table & Appender

  // 0.0 create strings for types
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
  char joinTbl_init_query[250 + strlen(Join_tbl_name) + 30 + (R_col_count-1)*35 + (S_col_count-1)*35];
  int joinTbl_strLen = (saveAsTempTable)?
	  sprintf(
	    joinTbl_init_query,
	    "CREATE OR REPLACE TEMP TABLE %s (%s %s",
	    Join_tbl_name, R_keyName, R_type_strs[0]
	  ):
	  sprintf(
	    joinTbl_init_query,
	    "CREATE OR REPLACE TABLE %s (%s %s",
	    Join_tbl_name, R_keyName, R_type_strs[0]
	  );
  for(idx_t col=0; col<R_col_count; col++) {
    if(col==R_keyCol_Idx) continue; // payloads only
    joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ", %s %s",
      duckdb_column_name(&res_Rk, col), R_type_strs[col]);
  }
  for(idx_t col=0; col<S_col_count; col++) {
    if(col==S_keyCol_Idx) continue; // payloads only
    // if R has a column with the same name, append _1 to the end
    char *colName = malloc(7 + strlen(duckdb_column_name(&S_dummyRes, col)));
    int cnlen = sprintf(colName, "%s", duckdb_column_name(&S_dummyRes, col));
    idx_t appendNum = 1;
    while(true) {
      int R_has_same = false;
      for(idx_t rcol=0; rcol<R_col_count; rcol++) {
        R_has_same = (strcmp(colName, duckdb_column_name(&res_Rk, rcol)) == 0);
        if(R_has_same) break;
      }
      if(!R_has_same) break;
      sprintf(colName + cnlen, "_%ld", appendNum++);
    }

    joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ", %s %s",
      colName, S_type_strs[col]);
    free(colName);
  }
  joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ");");
  // EXECUTE THE QUERY TO CREATE THE TABLE
  if (duckdb_query(con, joinTbl_init_query, NULL) == DuckDBError) {
    perror("Failed to create Join Result Table.");
    //printf("%s\n", joinTbl_init_query);
    return;
  }
  mylog(logfile, "Created result table where join pairs will be stored.");

  duckdb_destroy_result(&S_dummyRes);

  // Create composite logical_type id info
  duckdb_logical_type join_type_ids[R_col_count + S_col_count - 1];
  for(idx_t col=0; col<R_col_count; col++) {
    join_type_ids[col] = duckdb_create_logical_type(R_type_ids[col]);
  }
  for(idx_t col=0; col<S_col_count; col++) {
    if(col==S_keyCol_Idx) continue; // payloads only
    idx_t accIdx = (col<S_keyCol_Idx)? col: col-1; // adjust index
    join_type_ids[accIdx + R_col_count] = duckdb_create_logical_type(S_type_ids[col]);
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
        duckdb_destroy_data_chunk(&cnk);
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
        duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk,S_keyCol_Idx);
        void* kdat = (void*)duckdb_vector_get_data(kvec);
        // ----- Deal with irrelevant chunks
        // a. If cnk.max < Rbuffer.min -> continue to next buffer
        if(compare_max_to_min(key_type, kdat, Rbuff[R_keyCol_Idx], curRows, R_rowCount) < 0) {
          //mylog(logfile, "Skipping this S chunk (not reached relevant ones yet)...");
          S_curIdx += curRows;
          S_minimum_relevant_idx = S_curIdx;
          duckdb_destroy_data_chunk(&cnk);
          continue;
        }
        // b. If Rbuffer.max < cnk.min -> break
        if(compare_max_to_min(key_type, Rbuff[R_keyCol_Idx], kdat, R_rowCount, curRows) < 0) {
          //mylog(logfile, "S chunks no longer relevant, break...");
          duckdb_destroy_data_chunk(&cnk);
          flag_continueWithThisR_partition = false;
          break;
        }
        // c. Chunk is relevant, proceed with buffering...
        //mylog(logfile, "Buffering this (relevant) S chunk...");
        // d. CHECK IF IT'LL BE RELEVANT FOR NEXT R PARTITION
        int Smax_vs_Rmax = compare_maxima(key_type, kdat, Rbuff[R_keyCol_Idx], curRows, R_rowCount);
        if(Smax_vs_Rmax < 0) {
          //mylog(logfile, "(This chunk will be irrelevant in the next iteration over R.)");
          S_minimum_relevant_idx += curRows;
        }
        // Copy to buffer
        memcpy(
          Sbuff[S_keyCol_Idx] + S_rowCount*colType_bytes(key_type),
          kdat,
          curRows*colType_bytes(key_type)
        );
        // NOW BUFFER PAYLOAD COLUMNS
        for(idx_t col=0; col<S_col_count; col++) {
          if(col==S_keyCol_Idx) continue; // payloads only
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
          //mylog(logfile, "(This is the last relevant chunk for this partition of R)");
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
      // TODO might want to do that anyway for "correctness" (sorted R-wise then S-wise)
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
          Rbuff[R_keyCol_Idx], // R keys
          Sbuff[S_keyCol_Idx], // S keys
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
          Sbuff[S_keyCol_Idx], // S keys
          Rbuff[R_keyCol_Idx], // R keys
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
      for(idx_t col=0; col<R_col_count; col++) {
        if(col==R_keyCol_Idx) continue; // payloads only
        idx_t accIdx = (col<R_keyCol_Idx)? col: col-1; // adjust index
        Rpl[accIdx] = colType_malloc(R_type_ids[col], numPairs);
        gatherPayloads(ctx, Rpl[accIdx], R_type_ids[col], R_curIdx, BLOCK_SIZE, idxR_ft, Rbuff[col], R_rowCount, numPairs);
      }
      mylog(logfile, "Gathered R payloads.");
      // Gather S's payloads
      void* Spl[S_col_count-1];
      for(idx_t col=0; col<S_col_count; col++) {
        if(col==S_keyCol_Idx) continue; // payloads only
        idx_t accIdx = (col<S_keyCol_Idx)? col: col-1; // adjust index
        Spl[accIdx] = colType_malloc(S_type_ids[col], numPairs);
        gatherPayloads(ctx, Spl[accIdx], S_type_ids[col], S_curIdx, BLOCK_SIZE, idxS_ft, Sbuff[col], S_rowCount, numPairs);
      }
      mylog(logfile, "Gathered S payloads.");

      // CREATE DATA CHUNK AND APPEND
      // as loop so no individual chunk exceeds CHUNK_SIZE
      mylog(logfile, "Preparing to load join results into database...");
      for(idx_t appendRow=0; appendRow<numPairs; appendRow += CHUNK_SIZE) {
        duckdb_data_chunk joinCnk = duckdb_create_data_chunk(join_type_ids, (R_col_count+S_col_count-1));
        idx_t joinCnk_size = (numPairs-appendRow>=CHUNK_SIZE)? CHUNK_SIZE: (numPairs-appendRow);
        duckdb_data_chunk_set_size(joinCnk, joinCnk_size);
        // Load keys
        duckdb_vector join_kvec = duckdb_data_chunk_get_vector(joinCnk, 0);
        void* join_kdat = duckdb_vector_get_data(join_kvec);
        memcpy(join_kdat, joinedKeys + appendRow*colType_bytes(key_type), joinCnk_size*colType_bytes(key_type));
        // Proceed with payload columns - first for R
        for(idx_t col=0; col<R_col_count; col++) {
          if(col==R_keyCol_Idx) continue; // payloads only
          idx_t accIdx = (col<R_keyCol_Idx)? col: col-1; // adjust index
          duckdb_vector join_vec = duckdb_data_chunk_get_vector(joinCnk, col);
          void* join_dat = duckdb_vector_get_data(join_vec);
          memcpy(join_dat, Rpl[accIdx] + appendRow*colType_bytes(R_type_ids[col]), joinCnk_size*colType_bytes(R_type_ids[col]));
        }
        // Then for S
        for(idx_t col=0; col<S_col_count; col++) {
          if(col==S_keyCol_Idx) continue; // payloads only
          idx_t accIdx = (col<S_keyCol_Idx)? col: col-1; // adjust index
          duckdb_vector join_vec = duckdb_data_chunk_get_vector(joinCnk, col + R_col_count - 1);
          void* join_dat = duckdb_vector_get_data(join_vec);
          memcpy(join_dat, Spl[accIdx] + appendRow*colType_bytes(S_type_ids[col]), joinCnk_size*colType_bytes(S_type_ids[col]));
        }

        // Append chunk
        if(duckdb_append_data_chunk(join_appender, joinCnk) == DuckDBError) {
          perror("Failed to append data chunk.\n");
          return;
        }
        // If quicksaves, flush right away
        if(quicksaves) {
          duckdb_appender_flush(join_appender);
          mylog(logfile, "Appended a datachunk to result table.");
        }
        // Cleanup
        duckdb_destroy_data_chunk(&joinCnk);
      }
      duckdb_appender_flush(join_appender);

      mylog(logfile, "Finished preparing datachunks for this batch.");
      /*if(!quicksaves) {
        duckdb_appender_flush(join_appender);
        mylog(logfile, "Appended to result table.");
      }*/

      // CLEANUP
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
  const char *R_keyName,
  const char *S_keyName,
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
    R_keyName,
    S_keyName,
		Join_temp_tbl_name,
		quicksaves,
		true
	);

  // 0. Get key & payload info from R_tbl, S_tbl
  duckdb_result R_kcInfo, R_plInfo, S_plInfo; // use these results to obtain info about original payloads
  char R_kcQ[100 + strlen(R_keyName) + strlen(R_tbl_name)];
  char R_pLQ[100 + strlen(R_keyName) + strlen(R_tbl_name)];
  char S_pLQ[100 + strlen(S_keyName) + strlen(S_tbl_name)];
  sprintf(R_kcQ, "SELECT %s FROM %s LIMIT 0;", R_keyName, R_tbl_name);
  sprintf(R_pLQ, "SELECT * EXCLUDE(%s) FROM %s LIMIT 0;", R_keyName, R_tbl_name);
  sprintf(S_pLQ, "SELECT * EXCLUDE(%s) FROM %s LIMIT 0;", S_keyName, S_tbl_name);
  if (
    duckdb_query(con, R_kcQ, &R_kcInfo) == DuckDBError
    ||
    duckdb_query(con, R_pLQ, &R_plInfo) == DuckDBError
    ||
    duckdb_query(con, S_pLQ, &S_plInfo) == DuckDBError
  ) {
    perror("Failed to obtain original column info.");
    return;
  }
  mylog(logfile, "GFUR: Obtained original column info.");
  idx_t R_payload_count = duckdb_column_count(&R_plInfo);
  idx_t S_payload_count = duckdb_column_count(&S_plInfo);
  idx_t total_payload_count = R_payload_count + S_payload_count;

  duckdb_type key_type = duckdb_column_type(&R_kcInfo, 0);
  duckdb_destroy_result(&R_kcInfo);
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
    payload_types[col] = duckdb_column_type(&R_plInfo, col);
  }
  for(idx_t col=0; col<S_payload_count; col++) {
    payload_types[col + R_payload_count] = duckdb_column_type(&S_plInfo, col);
  }

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
  int finTbl_strLen = (saveAsTempTable)?
    sprintf(
      finTbl_init_query,
      "CREATE OR REPLACE TEMP TABLE %s (%s %s",
      Join_tbl_name, R_keyName, key_type_str
    ):
    sprintf(
      finTbl_init_query,
      "CREATE OR REPLACE TABLE %s (%s %s",
      Join_tbl_name,  R_keyName, key_type_str
    );
  for(idx_t col=0; col<R_payload_count; col++) {
    finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ", %s %s",
      duckdb_column_name(&R_plInfo, col), payload_type_strs[col]);
  }
  for(idx_t col=R_payload_count; col<total_payload_count; col++) {
    // if R has a column with the same name, append _1 to the end
    char *colName = malloc(7 + strlen(duckdb_column_name(&S_plInfo, col-R_payload_count)));
    int cnlen = sprintf(colName, "%s", duckdb_column_name(&S_plInfo, col-R_payload_count));
    idx_t appendNum = 1;
    while(true) {
      int R_has_same = false;
      for(idx_t rcol=0; rcol<R_payload_count; rcol++) {
        const char* compWith = (rcol==0)? R_keyName: duckdb_column_name(&R_plInfo, rcol);
        R_has_same = (strcmp(colName, compWith) == 0);
        if(R_has_same) break;
      }
      if(!R_has_same) break;
      sprintf(colName + cnlen, "_%ld", appendNum++);
    }

    finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ", %s %s",
      colName, payload_type_strs[col]);
    free(colName);
  }

  duckdb_destroy_result(&R_plInfo);
  duckdb_destroy_result(&S_plInfo);

  finTbl_strLen += sprintf(finTbl_init_query + finTbl_strLen, ");");
  // EXECUTE THE QUERY TO CREATE THE TABLE
  if (duckdb_query(con, finTbl_init_query, NULL) == DuckDBError) {
    perror("Failed to create Final Join Result Table.");
    //printf("%s\n", finTbl_init_query);
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

  int finalFlag = true; // still have join IDs
  mylog(logfile, "Entering materialisation phase nested loop...");
  while(finalFlag) {
    // Obtain key-index tuples from GFTR result
    idx_t iter_pairs = 0; // pairs for this loop iteration
    while(iter_pairs <= PAYLOAD_INDEX_BLOCK - CHUNK_SIZE) {
      duckdb_data_chunk id_cnk = duckdb_fetch_chunk(resIDs);
      if(!id_cnk) {
        mylog(logfile, "Exhausted join IDs.");
        finalFlag = false;
        break;
      }
      idx_t curRows = duckdb_data_chunk_get_size(id_cnk);

      duckdb_vector kvec  = duckdb_data_chunk_get_vector(id_cnk, 0);
      duckdb_vector ir_vec  = duckdb_data_chunk_get_vector(id_cnk, 1);
      duckdb_vector is_vec  = duckdb_data_chunk_get_vector(id_cnk, 2);

      void* kdat = duckdb_vector_get_data(kvec);
      int64_t* ir_dat = (int64_t*)duckdb_vector_get_data(ir_vec);
      int64_t* is_dat = (int64_t*)duckdb_vector_get_data(is_vec);

      memcpy(buffer_keys + iter_pairs*sizeof(colType_bytes(key_type)), kdat, curRows*colType_bytes(key_type));
      memcpy(buffer_R_is + iter_pairs, ir_dat, curRows*sizeof(int64_t));
      memcpy(buffer_S_is + iter_pairs, is_dat, curRows*sizeof(int64_t));
      /*for(idx_t i=0; i<curRows; i++) {
        buffer_R_is[iter_pairs+i] = ir_dat[i];
      }
      for(idx_t i=0; i<curRows; i++) {
        buffer_S_is[iter_pairs+i] = is_dat[i];
      }*/

      iter_pairs += curRows;
    }
    mylog(logfile, "Obtained join id's for this iteration.");

    if(iter_pairs == 0) {
      mylog(logfile, "No more join IDs.");
      break;
    }

    void **PAYLOAD_BUFFERS = malloc(total_payload_count * sizeof(void*));
    void **MATERIALISED = malloc(total_payload_count * sizeof(void*));
    for(idx_t col=0; col<total_payload_count; col++) {
      PAYLOAD_BUFFERS[col] = colType_malloc(payload_types[col], PAYLOAD_GATHER_BLOCK);
      MATERIALISED[col] = colType_malloc(payload_types[col], iter_pairs);
    }

    // Get min relevant indices - not useful since they alter the duckdb result...
    idx_t min_R_idx, max_R_idx, min_S_idx, max_S_idx;
    indexRange(ctx, &min_R_idx, &max_R_idx, buffer_R_is, iter_pairs);
    indexRange(ctx, &min_S_idx, &max_S_idx, buffer_S_is, iter_pairs);
    // Prepare queries to scan tables
    duckdb_result R_joinPL, S_joinPL;
    char R_joinPL_q[250 + strlen(R_keyName) + strlen(R_tbl_name)];
    char S_joinPL_q[250 + strlen(S_keyName) + strlen(S_tbl_name)];
    sprintf(R_joinPL_q, "SELECT * FROM %s;", R_tbl_name);
    sprintf(S_joinPL_q, "SELECT * FROM %s;", S_tbl_name);
    // excluding key column & limiting range seems to ruin order...

    // Obtain R payloads
    duckdb_query(con, R_joinPL_q, &R_joinPL);
    idx_t R_keyCol_Idx=-1;
    for(idx_t c=0; c<R_payload_count+1; c++) {
      if(strcmp(R_keyName, duckdb_column_name(&R_joinPL, c))==0) {
        R_keyCol_Idx = c;
        break;
      }
    }
    mylog(logfile, "Gathering R's payloads...");
    idx_t R_rows_filled = 0;
    idx_t R_timesBufferHasBeenFilled = 0;
    int R_exhausted = false;
    // Fill PAYLOAD_BUFFERS up
    // When full, perform a gather and reset row counter
    while(!R_exhausted) {
      duckdb_data_chunk R_pl_cnk = duckdb_fetch_chunk(R_joinPL);
      if(!R_pl_cnk) {
        mylog(logfile, "Completed a scan over R's payloads.");
        R_exhausted = true;
      }
      idx_t curRows = (R_exhausted)? 0: duckdb_data_chunk_get_size(R_pl_cnk);

      for(idx_t col=0; (!R_exhausted) && col<R_payload_count+1; col++) {
        if(col==R_keyCol_Idx) continue; // skip key column
        idx_t accIdx = (col<R_keyCol_Idx)? col: col-1; // adjust column index

        duckdb_vector vec = duckdb_data_chunk_get_vector(R_pl_cnk, col);
        void* dat = duckdb_vector_get_data(vec);
        
        memcpy(
          PAYLOAD_BUFFERS[accIdx] + R_rows_filled*colType_bytes(payload_types[accIdx]),
          dat,
          curRows*colType_bytes(payload_types[accIdx])
        );
      }
      if(R_pl_cnk) duckdb_destroy_data_chunk(&R_pl_cnk);

      R_rows_filled += curRows;
      // If PAYLOAD_BUFFERS full (or R exhausted), gather & reset
      if((R_rows_filled) > PAYLOAD_GATHER_BLOCK-CHUNK_SIZE || R_exhausted) {
        for(idx_t col=0; col<R_payload_count; col++) {
          gatherPayloads_GFUR(
            ctx,
            MATERIALISED[col],
            payload_types[col],
            R_timesBufferHasBeenFilled*PAYLOAD_GATHER_BLOCK,
            BLOCK_SIZE,
            buffer_R_is,
            PAYLOAD_BUFFERS[col],
            R_rows_filled,
            iter_pairs
          );
        }
        R_rows_filled = 0;
        R_timesBufferHasBeenFilled += 1;
      }
    }
    duckdb_destroy_result(&R_joinPL);
    mylog(logfile, "Finished this iteration of gathers from R.");

    // Obtain S payloads
    duckdb_query(con, S_joinPL_q, &S_joinPL);
    idx_t S_keyCol_Idx=-1;
    for(idx_t c=0; c<S_payload_count+1; c++) {
      if(strcmp(S_keyName, duckdb_column_name(&S_joinPL, c))==0) {
        S_keyCol_Idx = c;
        break;
      }
    }
    mylog(logfile, "Gathering S's payloads...");
    idx_t S_rows_filled = 0;
    idx_t S_timesBufferHasBeenFilled = 0;
    int S_exhausted = false;
    // Fill PAYLOAD_BUFFERS up
    // When full, perform a gather and reset row counter
    while(!S_exhausted) {
      duckdb_data_chunk S_pl_cnk = duckdb_fetch_chunk(S_joinPL);
      if(!S_pl_cnk) {
        mylog(logfile, "Completed a scan over S's payloads.");
        S_exhausted = true;
      }
      idx_t curRows = (S_exhausted)? 0: duckdb_data_chunk_get_size(S_pl_cnk);

      for(idx_t col=0; (!S_exhausted) && col<S_payload_count+1; col++) {
        if(col==S_keyCol_Idx) continue; // skip key column
        idx_t accIdx = (col<S_keyCol_Idx)? col: col-1; // adjust column index

        duckdb_vector vec = duckdb_data_chunk_get_vector(S_pl_cnk, col);
        void* dat = duckdb_vector_get_data(vec);
        
        memcpy(
          PAYLOAD_BUFFERS[R_payload_count + accIdx] + S_rows_filled*colType_bytes(payload_types[R_payload_count + accIdx]),
          dat,
          curRows*colType_bytes(payload_types[R_payload_count + accIdx])
        );
      }
      if(S_pl_cnk) duckdb_destroy_data_chunk(&S_pl_cnk);

      S_rows_filled += curRows;
      // If PAYLOAD_BUFFERS full (or S exhausted), gather & reset
      if((S_rows_filled) > PAYLOAD_GATHER_BLOCK-CHUNK_SIZE || S_exhausted) {
        for(idx_t col=0; col<S_payload_count; col++) {
          gatherPayloads_GFUR(
            ctx,
            MATERIALISED[col + R_payload_count],
            payload_types[col + R_payload_count],
            S_timesBufferHasBeenFilled*PAYLOAD_GATHER_BLOCK,
            BLOCK_SIZE,
            buffer_S_is,
            PAYLOAD_BUFFERS[col + R_payload_count],
            S_rows_filled,
            iter_pairs
          );
        }
        S_rows_filled = 0;
        S_timesBufferHasBeenFilled += 1;
      }
    }
    duckdb_destroy_result(&S_joinPL);
    mylog(logfile, "Finished this iteration of gathers from S.");

    // Don't need PAYLOAD_BUFFERS anymore...
    for(idx_t col=0; col<total_payload_count; col++) {
      free(PAYLOAD_BUFFERS[col]);
    }
    free(PAYLOAD_BUFFERS);

    // Append payloads
    for(idx_t appendRow = 0; appendRow < iter_pairs; appendRow += CHUNK_SIZE) {
      duckdb_data_chunk joinCnk = duckdb_create_data_chunk(payload_ltypes, 1 + total_payload_count);
      idx_t joinCnk_size = (iter_pairs - appendRow >= CHUNK_SIZE)? CHUNK_SIZE: iter_pairs-appendRow;
      duckdb_data_chunk_set_size(joinCnk, joinCnk_size);
      // key column first
      duckdb_vector kvec = duckdb_data_chunk_get_vector(joinCnk, 0);
      void *kdat = duckdb_vector_get_data(kvec);
      memcpy(kdat, buffer_keys + appendRow*colType_bytes(key_type), joinCnk_size*colType_bytes(key_type));
      /*for(idx_t i = 0; i<joinCnk_size*colType_bytes(key_type); i++) {
        ((char*)kdat)[i] = ((char*)buffer_keys)[appendRow*colType_bytes(key_type) + i];
      }*/
      // payload columns next
      for(idx_t col=0; col<total_payload_count; col++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(joinCnk, col+1);
        void *dat = duckdb_vector_get_data(vec);
        memcpy(dat, MATERIALISED[col] + appendRow*colType_bytes(payload_types[col]), joinCnk_size*colType_bytes(payload_types[col]));
        /*for(idx_t i = 0; i<joinCnk_size*colType_bytes(payload_types[col]); i++) {
          ((char*)dat)[i] = ((char*)MATERIALISED[col])[appendRow*colType_bytes(payload_types[col]) + i];
        }*/
      }
      if (duckdb_append_data_chunk(final_join_appender, joinCnk) == DuckDBError) {
        perror("Failed to append data chunk to final table.");
        return;
      }
      duckdb_appender_flush(final_join_appender);
      duckdb_destroy_data_chunk(&joinCnk);
    }

    mylog(logfile, "Appended materialised results for this iteration.");

    // Cleanup
    for(idx_t col=0; col<total_payload_count; col++) {
      free(MATERIALISED[col]);
    }
    free(MATERIALISED);
  }
  mylog(logfile, "Materialisation has finished.");
  duckdb_destroy_result(&resIDs);
  duckdb_appender_destroy(&final_join_appender);
  free(buffer_keys);
  free(buffer_R_is);
  free(buffer_S_is);
}