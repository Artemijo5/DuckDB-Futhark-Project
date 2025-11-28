#include "SMJstages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "join_util.h"
#include "smjutil.h"
#include "db_util.h"

void SortMergeJoin_GFTR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  int radix_sort,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_sorted,
  int is_S_sorted,
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
    void *Rbuff_ft;
    struct futhark_u8_2d *R_payload_ft;

    void *R_minimum = malloc(colType_bytes(key_type));
    void *R_maximum = malloc(colType_bytes(key_type));

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
    if(!is_R_sorted) {
      sortRelationByKey_inFuthark(
        ctx,
        &Rbuff_ft,
        &R_payload_ft,
        key_type,
        radix_sort,
        BLOCK_SIZE,
        Rbuff,
        R_payload,
        R_pL_bytesPerRow,
        R_rowCount,
        R_minimum,
        R_maximum
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          Rbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_short(ctx, (int16_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_short(ctx, (int16_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          Rbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_int(ctx, (int32_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_int(ctx, (int32_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          Rbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_long(ctx, (int64_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_long(ctx, (int64_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          Rbuff_ft = futhark_new_f32_1d(ctx, (float*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_float(ctx, (float*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_float(ctx, (float*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          Rbuff_ft = futhark_new_f64_1d(ctx, (double*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_double(ctx, (double*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_double(ctx, (double*)R_maximum, Rbuff_ft);
          break;
        default:
          perror("Invalid type.");
          return;
      }
      R_payload_ft = futhark_new_u8_2d(ctx, R_payload, R_rowCount, R_pL_bytesPerRow);
      futhark_context_sync(ctx);
    }
    free(Rbuff);
    Rbuff = NULL;
    free(R_payload);
    R_payload = NULL;

    mylog(logfile, "Obtain S's keys...");
    duckdb_result res_Sk;
    duckdb_state S_query =
    selective_query(
      key_type,
      con,
      &res_Sk,
      S_tbl_name,
      S_keyName,
      R_minimum,
      R_maximum
    );
    if(S_query == DuckDBError) {
      perror("Failed to perform selective query over right-side table.");
      return;
    }
    mylog(logfile, "Obtained S's keys.");
    free(R_minimum);
    free(R_maximum);

    mylog(logfile, "Now iterating over S for the join...");
    // that is, only when S is sorted
    int exhaustedRes_S = false;
    while(!(exhaustedRes_S)) {
      //printf("Marco!\n");
      char *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
      char *S_payload;
      void *Sbuff_ft;
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

      if(!is_S_sorted) {
        sortRelationByKey_inFuthark(
          ctx,
          &Sbuff_ft,
          &S_payload_ft,
          key_type,
          radix_sort,
          BLOCK_SIZE,
          Sbuff,
          S_payload,
          S_pL_bytesPerRow,
          S_rowCount,
          NULL,
          NULL
        );
        mylog(logfile, "Sorted this buffer of S.");
      } else {
        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            Sbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_INTEGER:
            Sbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_BIGINT:
            Sbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_FLOAT:
            Sbuff_ft = futhark_new_f32_1d(ctx, (float*)Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_DOUBLE:
            Sbuff_ft = futhark_new_f64_1d(ctx, (double*)Sbuff, S_rowCount);
            break;
          default:
            perror("Invalid type.");
            return;
        }
        S_payload_ft = futhark_new_u8_2d(ctx, S_payload, S_rowCount, S_pL_bytesPerRow);
        futhark_context_sync(ctx);
      }
      free(Sbuff);
      Sbuff = NULL;
      free(S_payload);
      S_payload = NULL;

      // Perform join
      idx_t numPairs = 0;
      void *joinedKeys;
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
      InnerJoin_joinKeyColumns_inFuthark(
        ctx,
        &numPairs,
        &joinedKeys,
        &idxR_ft,
        &idxS_ft,
        key_type,
        0,
        0,
        Rbuff_ft,
        Sbuff_ft,
        R_rowCount,
        S_rowCount,
        MERGE_PARTITION_SIZE,
        GATHER_PSIZE
      );
      mylog(logfile, "Performed key join.");
      
      // Gather R's payloads
      char* Rpl_asBytes;
      Rpl_asBytes = malloc(numPairs * R_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Rpl_asBytes, R_pL_bytesPerRow, 0, GATHER_PSIZE, idxR_ft, R_payload_ft, R_rowCount, numPairs);
      mylog(logfile, "Gathered R payloads.");
      void* Rpl[R_col_count-1];
      payloadColumnsFromByteArray(Rpl, R_payloadTypes, Rpl_asBytes, R_col_count-1, numPairs);
      free(Rpl_asBytes);
      mylog(logfile, "Recovered gathered R payloads from byte array.");

      // Gather S's payloads
      char* Spl_asBytes;
      Spl_asBytes = malloc(numPairs * S_pL_bytesPerRow);
      gatherPayloads_GFTR(ctx, Spl_asBytes, S_pL_bytesPerRow, 0, GATHER_PSIZE, idxS_ft, S_payload_ft, S_rowCount, numPairs);
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
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          futhark_free_i16_1d(ctx, Sbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          futhark_free_i32_1d(ctx, Sbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          futhark_free_i64_1d(ctx, Sbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          futhark_free_f32_1d(ctx, Sbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          futhark_free_f64_1d(ctx, Sbuff_ft);
          break;
        default:
          perror("Invalid type!");
          return;
      }
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

    switch(key_type) {
      case DUCKDB_TYPE_SMALLINT:
        futhark_free_i16_1d(ctx, Rbuff_ft);
        break;
      case DUCKDB_TYPE_INTEGER:
        futhark_free_i32_1d(ctx, Rbuff_ft);
        break;
      case DUCKDB_TYPE_BIGINT:
        futhark_free_i64_1d(ctx, Rbuff_ft);
        break;
      case DUCKDB_TYPE_FLOAT:
        futhark_free_f32_1d(ctx, Rbuff_ft);
        break;
      case DUCKDB_TYPE_DOUBLE:
        futhark_free_f64_1d(ctx, Rbuff_ft);
        break;
      default:
        perror("Invalid type!");
        return;
    }
    futhark_free_u8_2d(ctx, R_payload_ft);
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

void Inner_MergeJoin_GFTR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	idx_t BLOCK_SIZE,
	idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  int radix_sort,
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
  SortMergeJoin_GFTR(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    radix_sort,
    logfile,
    ctx,
    con,
    sorted_R_tbl_name,
    sorted_S_tbl_name,
    true,
    true,
    R_keyName,
    S_keyName,
    Join_tbl_name,
    quicksaves,
    saveAsTempTable
  );
}

void SortMergeJoin_GFUR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  int radix_sort,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_sorted,
  int is_S_sorted,
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

  mylog(logfile, "Iterating over R...");
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    void* Rbuff;
    void* Rbuff_ft;
    char* R_payload;
    struct futhark_i64_1d* R_idx_ft;
    Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);

    char R_minimum[colType_bytes(key_type)];
    char R_maximum[colType_bytes(key_type)];

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

    // If R is not sorted, sort
    // Either way, maintain as Rbuff_ft
    if(!is_R_sorted) {
      sortKeyColumn_inFuthark(
        ctx,
        &Rbuff_ft,
        key_type,
        0,
        radix_sort, // TODO argument
        BLOCK_SIZE, // TODO
        &R_idx_ft,
        Rbuff,
        R_rowCount,
        R_minimum,
        R_maximum
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          Rbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_short(ctx, (int16_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_short(ctx, (int16_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          Rbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_int(ctx, (int32_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_int(ctx, (int32_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          Rbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_long(ctx, (int64_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_long(ctx, (int64_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          Rbuff_ft = futhark_new_f32_1d(ctx, (float*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_float(ctx, (float*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_float(ctx, (float*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          Rbuff_ft = futhark_new_f64_1d(ctx, (double*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_double(ctx, (double*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_double(ctx, (double*)R_maximum, Rbuff_ft);
          break;
        default:
          perror("Invalid type.");
          return;
      }
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

    logdbg(logfile, is_S_sorted, "Obtain S's sorted keys...", "Obtain S's keys...");
    duckdb_result res_Sk;
    duckdb_state S_query = selective_query(
      key_type,
      con,
      &res_Sk,
      S_tbl_name,
      S_keyName,
      R_minimum,
      R_maximum
    );
    if(S_query == DuckDBError) {
      perror("Failed to perform selective query over right-side table.");
      return;
    }
    mylog(logfile, "Obtained S's sorted keys.");

    mylog(logfile, "Now iterating over S for the join...");
    int flag_continueWithThisR_partition = true; // when Sbuff.max > Rbuff.max, stop reading S chunks for this R partition
    // that is, only when S is sorted
    int exhaustedRes_S = false;
    while((flag_continueWithThisR_partition || !is_S_sorted) && !(exhaustedRes_S)) {
      //printf("Marco!\n");
      void *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
      void *Sbuff_ft;
      char *S_payload;
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

      if(!is_S_sorted) {
        sortKeyColumn_inFuthark(
          ctx,
          &Sbuff_ft,
          key_type,
          0,
          radix_sort, // TODO argument
          BLOCK_SIZE, // TODO
          &S_idx_ft,
          Sbuff,
          S_rowCount,
          NULL,
          NULL
        );
        mylog(logfile, "Sorted this buffer of S.");
      } else {
        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            Sbuff_ft = (void*)futhark_new_i16_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_INTEGER:
            Sbuff_ft = (void*)futhark_new_i32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_BIGINT:
            Sbuff_ft = (void*)futhark_new_i64_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_FLOAT:
            Sbuff_ft = (void*)futhark_new_f32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_DOUBLE:
            Sbuff_ft = (void*)futhark_new_f64_1d(ctx, Sbuff, S_rowCount);
            break;
          default:
            perror("Invalid type!");
            break;
        }
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
      void* joinedKeys;
      struct futhark_i64_1d *idxR_ft;
      struct futhark_i64_1d *idxS_ft;
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      // PERFORM THE JOIN
      // #######################################################################################################
      // #######################################################################################################
      // #######################################################################################################
      InnerJoin_joinKeyColumns_inFuthark(
        ctx,
        &numPairs,
        &joinedKeys, // vs
        &idxR_ft, // R indices
        &idxS_ft, // S indices
        key_type,
        R_curIdx, // R_idx
        S_curIdx, // S_idx
        Rbuff_ft, // R keys
        Sbuff_ft, // S keys
        R_rowCount, // card1
        S_rowCount, // card2
        MERGE_PARTITION_SIZE,
        BLOCK_SIZE // for multi-pass scatter
      );
      mylog(logfile, "Join has been performed.");
      // Gather R's payloads
      char* gathered_R_payload = malloc(numPairs * R_pL_bytesPerRow);
      gatherPayloads_GFUR_inFuthark(ctx, gathered_R_payload, R_pL_bytesPerRow, 0, R_curIdx, GATHER_PSIZE, R_idx_ft, idxR_ft, R_payload, R_rowCount, numPairs);
      futhark_free_i64_1d(ctx, idxR_ft); // no longer needed
      mylog(logfile, "Gathered R payloads.");
      void* Rpl[R_col_count-1];
      payloadColumnsFromByteArray(Rpl, R_payloadTypes, gathered_R_payload, R_col_count-1, numPairs);
      free(gathered_R_payload);
      mylog(logfile, "Recovered gathered R payloads from byte array.");

      // Gather S's payloads
      char* gathered_S_payload = malloc(numPairs * S_pL_bytesPerRow);
      gatherPayloads_GFUR_inFuthark(ctx, gathered_S_payload, S_pL_bytesPerRow, 0, S_curIdx, GATHER_PSIZE, S_idx_ft, idxS_ft, S_payload, S_rowCount, numPairs);
      futhark_free_i64_1d(ctx, idxS_ft); // no longer needed
      mylog(logfile, "Gathered R payloads.");
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
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Sbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Sbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Sbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Sbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Sbuff_ft);
          break;
        default:
          perror("Invalid type!");
          return;
      }
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

    switch(key_type) {
      case DUCKDB_TYPE_SMALLINT:
        futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_INTEGER:
        futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_BIGINT:
        futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_FLOAT:
        futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_DOUBLE:
        futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Rbuff_ft);
        break;
      default:
        perror("Invalid type!");
        return;
    }
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

void Inner_MergeJoin_GFUR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	idx_t BLOCK_SIZE,
	idx_t MERGE_PARTITION_SIZE,
	idx_t GATHER_PSIZE,
  int radix_sort,
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
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    radix_sort,
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

// ------------------------------------------------------------------------------------
// WITH S SEMISORTED
// ------------------------------------------------------------------------------------

void SortMergeJoin_GFTR_with_S_semisorted(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  int radix_sort,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_sorted,
  idx_t S_num_tbls,
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
    true,
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

  mylog(logfile, "Iterating over R...");
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    void* Rbuff;
    void* Rbuff_ft;
    char* R_payload;
    struct futhark_u8_2d* R_payload_ft;
    Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);

    char R_minimum[colType_bytes(key_type)];
    char R_maximum[colType_bytes(key_type)];

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

    // If R is not sorted, sort
    // Either way, maintain as Rbuff_ft
    if(!is_R_sorted) {
      sortRelationByKey_inFuthark(
        ctx,
        &Rbuff_ft,
        &R_payload_ft,
        key_type,
        radix_sort, // TODO argument
        BLOCK_SIZE, // TODO
        Rbuff,
        R_payload,
        R_pL_bytesPerRow,
        R_rowCount,
        R_minimum,
        R_maximum
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          Rbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_short(ctx, (int16_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_short(ctx, (int16_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          Rbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_int(ctx, (int32_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_int(ctx, (int32_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          Rbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_long(ctx, (int64_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_long(ctx, (int64_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          Rbuff_ft = futhark_new_f32_1d(ctx, (float*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_float(ctx, (float*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_float(ctx, (float*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          Rbuff_ft = futhark_new_f64_1d(ctx, (double*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_double(ctx, (double*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_double(ctx, (double*)R_maximum, Rbuff_ft);
          break;
        default:
          perror("Invalid type.");
          return;
      }
      R_payload_ft = futhark_new_u8_2d(ctx, R_payload, R_rowCount, R_pL_bytesPerRow);
      futhark_context_sync(ctx);
    }
    free(Rbuff);
    Rbuff = NULL;
    free(R_payload);
    R_payload = NULL;

    mylog(logfile, "Iterating over S tables...");
    for (idx_t interm=0; interm<S_num_tbls; interm++) {
      duckdb_result res_Sk;

      char S_this_tbl_name[strlen(S_tbl_name + 50)];
      sprintf(S_this_tbl_name, "%s%ld", S_tbl_name, interm);
      
      duckdb_state S_query = selective_query(
        key_type,
        con,
        &res_Sk,
        S_this_tbl_name,
        S_keyName,
        R_minimum,
        R_maximum
      );
      if(S_query == DuckDBError) {
        perror("Failed to perform selective query over right-side table.");
        return;
      }
      mylog(logfile, "Obtained S's sorted keys.");

      mylog(logfile, "Now iterating over S for the join...");
      // that is, only when S is sorted
      int exhaustedRes_S = false;
      while(!(exhaustedRes_S)) {
        //printf("Marco!\n");
        void *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
        void *Sbuff_ft;
        char *S_payload;
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

        
        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            Sbuff_ft = (void*)futhark_new_i16_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_INTEGER:
            Sbuff_ft = (void*)futhark_new_i32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_BIGINT:
            Sbuff_ft = (void*)futhark_new_i64_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_FLOAT:
            Sbuff_ft = (void*)futhark_new_f32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_DOUBLE:
            Sbuff_ft = (void*)futhark_new_f64_1d(ctx, Sbuff, S_rowCount);
            break;
          default:
            perror("Invalid type!");
            break;
        }
          S_payload_ft = futhark_new_u8_2d(ctx, S_payload, S_rowCount, S_pL_bytesPerRow);
          futhark_context_sync(ctx);
        free(Sbuff);
        Sbuff = NULL;
        free(S_payload);
        S_payload = NULL;

        // Perform join
        idx_t numPairs = 0;
        void* joinedKeys;
        struct futhark_i64_1d *idxR_ft;
        struct futhark_i64_1d *idxS_ft;
        // #######################################################################################################
        // #######################################################################################################
        // #######################################################################################################
        // PERFORM THE JOIN
        // #######################################################################################################
        // #######################################################################################################
        // #######################################################################################################
        InnerJoin_joinKeyColumns_inFuthark(
          ctx,
          &numPairs,
          &joinedKeys, // vs
          &idxR_ft, // R indices
          &idxS_ft, // S indices
          key_type,
          R_curIdx, // R_idx
          S_curIdx, // S_idx
          Rbuff_ft, // R keys
          Sbuff_ft, // S keys
          R_rowCount, // card1
          S_rowCount, // card2
          MERGE_PARTITION_SIZE,
          BLOCK_SIZE // for multi-pass scatter
        );
        mylog(logfile, "Join has been performed.");
        // Gather R's payloads
        char* Rpl_asBytes;
        Rpl_asBytes = malloc(numPairs * R_pL_bytesPerRow);
        gatherPayloads_GFTR(ctx, Rpl_asBytes, R_pL_bytesPerRow, R_curIdx, GATHER_PSIZE, idxR_ft, R_payload_ft, R_rowCount, numPairs);
        mylog(logfile, "Gathered R payloads.");
        void* Rpl[R_col_count-1];
        payloadColumnsFromByteArray(Rpl, R_payloadTypes, Rpl_asBytes, R_col_count-1, numPairs);
        free(Rpl_asBytes);
        mylog(logfile, "Recovered gathered R payloads from byte array.");

        // Gather S's payloads
        char* Spl_asBytes;
        Spl_asBytes = malloc(numPairs * S_pL_bytesPerRow);
        gatherPayloads_GFTR(ctx, Spl_asBytes, S_pL_bytesPerRow, S_curIdx, GATHER_PSIZE, idxS_ft, S_payload_ft, S_rowCount, numPairs);
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

        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_INTEGER:
            futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_BIGINT:
            futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_FLOAT:
            futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_DOUBLE:
            futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Sbuff_ft);
            break;
          default:
            perror("Invalid type!");
            return;
        }
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
      duckdb_destroy_result(&res_Sk);
    }
    duckdb_appender_flush(join_appender);

    switch(key_type) {
      case DUCKDB_TYPE_SMALLINT:
        futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_INTEGER:
        futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_BIGINT:
        futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_FLOAT:
        futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_DOUBLE:
        futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Rbuff_ft);
        break;
      default:
        perror("Invalid type!");
        return;
    }
    futhark_free_u8_2d(ctx, R_payload_ft);

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

void SortMergeJoin_GFUR_with_S_semisorted(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  int radix_sort,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_sorted,
  idx_t S_num_tbls,
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
    true,
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

  mylog(logfile, "Iterating over R...");
  int exhaustedRes_R = false;
  while(!exhaustedRes_R) {
    void* Rbuff;
    void* Rbuff_ft;
    char* R_payload;
    struct futhark_i64_1d* R_idx_ft;
    Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);

    char R_minimum[colType_bytes(key_type)];
    char R_maximum[colType_bytes(key_type)];

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

    // If R is not sorted, sort
    // Either way, maintain as Rbuff_ft
    if(!is_R_sorted) {
      sortKeyColumn_inFuthark(
        ctx,
        &Rbuff_ft,
        key_type,
        0,
        radix_sort, // TODO argument
        BLOCK_SIZE, // TODO
        &R_idx_ft,
        Rbuff,
        R_rowCount,
        R_minimum,
        R_maximum
      );
      mylog(logfile, "Sorted this buffer of R.");
    } else {
      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          Rbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_short(ctx, (int16_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_short(ctx, (int16_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          Rbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_int(ctx, (int32_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_int(ctx, (int32_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          Rbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_long(ctx, (int64_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_long(ctx, (int64_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          Rbuff_ft = futhark_new_f32_1d(ctx, (float*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_float(ctx, (float*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_float(ctx, (float*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          Rbuff_ft = futhark_new_f64_1d(ctx, (double*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_double(ctx, (double*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_double(ctx, (double*)R_maximum, Rbuff_ft);
          break;
        default:
          perror("Invalid type.");
          return;
      }
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

    mylog(logfile, "Iterate over S tables...");
    for (idx_t interm=0; interm<S_num_tbls; interm++) {
      duckdb_result res_Sk;
      char S_this_tbl_name[strlen(S_tbl_name) + 50];
      sprintf(S_this_tbl_name, "%s%ld", S_tbl_name, interm);
      duckdb_state S_query = selective_query(
        key_type,
        con,
        &res_Sk,
        S_this_tbl_name,
        S_keyName,
        R_minimum,
        R_maximum
      );
      if(S_query == DuckDBError) {
        perror("Failed to perform selective query over right-side table.");
        return;
      }
      mylog(logfile, "Obtained S's sorted keys.");

      mylog(logfile, "Now iterating over S for the join...");
      // that is, only when S is sorted
      int exhaustedRes_S = false;
      while(!(exhaustedRes_S)) {
        //printf("Marco!\n");
        void *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
        void *Sbuff_ft;
        char *S_payload;
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

        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            Sbuff_ft = (void*)futhark_new_i16_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_INTEGER:
            Sbuff_ft = (void*)futhark_new_i32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_BIGINT:
            Sbuff_ft = (void*)futhark_new_i64_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_FLOAT:
            Sbuff_ft = (void*)futhark_new_f32_1d(ctx, Sbuff, S_rowCount);
            break;
          case DUCKDB_TYPE_DOUBLE:
            Sbuff_ft = (void*)futhark_new_f64_1d(ctx, Sbuff, S_rowCount);
            break;
          default:
            perror("Invalid type!");
            break;
        }
        idx_t *S_indices = malloc(S_rowCount * sizeof(idx_t));
        for(idx_t i=0; i<S_rowCount; i++) {
          S_indices[i] = i;
        }
        S_idx_ft = futhark_new_i64_1d(ctx, S_indices, S_rowCount);
        free(S_indices);
        futhark_context_sync(ctx);
        free(Sbuff);
        Sbuff = NULL;

        // Perform join
        idx_t numPairs = 0;
        void* joinedKeys;
        struct futhark_i64_1d *idxR_ft;
        struct futhark_i64_1d *idxS_ft;
        // #######################################################################################################
        // #######################################################################################################
        // #######################################################################################################
        // PERFORM THE JOIN
        // #######################################################################################################
        // #######################################################################################################
        // #######################################################################################################
        InnerJoin_joinKeyColumns_inFuthark(
          ctx,
          &numPairs,
          &joinedKeys, // vs
          &idxR_ft, // R indices
          &idxS_ft, // S indices
          key_type,
          R_curIdx, // R_idx
          S_curIdx, // S_idx
          Rbuff_ft, // R keys
          Sbuff_ft, // S keys
          R_rowCount, // card1
          S_rowCount, // card2
          MERGE_PARTITION_SIZE,
          BLOCK_SIZE // for multi-pass scatter
        );
        mylog(logfile, "Join has been performed.");
        // Gather R's payloads
        char* gathered_R_payload = malloc(numPairs * R_pL_bytesPerRow);
        gatherPayloads_GFUR_inFuthark(ctx, gathered_R_payload, R_pL_bytesPerRow, 0, R_curIdx, GATHER_PSIZE, R_idx_ft, idxR_ft, R_payload, R_rowCount, numPairs);
        futhark_free_i64_1d(ctx, idxR_ft); // no longer needed
        mylog(logfile, "Gathered R payloads.");
        void* Rpl[R_col_count-1];
        payloadColumnsFromByteArray(Rpl, R_payloadTypes, gathered_R_payload, R_col_count-1, numPairs);
        free(gathered_R_payload);
        mylog(logfile, "Recovered gathered R payloads from byte array.");

        // Gather S's payloads
        char* gathered_S_payload = malloc(numPairs * S_pL_bytesPerRow);
        gatherPayloads_GFUR_inFuthark(ctx, gathered_S_payload, S_pL_bytesPerRow, 0, S_curIdx, GATHER_PSIZE, S_idx_ft, idxS_ft, S_payload, S_rowCount, numPairs);
        futhark_free_i64_1d(ctx, idxS_ft); // no longer needed
        mylog(logfile, "Gathered R payloads.");
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
        switch(key_type) {
          case DUCKDB_TYPE_SMALLINT:
            futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_INTEGER:
            futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_BIGINT:
            futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_FLOAT:
            futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Sbuff_ft);
            break;
          case DUCKDB_TYPE_DOUBLE:
            futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Sbuff_ft);
            break;
          default:
            perror("Invalid type!");
            return;
        }
        for(idx_t col=1; col<R_col_count; col++) {
          free(Rpl[col-1]);
        }
        for(idx_t col=1; col<S_col_count; col++) {
          free(Spl[col-1]);
        }
        free(joinedKeys);

        S_curIdx += S_rowCount;
      }
      duckdb_destroy_result(&res_Sk);
    }

    duckdb_appender_flush(join_appender);

    switch(key_type) {
      case DUCKDB_TYPE_SMALLINT:
        futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_INTEGER:
        futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_BIGINT:
        futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_FLOAT:
        futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Rbuff_ft);
        break;
      case DUCKDB_TYPE_DOUBLE:
        futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Rbuff_ft);
        break;
      default:
        perror("Invalid type!");
        return;
    }
    futhark_free_i64_1d(ctx, R_idx_ft);
    free(R_payload);

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

// ------------------------------------------------------------------------------------
// WITH BOTH SEMISORTED
// ------------------------------------------------------------------------------------

void MergeJoin_GFTR_semisorted(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  idx_t R_num_tbls,
  idx_t S_num_tbls,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
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
    R_tbl_name,
    S_tbl_name,
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

  mylog(logfile, "Iterating over R's intermediates...");
  
  for(idx_t iR=0; iR<R_num_tbls; iR++) {
    duckdb_result res_Rk;
    char this_R_query[100+strlen(R_tbl_name)];

    sprintf(this_R_query, "SELECT * FROM %s%ld;", R_tbl_name, iR);
    if (duckdb_query(con, this_R_query, &res_Rk) == DuckDBError) {
      perror("Failed to obtain this intermediate from left-side table...");
      return;
    }

    int exhaustedRes_R = false;
    while(!exhaustedRes_R) {
      void* Rbuff;
      void* Rbuff_ft;
      char* R_payload;
      struct futhark_u8_2d* R_payload_ft;
      Rbuff = colType_malloc(key_type, R_JOIN_BUFFER);

      char R_minimum[colType_bytes(key_type)];
      char R_maximum[colType_bytes(key_type)];

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

      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          Rbuff_ft = futhark_new_i16_1d(ctx, (int16_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_short(ctx, (int16_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_short(ctx, (int16_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          Rbuff_ft = futhark_new_i32_1d(ctx, (int32_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_int(ctx, (int32_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_int(ctx, (int32_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          Rbuff_ft = futhark_new_i64_1d(ctx, (int64_t*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_long(ctx, (int64_t*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_long(ctx, (int64_t*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          Rbuff_ft = futhark_new_f32_1d(ctx, (float*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_float(ctx, (float*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_float(ctx, (float*)R_maximum, Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          Rbuff_ft = futhark_new_f64_1d(ctx, (double*)Rbuff, R_rowCount);
          futhark_context_sync(ctx);
          futhark_entry_minimum_double(ctx, (double*)R_minimum, Rbuff_ft);
          futhark_entry_maximum_double(ctx, (double*)R_maximum, Rbuff_ft);
          break;
        default:
          perror("Invalid type.");
          return;
      }
      R_payload_ft = futhark_new_u8_2d(ctx, R_payload, R_rowCount, R_pL_bytesPerRow);

      free(Rbuff);
      Rbuff = NULL;
      free(R_payload);
      R_payload = NULL;

      mylog(logfile, "Iterating over S tables...");
      for (idx_t interm=0; interm<S_num_tbls; interm++) {
        duckdb_result res_Sk;

        char S_this_tbl_name[strlen(S_tbl_name + 50)];
        sprintf(S_this_tbl_name, "%s%ld", S_tbl_name, interm);
        
        duckdb_state S_query = selective_query(
          key_type,
          con,
          &res_Sk,
          S_this_tbl_name,
          S_keyName,
          R_minimum,
          R_maximum
        );
        if(S_query == DuckDBError) {
          perror("Failed to perform selective query over right-side table.");
          return;
        }
        mylog(logfile, "Obtained S's sorted keys.");

        mylog(logfile, "Now iterating over S for the join...");
        // that is, only when S is sorted
        int exhaustedRes_S = false;
        while(!(exhaustedRes_S)) {
          //printf("Marco!\n");
          void *Sbuff = colType_malloc(key_type, S_JOIN_BUFFER);
          void *Sbuff_ft;
          char *S_payload;
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

          
          switch(key_type) {
            case DUCKDB_TYPE_SMALLINT:
              Sbuff_ft = (void*)futhark_new_i16_1d(ctx, Sbuff, S_rowCount);
              break;
            case DUCKDB_TYPE_INTEGER:
              Sbuff_ft = (void*)futhark_new_i32_1d(ctx, Sbuff, S_rowCount);
              break;
            case DUCKDB_TYPE_BIGINT:
              Sbuff_ft = (void*)futhark_new_i64_1d(ctx, Sbuff, S_rowCount);
              break;
            case DUCKDB_TYPE_FLOAT:
              Sbuff_ft = (void*)futhark_new_f32_1d(ctx, Sbuff, S_rowCount);
              break;
            case DUCKDB_TYPE_DOUBLE:
              Sbuff_ft = (void*)futhark_new_f64_1d(ctx, Sbuff, S_rowCount);
              break;
            default:
              perror("Invalid type!");
              break;
          }
            S_payload_ft = futhark_new_u8_2d(ctx, S_payload, S_rowCount, S_pL_bytesPerRow);
            futhark_context_sync(ctx);
          free(Sbuff);
          Sbuff = NULL;
          free(S_payload);
          S_payload = NULL;

          // Perform join
          idx_t numPairs = 0;
          void* joinedKeys;
          struct futhark_i64_1d *idxR_ft;
          struct futhark_i64_1d *idxS_ft;
          // #######################################################################################################
          // #######################################################################################################
          // #######################################################################################################
          // PERFORM THE JOIN
          // #######################################################################################################
          // #######################################################################################################
          // #######################################################################################################
          InnerJoin_joinKeyColumns_inFuthark(
            ctx,
            &numPairs,
            &joinedKeys, // vs
            &idxR_ft, // R indices
            &idxS_ft, // S indices
            key_type,
            R_curIdx, // R_idx
            S_curIdx, // S_idx
            Rbuff_ft, // R keys
            Sbuff_ft, // S keys
            R_rowCount, // card1
            S_rowCount, // card2
            MERGE_PARTITION_SIZE,
            BLOCK_SIZE // for multi-pass scatter
          );
          mylog(logfile, "Join has been performed.");
          // Gather R's payloads
          char* Rpl_asBytes;
          Rpl_asBytes = malloc(numPairs * R_pL_bytesPerRow);
          gatherPayloads_GFTR(ctx, Rpl_asBytes, R_pL_bytesPerRow, R_curIdx, GATHER_PSIZE, idxR_ft, R_payload_ft, R_rowCount, numPairs);
          mylog(logfile, "Gathered R payloads.");
          void* Rpl[R_col_count-1];
          payloadColumnsFromByteArray(Rpl, R_payloadTypes, Rpl_asBytes, R_col_count-1, numPairs);
          free(Rpl_asBytes);
          mylog(logfile, "Recovered gathered R payloads from byte array.");

          // Gather S's payloads
          char* Spl_asBytes;
          Spl_asBytes = malloc(numPairs * S_pL_bytesPerRow);
          gatherPayloads_GFTR(ctx, Spl_asBytes, S_pL_bytesPerRow, S_curIdx, GATHER_PSIZE, idxS_ft, S_payload_ft, S_rowCount, numPairs);
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

          switch(key_type) {
            case DUCKDB_TYPE_SMALLINT:
              futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Sbuff_ft);
              break;
            case DUCKDB_TYPE_INTEGER:
              futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Sbuff_ft);
              break;
            case DUCKDB_TYPE_BIGINT:
              futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Sbuff_ft);
              break;
            case DUCKDB_TYPE_FLOAT:
              futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Sbuff_ft);
              break;
            case DUCKDB_TYPE_DOUBLE:
              futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Sbuff_ft);
              break;
            default:
              perror("Invalid type!");
              return;
          }
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
        duckdb_destroy_result(&res_Sk);
      }
      duckdb_appender_flush(join_appender);

      switch(key_type) {
        case DUCKDB_TYPE_SMALLINT:
          futhark_free_i16_1d(ctx, (struct futhark_i16_1d*)Rbuff_ft);
          break;
        case DUCKDB_TYPE_INTEGER:
          futhark_free_i32_1d(ctx, (struct futhark_i32_1d*)Rbuff_ft);
          break;
        case DUCKDB_TYPE_BIGINT:
          futhark_free_i64_1d(ctx, (struct futhark_i64_1d*)Rbuff_ft);
          break;
        case DUCKDB_TYPE_FLOAT:
          futhark_free_f32_1d(ctx, (struct futhark_f32_1d*)Rbuff_ft);
          break;
        case DUCKDB_TYPE_DOUBLE:
          futhark_free_f64_1d(ctx, (struct futhark_f64_1d*)Rbuff_ft);
          break;
        default:
          perror("Invalid type!");
          return;
      }
      futhark_free_u8_2d(ctx, R_payload_ft);

      R_curIdx += R_rowCount;
    }
    duckdb_destroy_result(&res_Rk);
  }
  
  free(R_type_ids);
  free(S_type_ids);
  free(R_payloadTypes);
  free(S_payloadTypes);
  free(join_type_ids);
  duckdb_appender_flush(join_appender);
  duckdb_appender_destroy(&join_appender);
}

void MergeJoin_GFUR_semisorted(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  idx_t BLOCK_SIZE,
  idx_t MERGE_PARTITION_SIZE,
  idx_t GATHER_PSIZE,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char *R_tbl_name,
  const char *S_tbl_name,
  const char *sorted_R_tbl_name,
  const char *sorted_S_tbl_name,
  idx_t R_num_tbls,
  idx_t S_num_tbls,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_temp_tbl_name,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
) {
  MergeJoin_GFTR_semisorted(
    CHUNK_SIZE,
    R_JOIN_BUFFER,
    S_JOIN_BUFFER,
    BLOCK_SIZE,
    MERGE_PARTITION_SIZE,
    GATHER_PSIZE,
    logfile,
    ctx,
    con,
    sorted_R_tbl_name,
    sorted_S_tbl_name,
    R_num_tbls,
    S_num_tbls,
    R_keyName,
    S_keyName,
    Join_temp_tbl_name,
    quicksaves,
    true
  );
  // TODO omits a few tuples at the end (...)
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
