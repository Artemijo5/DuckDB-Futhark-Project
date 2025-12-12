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
  int32_t bit_step,
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
    false,
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
        bit_step,
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
          bit_step,
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
  int32_t bit_step,
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
    false,
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
        bit_step,
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
          bit_step,
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
  int32_t bit_step,
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
      bit_step,
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
      bit_step,
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
  int32_t bit_step,
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
      bit_step,
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
      bit_step,
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

// ----------------------------------------------------------------
// SEMI-PARTITIONING FUNCS
// ----------------------------------------------------------------

idx_t radix_semiPartition_GFTR(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  int32_t radix_bits,
  int32_t bit_step,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  int quicksaves
) {
  idx_t numInterm = 0;
  duckdb_result res;
  char q[25 + strlen(tblName)];
  sprintf(q, "SELECT * FROM %s;", tblName);
  if(duckdb_query(con, q, &res) == DuckDBError) {
    perror("Failed to read table in radix_semiPartition_GFTR.");
    return -1;
  }
  mylog(logfile, "radix_semiPartition_GFTR: Read table.\n");

  idx_t col_count = duckdb_column_count(&res);
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    //mylog(logfile, "Obtained column's type.");
  }
  idx_t keyCol_idx = -1; // Column index of the key column
  char** colNames = malloc(col_count*sizeof(char*));
  for(idx_t i=0; i<col_count; i++) {
    colNames[i] = malloc(strlen(duckdb_column_name(&res, i)) + 1);
    sprintf(colNames[i], "%s", duckdb_column_name(&res, i));

    if(strcmp(colNames[i], keyName) == 0) keyCol_idx = i;
  }
  duckdb_type key_type = type_ids[keyCol_idx];

  int exhaustedRes = false;
  while(!exhaustedRes) {
    mylog(logfile, "Next page...");

    void *keyBuffer = colType_malloc(type_ids[keyCol_idx], BUFFER_SIZE);
    struct futhark_u8_2d *keyBuffer_ft;

    char* payloadBuffer;
    mylog(logfile, "Initalising buffers for each column...");
    idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
    idx_t pL_byteSizes[col_count-1]; // bytes taken by each column
    idx_t pL_prefixSizes[col_count-1]; // bytes taken up to each column
    duckdb_type payload_types[col_count-1]; // types of payload columns only
    for(idx_t col=0; col<col_count; col++) {
      if(col == keyCol_idx) continue; // payloads only
      idx_t accIdx = (col < keyCol_idx)? col: col-1; // adjust index
      pL_byteSizes[accIdx] = colType_bytes(type_ids[col]);
      pL_prefixSizes[accIdx] = pL_bytes;
      pL_bytes += pL_byteSizes[accIdx];
      payload_types[accIdx] = type_ids[col];
    }
    struct futhark_u8_2d *payloadBuffer_ft;

    idx_t cur_rows = bulk_load_chunks_GFTR(
      CHUNK_SIZE,
      res,
      60, // TODO make argument
      col_count,
      keyCol_idx,
      type_ids,
      keyBuffer,
      &payloadBuffer,
      BUFFER_SIZE,
      &exhaustedRes
    );
    if(exhaustedRes && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      free(keyBuffer);
      free(payloadBuffer);
      break;
    }
    char msgbuffer[100];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    radixPartition_GFTR(
      ctx,
      &keyBuffer_ft,
      &payloadBuffer_ft,
      keyBuffer,
      payloadBuffer,
      cur_rows,
      colType_bytes(key_type),
      pL_bytes,
      radix_bits,
      MAX_DEPTH,
      bit_step,
      MAX_PARTITION_SIZE,
      SCATTER_PSIZE
    );
    futhark_context_sync(ctx);
    futhark_values_u8_2d(ctx, keyBuffer_ft, keyBuffer);
    futhark_free_u8_2d(ctx, keyBuffer_ft);
    futhark_values_u8_2d(ctx, payloadBuffer_ft, payloadBuffer);
    futhark_free_u8_2d(ctx, payloadBuffer_ft);
    mylog(logfile, "Performed partitioning.");

    numInterm = store_intermediate_GFTR(
      numInterm,
      intermName,
      con,
      CHUNK_SIZE,
      col_count,
      cur_rows,
      type_ids,
      colNames,
      keyBuffer,
      payloadBuffer,
      keyCol_idx
    );
    if(numInterm == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
    free(keyBuffer);
    free(payloadBuffer);
    mylog(logfile, "Freed this page's buffers.");
  }
  duckdb_destroy_result(&res);
  for(idx_t i=0; i<col_count; i++) {
    free(colNames[i]);
  }
  free(colNames);

  return numInterm;
}
idx_t radix_semiPartition_GFUR(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  int32_t radix_bits,
  int32_t bit_step,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  int quicksaves
) {
  idx_t numInterm = 0;
  idx_t total_read = 0;
  duckdb_result res;
  char q[50 + strlen(keyName) + strlen(tblName)];
  sprintf(q, "SELECT %s FROM %s;", keyName, tblName);
  if(duckdb_query(con, q, &res) == DuckDBError) {
    perror("Failed to read table in radix_semiPartition_GFTR.");
    return -1;
  }
  mylog(logfile, "radix_semiPartition_GFTR: Read table.\n");

  duckdb_type key_type = duckdb_column_type(&res, 0);

  int exhaustedRes = false;
  while(!exhaustedRes) {
    mylog(logfile, "Next page...");

    void *keyBuffer = colType_malloc(key_type, BUFFER_SIZE);
    struct futhark_u8_2d *keyBuffer_ft;

    void *idxBuffer = colType_malloc(key_type, BUFFER_SIZE);
    struct futhark_i64_1d *idxBuffer_ft;

    idx_t cur_rows = bulk_load_chunks_GFTR(
      CHUNK_SIZE,
      res,
      60, // TODO make argument
      1,
      0,
      &key_type,
      keyBuffer,
      NULL,
      BUFFER_SIZE,
      &exhaustedRes
    );
    if(exhaustedRes && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      free(keyBuffer);
      free(idxBuffer);
      break;
    }
    char msgbuffer[100];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    radixPartition_GFUR(
      ctx,
      &keyBuffer_ft,
      &idxBuffer_ft,
      keyBuffer,
      cur_rows,
      colType_bytes(key_type),
      radix_bits,
      MAX_DEPTH,
      bit_step,
      MAX_PARTITION_SIZE,
      total_read,
      SCATTER_PSIZE
    );
    futhark_context_sync(ctx);
    futhark_values_u8_2d(ctx, keyBuffer_ft, keyBuffer);
    futhark_free_u8_2d(ctx, keyBuffer_ft);
    futhark_values_i64_1d(ctx, idxBuffer_ft, idxBuffer);
    futhark_free_i64_1d(ctx, idxBuffer_ft);
    mylog(logfile, "Performed partitioning.");
    total_read+=cur_rows;

    //void *Buffers[2] = {keyBuffer, (void*)idxBuffer};
    duckdb_type type_ids[2] = {key_type, DUCKDB_TYPE_BIGINT};

    char *colNames[2];
    colNames[0] = malloc(1+strlen(keyName));
    colNames[1] = malloc(7);
    memcpy(colNames[0], keyName, 1+strlen(keyName));
    memcpy(colNames[1], "rowIdx", 7);

    numInterm = store_intermediate_GFTR(
      numInterm,
      intermName,
      con,
      CHUNK_SIZE,
      2,
      cur_rows,
      type_ids,
      colNames,
      keyBuffer,
      idxBuffer,
      0
    );
    if(numInterm == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
    free(keyBuffer);
    free(idxBuffer);
    free(colNames[0]);
    free(colNames[1]);
    mylog(logfile, "Freed this page's buffers.");
  }
  duckdb_destroy_result(&res);

  return numInterm;
}

void RadixHashJoin_semisorted_GFTR(
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
  int32_t bit_step,
  const char *R_interm_name,
  const char *S_interm_name,
  idx_t R_numInterms,
  idx_t S_numInterms,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  /*
    idx_t R_numInterms = (is_R_semi_partitioned)?
      R_numInterms_:
      radix_semiPartition_GFTR(
        CHUNK_SIZE,
        R_JOIN_BUFFER+S_JOIN_BUFFER,
        BLOCK_SIZE,
        SCATTER_PSIZE,
        MAX_PARTITION_SIZE,
        MAX_DEPTH,
        logfile,
        ctx,
        radix_bits,
        bit_step,
        ctx,
        con,
        R_tbl_name,
        R_keyName,
        R_interm_name,
        quicksaves
      );
    idx_t S_numInterms = (is_S_semi_partitioned)?
      S_numInterms_:
      radix_semiPartition_GFTR(
        CHUNK_SIZE,
        R_JOIN_BUFFER+S_JOIN_BUFFER,
        BLOCK_SIZE,
        SCATTER_PSIZE,
        MAX_PARTITION_SIZE,
        MAX_DEPTH,
        logfile,
        ctx,
        radix_bits,
        bit_step,
        ctx,
        con,
        S_tbl_name,
        S_keyName,
        S_interm_name,
        quicksaves
      );

    char cur_join[R_numInterms*S_numInterms][25 + strlen(Join_interm_name)];
    char Union_Q[250 + strlen(Join_tbl_name) + R_numInterms*S_numInterms*(50 + strlen(Join_interm_name))];
    int len_UQ = (!saveAsTempTable)?
      sprintf(Union_Q, "CREATE OR REPLACE TABLE %s AS ", Join_tbl_name):
      sprintf(Union_Q, "CREATE OR REPLACE TEMP TABLE %s AS ", Join_tbl_name):
    for(idx_t ij=0; ij<R_numInterms*S_numInterms; ij++) {
      sprintf(cur_join[ij], "%s%ld", Join_interm_name, ij);
    }

    for(idx_t ir=0; ir<R_numInterms; ir++) {
      char cur_R[25 + strlen(R_interm_name)];
      sprintf(cur_R, "%s%ld", R_interm_name, ir);
      for(idx_t is=0; is<S_numInterms; is++) {
        char cur_S[25 + strlen(S_interm_name)];
        sprintf(cur_S, "%s%ld", S_interm_name, is);


      }
    }
  */
  duckdb_type key_type;
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
    R_interm_name,
    S_interm_name,
    Join_tbl_name,
    true,
    true,
    R_keyName,
    S_keyName,
    &key_type,
    NULL,
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
    false,
    saveAsTempTable
  );
  if(jp<0) {
    perror("Failed in join preparation.");
    return;
  }
  idx_t R_curIdx = 0;
  idx_t S_curIdx = 0;

  mylog(logfile, "Iterating over R...");
  for(idx_t ir=0; ir<R_numInterms; ir++) {
    duckdb_result res_Rk;
    char rq[100 + strlen(R_interm_name)];
    sprintf(rq, "SELECT * FROM %s%ld;", R_interm_name, ir);
    if(duckdb_query(con, rq, &res_Rk) == DuckDBError) {
      perror("Failed to obtain R intermediate.");
      return;
    }

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
      Rbuff_ft = futhark_new_u8_2d(ctx, Rbuff, R_rowCount, colType_bytes(key_type));
      R_payload_ft = futhark_new_u8_2d(ctx, R_payload, R_rowCount, R_pL_bytesPerRow);
      futhark_context_sync(ctx);
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
      for(idx_t is=0; is<S_numInterms; is++) {
        duckdb_result res_Sk;
        char this_S[25+strlen(S_interm_name)];
        sprintf(this_S, "%s%ld", S_interm_name, is);
        // TODO construct S query to read from minimum relevant index
        duckdb_state S_query = radix_selective_query(
          key_type,
          con,
          &res_Sk,
          this_S,
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

          Sbuff_ft = futhark_new_u8_2d(ctx, Sbuff, S_rowCount, colType_bytes(key_type));
          S_payload_ft = futhark_new_u8_2d(ctx, S_payload, S_rowCount, S_pL_bytesPerRow);
          futhark_context_sync(ctx);
          
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

        duckdb_destroy_result(&res_Sk);
      }
      futhark_free_u8_2d(ctx, Rbuff_ft);
      futhark_free_u8_2d(ctx, R_payload_ft);
      futhark_free_opaque_partitionInfo(ctx, R_pInfo);
      futhark_free_opaque_radix_hashTable(ctx, R_RHT);
      R_curIdx += R_rowCount;
    }
    duckdb_destroy_result(&res_Rk);
  }
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);
  free(R_type_ids);
  free(S_type_ids);
  free(R_payloadTypes);
  free(S_payloadTypes);
  free(join_type_ids);
}
void RadixHashJoin_semisorted_GFUR(
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
  int32_t bit_step,
  const char *R_tbl_name,
  const char *S_tbl_name,
  const char *R_interm_name,
  const char *S_interm_name,
  idx_t R_numInterms,
  idx_t S_numInterms,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_temp_tbl_name,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  RadixHashJoin_semisorted_GFTR(
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
    bit_step,
    R_interm_name,
    S_interm_name,
    R_numInterms,
    S_numInterms,
    R_keyName,
    S_keyName,
    Join_temp_tbl_name,
    quicksaves,
    true
  );
  // TODO could use my own joins instead of queries?
  // altho that would complicate things with rowid...
  // - first joins could be done with duckdb, final one with futhark...
  char final_Query[
    2500 + 
    strlen(Join_tbl_name) + 
    strlen(S_keyName) + 
    strlen(R_tbl_name) + 
    strlen(S_tbl_name) + 
    2*strlen(Join_temp_tbl_name)
  ];
  int qlen = (!saveAsTempTable)?
    sprintf(final_Query, "CREATE OR REPLACE TABLE %s AS (", Join_tbl_name):
    sprintf(final_Query, "CREATE OR REPLACE TEMP TABLE %s AS \n(\t", Join_tbl_name);
  qlen += sprintf(final_Query + qlen, "SELECT rt.* EXCLUDE rowIdx, st.* EXCLUDE(rowIdx,%s) FROM \n\t\t", S_keyName);
  qlen += sprintf(
    final_Query+qlen,
    "(SELECT b.*, a.rowid AS rowIdx FROM %s a JOIN %s b ON (a.rowIdx == b.rowid) ORDER BY a.rowid) rt JOIN\n\t\t",
    Join_temp_tbl_name, R_tbl_name
  );
  qlen += sprintf(
    final_Query+qlen,
    "(SELECT b.*, a.rowid AS rowIdx FROM %s a JOIN %s b ON (a.rowIdx_1 == b.rowid) ORDER BY a.rowid) st\n\t",
    Join_temp_tbl_name, S_tbl_name
  );
  qlen += sprintf(final_Query+qlen, "ON rt.rowIdx == st.rowIdx);");
  if(duckdb_query(con, final_Query, NULL) == DuckDBError) {
    perror("Failed to perform final gathering query.");
    return;
  }
  mylog(logfile, "Performed query to gather payloads.");
}