#include "RadixJoinStages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "radixJoin_util.h"
#include "smjutil.h"
#include "db_util.h"

void RadixHashJoin_GFTR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  int32_t radix_bits,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_partitioned,
  int is_S_partitioned,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  // Read R's sorted keys
  mylog(logfile, "Preparing for join - obtain R's keys...");
  duckdb_result res_Rk;
  char readRq[100 + strlen(R_tbl_name)];
  sprintf(readRq, "SELECT * FROM %s;", R_tbl_name);
  if( duckdb_query(con, readRq, &res_Rk) == DuckDBError) {
    perror("Failed to read R_tbl_sorted...\n");
    return;
  }
  mylog(logfile, "Obtained R result.");
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
  char dummySq[100 + strlen(S_tbl_name)];
  sprintf(dummySq, "SELECT * FROM %s LIMIT 0;", S_tbl_name);
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
  idx_t S_curIdx = 0;

  // ##### 0 --- Create Join Table & Appender

  // 0.0 create strings for types
  char R_type_strs[R_col_count][25];
  for(idx_t col=0; col<R_col_count; col++) {
    colType_name(R_type_ids[col], R_type_strs[col]);
  }
  char S_type_strs[S_col_count][25];
  for(idx_t col=0; col<S_col_count; col++) {
    colType_name(S_type_ids[col], S_type_strs[col]);
  }

  // Create the Table
  char joinTbl_init_query[250 + strlen(Join_tbl_name) + 30 + (R_col_count-1)*35 + (S_col_count-1)*35];
  int joinTbl_strLen = (saveAsTempTable)?
    sprintf(
      joinTbl_init_query,
      "CREATE OR REPLACE TEMP TABLE %s (%s %s",
      Join_tbl_name, duckdb_column_name(&res_Rk, 0), R_type_strs[0]
    ):
    sprintf(
      joinTbl_init_query,
      "CREATE OR REPLACE TABLE %s (%s %s",
      Join_tbl_name, duckdb_column_name(&res_Rk, 0), R_type_strs[0]
    );
  for(idx_t col=1; col<R_col_count; col++) {
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

  // Get payload info
  idx_t R_pL_bytesPerRow = 0;
  idx_t S_pL_bytesPerRow = 0;
  duckdb_type R_payloadTypes[R_col_count-1];
  duckdb_type S_payloadTypes[S_col_count-1];
  for(idx_t col=0; col<R_col_count; col++) {
    if(col==R_keyCol_Idx) continue;
    idx_t accIdx = (col<R_keyCol_Idx)? col: col-1;

    R_pL_bytesPerRow += colType_bytes(R_type_ids[col]);
    R_payloadTypes[accIdx] = R_type_ids[col];
  }
  for(idx_t col=0; col<S_col_count; col++) {
    if(col==S_keyCol_Idx) continue;
    idx_t accIdx = (col<S_keyCol_Idx)? col: col-1;

    S_pL_bytesPerRow += colType_bytes(S_type_ids[col]);
    S_payloadTypes[accIdx] = S_type_ids[col];
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
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    char *Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);
    char *R_payload;
    struct futhark_u8_2d *Rbuff_ft;
    struct futhark_u8_2d *R_payload_ft;
    struct futhark_opaque_partitionInfo *R_pInfo;

    // READ R DATA INTO BUFFER
    idx_t R_rowCount = bulk_load_chunks_GFTR(
      CHUNK_SIZE,
      res_Rk,
      60,
      R_col_count,
      R_keyCol_Idx,
      R_type_ids,
      Rbuff,
      &R_payload,
      R_JOIN_BUFFER,
      &exhaustedRes_R
    );
    if(R_rowCount == 0) break; // Nothing left to join.
    mylog(logfile, "Buffered this partition of R.");

    // If R is not partitioned, partition
    // Either way, maintain as Rbuff_ft
    if(!is_R_partitioned) {
      radixPartition_GFTR(
        ctx,
        &Rbuff_ft,
        &R_payload_ft,
        Rbuff,
        R_payload,
        R_rowCount,
        colType_bytes(key_type),
        R_pL_bytesPerRow,
        radix_bits,
        MAX_DEPTH,
        MAX_PARTITION_SIZE,
        SCATTER_PSIZE
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      Rbuff_ft = futhark_new_u8_2d(ctx, Rbuff, R_rowCount, colType_bytes(key_type));
      R_payload_ft = futhark_new_u8_2d(ctx, R_payload, R_rowCount, R_pL_bytesPerRow);
      futhark_context_sync(ctx);
    }
    free(Rbuff);
    Rbuff = NULL;
    free(R_payload);
    R_payload = NULL;

    // TODO get min & max radix (?)

    mylog(logfile, "Obtaining R's partitionInfo...");
    getPartitionInfo(
      ctx,
      &R_pInfo,
      Rbuff_ft,
      radix_bits,
      MAX_PARTITION_SIZE,
      MAX_DEPTH,
      0
    );
    mylog(logfile, "Obtained R's partitionInfo.");

    mylog(logfile, "Obtain S's keys...");
    duckdb_result res_Sk;
    // TODO construct S query to read from minimum relevant index
    duckdb_state S_query = radix_selective_query(
      key_type,
      con,
      &res_Sk,
      S_tbl_name,
      S_keyName,
      NULL,
      NULL,
      radix_bits
    );
    if(S_query == DuckDBError) {
      perror("Failed to perform selective query over right-side table.");
      return;
    }
    mylog(logfile, "Obtained S's keys.");

    mylog(logfile, "Now iterating over S for the join...");
    // that is, only when S is sorted
    int exhaustedRes_S = false;
    while(!(exhaustedRes_S)) {
      //printf("Marco!\n");
      char *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
      char *S_payload;
      struct futhark_u8_2d *Sbuff_ft;
      struct futhark_u8_2d *S_payload_ft;
      struct futhark_opaque_partitionInfo *S_pInfo;
      struct futhark_opaque_radix_hashTable *S_RHT;

      idx_t S_rowCount = bulk_load_chunks_GFTR(
        CHUNK_SIZE,
        res_Sk,
        60,
        S_col_count,
        S_keyCol_Idx,
        S_type_ids,
        Sbuff,
        &S_payload,
        S_JOIN_BUFFER,
        &exhaustedRes_S
      );
      //if(exhaustedRes_S) printf("Polo! - %ld rows read.\n", S_curIdx + S_rowCount);
      //if(S_rowCount == 0) printf("Venezziano!\n");
      if(S_rowCount == 0) break; // Nothing left to join.
      mylog(logfile, "Buffered this partition of S.");

      if(!is_S_partitioned) {
        radixPartition_GFTR(
          ctx,
          &Sbuff_ft,
          &S_payload_ft,
          Sbuff,
          S_payload,
          S_rowCount,
          colType_bytes(key_type),
          S_pL_bytesPerRow,
          radix_bits,
          MAX_DEPTH,
          MAX_PARTITION_SIZE,
          SCATTER_PSIZE
        );
        mylog(logfile, "Sorted this buffer of S.");
      } else {
        Sbuff_ft = futhark_new_u8_2d(ctx, Sbuff, S_rowCount, colType_bytes(key_type));
        S_payload_ft = futhark_new_u8_2d(ctx, S_payload, S_rowCount, S_pL_bytesPerRow);
        futhark_context_sync(ctx);
      }
      free(Sbuff);
      Sbuff = NULL;
      free(S_payload);
      S_payload = NULL;

      mylog(logfile, "Obtaining S's partitionInfo...");
      getPartitionInfo(
        ctx,
        &S_pInfo,
        Sbuff_ft,
        radix_bits,
        MAX_PARTITION_SIZE,
        MAX_DEPTH,
        0
      );
      mylog(logfile, "Obtained S's partitionInfo.");

      mylog(logfile, "Obtaining S's radix hash table...");
      getRadixHashTable(
        ctx,
        &S_RHT,
        Sbuff_ft,
        S_pInfo,
        radix_bits,
        SCATTER_PSIZE
      );
      mylog(logfile, "Obtained S's radix hash table.");

      // Perform join
      idx_t numPairs = 0;
      char *joinedKeys;
      struct futhark_i64_1d *idxR_ft;
      struct futhark_i64_1d *idxS_ft;
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
      mylog(logfile, "Performing the key join...");
      HashJoin_joinKeyColumns_inFuthark(
        ctx,
        &numPairs,
        &joinedKeys,
        &idxR_ft,
        &idxS_ft,
        colType_bytes(key_type),
        radix_bits,
        0,
        0,
        Rbuff_ft,
        Sbuff_ft,
        R_pInfo,
        S_pInfo,
        S_RHT,
        R_rowCount,
        S_rowCount,
        SCATTER_PSIZE
      );
      mylog(logfile, "Performed key join.");
      
      // Gather R's payloads
      char* Rpl_asBytes;
      Rpl_asBytes = malloc(numPairs * R_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Rpl_asBytes, R_pL_bytesPerRow, R_curIdx, BLOCK_SIZE, idxR_ft, R_payload_ft, R_rowCount, numPairs);
      mylog(logfile, "Gathered R payloads.");
      void* Rpl[R_col_count-1];
      payloadColumnsFromByteArray(Rpl, R_payloadTypes, Rpl_asBytes, R_col_count-1, numPairs);
      free(Rpl_asBytes);
      mylog(logfile, "Recovered gathered R payloads from byte array.");

      // Gather S's payloads
      char* Spl_asBytes;
      Spl_asBytes = malloc(numPairs * S_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Spl_asBytes, S_pL_bytesPerRow, S_curIdx, BLOCK_SIZE, idxS_ft, S_payload_ft, S_rowCount, numPairs);
      mylog(logfile, "Gathered S payloads.");
      void* Spl[S_col_count-1];
      payloadColumnsFromByteArray(Spl, S_payloadTypes, Spl_asBytes, S_col_count-1, numPairs);
      free(Spl_asBytes);
      mylog(logfile, "Recovered gathered S payloads from byte array.");

      // CREATE DATA CHUNK AND APPEND
      // as loop so no individual chunk exceeds CHUNK_SIZE
      // TODO revamp (...)
      mylog(logfile, "Preparing to load join results into database...");
      for(idx_t appendRow=0; appendRow<numPairs; appendRow += CHUNK_SIZE) {
        duckdb_data_chunk joinCnk = duckdb_create_data_chunk(join_type_ids, (R_col_count+S_col_count-1));
        idx_t joinCnk_size = (numPairs-appendRow>=CHUNK_SIZE)? CHUNK_SIZE: (numPairs-appendRow);
        duckdb_data_chunk_set_size(joinCnk, joinCnk_size);
        // Load keys
        duckdb_vector join_kvec = duckdb_data_chunk_get_vector(joinCnk, R_keyCol_Idx);
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
          duckdb_vector join_vec = duckdb_data_chunk_get_vector(joinCnk, accIdx + R_col_count);
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
      futhark_free_u8_2d(ctx, Sbuff_ft);
      futhark_free_u8_2d(ctx, S_payload_ft);
      futhark_free_opaque_partitionInfo(ctx, S_pInfo);
      futhark_free_opaque_radix_hashTable(ctx, S_RHT);
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

    futhark_free_u8_2d(ctx, Rbuff_ft);
    futhark_free_u8_2d(ctx, R_payload_ft);
    futhark_free_opaque_partitionInfo(ctx, R_pInfo);
    duckdb_destroy_result(&res_Sk);

    R_curIdx += R_rowCount;
  }
  duckdb_destroy_result(&res_Rk);
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);
}
void RadixHashJoin_GFUR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  int32_t radix_bits,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_partitioned,
  int is_S_partitioned,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  // TODO REWRITE FROM SCRATCH (...)
}