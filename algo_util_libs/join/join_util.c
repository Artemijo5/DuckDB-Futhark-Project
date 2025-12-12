#include "join_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "db_util.h"
#include <string.h>

#include "ftRelational.h"

int join_preparation(
	FILE *logfile,
	duckdb_connection con,
	const char* R_tbl_name,
	const char* S_tbl_name,
	const char* Join_tbl_name,
	int is_R_semisorted,
	int is_S_semisorted,
	const char* R_keyName,
	const char* S_keyName,
	duckdb_type *key_type,
	duckdb_result *out_res_R,
	duckdb_appender *join_appender,
	idx_t *R_keyCol_Idx,
	idx_t *S_keyCol_Idx,
	idx_t *R_col_count,
	idx_t *S_col_count,
	idx_t *R_pL_bytesPerRow,
	idx_t *S_pL_bytesPerRow,
	duckdb_type **R_type_ids,
	duckdb_type **S_type_ids,
	duckdb_type **R_payloadTypes,
	duckdb_type **S_payloadTypes,
	duckdb_logical_type **join_type_ids,
	int is_in_GFUR,
	int saveAsTempTable
) {
  // Read R's sorted keys
  mylog(logfile, "Preparing for join - obtain R's sorted keys...");
  duckdb_result res_Rk;
  char readRq[100 + strlen(R_tbl_name)];
  if(is_R_semisorted) {
  	sprintf(readRq, "SELECT * FROM %s0 LIMIT 0;", R_tbl_name);
  }
  else {
  	sprintf(readRq, "SELECT * FROM %s;", R_tbl_name);
  }
  if( duckdb_query(con, readRq, &res_Rk) == DuckDBError) {
    perror("Failed to read R_tbl_sorted...\n");
    return -1;
  }
  mylog(logfile, "Obtained R result.");
  // result info
  *R_keyCol_Idx = -1;
  *R_col_count = duckdb_column_count(&res_Rk);
  *R_type_ids = malloc(*R_col_count * sizeof(duckdb_type));
  for(idx_t col=0; col<*R_col_count; col++) {
    (*R_type_ids)[col] = duckdb_column_type(&res_Rk, col);
    if(strcmp(R_keyName, duckdb_column_name(&res_Rk, col)) == 0) *R_keyCol_Idx = col;
  }
  mylog(logfile, "Obtained R's column & type info.");
  // Key type
  if(*R_keyCol_Idx<0) {
    mylog(logfile, "Invalid column for left-side table.");
    duckdb_destroy_result(&res_Rk);
    return -1;
  }
  if(!is_R_semisorted && !is_in_GFUR) *out_res_R = res_Rk;
  *key_type = (*R_type_ids)[*R_keyCol_Idx];
  // ALSO OBTAIN S INFO
  duckdb_result S_dummyRes;
  char dummySq[100 + strlen(S_tbl_name)];

  if(is_S_semisorted) {
  	sprintf(dummySq, "SELECT * FROM %s0 LIMIT 0;", S_tbl_name);
  }
  else {
  	sprintf(dummySq, "SELECT * FROM %s LIMIT 0;", S_tbl_name);
  }

  if( duckdb_query(con, dummySq, &S_dummyRes) == DuckDBError) {
    perror("Failed to obtain info for S table...\n");
    duckdb_destroy_result(&res_Rk);
    return -1;
  }
  *S_keyCol_Idx = -1;
  *S_col_count = duckdb_column_count(&S_dummyRes);
  *S_type_ids = malloc(*S_col_count * sizeof(duckdb_type));
  for(idx_t col=0; col<*S_col_count; col++) {
    (*S_type_ids)[col] = duckdb_column_type(&S_dummyRes, col);
    if(strcmp(S_keyName, duckdb_column_name(&S_dummyRes, col)) == 0) *S_keyCol_Idx = col;
  }
  mylog(logfile, "Obtained S's column & type info.");
  if(*S_keyCol_Idx<0) {
    mylog(logfile, "Invalid column for right-side table.");
    duckdb_destroy_result(&res_Rk);
    duckdb_destroy_result(&S_dummyRes);
    return -1;
  }
  if((*S_type_ids)[*S_keyCol_Idx] != *key_type) {
    mylog(logfile, "Key type mismatch!!!!!");
    duckdb_destroy_result(&res_Rk);
    duckdb_destroy_result(&S_dummyRes);
    return -1;
  }

  // ##### 0 --- Create Join Table & Appender

  // 0.0 create strings for types
  char R_type_strs[*R_col_count][25];
  for(idx_t col=0; col<*R_col_count; col++) {
    colType_name((*R_type_ids)[col], R_type_strs[col]);
  }
  char S_type_strs[*S_col_count][25];
  for(idx_t col=0; col<*S_col_count; col++) {
    colType_name((*S_type_ids)[col], S_type_strs[col]);
  }

  // Create the Table
  char joinTbl_init_query[250 + strlen(Join_tbl_name) + 30 + (*R_col_count-1)*35 + (*S_col_count-1)*35];
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
  for(idx_t col=1; col<*R_col_count; col++) {
    joinTbl_strLen += sprintf(joinTbl_init_query + joinTbl_strLen, ", %s %s",
      duckdb_column_name(&res_Rk, col), R_type_strs[col]);
  }
  for(idx_t col=0; col<*S_col_count; col++) {
    if(col==*S_keyCol_Idx) continue; // payloads only
    // if R has a column with the same name, append _1 to the end
    char *colName = malloc(7 + strlen(duckdb_column_name(&S_dummyRes, col)));
    int cnlen = sprintf(colName, "%s", duckdb_column_name(&S_dummyRes, col));
    idx_t appendNum = 1;
    while(true) {
      int R_has_same = false;
      for(idx_t rcol=0; rcol<*R_col_count; rcol++) {
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
    return -1;
  }
  mylog(logfile, "Created result table where join pairs will be stored.");

  if (is_R_semisorted) duckdb_destroy_result(&res_Rk);
  duckdb_destroy_result(&S_dummyRes);

  // Create composite logical_type id info
  *join_type_ids = malloc((*R_col_count + *S_col_count - 1)*sizeof(duckdb_logical_type));
  for(idx_t col=0; col<*R_col_count; col++) {
    (*join_type_ids)[col] = duckdb_create_logical_type((*R_type_ids)[col]);
  }
  for(idx_t col=0; col<*S_col_count; col++) {
    if(col==*S_keyCol_Idx) continue; // payloads only
    idx_t accIdx = (col<*S_keyCol_Idx)? col: col-1; // adjust index
    (*join_type_ids)[accIdx + *R_col_count] = duckdb_create_logical_type((*S_type_ids)[col]);
  }

  // Get payload info
  *R_pL_bytesPerRow = 0;
  *S_pL_bytesPerRow = 0;
  *R_payloadTypes = malloc((*R_col_count-1)*sizeof(duckdb_type));
  *S_payloadTypes = malloc((*S_col_count-1)*sizeof(duckdb_type));
  for(idx_t col=0; col<*R_col_count; col++) {
    if(col==*R_keyCol_Idx) continue;
    idx_t accIdx = (col<*R_keyCol_Idx)? col: col-1;

    *R_pL_bytesPerRow += colType_bytes((*R_type_ids)[col]);
    (*R_payloadTypes)[accIdx] = (*R_type_ids)[col];
  }
  for(idx_t col=0; col<*S_col_count; col++) {
    if(col==*S_keyCol_Idx) continue;
    idx_t accIdx = (col<*S_keyCol_Idx)? col: col-1;

    *S_pL_bytesPerRow += colType_bytes((*S_type_ids)[col]);
    (*S_payloadTypes)[accIdx] = (*S_type_ids)[col];
  }

  // Create the Appender
  if( duckdb_appender_create(con, NULL, Join_tbl_name, join_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    duckdb_appender_destroy(join_appender);
    return -1;
  }

  mylog(logfile, "Created result table and its appender.");
  return 0;
}

void gatherPayloads_GFTR(
	struct futhark_context *ctx,
	char *outCol,
	idx_t payloadBytes,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	struct futhark_u8_2d *inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Gather
	struct futhark_u8_2d *outCol_ft;
	futhark_entry_gather_payloads_GFTR(ctx, &outCol_ft, incr, block_size, gatherIs, payloadBytes, inCol);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_u8_2d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_u8_2d(ctx, outCol_ft);
}

void gatherPayloads_GFUR_inFuthark(
	struct futhark_context *ctx,
	char *outCol,
	idx_t payloadBytes,
	idx_t sort_incr,
	idx_t join_incr,
	const int16_t block_size,
	struct futhark_i64_1d *sort_gatherIs,
	struct futhark_i64_1d *join_gatherIs,
	char *inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Gather indices
	struct futhark_i64_1d *true_gatherIs;
	futhark_entry_gather_payloads_long(ctx, &true_gatherIs, join_incr, block_size, join_gatherIs, sort_gatherIs);
	// Gather
	struct futhark_u8_2d *inCol_ft = futhark_new_u8_2d(ctx, inCol, card_columns, payloadBytes);
	struct futhark_u8_2d *outCol_ft;
	futhark_entry_gather_payloads_GFTR(ctx, &outCol_ft, sort_incr, block_size, true_gatherIs, payloadBytes, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_u8_2d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_u8_2d(ctx, outCol_ft);
	futhark_free_i64_1d(ctx, true_gatherIs);
}

void gatherPayloads_short_GFUR(
	struct futhark_context *ctx,
	short *outCol,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	short* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol & indices into futhark arrays
	struct futhark_i16_1d *inCol_ft = futhark_new_i16_1d(ctx, inCol, card_columns);
	struct futhark_i64_1d *gatherIs_ft = futhark_new_i64_1d(ctx, gatherIs, numPairs);
	// Gather
	struct futhark_i16_1d *preCol_ft = futhark_new_i16_1d(ctx, outCol, numPairs);
	struct futhark_i16_1d *outCol_ft;
	futhark_entry_gather_payloads_short_GFUR(ctx, &outCol_ft, incr, block_size, preCol_ft, gatherIs_ft, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i16_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i16_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, gatherIs_ft);
	futhark_free_i16_1d(ctx, preCol_ft);
	futhark_free_i16_1d(ctx, outCol_ft);
}
void gatherPayloads_int_GFUR(
	struct futhark_context *ctx,
	int *outCol,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	int* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol & indices into futhark arrays
	struct futhark_i32_1d *inCol_ft = futhark_new_i32_1d(ctx, inCol, card_columns);
	struct futhark_i64_1d *gatherIs_ft = futhark_new_i64_1d(ctx, gatherIs, numPairs);
	// Gather
	struct futhark_i32_1d *preCol_ft = futhark_new_i32_1d(ctx, outCol, numPairs);
	struct futhark_i32_1d *outCol_ft;
	futhark_entry_gather_payloads_int_GFUR(ctx, &outCol_ft, incr, block_size, preCol_ft, gatherIs_ft, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i32_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i32_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, gatherIs_ft);
	futhark_free_i32_1d(ctx, preCol_ft);
	futhark_free_i32_1d(ctx, outCol_ft);
}
void gatherPayloads_long_GFUR(
	struct futhark_context *ctx,
	long *outCol,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	long* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol & indices into futhark arrays
	struct futhark_i64_1d *inCol_ft = futhark_new_i64_1d(ctx, inCol, card_columns);
	struct futhark_i64_1d *gatherIs_ft = futhark_new_i64_1d(ctx, gatherIs, numPairs);
	// Gather
	struct futhark_i64_1d *preCol_ft = futhark_new_i64_1d(ctx, outCol, numPairs);
	struct futhark_i64_1d *outCol_ft;
	futhark_entry_gather_payloads_long_GFUR(ctx, &outCol_ft, incr, block_size, preCol_ft, gatherIs_ft, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i64_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i64_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, gatherIs_ft);
	futhark_free_i64_1d(ctx, preCol_ft);
	futhark_free_i64_1d(ctx, outCol_ft);
}
void gatherPayloads_float_GFUR(
	struct futhark_context *ctx,
	float *outCol,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	float* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol & indices into futhark arrays
	struct futhark_f32_1d *inCol_ft = futhark_new_f32_1d(ctx, inCol, card_columns);
	struct futhark_i64_1d *gatherIs_ft = futhark_new_i64_1d(ctx, gatherIs, numPairs);
	// Gather
	struct futhark_f32_1d *preCol_ft = futhark_new_f32_1d(ctx, outCol, numPairs);
	struct futhark_f32_1d *outCol_ft;
	futhark_entry_gather_payloads_float_GFUR(ctx, &outCol_ft, incr, block_size, preCol_ft, gatherIs_ft, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_f32_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f32_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, gatherIs_ft);
	futhark_free_f32_1d(ctx, preCol_ft);
	futhark_free_f32_1d(ctx, outCol_ft);
}
void gatherPayloads_double_GFUR(
	struct futhark_context *ctx,
	double *outCol,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	double* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol & indices into futhark arrays
	struct futhark_f64_1d *inCol_ft = futhark_new_f64_1d(ctx, inCol, card_columns);
	struct futhark_i64_1d *gatherIs_ft = futhark_new_i64_1d(ctx, gatherIs, numPairs);
	// Gather
	struct futhark_f64_1d *preCol_ft = futhark_new_f64_1d(ctx, outCol, numPairs);
	struct futhark_f64_1d *outCol_ft;
	futhark_entry_gather_payloads_double_GFUR(ctx, &outCol_ft, incr, block_size, preCol_ft, gatherIs_ft, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_f64_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f64_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, gatherIs_ft);
	futhark_free_f64_1d(ctx, preCol_ft);
	futhark_free_f64_1d(ctx, outCol_ft);
}
void gatherPayloads_GFUR(
	struct futhark_context *ctx,
	void *outCol,
	duckdb_type type,
	idx_t incr,
	const int16_t block_size,
	idx_t *gatherIs,
	void* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	switch(type) {
		case DUCKDB_TYPE_SMALLINT:
			gatherPayloads_short_GFUR(ctx, (short*)outCol, incr, block_size, gatherIs, (short*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_INTEGER:
			gatherPayloads_int_GFUR(ctx, (int*)outCol, incr, block_size, gatherIs, (int*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_BIGINT:
			gatherPayloads_long_GFUR(ctx, (long*)outCol, incr, block_size, gatherIs, (long*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_FLOAT:
			gatherPayloads_float_GFUR(ctx, (float*)outCol, incr, block_size, gatherIs, (float*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_DOUBLE:
			gatherPayloads_double_GFUR(ctx, (double*)outCol, incr, block_size, gatherIs, (double*)inCol, card_columns, numPairs);
			return;
		default:
			perror("gatherPayloads: Invalid duckdb type.");
			return;
	}
}