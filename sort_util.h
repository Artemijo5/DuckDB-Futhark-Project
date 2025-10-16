#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftRelational.h"

#ifndef SORT_UTIL_H
#define SORT_UTIL_H

/**
 * A function to return the index of the minimum element of an array.
 * Params:
 * ctx : pointer to the futhark context
 * type : the logical type of the data, expressed in enum duckdb_type
 * arr : the array to find the argmin of
 * card : the number of elements in the array
 * Returns:
 * index of the minimum element in the array (the smallest index in case of a tie)
 */
idx_t argmin(struct futhark_context *ctx, duckdb_type type, void* arr, idx_t card);

/**
 * A function to sort a relation by its key column.
 * Params:
 * ctx : pointer to the futhark context
 * outKeys : where to store sorted keys (must be pre-allocated)
 * outPayloads : where to store rearranged payloads (must be pre-allocated)
 * key_type : the logical type of the data, expressed in enum duckdb_type
 * blocked : if true, use blocked radix sort, otherwise use merge sort
 * block_size : the block size used by the blocked gpu sorting function
 * inKeys : the key column of the unsorted input relation
 * inPayloads : the payloads of the unsorted input relation, as a continuous byte array (elems of same row adjacent)
 * pL_bytesPerRow : how many bytes the payload columns of each row take
 * card : number of rows in the relation
*/
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
);

/**
 * A function to sort the key column of a table, also provides the reordered indices.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the sorted column, which must have enough space allocated
 * type : the logical type of the data, expressed in enum duckdb_type
 * incr : the index of the first row in the current data chunk
 * blocked : if true, use blocked radix sort, otherwise use merge sort
 * block_size : the block size used by the blocked gpu sorting function
 * outIdx : a double pointer for the output of the reordered indices (as a futhark array)
 * keys : the key column to be sorted
 * card : the cardinality of elements to process
 */
void sortKeyColumn(
	struct futhark_context *ctx,
	void *outCol,
	duckdb_type type,
	idx_t incr,
	int blocked,
	const int16_t block_size,
	struct futhark_i64_1d **outIdx,
	void* keys,
	idx_t card
);
/**
 * A function to reorder a payload column, according to the reordered indices from sorting the key column.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the reordered column, which must have enough space allocated
 * incr : the index of the first row in the current data chunk
 * block_size : the block size used by the blocked gpu sorting function
 * type : the logical type of the data, expressed in enum duckdb_type
 * orderBy : the reordered indices (as a futhark array)
 * inCol : the payload column to be reordered
 * card : the cardinality of elements to process
 */
void orderPayloadColumn(
	struct futhark_context *ctx,
	void *outCol,
	duckdb_type type,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *orderBy,
	void* inCol,
	idx_t card
);


// -------------------------------------------------------------------------------------- SORT-MERGE JOIN
/*
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
);
*/
// TODO unneeded + doesn't compile for cuda for now...

#endif