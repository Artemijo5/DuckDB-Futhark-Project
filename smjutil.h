#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftSMJ.h"

#ifndef SMJUTIL_H
#define SMJUTIL_H

/**
 * A function to compare the maximum element of arr1 to the minimum of arr2.
 * Both arr1 and arr2 must be sorted in increasing order!
 * Returns:
 * - if arr1.max < arr2.min, -1
 * - if arr1.max == arr2.min, 0
 * - if arr1.max > arr2.min, +1
 */
int compare_max_to_min(duckdb_type type, void* arr1, void* arr2, idx_t card1, idx_t card2);

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
);

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
);

#endif