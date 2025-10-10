#include "smjutil.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "db_util.h"
#include <string.h>

#include "ftSMJ.h"

int compare_max_to_min(duckdb_type type, void* arr1, void* arr2, idx_t card1, idx_t card2) {
	int ret = 0;
	switch(type) {
		case DUCKDB_TYPE_SMALLINT:
			if (((short*)arr1)[card1-1] < ((short*)arr2)[0]) ret = -1;
			else if (((short*)arr1)[card1-1] > ((short*)arr2)[0]) ret = +1;
			break;
		case DUCKDB_TYPE_INTEGER:
			if (((int*)arr1)[card1-1] < ((int*)arr2)[0]) ret = -1;
			else if (((int*)arr1)[card1-1] > ((int*)arr2)[0]) ret = +1;
			break;
		case DUCKDB_TYPE_BIGINT:
			if (((long*)arr1)[card1-1] < ((long*)arr2)[0]) ret = -1;
			else if (((long*)arr1)[card1-1] > ((long*)arr2)[0]) ret = +1;
			break;
		case DUCKDB_TYPE_FLOAT:
			if (((float*)arr1)[card1-1] < ((float*)arr2)[0]) ret = -1;
			else if (((float*)arr1)[card1-1] > ((float*)arr2)[0]) ret = +1;
			break;
		case DUCKDB_TYPE_DOUBLE:
			if (((double*)arr1)[card1-1] < ((double*)arr2)[0]) ret = -1;
			else if (((double*)arr1)[card1-1] > ((double*)arr2)[0]) ret = +1;
			break;
		default:
			perror("compare_max_to_min: Invalid duckdb type.");
			return 0;
	}
	return ret;
}

int compare_maxima(duckdb_type type, void* arr1, void* arr2, idx_t card1, idx_t card2) {
	int ret = 0;
	switch(type) {
		case DUCKDB_TYPE_SMALLINT:
			if (((short*)arr1)[card1-1] < ((short*)arr2)[card2-1]) ret = -1;
			else if (((short*)arr1)[card1-1] > ((short*)arr2)[card2-1]) ret = +1;
			break;
		case DUCKDB_TYPE_INTEGER:
			if (((int*)arr1)[card1-1] < ((int*)arr2)[card2-1]) ret = -1;
			else if (((int*)arr1)[card1-1] > ((int*)arr2)[card2-1]) ret = +1;
			break;
		case DUCKDB_TYPE_BIGINT:
			if (((long*)arr1)[card1-1] < ((long*)arr2)[card2-1]) ret = -1;
			else if (((long*)arr1)[card1-1] > ((long*)arr2)[card2-1]) ret = +1;
			break;
		case DUCKDB_TYPE_FLOAT:
			if (((float*)arr1)[card1-1] < ((float*)arr2)[card2-1]) ret = -1;
			else if (((float*)arr1)[card1-1] > ((float*)arr2)[card2-1]) ret = +1;
			break;
		case DUCKDB_TYPE_DOUBLE:
			if (((double*)arr1)[card1-1] < ((double*)arr2)[card2-1]) ret = -1;
			else if (((double*)arr1)[card1-1] > ((double*)arr2)[card2-1]) ret = +1;
			break;
		default:
			perror("compare_maxima: Invalid duckdb type.");
			return 0;
	}
	return ret;
}

duckdb_state selective_query(
	duckdb_type keyType,
	duckdb_connection con,
	duckdb_result* res_ptr,
	const char *tblName,
	const char *keyColName,
	void *minimum,
	void *maximum
) {
	char constraintStr[50 + 2*strlen(keyColName) + 10*colType_bytes(keyType)];
	switch(keyType) {
		case DUCKDB_TYPE_SMALLINT:
			sprintf(constraintStr,
				" WHERE (%s >= %d AND %s <= %d)",
				keyColName, (int)(*((short*)minimum)), keyColName, (int)(*((short*)maximum)));
			break;
		case DUCKDB_TYPE_INTEGER:
			sprintf(constraintStr,
				"WHERE (%s >= %d AND %s <= %d)",
				keyColName, *((int*)minimum), keyColName, *((int*)maximum));
			break;
		case DUCKDB_TYPE_BIGINT:
			sprintf(constraintStr,
				"WHERE (%s >= %ld AND %s <= %ld)",
				keyColName, *((long*)minimum), keyColName, *((long*)maximum));
			break;
		case DUCKDB_TYPE_FLOAT:
			sprintf(constraintStr,
				"WHERE (%s >= %f AND %s <= %f)",
				keyColName, *((float*)minimum), keyColName, *((float*)maximum));
			break;
		case DUCKDB_TYPE_DOUBLE:
			sprintf(constraintStr,
				"WHERE (%s >= %lf AND %s <= %lf)",
				keyColName, *((double*)minimum), keyColName, *((double*)maximum));
			break;
		default:
			sprintf(constraintStr, " ");// perform the query without constraints
			break;
	}
	char queryStr[50 + strlen(tblName) + strlen(constraintStr)];
	sprintf(queryStr, "SELECT * FROM %s %s;", tblName, constraintStr);
	//printf("%s\n", queryStr);
	return duckdb_query(con, queryStr, res_ptr);
}

