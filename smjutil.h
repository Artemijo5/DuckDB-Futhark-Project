#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftRelational.h"

#ifndef SMJUTIL_H
#define SMJUTIL_H

/**
 * A function to read a table with a value constraint.
 * According to duckdb's documentation, the WHERE clause guarantees order preservation.
 * Efficiency is guaranteed by the MIN-MAX index.
 */
duckdb_state selective_query(
	duckdb_type keyType,
	duckdb_connection con,
	duckdb_result* res_ptr,
	const char *tblName,
	const char *keyColName,
	void *minimum,
	void *maximum
);

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
	idx_t partitionSize,
	idx_t scatter_psize
);

void indexRange(
	struct futhark_context *ctx,
	idx_t *minIndex,
	idx_t *maxIndex,
	idx_t *indices,
	idx_t count
);

// TODO
// Entirely in-memory SMJ
// These return futhark obj's, and should be sync'd before using scalar output

void sortRelationByKey_inFuthark(
	struct futhark_context *ctx,
	void **outKeys,
	struct futhark_u8_2d **outPayloads,
	duckdb_type key_type,
	int blocked,
	idx_t block_size,
	void* inKeys,
	char *inPayloads,
	idx_t pL_bytesPerRow,
	idx_t card
);
void sortKeyColumn_inFuthark(
	struct futhark_context *ctx,
	void **outCol,
	duckdb_type type,
	idx_t incr,
	int blocked,
	idx_t block_size,
	struct futhark_i64_1d **outIdx,
	void* keys,
	idx_t card
);
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
	idx_t partitionSize,
	idx_t scatter_psize
);

#endif