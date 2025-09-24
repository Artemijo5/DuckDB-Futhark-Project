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

#endif