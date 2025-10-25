#include "RadixJoinStages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "radixJoin_util.h"
#include "join_util.h"
#include "db_util.h"

void do_RadixHashJoin_GFTR(
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
  int is_R_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  duckdb_type key_type;
  duckdb_result res_Rk;
  duckdb_appender join_appender;
  idx_t R_keyCol_Idx, S_keyCol_Idx, R_col_count, S_col_count, R_pL_bytesPerRow, S_pL_bytesPerRow;
  duckdb_type *R_type_ids;
  duckdb_type *S_type_ids;
  duckdb_type *R_payloadTypes;
  duckdb_type *S_payloadTypes;
  duckdb_logical_type *join_type_ids;
  int jp = join_preparation(
    logfile,
    con,
    R_tbl_name,
    S_tbl_name,
    Join_tbl_name,
    false,
    R_keyName,
    S_keyName,
    &key_type,
    &res_Rk,
    &join_appender,
    &R_keyCol_Idx,
    &S_keyCol_Idx,
    &R_col_count,
    &S_col_count,
    &R_pL_bytesPerRow,
    &S_pL_bytesPerRow,
    &R_type_ids,
    &S_type_ids,
    &R_payloadTypes,
    &S_payloadTypes,
    &join_type_ids,
    saveAsTempTable
  );
  if(jp<0) {
    perror("Failed in join preparation.");
    return;
  }
  idx_t R_curIdx = 0;
  idx_t S_curIdx = 0;

  // TODO ##### Loop over R (left table) -- for each chunk of R we will loop over S
  mylog(logfile, "Iterating over R...");
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    char *Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);
    char *R_payload;
    struct futhark_u8_2d *Rbuff_ft;
    struct futhark_u8_2d *R_payload_ft;
    struct futhark_opaque_partitionInfo *R_pInfo;
    struct futhark_opaque_radix_hashTable *R_RHT;

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

    mylog(logfile, "Obtaining R's radix hash table...");
    getRadixHashTable(
      ctx,
      &R_RHT,
      Rbuff_ft,
      R_pInfo,
      radix_bits,
      SCATTER_PSIZE
    );
    mylog(logfile, "Obtained R's radix hash table.");

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

      // Perform join
      idx_t numPairs = 0;
      char *joinedKeys;
      struct futhark_i64_1d *idxR_ft;
      struct futhark_i64_1d *idxS_ft;
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      // PERFORM THE JOIN
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      mylog(logfile, "Performing the key join...");
      // Invert R&S because calculating info for R is cheaper...
      if(is_R_unique) {
        HashJoin_joinKeyColumns_inFuthark_with_unique_keys2(
          ctx,
          &numPairs,
          &joinedKeys,
          &idxS_ft,
          &idxR_ft,
          colType_bytes(key_type),
          radix_bits,
          0,
          0,
          Sbuff_ft,
          Rbuff_ft,
          R_pInfo,
          R_RHT,
          S_rowCount,
          R_rowCount
        );
      }
      else {
        HashJoin_joinKeyColumns_inFuthark(
          ctx,
          &numPairs,
          &joinedKeys,
          &idxS_ft,
          &idxR_ft,
          colType_bytes(key_type),
          radix_bits,
          0,
          0,
          Sbuff_ft,
          Rbuff_ft,
          R_pInfo,
          R_RHT,
          S_rowCount,
          R_rowCount
        );
      }
      mylog(logfile, "Performed key join.");
      
      // Gather R's payloads
      char* Rpl_asBytes;
      Rpl_asBytes = malloc(numPairs * R_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Rpl_asBytes, R_pL_bytesPerRow, 0, BLOCK_SIZE, idxR_ft, R_payload_ft, R_rowCount, numPairs);
      mylog(logfile, "Gathered R payloads.");
      void* Rpl[R_col_count-1];
      payloadColumnsFromByteArray(Rpl, R_payloadTypes, Rpl_asBytes, R_col_count-1, numPairs);
      free(Rpl_asBytes);
      mylog(logfile, "Recovered gathered R payloads from byte array.");

      // Gather S's payloads
      char* Spl_asBytes;
      Spl_asBytes = malloc(numPairs * S_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Spl_asBytes, S_pL_bytesPerRow, 0, BLOCK_SIZE, idxS_ft, S_payload_ft, S_rowCount, numPairs);
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
    futhark_free_opaque_radix_hashTable(ctx, R_RHT);
    duckdb_destroy_result(&res_Sk);

    R_curIdx += R_rowCount;
  }
  duckdb_destroy_result(&res_Rk);
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);
  free(R_type_ids);
  free(S_type_ids);
  free(R_payloadTypes);
  free(S_payloadTypes);
  free(join_type_ids);
}

