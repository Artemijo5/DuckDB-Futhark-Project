#include "smjutil.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "myutil.h"
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