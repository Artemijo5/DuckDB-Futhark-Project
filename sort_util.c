#include "sort_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftRelational.h"
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

void sortRelationByKey_short(
  struct futhark_context *ctx,
  short *outKeys,
  char *outPayloads,
  int blocked,
  idx_t block_size,
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
  idx_t block_size,
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
  idx_t block_size,
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
  idx_t block_size,
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
  idx_t block_size,
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
  idx_t block_size,
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

void sortKeyColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, short* keys, idx_t card) {
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
void sortKeyColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, int* keys, idx_t card) {
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
void sortKeyColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, long* keys, idx_t card) {
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
void sortKeyColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, float* keys, idx_t card) {
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
void sortKeyColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, double* keys, idx_t card) {
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
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, int blocked, idx_t block_size, struct futhark_i64_1d **outIdx, void* keys, idx_t card) {
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

void orderPayloadColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, short* inCol, idx_t card) {
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
void orderPayloadColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, int* inCol, idx_t card) {
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
void orderPayloadColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, long* inCol, idx_t card) {
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
void orderPayloadColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, float* inCol, idx_t card) {
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
void orderPayloadColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, double* inCol, idx_t card) {
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
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, idx_t block_size, struct futhark_i64_1d *orderBy, void* inCol, idx_t card) {
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