void do_RadixHashJoin_GFUR(
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
  int is_R_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  duckdb_type key_type;
  duckdb_result res_Rk;
  duckdb_appender join_appender;
  idx_t R_keyCol_Idx, S_keyCol_Idx, R_col_count, S_col_count, R_pL_bytesPerRow, S_pL_bytesPerRow;
  duckdb_type *R_type_ids;
  duckdb_type *S_type_ids;
  duckdb_type *R_payloadTypes;
  duckdb_type *S_payloadTypes;
  duckdb_logical_type *join_type_ids;
  int jp = join_preparation(
    logfile,
    con,
    R_tbl_name,
    S_tbl_name,
    Join_tbl_name,
    false,
    R_keyName,
    S_keyName,
    &key_type,
    &res_Rk,
    &join_appender,
    &R_keyCol_Idx,
    &S_keyCol_Idx,
    &R_col_count,
    &S_col_count,
    &R_pL_bytesPerRow,
    &S_pL_bytesPerRow,
    &R_type_ids,
    &S_type_ids,
    &R_payloadTypes,
    &S_payloadTypes,
    &join_type_ids,
    saveAsTempTable
  );
  if(jp<0) {
    perror("Failed in join preparation.");
    return;
  }
  idx_t R_curIdx = 0;
  idx_t S_curIdx = 0;  

  // TODO ##### Loop over R (left table) -- for each chunk of R we will loop over S
  mylog(logfile, "Iterating over R...");
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    char *Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);
    char *R_payload;
    struct futhark_u8_2d *Rbuff_ft;
    struct futhark_i64_1d *R_idx_ft;
    struct futhark_opaque_partitionInfo *R_pInfo;
    struct futhark_opaque_radix_hashTable *R_RHT;

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
      radixPartition_GFUR(
        ctx,
        &Rbuff_ft,
        &R_idx_ft,
        Rbuff,
        R_rowCount,
        colType_bytes(key_type),
        radix_bits,
        MAX_DEPTH,
        MAX_PARTITION_SIZE,
        0,
        SCATTER_PSIZE
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      Rbuff_ft = futhark_new_u8_2d(ctx, Rbuff, R_rowCount, colType_bytes(key_type));
      idx_t *R_indices = malloc(R_rowCount * sizeof(idx_t));
      for(idx_t i=0; i<R_rowCount; i++) {
        R_indices[i] = i;
      }
      R_idx_ft = futhark_new_i64_1d(ctx, R_indices, R_rowCount);
      free(R_indices);
      futhark_context_sync(ctx);
    }
    free(Rbuff);
    Rbuff = NULL;

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

    mylog(logfile, "Obtaining R's radix hash table...");
      getRadixHashTable(
        ctx,
        &R_RHT,
        Rbuff_ft,
        R_pInfo,
        radix_bits,
        SCATTER_PSIZE
      );
      mylog(logfile, "Obtained R's radix hash table.");

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
      char *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
      char *S_payload;
      struct futhark_u8_2d *Sbuff_ft;
      struct futhark_i64_1d *S_idx_ft;

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
        radixPartition_GFUR(
          ctx,
          &Sbuff_ft,
          &S_idx_ft,
          Sbuff,
          S_rowCount,
          colType_bytes(key_type),
          radix_bits,
          MAX_DEPTH,
          MAX_PARTITION_SIZE,
          0,
          SCATTER_PSIZE
        );
        mylog(logfile, "Sorted this buffer of S.");
      } else {
        Sbuff_ft = futhark_new_u8_2d(ctx, Sbuff, S_rowCount, colType_bytes(key_type));
        idx_t *S_indices = malloc(S_rowCount * sizeof(idx_t));
        for(idx_t i=0; i<S_rowCount; i++) {
          S_indices[i] = i;
        }
        S_idx_ft = futhark_new_i64_1d(ctx, S_indices, S_rowCount);
        free(S_indices);
        futhark_context_sync(ctx);
      }
      free(Sbuff);
      Sbuff = NULL;

      // Perform join
      idx_t numPairs = 0;
      char *joinedKeys;
      struct futhark_i64_1d *idxR_ft;
      struct futhark_i64_1d *idxS_ft;
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      // PERFORM THE JOIN
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      mylog(logfile, "Performing the key join...");
      // Invert R&S because calculating info for R is cheaper...
      if(is_R_unique) {
        HashJoin_joinKeyColumns_inFuthark_with_unique_keys2(
          ctx,
          &numPairs,
          &joinedKeys,
          &idxS_ft,
          &idxR_ft,
          colType_bytes(key_type),
          radix_bits,
          0,
          0,
          Sbuff_ft,
          Rbuff_ft,
          R_pInfo,
          R_RHT,
          S_rowCount,
          R_rowCount
        );
      }
      else {
        HashJoin_joinKeyColumns_inFuthark(
          ctx,
          &numPairs,
          &joinedKeys,
          &idxS_ft,
          &idxR_ft,
          colType_bytes(key_type),
          radix_bits,
          0,
          0,
          Sbuff_ft,
          Rbuff_ft,
          R_pInfo,
          R_RHT,
          S_rowCount,
          R_rowCount
        );
      }
      mylog(logfile, "Performed key join.");

      // Gather R's payloads
      char* gathered_R_payload = malloc(numPairs * R_pL_bytesPerRow);
      gatherPayloads_GFUR_inFuthark(ctx, gathered_R_payload, R_pL_bytesPerRow, 0, 0, BLOCK_SIZE, R_idx_ft, idxR_ft, R_payload, R_rowCount, numPairs);
      futhark_free_i64_1d(ctx, idxR_ft); // no longer needed
      mylog(logfile, "Gathered R payloads.");
      void* Rpl[R_col_count-1];
      payloadColumnsFromByteArray(Rpl, R_payloadTypes, gathered_R_payload, R_col_count-1, numPairs);
      free(gathered_R_payload);
      mylog(logfile, "Recovered gathered R payloads from byte array.");

      // Gather S's payloads
      char* gathered_S_payload = malloc(numPairs * S_pL_bytesPerRow);
      gatherPayloads_GFUR_inFuthark(ctx, gathered_S_payload, S_pL_bytesPerRow, 0, 0, BLOCK_SIZE, S_idx_ft, idxS_ft, S_payload, S_rowCount, numPairs);
      futhark_free_i64_1d(ctx, idxS_ft); // no longer needed
      mylog(logfile, "Gathered S payloads.");
      void* Spl[S_col_count-1];
      payloadColumnsFromByteArray(Spl, S_payloadTypes, gathered_S_payload, S_col_count-1, numPairs);
      free(gathered_S_payload);
      free(S_payload);
      futhark_free_i64_1d(ctx, S_idx_ft);
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
      for(idx_t col=1; col<R_col_count; col++) {
        free(Rpl[col-1]);
      }
      for(idx_t col=1; col<S_col_count; col++) {
        free(Spl[col-1]);
      }
      free(joinedKeys);

      S_curIdx += S_rowCount;
    }

    duckdb_appender_flush(join_appender);

    futhark_free_u8_2d(ctx, Rbuff_ft);
    futhark_free_opaque_partitionInfo(ctx, R_pInfo);
    futhark_free_opaque_radix_hashTable(ctx, R_RHT);
    futhark_free_i64_1d(ctx, R_idx_ft);
    free(R_payload);
    duckdb_destroy_result(&res_Sk);

    R_curIdx += R_rowCount;
  }
  duckdb_destroy_result(&res_Rk);
  free(R_type_ids);
  free(S_type_ids);
  free(R_payloadTypes);
  free(S_payloadTypes);
  free(join_type_ids);
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);
}

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
  int is_R_unique,
  int is_S_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  if ((!is_R_unique) && is_S_unique) {
    do_RadixHashJoin_GFTR(
      CHUNK_SIZE,
      S_JOIN_BUFFER,
      R_JOIN_BUFFER,
      BLOCK_SIZE,
      SCATTER_PSIZE,
      MAX_PARTITION_SIZE,
      MAX_DEPTH,
      logfile,
      ctx,
      con,
      radix_bits,
      S_tbl_name,
      R_tbl_name,
      is_S_partitioned,
      is_R_partitioned,
      true,
      S_keyName,
      R_keyName,
      Join_tbl_name,
      quicksaves,
      saveAsTempTable
    );
  }
  else {
    do_RadixHashJoin_GFTR(
      CHUNK_SIZE,
      R_JOIN_BUFFER,
      S_JOIN_BUFFER,
      BLOCK_SIZE,
      SCATTER_PSIZE,
      MAX_PARTITION_SIZE,
      MAX_DEPTH,
      logfile,
      ctx,
      con,
      radix_bits,
      R_tbl_name,
      S_tbl_name,
      is_R_partitioned,
      is_S_partitioned,
      is_R_unique,
      R_keyName,
      S_keyName,
      Join_tbl_name,
      quicksaves,
      saveAsTempTable
    );
  }
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
  int is_R_unique,
  int is_S_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  if ((!is_R_unique) && is_S_unique) {
    do_RadixHashJoin_GFUR(
      CHUNK_SIZE,
      S_JOIN_BUFFER,
      R_JOIN_BUFFER,
      BLOCK_SIZE,
      SCATTER_PSIZE,
      MAX_PARTITION_SIZE,
      MAX_DEPTH,
      logfile,
      ctx,
      con,
      radix_bits,
      S_tbl_name,
      R_tbl_name,
      is_S_partitioned,
      is_R_partitioned,
      true,
      S_keyName,
      R_keyName,
      Join_tbl_name,
      quicksaves,
      saveAsTempTable
    );
  }
  else {
    do_RadixHashJoin_GFUR(
      CHUNK_SIZE,
      R_JOIN_BUFFER,
      S_JOIN_BUFFER,
      BLOCK_SIZE,
      SCATTER_PSIZE,
      MAX_PARTITION_SIZE,
      MAX_DEPTH,
      logfile,
      ctx,
      con,
      radix_bits,
      R_tbl_name,
      S_tbl_name,
      is_R_partitioned,
      is_S_partitioned,
      is_R_unique,
      R_keyName,
      S_keyName,
      Join_tbl_name,
      quicksaves,
      saveAsTempTable
    );
  }
}