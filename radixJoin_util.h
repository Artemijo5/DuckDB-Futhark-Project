#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftRelational.h"

#ifndef RADIXJOIN_UTIL_H
#define RADIXJOIN_UTIL_H

void getRadix(char *dest, char *entry, int32_t radix_bits, idx_t num_bytes);

duckdb_state radix_selective_query(
	duckdb_type keyType,
	duckdb_connection con,
	duckdb_result* res_ptr,
	const char *tblName,
	const char *keyColName,
	char *first_radix,
	char *last_radix,
	int32_t radix_bits
);

void radixPartition_GFUR(
	struct futhark_context *ctx,
	struct futhark_u8_2d **outCol,
	struct futhark_i64_1d **outIdx,
	char *inCol,
	idx_t card,
	idx_t num_bytes,
	int32_t radix_bits,
	int32_t max_depth,
	idx_t size_thresh,
	idx_t offset,
	idx_t scatter_psize
);

void radixPartition_GFTR(
	struct futhark_context *ctx,
	struct futhark_u8_2d **outKeys,
	struct futhark_u8_2d **outPayloads,
	char *inKeys,
	char *inPayloads,
	idx_t card,
	idx_t num_bytes,
	idx_t pL_bytes,
	int32_t radix_bits,
	int32_t max_depth,
	idx_t size_thresh,
	idx_t scatter_psize
);

void getPartitionInfo(
	struct futhark_context *ctx,
	struct futhark_opaque_partitionInfo **outInfo,
	struct futhark_u8_2d *keys,
	int32_t radix_bits,
	idx_t size_thresh,
	int32_t max_depth,
	idx_t offset
);

void getRadixHashTable(
	struct futhark_context *ctx,
	struct futhark_opaque_radix_hashTable **outTable,
	struct futhark_u8_2d *keys,
	struct futhark_opaque_partitionInfo *pInfo,
	int32_t radix_bits,
	idx_t scatter_psize
);

void HashJoin_joinKeyColumns_inFuthark(
	struct futhark_context *ctx,
	idx_t *numPairs,
	char **outVs_dptr,
	struct futhark_i64_1d **outIdx1,
	struct futhark_i64_1d **outIdx2,
	idx_t num_bytes,
	int32_t radix_bits,
	idx_t incr1,
	idx_t incr2,
	struct futhark_u8_2d *keys1,
	struct futhark_u8_2d *keys2,
	struct futhark_opaque_partitionInfo *info2,
	struct futhark_opaque_radix_hashTable *hash_tbl2,
	idx_t card1,
	idx_t card2
);

#endif