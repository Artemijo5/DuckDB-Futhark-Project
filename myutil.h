#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftsort.h"

#ifndef MYUTIL_H
#define MYUTIL_H

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
 * A function to fill elements of an array with the maximum value of a given type.
 * Params:
 * dest : pointer to the starting position of the desired array
 * type : the logical type of the data, expressed in enum duckdb_type
 * n : the amount of array elements to pad (NOT the number of bytes)
 */
void max_padding(void* dest, duckdb_type type, idx_t n);

/**
 * A function to transform payload columns into a byte array.
 * Elements of the same row are adjacent.
 * (Note: it's more efficient to directly store payloads as bytes, rather than transforming them with this)
 * Params:
 * outBytes : where to store the output byte array (allocated within the function)
 * pL_rowBytes : pointer to store the total number of (payload) bytes per row
 * payload_types : array of the duckdb_type types of each payload row
 * inPayloads : input payload arrays
 * pL_col_count : number of payload columns
 * row_count : number of rows
*/
void payloadColumnsToByteArray(
	char** outBytes,
	idx_t* pL_rowBytes,
	duckdb_type* payload_types,
	void** inPayloads,
	idx_t pL_col_count,
	idx_t row_count
);

/**
 * A function to transform a byte array into payload columns.
 * Elements of the same row are adjacent.
 * Params:
 * outPayloads : where to output the payloads (double pointer must be preallocated; referenced pointers are allocated within function)
 * payload_types : array of the duckdb_type types of each columns (including key column)
 * inBytes : input byte array
 * pL_col_count : number of payload columns
 * row_count : number of rows
*/
void payloadColumnsFromByteArray(
	void** outPayloads,
	duckdb_type* payload_types,
	char* inBytes,
	idx_t pL_col_count,
	idx_t row_count
);

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

/**
 * A function to store a buffer of columns between the stages of processing.
 * Params:
 * numInter : number of intermediates that have been used so far
 * intermName : the naming scheme used for intermediates (with numInter appended) - make sure it's a valid name for duckdb temp tables
 * con : the duckdb connection used
 * chunkSize : the number of rows in a duckdb datachunk, as per the configuration
 * col_count : number of columns
 * row_count : number of rows that we are storing
 * types : the logical types of the data, expressed in enum duckdb_type
 * BuffersIn : the data we are storing
 * Returns:
 * new number of intermediates (numInter+1) on success, -1 on failure
 */
idx_t store_intermediate(
	idx_t numInter,
	const char *intermName,
	duckdb_connection con,
	idx_t chunkSize,
	idx_t col_count,
	idx_t row_count,
	duckdb_type* types,
	void** BuffersIn
);
/**
 * A function that outputs a result from a stored intermediate.
 * Params:
 * numInter : the numerial id of the intermediate to fetch from (set ordinally by store_intermediate)
 * intermName : the naming scheme used for intermediates (with numInter appended)
 * con : the duckdb connection used
 * result_ptr : pointer where the output is stored, must be destroyed with duckdb_destroy_result after use
 */
void prepareToFetch_intermediate(
	idx_t numInter,
	const char *intermName,
	duckdb_connection con,
	duckdb_result *result_ptr
);
/**
 * A function to fetch the next data chunk from an intermediate.
 * Params:
 * result : the result associated with an intermediate, obtained with prepareToFetch_intermediate
 * col_count : the number of columns
 * types : the logical types of the data, expressed in enum duckdb_type
 * BuffersOut : arrays to retrieve the data into
 * start_idx : index at which to start insertion into BuffersOut
 * Returns:
 * number of rows retrieved on success, 0 if table has been exhausted, -1 on failure
 */
idx_t fetch_intermediate(
	duckdb_result result,
	idx_t col_count,
	duckdb_type* types,
	void** BuffersOut,
	idx_t start_idx
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