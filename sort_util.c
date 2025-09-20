#include "sort_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftSMJ.h"
#include <string.h>
#include "db_util.h"

#include <limits.h> // for maximum values of integral types
#include <float.h> // for maximum values of floating-point types

// TODO make this choose between argmin_seq & argmin_par depending on card (?)
idx_t argmin(struct futhark_context *ctx, duckdb_type type, void* arr, idx_t card) {
  idx_t ind = 0;
  switch (type) {
    case DUCKDB_TYPE_SMALLINT:
      struct futhark_i16_1d *arr_short_ft = futhark_new_i16_1d(ctx, arr, card);
      futhark_entry_argmin_short(ctx, &ind, arr_short_ft);
      futhark_context_sync(ctx);
      futhark_free_i16_1d(ctx, arr_short_ft);
      break;
    case DUCKDB_TYPE_INTEGER:
      struct futhark_i32_1d *arr_int_ft = futhark_new_i32_1d(ctx, arr, card);
      futhark_entry_argmin_int(ctx, &ind, arr_int_ft);
      futhark_context_sync(ctx);
      futhark_free_i32_1d(ctx, arr_int_ft);
      break;
    case DUCKDB_TYPE_BIGINT:
      struct futhark_i64_1d *arr_long_ft = futhark_new_i64_1d(ctx, arr, card);
      futhark_entry_argmin_long(ctx, &ind, arr_long_ft);
      futhark_context_sync(ctx);
      futhark_free_i64_1d(ctx, arr_long_ft);
      break;
    case DUCKDB_TYPE_FLOAT:
      struct futhark_f32_1d *arr_float_ft = futhark_new_f32_1d(ctx, arr, card);
      futhark_entry_argmin_float(ctx, &ind, arr_float_ft);
      futhark_context_sync(ctx);
      futhark_free_f32_1d(ctx, arr_float_ft);
      break;
    case DUCKDB_TYPE_DOUBLE:
      struct futhark_f64_1d *arr_double_ft = futhark_new_f64_1d(ctx, arr, card);
      futhark_entry_argmin_double(ctx, &ind, arr_double_ft);
      futhark_context_sync(ctx);
      futhark_free_f64_1d(ctx, arr_double_ft);
      break;
    default:
      perror("Invalid type.");
      return -1;
  }
  return ind;
}

void max_padding(void* dest, duckdb_type type, idx_t n) {
  for(idx_t i=0; i<n; i++) {
    switch(type) {
      case DUCKDB_TYPE_SMALLINT:
        ((short*)dest)[i] = SHRT_MAX;
        break;
      case DUCKDB_TYPE_INTEGER:
        ((int*)dest)[i] = INT_MAX;
        break;
      case DUCKDB_TYPE_BIGINT:
        ((long*)dest)[i] = LONG_MAX;
        break;
      case DUCKDB_TYPE_FLOAT:
        ((float*)dest)[i] = FLT_MAX;
        break;
      case DUCKDB_TYPE_DOUBLE:
        ((double*)dest)[i] = DBL_MAX;
        break;
      default:
        perror("Invalid type.");
        return;
    }
  }
}

void payloadColumnsToByteArray(
  char** outBytes,
  idx_t* pL_rowBytes,
  duckdb_type* payload_types,
  void** inPayloads,
  idx_t pL_col_count,
  idx_t row_count
) {
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[pL_col_count]; // bytes taken by each column
  idx_t pL_prefixSizes[pL_col_count]; // bytes taken up to each column
  for(idx_t col=0; col<pL_col_count; col++) {
    pL_byteSizes[col] = colType_bytes(payload_types[col]);
    pL_prefixSizes[col] = pL_bytes;
    pL_bytes += pL_byteSizes[col];
  }
  // Allocate
  *outBytes = malloc(row_count*pL_bytes);
  // Transform
  for(idx_t r=0; r<row_count; r++) {
    for(idx_t col=0; col<pL_col_count; col++) {
      memcpy(
        (*outBytes) + r*pL_bytes + pL_prefixSizes[col],
        &((char*)inPayloads[col])[r*pL_byteSizes[col]],
        pL_byteSizes[col]
      );
    }
  }
  *pL_rowBytes = pL_bytes;
}

