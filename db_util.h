#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"

#ifndef DB_UTIL_H
#define DB_UTIL_H

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

int colType_name(duckdb_type type, char *dest_str);

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

/** A function to load multiple chunks at once.
 * This is done to improve memory locality.
 * Params:
 * result: duckdb result that chunks are being streamed from
 * num_chunks: maximum number of chunks to be read
 * out_chunks: double pointer (pre-allocated) where chunks are stored
 * Returns:
 * total number of chunks read
 * Note:
 * If returns < num_chunks, or if final chunk is not full, result has been exhausted
 */
idx_t bulk_fetch_chunks(duckdb_result res, idx_t num_chunks, duckdb_data_chunk* out_chunks);

/** A function to load chunks that were bulk-loaded into destination buffers.
 * Chunks are destroyed after being buffered.
 * Params:
 * chunks: chunks to be buffered
 * num_chunks: number of chunks to be buffered
 * col_count: number of columns
 * excludeCol: column to exclude ie key column (if none, set negative)
 * types: type of each column
 * dests: destination pointers (pre-allocated)
 * excludeDest: dest for excluded column (set to NULL to just skip, otherwise pre-allocated)
 * offset: index where to start writing into the buffers
 * Returns:
 * total number of rows buffered
 * Note:
 * If returned value is not a multiple of CHUNK_SIZE, result has been exhausted
 */
idx_t bulk_buffer_chunks(
	duckdb_data_chunk* chunks,
	idx_t num_chunks,
	idx_t col_count,
	idx_t excludeCol,
	duckdb_type* types,
	void** dests,
	void* excludeDest,
	idx_t offset
);
/** A function that bulk-reads and buffers chunks until either buffers are full or result is exhausted.
 * Params: 
 * CHUNK_SIZE: maximum number of rows in a chunk
 * res: duckdb result that chunks are being streamed from
 * num_chunks: chunks to be processed at once
 * col_count: number of columns
 * excludeCol: column to exclude ie key column (if none, set negative)
 * types: type of each column
 * dests: destination pointers (pre-allocated)
 * excludeDest: dest for excluded column (set to NULL to just skip, otherwise pre-allocated)
 * capacity: buffer capacity (should be a multiple of CHUNK_SIZE * num_chunks)
 * exhaustedRes: set to true if result is definitely exhausted at the end, otherwise false
 * Returns:
 * number of rows buffered
 */
// TODO make relevance-checking version for joins (in join util rather than here ig...)
idx_t bulk_load_chunks(
	idx_t CHUNK_SIZE,
	duckdb_result res,
	idx_t num_chunks,
	idx_t col_count,
	idx_t excludeCol,
	duckdb_type* types,
	void** dests,
	void* excludeDest,
	idx_t capacity,
	int* exhaustedRes
);

idx_t bulk_buffer_chunks_GFTR(
	duckdb_data_chunk* chunks,
	idx_t num_chunks,
	idx_t col_count,
	idx_t keyCol,
	duckdb_type* types,
	idx_t pL_bytes,
	idx_t *pL_byteSizes,
	idx_t *pL_prefixSizes,
	void* keyDest,
	char* plDest,
	idx_t offset
);
idx_t bulk_load_chunks_GFTR(
	idx_t CHUNK_SIZE,
	duckdb_result res,
	idx_t num_chunks,
	idx_t col_count,
	idx_t keyCol,
	duckdb_type* types,
	void* keyDest,
	char** plDest,
	idx_t capacity,
	int* exhaustedRes
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
	char** colNames,
	void** BuffersIn
);
idx_t store_intermediate_GFTR(
	idx_t numInter,
	const char *intermName,
	duckdb_connection con,
	idx_t chunkSize,
	idx_t col_count,
	idx_t row_count,
	duckdb_type* types,
	char** colNames,
	void* key_data,
	char* pL_data,
	idx_t keyCol_idx
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

#endif