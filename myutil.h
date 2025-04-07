#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftsort.h"

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
 * A function to sort the key column of a table, also provides the reordered indices.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the sorted column, which must have enough space allocated
 * type : the logical type of the data, expressed in enum duckdb_type
 * incr : the index of the first row in the current data chunk
 * outIdx : a double pointer for the output of the reordered indices (as a futhark array)
 * keys : the key column to be sorted
 * card : the cardinality of elements to process
 */
void sortKeyColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d **outIdx, void* keys, idx_t card);
/**
 * A function to reorder a payload column, according to the reordered indices from sorting the key column.
 * Params:
 * ctx : pointer to the futhark context
 * outCol : a pointer for the output of the reordered column, which must have enough space allocated
 * type : the logical type of the data, expressed in enum duckdb_type
 * orderBy : the reordered indices (as a futhark array)
 * inCol : the payload column to be reordered
 * card : the cardinality of elements to process
 */
void orderPayloadColumn(struct futhark_context *ctx, void *outCol, duckdb_type type, idx_t incr, struct futhark_i64_1d *orderBy, void* inCol, idx_t card);