void payloadColumnsFromByteArray(
  void** outPayloads,
  duckdb_type* payload_types,
  char* inBytes,
  idx_t pL_col_count,
  idx_t row_count
) {
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[pL_col_count]; // bytes taken by each column
  idx_t pL_prefixSizes[pL_col_count]; // bytes taken up to each column
  for(idx_t col=0; col<pL_col_count; col++) {
    pL_byteSizes[col] = colType_bytes(payload_types[col]);
    pL_prefixSizes[col] = pL_bytes;
    pL_bytes += pL_byteSizes[col];
  }
  for(idx_t col=0; col<pL_col_count; col++) {
    outPayloads[col] = colType_malloc(payload_types[col], row_count);
  }
  for(idx_t r=0; r<row_count; r++) {
    for(idx_t col=0; col<pL_col_count; col++) {
      memcpy(
        outPayloads[col] + r*pL_byteSizes[col],
        inBytes + r*pL_bytes + pL_prefixSizes[col],
        pL_byteSizes[col]
      );
    }
  }
}

void sortRelationByKey_short(
  struct futhark_context *ctx,
  short *outKeys,
  char *outPayloads,
  int blocked,
  const int16_t block_size,
  short* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap keys & payloads into futhark sortStruct
  struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, (uint8_t*)inPayloads, card, pL_bytesPerRow);
  struct futhark_i16_1d *inKeys_ft = futhark_new_i16_1d(ctx, inKeys, card);
  struct futhark_opaque_sortStruct_short *sortStruct_in;
  futhark_new_opaque_sortStruct_short(ctx, &sortStruct_in, inKeys_ft, inPayloads_ft);
  // Perform the sorting
  struct futhark_opaque_sortStruct_short *sortStruct_out;
  if(blocked) futhark_entry_radixSortRelation_short(ctx, &sortStruct_out, block_size, sortStruct_in);
  else futhark_entry_mergeSortRelation_short(ctx, &sortStruct_out, sortStruct_in);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i16_1d(ctx, inKeys_ft);
  futhark_free_u8_2d(ctx, inPayloads_ft);
  futhark_free_opaque_sortStruct_short(ctx, sortStruct_in);
  // Unwrap sorted output
  struct futhark_i16_1d *outKeys_ft;
  struct futhark_u8_2d *outPayloads_ft;
  futhark_project_opaque_sortStruct_short_k(ctx, &outKeys_ft, sortStruct_out);
  futhark_project_opaque_sortStruct_short_pL(ctx, &outPayloads_ft, sortStruct_out);
  futhark_values_i16_1d(ctx, outKeys_ft, outKeys);
  futhark_values_u8_2d(ctx, outPayloads_ft, outPayloads);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i16_1d(ctx, outKeys_ft);
  futhark_free_u8_2d(ctx, outPayloads_ft);
  futhark_free_opaque_sortStruct_short(ctx, sortStruct_out);
}
void sortRelationByKey_int(
  struct futhark_context *ctx,
  int *outKeys,
  char *outPayloads,
  int blocked,
  const int16_t block_size,
  int* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap keys & payloads into futhark sortStruct
  struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, (uint8_t*)inPayloads, card, pL_bytesPerRow);
  struct futhark_i32_1d *inKeys_ft = futhark_new_i32_1d(ctx, inKeys, card);
  struct futhark_opaque_sortStruct_int *sortStruct_in;
  futhark_new_opaque_sortStruct_int(ctx, &sortStruct_in, inKeys_ft, inPayloads_ft);
  // Perform the sorting
  struct futhark_opaque_sortStruct_int *sortStruct_out;
  if(blocked) futhark_entry_radixSortRelation_int(ctx, &sortStruct_out, block_size, sortStruct_in);
  else futhark_entry_mergeSortRelation_int(ctx, &sortStruct_out, sortStruct_in);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i32_1d(ctx, inKeys_ft);
  futhark_free_u8_2d(ctx, inPayloads_ft);
  futhark_free_opaque_sortStruct_int(ctx, sortStruct_in);
  // Unwrap sorted output
  struct futhark_i32_1d *outKeys_ft;
  struct futhark_u8_2d *outPayloads_ft;
  futhark_project_opaque_sortStruct_int_k(ctx, &outKeys_ft, sortStruct_out);
  futhark_project_opaque_sortStruct_int_pL(ctx, &outPayloads_ft, sortStruct_out);
  futhark_values_i32_1d(ctx, outKeys_ft, outKeys);
  futhark_values_u8_2d(ctx, outPayloads_ft, outPayloads);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i32_1d(ctx, outKeys_ft);
  futhark_free_u8_2d(ctx, outPayloads_ft);
  futhark_free_opaque_sortStruct_int(ctx, sortStruct_out);
}
void sortRelationByKey_long(
  struct futhark_context *ctx,
  long *outKeys,
  char *outPayloads,
  int blocked,
  const int16_t block_size,
  long* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap keys & payloads into futhark sortStruct
  struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, (uint8_t*)inPayloads, card, pL_bytesPerRow);
  struct futhark_i64_1d *inKeys_ft = futhark_new_i64_1d(ctx, inKeys, card);
  struct futhark_opaque_sortStruct_long *sortStruct_in;
  futhark_new_opaque_sortStruct_long(ctx, &sortStruct_in, inKeys_ft, inPayloads_ft);
  // Perform the sorting
  struct futhark_opaque_sortStruct_long *sortStruct_out;
  if(blocked) futhark_entry_radixSortRelation_long(ctx, &sortStruct_out, block_size, sortStruct_in);
  else futhark_entry_mergeSortRelation_long(ctx, &sortStruct_out, sortStruct_in);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i64_1d(ctx, inKeys_ft);
  futhark_free_u8_2d(ctx, inPayloads_ft);
  futhark_free_opaque_sortStruct_long(ctx, sortStruct_in);
  // Unwrap sorted output
  struct futhark_i64_1d *outKeys_ft;
  struct futhark_u8_2d *outPayloads_ft;
  futhark_project_opaque_sortStruct_long_k(ctx, &outKeys_ft, sortStruct_out);
  futhark_project_opaque_sortStruct_long_pL(ctx, &outPayloads_ft, sortStruct_out);
  futhark_values_i64_1d(ctx, outKeys_ft, outKeys);
  futhark_values_u8_2d(ctx, outPayloads_ft, outPayloads);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_i64_1d(ctx, outKeys_ft);
  futhark_free_u8_2d(ctx, outPayloads_ft);
  futhark_free_opaque_sortStruct_long(ctx, sortStruct_out);
}
void sortRelationByKey_float(
  struct futhark_context *ctx,
  float *outKeys,
  char *outPayloads,
  int blocked,
  const int16_t block_size,
  float* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap keys & payloads into futhark sortStruct
  struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, (uint8_t*)inPayloads, card, pL_bytesPerRow);
  struct futhark_f32_1d *inKeys_ft = futhark_new_f32_1d(ctx, inKeys, card);
  struct futhark_opaque_sortStruct_float *sortStruct_in;
  futhark_new_opaque_sortStruct_float(ctx, &sortStruct_in, inKeys_ft, inPayloads_ft);
  // Perform the sorting
  struct futhark_opaque_sortStruct_float *sortStruct_out;
  if(blocked) futhark_entry_radixSortRelation_float(ctx, &sortStruct_out, block_size, sortStruct_in);
  else futhark_entry_mergeSortRelation_float(ctx, &sortStruct_out, sortStruct_in);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_f32_1d(ctx, inKeys_ft);
  futhark_free_u8_2d(ctx, inPayloads_ft);
  futhark_free_opaque_sortStruct_float(ctx, sortStruct_in);
  // Unwrap sorted output
  struct futhark_f32_1d *outKeys_ft;
  struct futhark_u8_2d *outPayloads_ft;
  futhark_project_opaque_sortStruct_float_k(ctx, &outKeys_ft, sortStruct_out);
  futhark_project_opaque_sortStruct_float_pL(ctx, &outPayloads_ft, sortStruct_out);
  futhark_values_f32_1d(ctx, outKeys_ft, outKeys);
  futhark_values_u8_2d(ctx, outPayloads_ft, outPayloads);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_f32_1d(ctx, outKeys_ft);
  futhark_free_u8_2d(ctx, outPayloads_ft);
  futhark_free_opaque_sortStruct_float(ctx, sortStruct_out);
}
void sortRelationByKey_double(
  struct futhark_context *ctx,
  double *outKeys,
  char *outPayloads,
  int blocked,
  const int16_t block_size,
  double* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap keys & payloads into futhark sortStruct
  struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, (uint8_t*)inPayloads, card, pL_bytesPerRow);
  struct futhark_f64_1d *inKeys_ft = futhark_new_f64_1d(ctx, inKeys, card);
  struct futhark_opaque_sortStruct_double *sortStruct_in;
  futhark_new_opaque_sortStruct_double(ctx, &sortStruct_in, inKeys_ft, inPayloads_ft);
  // Perform the sorting
  struct futhark_opaque_sortStruct_double *sortStruct_out;
  if(blocked) futhark_entry_radixSortRelation_double(ctx, &sortStruct_out, block_size, sortStruct_in);
  else futhark_entry_mergeSortRelation_double(ctx, &sortStruct_out, sortStruct_in);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_f64_1d(ctx, inKeys_ft);
  futhark_free_u8_2d(ctx, inPayloads_ft);
  futhark_free_opaque_sortStruct_double(ctx, sortStruct_in);
  // Unwrap sorted output
  struct futhark_f64_1d *outKeys_ft;
  struct futhark_u8_2d *outPayloads_ft;
  futhark_project_opaque_sortStruct_double_k(ctx, &outKeys_ft, sortStruct_out);
  futhark_project_opaque_sortStruct_double_pL(ctx, &outPayloads_ft, sortStruct_out);
  futhark_values_f64_1d(ctx, outKeys_ft, outKeys);
  futhark_values_u8_2d(ctx, outPayloads_ft, outPayloads);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_f64_1d(ctx, outKeys_ft);
  futhark_free_u8_2d(ctx, outPayloads_ft);
  futhark_free_opaque_sortStruct_double(ctx, sortStruct_out);
}
void sortRelationByKey(
  struct futhark_context *ctx,
  void *outKeys,
  char *outPayloads,
  duckdb_type key_type,
  int blocked,
  const int16_t block_size,
  void* inKeys,
  char* inPayloads,
  idx_t pL_bytesPerRow,
  idx_t card
) {
  // Wrap payloads and keys
  switch(key_type) {
    case DUCKDB_TYPE_SMALLINT:
      sortRelationByKey_short(ctx, (short*)outKeys, outPayloads, blocked, block_size,
        (short*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortRelationByKey_int(ctx, (int*)outKeys, outPayloads, blocked, block_size,
        (int*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortRelationByKey_long(ctx, (long*)outKeys, outPayloads, blocked, block_size,
        (long*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortRelationByKey_float(ctx, (float*)outKeys, outPayloads, blocked, block_size,
        (float*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortRelationByKey_double(ctx, (double*)outKeys, outPayloads, blocked, block_size,
        (double*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    default:
      perror("sortRelationByKey: Invalid Type.");
      return;
  }
}

void sortKeyColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, short* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i16_1d *x_ft = futhark_new_i16_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_short *sortInfo;
  if (blocked) {
    futhark_entry_radixSortColumn_short(ctx, &sortInfo, (long)incr, block_size, x_ft);
  }
  else {
    futhark_entry_mergeSortColumn_short(ctx, &sortInfo, (long)incr, x_ft);
  }
  // Decouple indices from keys
  struct futhark_i16_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_short_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_short_xs(ctx, &sorted_x_ft, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap sorted keys into outCol
  futhark_values_i16_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i16_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_short(ctx, sortInfo);
  futhark_free_i16_1d(ctx, x_ft);
}
void sortKeyColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, int* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i32_1d *x_ft = futhark_new_i32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_int *sortInfo;
  if (blocked) {
    futhark_entry_radixSortColumn_int(ctx, &sortInfo, (long)incr, block_size, x_ft);
  }
  else {
    futhark_entry_mergeSortColumn_int(ctx, &sortInfo, (long)incr, x_ft);
  }
  // Decouple indices from keys
  struct futhark_i32_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_int_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_int_xs(ctx, &sorted_x_ft, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap sorted keys into outCol
  futhark_values_i32_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i32_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_int(ctx, sortInfo);
  futhark_free_i32_1d(ctx, x_ft);
}
void sortKeyColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, long* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i64_1d *x_ft = futhark_new_i64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_long *sortInfo;
  if (blocked) {
    futhark_entry_radixSortColumn_long(ctx, &sortInfo, (long)incr, block_size, x_ft);
  }
  else {
    futhark_entry_mergeSortColumn_long(ctx, &sortInfo, (long)incr, x_ft);
  }
  // Decouple indices from keys
  struct futhark_i64_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_long_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_long_xs(ctx, &sorted_x_ft, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap sorted keys into outCol
  futhark_values_i64_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_i64_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_long(ctx, sortInfo);
  futhark_free_i64_1d(ctx, x_ft);
}
void sortKeyColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, float* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f32_1d *x_ft = futhark_new_f32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_float *sortInfo;
  if (blocked) {
    futhark_entry_radixSortColumn_float(ctx, &sortInfo, (long)incr, block_size, x_ft);
  }
  else {
    futhark_entry_mergeSortColumn_float(ctx, &sortInfo, (long)incr, x_ft);
  }
  // Decouple indices from keys
  struct futhark_f32_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_float_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_float_xs(ctx, &sorted_x_ft, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap sorted keys into outCol
  futhark_values_f32_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_f32_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_float(ctx, sortInfo);
  futhark_free_f32_1d(ctx, x_ft);
}
void sortKeyColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, double* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f64_1d *x_ft = futhark_new_f64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_double *sortInfo;
  if (blocked) {
    futhark_entry_radixSortColumn_double(ctx, &sortInfo, (long)incr, block_size, x_ft);
  }
  else {
    futhark_entry_mergeSortColumn_double(ctx, &sortInfo, (long)incr, x_ft);
  }
  // Decouple indices from keys
  struct futhark_f64_1d *sorted_x_ft;
  futhark_project_opaque_sortInfo_double_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_double_xs(ctx, &sorted_x_ft, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap sorted keys into outCol
  futhark_values_f64_1d(ctx, sorted_x_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_f64_1d(ctx, sorted_x_ft);
  futhark_free_opaque_sortInfo_double(ctx, sortInfo);
  futhark_free_f64_1d(ctx, x_ft);
}
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, void* keys, idx_t card) {
  switch (type){
  case DUCKDB_TYPE_SMALLINT:
      sortKeyColumn_short(ctx, (short*)outCol, incr, blocked, block_size, outIdx, (short*)keys, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortKeyColumn_int(ctx, (int*)outCol, incr, blocked, block_size, outIdx, (int*)keys, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortKeyColumn_long(ctx, (long*)outCol, incr, blocked, block_size, outIdx, (long*)keys, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortKeyColumn_float(ctx, (float*)outCol, incr, blocked, block_size, outIdx, (float*)keys, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortKeyColumn_double(ctx, (double*)outCol, incr, blocked, block_size, outIdx, (double*)keys, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}

void orderPayloadColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, short* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i16_1d *y_ft = futhark_new_i16_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i16_1d *sorted_y_ft;
  futhark_entry_orderByIndices_short(ctx, &sorted_y_ft, (long)incr, block_size, orderBy, y_ft);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap reordered payload into outCol
  futhark_values_i16_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i16_1d(ctx, sorted_y_ft);
  futhark_free_i16_1d(ctx, y_ft);
}
void orderPayloadColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, int* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i32_1d *y_ft = futhark_new_i32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_int(ctx, &sorted_y_ft, (long)incr, block_size, orderBy, y_ft);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap reordered payload into outCol
  futhark_values_i32_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i32_1d(ctx, sorted_y_ft);
  futhark_free_i32_1d(ctx, y_ft);
}
void orderPayloadColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, long* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i64_1d *y_ft = futhark_new_i64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_long(ctx, &sorted_y_ft, (long)incr, block_size, orderBy, y_ft);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap reordered payload into outCol
  futhark_values_i64_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i64_1d(ctx, sorted_y_ft);
  futhark_free_i64_1d(ctx, y_ft);
}
void orderPayloadColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, float* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f32_1d *y_ft = futhark_new_f32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_float(ctx, &sorted_y_ft, (long)incr, block_size, orderBy, y_ft);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap reordered payload into outCol
  futhark_values_f32_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f32_1d(ctx, sorted_y_ft);
  futhark_free_f32_1d(ctx, y_ft);
}
void orderPayloadColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, double* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f64_1d *y_ft = futhark_new_f64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_double(ctx, &sorted_y_ft, (long)incr, block_size, orderBy, y_ft);
  // Sync context
  futhark_context_sync(ctx);
  // Unwrap reordered payload into outCol
  futhark_values_f64_1d(ctx, sorted_y_ft, outCol);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f64_1d(ctx, sorted_y_ft);
  futhark_free_f64_1d(ctx, y_ft);
}
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, const int16_t block_size, struct futhark_i64_1d *orderBy, void* inCol, idx_t card) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      orderPayloadColumn_short(ctx, (short*)outCol, incr, block_size, orderBy, (short*)inCol, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      orderPayloadColumn_int(ctx, (int*)outCol, incr, block_size, orderBy, (int*)inCol, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      orderPayloadColumn_long(ctx, (long*)outCol, incr, block_size, orderBy, (long*)inCol, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      orderPayloadColumn_float(ctx, (float*)outCol, incr, block_size, orderBy, (float*)inCol, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      orderPayloadColumn_double(ctx, (double*)outCol, incr, block_size, orderBy, (double*)inCol, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}

idx_t store_intermediate(
  idx_t numInter,
  const char *intermName,
  duckdb_connection con,
  idx_t chunkSize,
  idx_t col_count,
  idx_t row_count,
  duckdb_type* types,
  char** colNames,
  void** BuffersIn
) {
  // 0 turn types into logical_types & strings to create the table
  duckdb_logical_type ltypes[col_count];
  char type_strs[col_count][25];
  for(idx_t i=0; i<col_count; i++) {
    ltypes[i] = duckdb_create_logical_type(types[i]);
    switch(types[i]){
      case DUCKDB_TYPE_SMALLINT:
        sprintf( type_strs[i], "SMALLINT" );
        break;
      case DUCKDB_TYPE_INTEGER:
        sprintf( type_strs[i], "INTEGER" );
        break;
      case DUCKDB_TYPE_BIGINT:
        sprintf( type_strs[i], "BIGINT" );
        break;
      case DUCKDB_TYPE_FLOAT:
        sprintf( type_strs[i], "FLOAT" );
        break;
      case DUCKDB_TYPE_DOUBLE:
        sprintf( type_strs[i], "DOUBLE" );
        break;
      default:
        perror("Invalid type.");
        return -1;
    }
  }

  // 1 create temporary table
  char tblName[strlen(intermName) + 25];
  sprintf(tblName, "%s%ld", intermName, numInter);
  char queryStr[100 + 35*col_count];
  int queryStr_len = sprintf(queryStr, "CREATE OR REPLACE TEMP TABLE %s (", tblName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s, ", colNames[i], type_strs[i]);
    }
    else {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s);", colNames[i], type_strs[i]);
    }
  }
  // TODO for testing
  //printf("%s\n", queryStr);
  if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
    perror("Failed to create temporary table.\n");
    return -1;
  }
  // TODO for testing
  //printf("Created temporary table #%ld.\n", numInter);

  // 2 create an appender for the table
  duckdb_appender tmp_appender;
  if( duckdb_appender_create(con, NULL, tblName, &tmp_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return -1;
  }
  //printf("Created appender #%ld.\n", numInter);

  // 3 insert the data into the table, 1 chunk at a time
  for(idx_t r=0; r<row_count; r+=chunkSize) {
    size_t this_size = (row_count-r > chunkSize)? chunkSize: row_count-r;
    duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, col_count);
    duckdb_data_chunk_set_size(cnk, this_size);
    for(idx_t c=0; c<col_count; c++) {
      duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
      void *dat = (void*)duckdb_vector_get_data(vec);
      memcpy(dat, &((char*)BuffersIn[c])[r*colType_bytes(types[c])], this_size*colType_bytes(types[c]));
    }
    if(duckdb_append_data_chunk(tmp_appender, cnk) == DuckDBError) {
      perror("Failed to append data chunk.\n");
      return -1;
    }
    //printf("Appended %ld elements.\n", this_size);
    //duckdb_appender_flush(tmp_appender);
    duckdb_destroy_data_chunk(&cnk);
  }
  duckdb_appender_flush(tmp_appender);
  // IF USING PARQUET STORAGE:
  /*
  char storagePart[100 + 2*strlen(tblName)];
  sprintf(storagePart, "COPY %s TO %s.parquet (FORMAT parquet);", tblName, tblName);
  char clearPart[100];
  sprintf(clearPart, "DROP TABLE %s;", tblName);
  if(duckdb_query(con, storagePart, NULL) == DuckDBError) {
    perror("Failed to spill intermediate result to disk.");
    return -1;
  }
  if(duckdb_query(con, clearPart, NULL) == DuckDBError) {
    perror("Failed to drop temp table.");
    return -1;
  }
  */

  // Cleanup
  duckdb_appender_destroy(&tmp_appender);
  for(int i=0; i<col_count; i++) {
    duckdb_destroy_logical_type(&ltypes[i]);
  }

  return numInter + 1;
}

void prepareToFetch_intermediate(
  idx_t numInter,
  const char *intermName,
  duckdb_connection con,
  duckdb_result *result_ptr
) {
  char tblName[strlen(intermName) + 25];
  // IF _NOT_ USING PARQUET STORAGE:
  sprintf(tblName, "%s%ld", intermName, numInter);
  // IF USING PARQUET STORAGE:
  //sprintf(tblName, "tempholder%ld.parquet", numInter);
  char queryStr[100];
  sprintf(queryStr, "SELECT * FROM %s;", tblName);
  // TODO for testing
  //printf("%s\n", queryStr);
  if( duckdb_query(con, queryStr, result_ptr) == DuckDBError ) {
    perror("Failed to retrieve intermediate.");
  }
  // TODO better error handling
}

idx_t fetch_intermediate(
  duckdb_result result,
  idx_t col_count,
  duckdb_type* types,
  void** BuffersOut,
  idx_t start_idx
) {
  duckdb_data_chunk cnk = duckdb_fetch_chunk(result);
  if(!cnk) {
    // Result is exhausted.
    return 0;
  }
  idx_t row_count = duckdb_data_chunk_get_size(cnk);

  for(idx_t c=0; c<col_count; c++) {
    duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
    void* dat = duckdb_vector_get_data(vec);
    size_t colSize = colType_bytes(types[c]);
    char *curBuffer = (char*)BuffersOut[c];
    memcpy(curBuffer + start_idx*colSize, dat, row_count*colSize);
  }
  duckdb_destroy_data_chunk(&cnk);
  return row_count;
}


// -------------------------------------------------------------------------------------- MERGING FUNCTIONS
/*
void mergeSortedKeys_short(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  short *outValues,
  idx_t threads,
  idx_t window_size,
  short *As,
  short *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  // Wrap arrays into futhark arays.
  struct futhark_i16_1d *As_ft = futhark_new_i16_1d(ctx, As, card_As);
  struct futhark_i16_1d *Bs_ft = futhark_new_i16_1d(ctx, Bs, card_Bs);
  // Perform the merging.
  struct futhark_opaque_mergeInfo_short *mergeInfo;
  futhark_entry_mergeSorted_short(ctx, &mergeInfo, window_size, threads, As_ft, Bs_ft, inParallel);
  // Sync context
  futhark_context_sync(ctx);
  // Output relation info, indices, and merged values
  futhark_project_opaque_mergeInfo_short_rs(ctx, outRelation, mergeInfo);
  futhark_project_opaque_mergeInfo_short_is(ctx, outIdx, mergeInfo);
  struct futhark_i16_1d *M_ft;
  futhark_project_opaque_mergeInfo_short_vs(ctx, &M_ft, mergeInfo);
  futhark_values_i16_1d(ctx, M_ft, outValues);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i16_1d(ctx, As_ft);
  futhark_free_i16_1d(ctx, Bs_ft);
  futhark_free_i16_1d(ctx, M_ft);
  futhark_free_opaque_mergeInfo_short(ctx, mergeInfo);
}
void mergeSortedKeys_int(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  int *outValues,
  idx_t threads,
  idx_t window_size,
  int *As,
  int *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  // Wrap arrays into futhark arays.
  struct futhark_i32_1d *As_ft = futhark_new_i32_1d(ctx, As, card_As);
  struct futhark_i32_1d *Bs_ft = futhark_new_i32_1d(ctx, Bs, card_Bs);
  // Perform the merging.
  struct futhark_opaque_mergeInfo_int *mergeInfo;
  futhark_entry_mergeSorted_int(ctx, &mergeInfo, window_size, threads, As_ft, Bs_ft, inParallel);
  // Sync context
  futhark_context_sync(ctx);
  // Output relation info, indices, and merged values
  futhark_project_opaque_mergeInfo_int_rs(ctx, outRelation, mergeInfo);
  futhark_project_opaque_mergeInfo_int_is(ctx, outIdx, mergeInfo);
  struct futhark_i32_1d *M_ft;
  futhark_project_opaque_mergeInfo_int_vs(ctx, &M_ft, mergeInfo);
  futhark_values_i32_1d(ctx, M_ft, outValues);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i32_1d(ctx, As_ft);
  futhark_free_i32_1d(ctx, Bs_ft);
  futhark_free_i32_1d(ctx, M_ft);
  futhark_free_opaque_mergeInfo_int(ctx, mergeInfo);
}
void mergeSortedKeys_long(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  long *outValues,
  idx_t threads,
  idx_t window_size,
  long *As,
  long *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  // Wrap arrays into futhark arays.
  struct futhark_i64_1d *As_ft = futhark_new_i64_1d(ctx, As, card_As);
  struct futhark_i64_1d *Bs_ft = futhark_new_i64_1d(ctx, Bs, card_Bs);
  // Perform the merging.
  struct futhark_opaque_mergeInfo_long *mergeInfo;
  futhark_entry_mergeSorted_long(ctx, &mergeInfo, window_size, threads, As_ft, Bs_ft, inParallel);
  // Sync context
  futhark_context_sync(ctx);
  // Output relation info, indices, and merged values
  futhark_project_opaque_mergeInfo_long_rs(ctx, outRelation, mergeInfo);
  futhark_project_opaque_mergeInfo_long_is(ctx, outIdx, mergeInfo);
  struct futhark_i64_1d *M_ft;
  futhark_project_opaque_mergeInfo_long_vs(ctx, &M_ft, mergeInfo);
  futhark_values_i64_1d(ctx, M_ft, outValues);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_i64_1d(ctx, As_ft);
  futhark_free_i64_1d(ctx, Bs_ft);
  futhark_free_i64_1d(ctx, M_ft);
  futhark_free_opaque_mergeInfo_long(ctx, mergeInfo);
}
void mergeSortedKeys_float(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  float *outValues,
  idx_t threads,
  idx_t window_size,
  float *As,
  float *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  // Wrap arrays into futhark arays.
  struct futhark_f32_1d *As_ft = futhark_new_f32_1d(ctx, As, card_As);
  struct futhark_f32_1d *Bs_ft = futhark_new_f32_1d(ctx, Bs, card_Bs);
  // Perform the merging.
  struct futhark_opaque_mergeInfo_float *mergeInfo;
  futhark_entry_mergeSorted_float(ctx, &mergeInfo, window_size, threads, As_ft, Bs_ft, inParallel);
  // Sync context
  futhark_context_sync(ctx);
  // Output relation info, indices, and merged values
  futhark_project_opaque_mergeInfo_float_rs(ctx, outRelation, mergeInfo);
  futhark_project_opaque_mergeInfo_float_is(ctx, outIdx, mergeInfo);
  struct futhark_f32_1d *M_ft;
  futhark_project_opaque_mergeInfo_float_vs(ctx, &M_ft, mergeInfo);
  futhark_values_f32_1d(ctx, M_ft, outValues);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f32_1d(ctx, As_ft);
  futhark_free_f32_1d(ctx, Bs_ft);
  futhark_free_f32_1d(ctx, M_ft);
  futhark_free_opaque_mergeInfo_float(ctx, mergeInfo);
}
void mergeSortedKeys_double(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  double *outValues,
  idx_t threads,
  idx_t window_size,
  double *As,
  double *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  // Wrap arrays into futhark arays.
  struct futhark_f64_1d *As_ft = futhark_new_f64_1d(ctx, As, card_As);
  struct futhark_f64_1d *Bs_ft = futhark_new_f64_1d(ctx, Bs, card_Bs);
  // Perform the merging.
  struct futhark_opaque_mergeInfo_double *mergeInfo;
  futhark_entry_mergeSorted_double(ctx, &mergeInfo, window_size, threads, As_ft, Bs_ft, inParallel);
  // Sync context
  futhark_context_sync(ctx);
  // Output relation info, indices, and merged values
  futhark_project_opaque_mergeInfo_double_rs(ctx, outRelation, mergeInfo);
  futhark_project_opaque_mergeInfo_double_is(ctx, outIdx, mergeInfo);
  struct futhark_f64_1d *M_ft;
  futhark_project_opaque_mergeInfo_double_vs(ctx, &M_ft, mergeInfo);
  futhark_values_f64_1d(ctx, M_ft, outValues);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_f64_1d(ctx, As_ft);
  futhark_free_f64_1d(ctx, Bs_ft);
  futhark_free_f64_1d(ctx, M_ft);
  futhark_free_opaque_mergeInfo_double(ctx, mergeInfo);
}
void mergeSortedKeys(
  struct futhark_context *ctx,
  struct futhark_i8_1d **outRelation,
  struct futhark_i64_1d **outIdx,
  void *outValues,
  duckdb_type type,
  idx_t threads,
  idx_t window_size,
  void *As,
  void *Bs,
  idx_t card_As,
  idx_t card_Bs,
  int inParallel
) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      mergeSortedKeys_short(
        ctx, outRelation, outIdx, (short*)outValues, threads, window_size, (short*)As, (short*)Bs, card_As, card_Bs, inParallel
      );
      return;
    case DUCKDB_TYPE_INTEGER:
      mergeSortedKeys_int(
        ctx, outRelation, outIdx, (int*)outValues, threads, window_size, (int*)As, (int*)Bs, card_As, card_Bs, inParallel
      );
      return;
    case DUCKDB_TYPE_BIGINT:
      mergeSortedKeys_long(
        ctx, outRelation, outIdx, (long*)outValues, threads, window_size, (long*)As, (long*)Bs, card_As, card_Bs, inParallel
      );
      return;
    case DUCKDB_TYPE_FLOAT:
      mergeSortedKeys_float(
        ctx, outRelation, outIdx, (float*)outValues, threads, window_size, (float*)As, (float*)Bs, card_As, card_Bs, inParallel
      );
      return;
    case DUCKDB_TYPE_DOUBLE:
      mergeSortedKeys_double(
        ctx, outRelation, outIdx, (double*)outValues, threads, window_size, (double*)As, (double*)Bs, card_As, card_Bs, inParallel
      );
      return;
    default:
      perror("Invalid type.");
      return;
  }
}
*/
