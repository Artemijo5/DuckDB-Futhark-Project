#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "ftsort.h"
#include <string.h>
#include <limits.h> // for maximum values of integral types
#include <float.h> // for maximum values of floating-point types

size_t colType_bytes(duckdb_type type) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      return sizeof(int16_t);
    case DUCKDB_TYPE_INTEGER:
      return sizeof(int32_t);
    case DUCKDB_TYPE_BIGINT:
      return sizeof(int64_t);
    case DUCKDB_TYPE_FLOAT:
      return sizeof(float);
    case DUCKDB_TYPE_DOUBLE:
      return sizeof(double);
    default:
      perror("Invalid type.");
      return 0;
  }
}

void* colType_malloc(duckdb_type type, idx_t card) {
  size_t ms = colType_bytes(type);
  if (ms == 0) return NULL;
  return malloc(card * ms);
}

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

void sortKeyColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, short* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i16_1d *x_ft = futhark_new_i16_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_short *sortInfo;
  futhark_entry_sortColumn_short(ctx, &sortInfo, (long)incr, block_size, x_ft);
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
void sortKeyColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, int* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i32_1d *x_ft = futhark_new_i32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_int *sortInfo;
  futhark_entry_sortColumn_int(ctx, &sortInfo, (long)incr, block_size, x_ft);
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
void sortKeyColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, long* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_i64_1d *x_ft = futhark_new_i64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_long *sortInfo;
  futhark_entry_sortColumn_long(ctx, &sortInfo, (long)incr, block_size, x_ft);
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
void sortKeyColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, float* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f32_1d *x_ft = futhark_new_f32_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_float *sortInfo;
  futhark_entry_sortColumn_float(ctx, &sortInfo, (long)incr, block_size, x_ft);
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
void sortKeyColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, double* keys, idx_t card) {
  // Wrap x into a futhark array x_ft
  struct futhark_f64_1d *x_ft = futhark_new_f64_1d(ctx, keys, card);
  // Sort x_ft
  struct futhark_opaque_sortInfo_double *sortInfo;
  futhark_entry_sortColumn_double(ctx, &sortInfo, (long)incr, block_size, x_ft);
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
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, const int16_t block_size, struct futhark_i64_1d **outIdx, void* keys, idx_t card) {
  switch (type){
  case DUCKDB_TYPE_SMALLINT:
      sortKeyColumn_short(ctx, (short*)outCol, incr, block_size, outIdx, (short*)keys, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortKeyColumn_int(ctx, (int*)outCol, incr, block_size, outIdx, (int*)keys, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortKeyColumn_long(ctx, (long*)outCol, incr, block_size, outIdx, (long*)keys, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortKeyColumn_float(ctx, (float*)outCol, incr, block_size, outIdx, (float*)keys, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortKeyColumn_double(ctx, (double*)outCol, incr, block_size, outIdx, (double*)keys, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}

void orderPayloadColumn_short(struct futhark_context *ctx, short *outCol, idx_t incr, struct futhark_i64_1d *orderBy, short* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i16_1d *y_ft = futhark_new_i16_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i16_1d *sorted_y_ft;
  futhark_entry_orderByIndices_short(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
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
void orderPayloadColumn_int(struct futhark_context *ctx, int *outCol, idx_t incr, struct futhark_i64_1d *orderBy, int* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i32_1d *y_ft = futhark_new_i32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_int(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
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
void orderPayloadColumn_long(struct futhark_context *ctx, long *outCol, idx_t incr, struct futhark_i64_1d *orderBy, long* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_i64_1d *y_ft = futhark_new_i64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_i64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_long(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
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
void orderPayloadColumn_float(struct futhark_context *ctx, float *outCol, idx_t incr, struct futhark_i64_1d *orderBy, float* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f32_1d *y_ft = futhark_new_f32_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f32_1d *sorted_y_ft;
  futhark_entry_orderByIndices_float(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
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
void orderPayloadColumn_double(struct futhark_context *ctx, double *outCol, idx_t incr, struct futhark_i64_1d *orderBy, double* inCol, idx_t card) {
  // Wrap y into a futhark array y_ft
  struct futhark_f64_1d *y_ft = futhark_new_f64_1d(ctx, inCol, card);
  // Reorder y_ft according to orderBy
  struct futhark_f64_1d *sorted_y_ft;
  futhark_entry_orderByIndices_double(ctx, &sorted_y_ft, (long)incr, orderBy, y_ft);
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
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d *orderBy, void* inCol, idx_t card) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      orderPayloadColumn_short(ctx, (short*)outCol, incr, orderBy, (short*)inCol, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      orderPayloadColumn_int(ctx, (int*)outCol, incr, orderBy, (int*)inCol, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      orderPayloadColumn_long(ctx, (long*)outCol, incr, orderBy, (long*)inCol, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      orderPayloadColumn_float(ctx, (float*)outCol, incr, orderBy, (float*)inCol, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      orderPayloadColumn_double(ctx, (double*)outCol, incr, orderBy, (double*)inCol, card);
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
      queryStr_len += sprintf(queryStr + queryStr_len, "x%ld %s, ", i, type_strs[i]);
    }
    else {
      queryStr_len += sprintf(queryStr + queryStr_len, "x%ld %s);", i, type_strs[i]);
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
    duckdb_appender_flush(tmp_appender);
    duckdb_destroy_data_chunk(&cnk);
  }
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