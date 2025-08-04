#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftSMJ.h"

#ifndef SMJUTIL_H
#define SMJUTIL_H

/** A function like sizeof, but for the duckdb types used.
 * Params:
 * type : the logical type of the data, expressed in enum duckdb_type
 */
size_t colType_bytes(duckdb_type type);
/** A function to allocate memory for any type of column data.
 * Params:
 * type : the logical type of the data, expressed in enum duckdb_type
 * card : the cardinality of elements to store 
 */
void* colType_malloc(duckdb_type type, idx_t card);

/**
 * A function to compare the maximum element of arr1 to the minimum of arr2.
 * Both arr1 and arr2 must be sorted in increasing order!
 * Returns:
 * - if arr1.max < arr2.min, -1
 * - if arr1.max == arr2.min, 0
 * - if arr1.max > arr2.min, +1
 */
int compare_max_to_min(duckdb_type type, void* arr1, void* arr2, idx_t card1, idx_t card2);

/**
 * A function to compare the maximum elements of arr1 and arr2.
 * Both arr1 and arr2 must be sorted in increasing order!
 * Returns:
 * - if arr1.max < arr2.max, -1
 * - if arr1.max == arr2.max, 0
 * - if arr1.max > arr2.max, +1
 */
int compare_maxima(duckdb_type type, void* arr1, void* arr2, idx_t card1, idx_t card2);

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

void indexRange(
	struct futhark_context *ctx,
	idx_t *minIndex,
	idx_t *maxIndex,
	idx_t *indices,
	idx_t count
);

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
);

#endif