void InnerJoin_joinKeyColumns_short(
	struct futhark_context *ctx,
	idx_t *numPairs,
	short **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	short *keys1,
	short *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Wrap keys into futhark arrays
	struct futhark_i16_1d *keys1_ft = futhark_new_i16_1d(ctx, keys1, card1);
	struct futhark_i16_1d *keys2_ft = futhark_new_i16_1d(ctx, keys2, card2);
	// Prepare output array
	struct futhark_opaque_joinPairs_short *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_short(ctx, &joinPairs, keys1_ft, keys2_ft, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_short_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_short_iy(ctx, outIdx2, joinPairs);
	struct futhark_i16_1d *outVs_ft;
	futhark_project_opaque_joinPairs_short_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i16_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(short)));
	futhark_values_i16_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i16_1d(ctx, keys1_ft);
	futhark_free_i16_1d(ctx, keys2_ft);
	futhark_free_i16_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_short(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_int(
	struct futhark_context *ctx,
	idx_t *numPairs,
	int **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	int *keys1,
	int *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Wrap keys into futhark arrays
	struct futhark_i32_1d *keys1_ft = futhark_new_i32_1d(ctx, keys1, card1);
	struct futhark_i32_1d *keys2_ft = futhark_new_i32_1d(ctx, keys2, card2);
	// Prepare output array
	struct futhark_opaque_joinPairs_int *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_int(ctx, &joinPairs, keys1_ft, keys2_ft, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_int_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_int_iy(ctx, outIdx2, joinPairs);
	struct futhark_i32_1d *outVs_ft;
	futhark_project_opaque_joinPairs_int_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i32_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(int)));
	futhark_values_i32_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i32_1d(ctx, keys1_ft);
	futhark_free_i32_1d(ctx, keys2_ft);
	futhark_free_i32_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_int(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_long(
	struct futhark_context *ctx,
	idx_t *numPairs,
	long **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	long *keys1,
	long *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Wrap keys into futhark arrays
	struct futhark_i64_1d *keys1_ft = futhark_new_i64_1d(ctx, keys1, card1);
	struct futhark_i64_1d *keys2_ft = futhark_new_i64_1d(ctx, keys2, card2);
	// Prepare output array
	struct futhark_opaque_joinPairs_long *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_long(ctx, &joinPairs, keys1_ft, keys2_ft, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_long_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_long_iy(ctx, outIdx2, joinPairs);
	struct futhark_i64_1d *outVs_ft;
	futhark_project_opaque_joinPairs_long_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i64_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(long)));
	futhark_values_i64_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i64_1d(ctx, keys1_ft);
	futhark_free_i64_1d(ctx, keys2_ft);
	futhark_free_i64_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_long(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_float(
	struct futhark_context *ctx,
	idx_t *numPairs,
	float **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	float *keys1,
	float *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Wrap keys into futhark arrays
	struct futhark_f32_1d *keys1_ft = futhark_new_f32_1d(ctx, keys1, card1);
	struct futhark_f32_1d *keys2_ft = futhark_new_f32_1d(ctx, keys2, card2);
	// Prepare output array
	struct futhark_opaque_joinPairs_float *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_float(ctx, &joinPairs, keys1_ft, keys2_ft, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_float_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_float_iy(ctx, outIdx2, joinPairs);
	struct futhark_f32_1d *outVs_ft;
	futhark_project_opaque_joinPairs_float_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_f32_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(float)));
	futhark_values_f32_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f32_1d(ctx, keys1_ft);
	futhark_free_f32_1d(ctx, keys2_ft);
	futhark_free_f32_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_float(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_double(
	struct futhark_context *ctx,
	idx_t *numPairs,
	double **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	double *keys1,
	double *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Wrap keys into futhark arrays
	struct futhark_f64_1d *keys1_ft = futhark_new_f64_1d(ctx, keys1, card1);
	struct futhark_f64_1d *keys2_ft = futhark_new_f64_1d(ctx, keys2, card2);
	// Prepare output array
	struct futhark_opaque_joinPairs_double *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_double(ctx, &joinPairs, keys1_ft, keys2_ft, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_double_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_double_iy(ctx, outIdx2, joinPairs);
	struct futhark_f64_1d *outVs_ft;
	futhark_project_opaque_joinPairs_double_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_f64_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(double)));
	futhark_values_f64_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f64_1d(ctx, keys1_ft);
	futhark_free_f64_1d(ctx, keys2_ft);
	futhark_free_f64_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_double(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns(
	struct futhark_context *ctx,
	idx_t *numPairs,
	void **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	duckdb_type type,
	idx_t incr1,
	idx_t incr2,
	void *keys1,
	void *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	switch (type) {
		case DUCKDB_TYPE_SMALLINT:
			InnerJoin_joinKeyColumns_short(ctx, numPairs, (short**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (short*)keys1, (short*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_INTEGER:
			InnerJoin_joinKeyColumns_int(ctx, numPairs, (int**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (int*)keys1, (int*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_BIGINT:
			InnerJoin_joinKeyColumns_long(ctx, numPairs, (long**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (long*)keys1, (long*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_FLOAT:
			InnerJoin_joinKeyColumns_float(ctx, numPairs, (float**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (float*)keys1, (float*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_DOUBLE:
			InnerJoin_joinKeyColumns_double(ctx, numPairs, (double**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (double*)keys1, (double*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		default:
			perror("InnerJoin_joinKeyColumns: Invalid duckdb type.");
			return;
	}
}


void gatherPayloads_short(
	struct futhark_context *ctx,
	short *outCol,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	short* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol into a futhark array
	struct futhark_i16_1d *inCol_ft = futhark_new_i16_1d(ctx, inCol, card_columns);
	// Gather
	struct futhark_i16_1d *outCol_ft;
	futhark_entry_gather_payloads_short(ctx, &outCol_ft, incr, block_size, gatherIs, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i16_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i16_1d(ctx, inCol_ft);
	futhark_free_i16_1d(ctx, outCol_ft);
}
void gatherPayloads_int(
	struct futhark_context *ctx,
	int *outCol,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	int* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol into a futhark array
	struct futhark_i32_1d *inCol_ft = futhark_new_i32_1d(ctx, inCol, card_columns);
	// Gather
	struct futhark_i32_1d *outCol_ft;
	futhark_entry_gather_payloads_int(ctx, &outCol_ft, incr, block_size, gatherIs, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i32_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i32_1d(ctx, inCol_ft);
	futhark_free_i32_1d(ctx, outCol_ft);
}
void gatherPayloads_long(
	struct futhark_context *ctx,
	long *outCol,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	long* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol into a futhark array
	struct futhark_i64_1d *inCol_ft = futhark_new_i64_1d(ctx, inCol, card_columns);
	// Gather
	struct futhark_i64_1d *outCol_ft;
	futhark_entry_gather_payloads_long(ctx, &outCol_ft, incr, block_size, gatherIs, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_i64_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i64_1d(ctx, inCol_ft);
	futhark_free_i64_1d(ctx, outCol_ft);
}
void gatherPayloads_float(
	struct futhark_context *ctx,
	float *outCol,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	float* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol into a futhark array
	struct futhark_f32_1d *inCol_ft = futhark_new_f32_1d(ctx, inCol, card_columns);
	// Gather
	struct futhark_f32_1d *outCol_ft;
	futhark_entry_gather_payloads_float(ctx, &outCol_ft, incr, block_size, gatherIs, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_f32_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f32_1d(ctx, inCol_ft);
	futhark_free_f32_1d(ctx, outCol_ft);
}
void gatherPayloads_double(
	struct futhark_context *ctx,
	double *outCol,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	double* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Wrap inCol into a futhark array
	struct futhark_f64_1d *inCol_ft = futhark_new_f64_1d(ctx, inCol, card_columns);
	// Gather
	struct futhark_f64_1d *outCol_ft;
	futhark_entry_gather_payloads_double(ctx, &outCol_ft, incr, block_size, gatherIs, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_f64_1d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f64_1d(ctx, inCol_ft);
	futhark_free_f64_1d(ctx, outCol_ft);
}

void gatherPayloads(
	struct futhark_context *ctx,
	void *outCol,
	duckdb_type type,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	void* inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	switch(type) {
		case DUCKDB_TYPE_SMALLINT:
			gatherPayloads_short(ctx, (short*)outCol, incr, block_size, gatherIs, (short*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_INTEGER:
			gatherPayloads_int(ctx, (int*)outCol, incr, block_size, gatherIs, (int*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_BIGINT:
			gatherPayloads_long(ctx, (long*)outCol, incr, block_size, gatherIs, (long*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_FLOAT:
			gatherPayloads_float(ctx, (float*)outCol, incr, block_size, gatherIs, (float*)inCol, card_columns, numPairs);
			return;
		case DUCKDB_TYPE_DOUBLE:
			gatherPayloads_double(ctx, (double*)outCol, incr, block_size, gatherIs, (double*)inCol, card_columns, numPairs);
			return;
		default:
			perror("gatherPayloads: Invalid duckdb type.");
			return;
	}
}

void indexRange(
	struct futhark_context *ctx,
	idx_t *minIndex,
	idx_t *maxIndex,
	idx_t *indices,
	idx_t count
) {
	// Wrap indices in futhark context
	struct futhark_i64_1d *indices_ft = futhark_new_i64_1d(ctx, indices, count);
	// Obtain min & max
	futhark_entry_min_idx(ctx, minIndex, indices_ft);
	futhark_entry_max_idx(ctx, maxIndex, indices_ft);
	// Cleanup
	futhark_free_i64_1d(ctx, indices_ft);
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

// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------
// Entirely in-memory SMJ
// These return futhark obj's, and should be sync'd before using scalar output
// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------

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

void sortRelationByKey_inFuthark_short(
  struct futhark_context *ctx,
  struct futhark_i16_1d **outKeys,
  struct futhark_u8_2d **outPayloads,
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
  futhark_project_opaque_sortStruct_short_k(ctx, outKeys, sortStruct_out);
  futhark_project_opaque_sortStruct_short_pL(ctx, outPayloads, sortStruct_out);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_opaque_sortStruct_short(ctx, sortStruct_out);
}
void sortRelationByKey_inFuthark_int(
  struct futhark_context *ctx,
  struct futhark_i32_1d **outKeys,
  struct futhark_u8_2d **outPayloads,
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
  futhark_project_opaque_sortStruct_int_k(ctx, outKeys, sortStruct_out);
  futhark_project_opaque_sortStruct_int_pL(ctx, outPayloads, sortStruct_out);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_opaque_sortStruct_int(ctx, sortStruct_out);
}
void sortRelationByKey_inFuthark_long(
  struct futhark_context *ctx,
  struct futhark_i64_1d **outKeys,
  struct futhark_u8_2d **outPayloads,
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
  futhark_project_opaque_sortStruct_long_k(ctx, outKeys, sortStruct_out);
  futhark_project_opaque_sortStruct_long_pL(ctx, outPayloads, sortStruct_out);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_opaque_sortStruct_long(ctx, sortStruct_out);
}
void sortRelationByKey_inFuthark_float(
  struct futhark_context *ctx,
  struct futhark_f32_1d **outKeys,
  struct futhark_u8_2d **outPayloads,
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
  futhark_project_opaque_sortStruct_float_k(ctx, outKeys, sortStruct_out);
  futhark_project_opaque_sortStruct_float_pL(ctx, outPayloads, sortStruct_out);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_opaque_sortStruct_float(ctx, sortStruct_out);
}
void sortRelationByKey_inFuthark_double(
  struct futhark_context *ctx,
  struct futhark_f64_1d **outKeys,
  struct futhark_u8_2d **outPayloads,
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
  futhark_project_opaque_sortStruct_double_k(ctx, outKeys, sortStruct_out);
  futhark_project_opaque_sortStruct_double_pL(ctx, outPayloads, sortStruct_out);
  // Sync context, cleanup
  futhark_context_sync(ctx);
  futhark_free_opaque_sortStruct_double(ctx, sortStruct_out);
}
void sortRelationByKey_inFuthark(
  struct futhark_context *ctx,
  void **outKeys,
  struct futhark_u8_2d **outPayloads,
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
      sortRelationByKey_inFuthark_short(ctx, (struct futhark_i16_1d **)outKeys, outPayloads, blocked, block_size,
        (short*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortRelationByKey_inFuthark_int(ctx, (struct futhark_i32_1d **)outKeys, outPayloads, blocked, block_size,
        (int*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortRelationByKey_inFuthark_long(ctx, (struct futhark_i64_1d **)outKeys, outPayloads, blocked, block_size,
        (long*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortRelationByKey_inFuthark_float(ctx, (struct futhark_f32_1d **)outKeys, outPayloads, blocked, block_size,
        (float*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortRelationByKey_inFuthark_double(ctx, (struct futhark_f64_1d **)outKeys, outPayloads, blocked, block_size,
        (double*)inKeys, inPayloads, pL_bytesPerRow, card);
      return;
    default:
      perror("sortRelationByKey: Invalid Type.");
      return;
  }
}

void sortKeyColumn_inFuthark_short(struct futhark_context *ctx, struct futhark_i16_1d **outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, short* keys, idx_t card) {
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
  futhark_project_opaque_sortInfo_short_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_short_xs(ctx, outCol, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_opaque_sortInfo_short(ctx, sortInfo);
  futhark_free_i16_1d(ctx, x_ft);
}
void sortKeyColumn_inFuthark_int(struct futhark_context *ctx, struct futhark_i32_1d **outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, int* keys, idx_t card) {
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
  futhark_project_opaque_sortInfo_int_xs(ctx, outCol, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_opaque_sortInfo_int(ctx, sortInfo);
  futhark_free_i32_1d(ctx, x_ft);
}
void sortKeyColumn_inFuthark_long(struct futhark_context *ctx, struct futhark_i64_1d **outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, long* keys, idx_t card) {
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
  futhark_project_opaque_sortInfo_long_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_long_xs(ctx, outCol, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects
  futhark_free_opaque_sortInfo_long(ctx, sortInfo);
  futhark_free_i64_1d(ctx, x_ft);
}
void sortKeyColumn_inFuthark_float(struct futhark_context *ctx, struct futhark_f32_1d **outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, float* keys, idx_t card) {
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
  futhark_project_opaque_sortInfo_float_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_float_xs(ctx, outCol, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_opaque_sortInfo_float(ctx, sortInfo);
  futhark_free_f32_1d(ctx, x_ft);
}
void sortKeyColumn_inFuthark_double(struct futhark_context *ctx, struct futhark_f64_1d **outCol, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, double* keys, idx_t card) {
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
  futhark_project_opaque_sortInfo_double_is(ctx, outIdx, sortInfo);
  futhark_project_opaque_sortInfo_double_xs(ctx, outCol, sortInfo);
  // Sync context
  futhark_context_sync(ctx);
  // Free futhark objects (except indices)
  futhark_free_opaque_sortInfo_double(ctx, sortInfo);
  futhark_free_f64_1d(ctx, x_ft);
}
void sortKeyColumn_inFuthark(struct futhark_context *ctx, void **outCol, duckdb_type type, idx_t incr, int blocked, const int16_t block_size, struct futhark_i64_1d **outIdx, void* keys, idx_t card) {
  switch (type){
  case DUCKDB_TYPE_SMALLINT:
      sortKeyColumn_inFuthark_short(ctx, (struct futhark_i16_1d **)outCol, incr, blocked, block_size, outIdx, (short*)keys, card);
      return;
    case DUCKDB_TYPE_INTEGER:
      sortKeyColumn_inFuthark_int(ctx, (struct futhark_i32_1d **)outCol, incr, blocked, block_size, outIdx, (int*)keys, card);
      return;
    case DUCKDB_TYPE_BIGINT:
      sortKeyColumn_inFuthark_long(ctx, (struct futhark_i64_1d **)outCol, incr, blocked, block_size, outIdx, (long*)keys, card);
      return;
    case DUCKDB_TYPE_FLOAT:
      sortKeyColumn_inFuthark_float(ctx, (struct futhark_f32_1d **)outCol, incr, blocked, block_size, outIdx, (float*)keys, card);
      return;
    case DUCKDB_TYPE_DOUBLE:
      sortKeyColumn_inFuthark_double(ctx, (struct futhark_f64_1d **)outCol, incr, blocked, block_size, outIdx, (double*)keys, card);
      return;
    default:
      perror("Invalid type.");
      return;
  }
}

void InnerJoin_joinKeyColumns_inFuthark_short(
	struct futhark_context *ctx,
	idx_t *numPairs,
	short **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	struct futhark_i16_1d *keys1,
	struct futhark_i16_1d *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_short *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_short(ctx, &joinPairs, keys1, keys2, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_short_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_short_iy(ctx, outIdx2, joinPairs);
	struct futhark_i16_1d *outVs_ft;
	futhark_project_opaque_joinPairs_short_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i16_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(short)));
	futhark_values_i16_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i16_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_short(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_inFuthark_int(
	struct futhark_context *ctx,
	idx_t *numPairs,
	int **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	struct futhark_i32_1d *keys1,
	struct futhark_i32_1d *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_int *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_int(ctx, &joinPairs, keys1, keys2, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_int_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_int_iy(ctx, outIdx2, joinPairs);
	struct futhark_i32_1d *outVs_ft;
	futhark_project_opaque_joinPairs_int_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i32_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(int)));
	futhark_values_i32_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i32_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_int(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_inFuthark_long(
	struct futhark_context *ctx,
	idx_t *numPairs,
	long **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	struct futhark_i64_1d *keys1,
	struct futhark_i64_1d *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_long *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_long(ctx, &joinPairs, keys1, keys2, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_long_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_long_iy(ctx, outIdx2, joinPairs);
	struct futhark_i64_1d *outVs_ft;
	futhark_project_opaque_joinPairs_long_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_i64_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(long)));
	futhark_values_i64_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_i64_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_long(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_inFuthark_float(
	struct futhark_context *ctx,
	idx_t *numPairs,
	float **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	struct futhark_f32_1d *keys1,
	struct futhark_f32_1d *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_float *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_float(ctx, &joinPairs, keys1, keys2, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_float_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_float_iy(ctx, outIdx2, joinPairs);
	struct futhark_f32_1d *outVs_ft;
	futhark_project_opaque_joinPairs_float_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_f32_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(float)));
	futhark_values_f32_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f32_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_float(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_inFuthark_double(
	struct futhark_context *ctx,
	idx_t *numPairs,
	double **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t incr1,
	idx_t incr2,
	struct futhark_f64_1d *keys1,
	struct futhark_f64_1d *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_double *joinPairs;
	// Do the join
	futhark_entry_inner_SMJ_double(ctx, &joinPairs, keys1, keys2, incr1, incr2,
		partitionsPerWindow, numWindows, extParallelism, scatter_psize
	);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_double_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_double_iy(ctx, outIdx2, joinPairs);
	struct futhark_f64_1d *outVs_ft;
	futhark_project_opaque_joinPairs_double_vs(ctx, &outVs_ft, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_f64_1d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*(sizeof(double)));
	futhark_values_f64_1d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_f64_1d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_double(ctx, joinPairs);
	return;
}
void InnerJoin_joinKeyColumns_inFuthark(
	struct futhark_context *ctx,
	idx_t *numPairs,
	void **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	duckdb_type type,
	idx_t incr1,
	idx_t incr2,
	void *keys1,
	void *keys2,
	idx_t card1,
	idx_t card2,
	idx_t numWindows,
	idx_t partitionsPerWindow,
	idx_t extParallelism,
	idx_t scatter_psize
) {
	switch (type) {
		case DUCKDB_TYPE_SMALLINT:
			InnerJoin_joinKeyColumns_inFuthark_short(ctx, numPairs, (short**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (struct futhark_i16_1d*)keys1, (struct futhark_i16_1d*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_INTEGER:
			InnerJoin_joinKeyColumns_inFuthark_int(ctx, numPairs, (int**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (struct futhark_i32_1d*)keys1, (struct futhark_i32_1d*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_BIGINT:
			InnerJoin_joinKeyColumns_inFuthark_long(ctx, numPairs, (long**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (struct futhark_i64_1d*)keys1, (struct futhark_i64_1d*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_FLOAT:
			InnerJoin_joinKeyColumns_inFuthark_float(ctx, numPairs, (float**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (struct futhark_f32_1d*)keys1, (struct futhark_f32_1d*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		case DUCKDB_TYPE_DOUBLE:
			InnerJoin_joinKeyColumns_inFuthark_double(ctx, numPairs, (double**)outVs_dptr, outIdx1, outIdx2,
			 incr1, incr2, (struct futhark_f64_1d*)keys1, (struct futhark_f64_1d*)keys2, card1, card2,numWindows, partitionsPerWindow,
			 extParallelism, scatter_psize
			);
			return;
		default:
			perror("InnerJoin_joinKeyColumns: Invalid duckdb type.");
			return;
	}
}