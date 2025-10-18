#include "radixJoin_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "mylogger.h"
#include "db_util.h"
#include <string.h>

#include "ftRelational.h"

// TODO make it togglable whether bit 0 is LSB or MSB (...)

void getRadix(char *dest, char *entry, int32_t radix_bits, idx_t num_bytes) {
	int32_t radix_bytes = (radix_bits+7)/8;
	char moduloStr[num_bytes];
	for(idx_t b=0; b<num_bytes; b++) {
		moduloStr[b] = (char)0;
	}
	for(idx_t b=0; b<radix_bits; b++) {
		idx_t thisByte = num_bytes - (b/8) - 1;
		idx_t thisBit = b%8;
		moduloStr[thisByte] += (1 << b);
	}
	for(idx_t b=0; b<radix_bits; b++) {
		dest[b] = (entry[b] & moduloStr[b]);
	}
}

// TODO
duckdb_state radix_selective_query(
	duckdb_type keyType,
	duckdb_connection con,
	duckdb_result* res_ptr,
	const char *tblName,
	const char *keyColName,
	char *first_radix,
	char *last_radix,
	int32_t radix_bits
) {
	/*
	int32_t radix_bytes = (radix_bits+7)/8
	char moduloStr[radix_bytes];
	for(idx_t b=0; b<radix_bytes; b++) {
		moduloStr[b] = (char)0;
	}
	for(idx_t b=0; b<radix_bits; b++) {
		idx_t thisByte = radix_bytes - (b/8) - 1;
		idx_T thisBit = b%8;
		moduloStr[thisByte] += (1 << b);
	}

	char constraintStr[50 + 2*strlen(keyColName) + 10*colType_bytes(keyType)];
	switch(keyType) {
		case DUCKDB_TYPE_SMALLINT:
			sprintf(constraintStr,
				" WHERE (%s & %d >= %d AND %s & %d <= %d)",
				keyColName, (int)(*((short*)moduloStr)), (int)(*((short*)first_radix)),
				keyColName, (int)(*((short*)moduloStr)), (int)(*((short*)last_radix)));
			break;
		case DUCKDB_TYPE_INTEGER:
			sprintf(constraintStr,
				"WHERE (%s & %d >= %d AND %s &%d <= %d)",
				keyColName, *((int*)moduloStr), *((int*)first_radix),
				keyColName, *((int*)moduloStr), *((int*)last_radix));
			break;
		case DUCKDB_TYPE_BIGINT:
			sprintf(constraintStr,
				"WHERE (%s & %ld >= %ld AND %s & %ld <= %ld)",
				keyColName, *((int*)moduloStr), *((int*)first_radix),
				keyColName, *((int*)moduloStr), *((int*)last_radix));
				"WHERE (%s >= %ld AND %s <= %ld)",
				keyColName, *((long*)minimum), keyColName, *((long*)maximum));
			break;
		case DUCKDB_TYPE_FLOAT:
			sprintf(constraintStr,
				"WHERE (%s >= %f AND %s <= %f)",
				keyColName, *((float*)minimum), keyColName, *((float*)maximum));
			break;
		case DUCKDB_TYPE_DOUBLE:
			sprintf(constraintStr,
				"WHERE (%s >= %lf AND %s <= %lf)",
				keyColName, *((double*)minimum), keyColName, *((double*)maximum));
			break;
		default:
			sprintf(constraintStr, " ");// perform the query without constraints
			break;
	}
	*/
	char constraintStr[50] = " ";
	char queryStr[50 + strlen(tblName) + strlen(constraintStr)];
	sprintf(queryStr, "SELECT * FROM %s %s;", tblName, constraintStr);
	//printf("%s\n", queryStr);
	return duckdb_query(con, queryStr, res_ptr);
}

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
) {
	// Wrap keys
	struct futhark_u8_2d *inKeys_ft = futhark_new_u8_2d(ctx, inCol, card, num_bytes);
	// Perform the partitioning
	struct futhark_opaque_partitionedSet_GFUR *partitionRes;
	futhark_entry_partition_and_deepen_GFUR(
		ctx, &partitionRes, (int16_t)scatter_psize, scatter_psize, radix_bits, inKeys_ft, offset, size_thresh, max_depth
	);
	// Project fields
	futhark_project_opaque_partitionedSet_GFUR_ks(ctx, outCol, partitionRes);
	futhark_project_opaque_partitionedSet_GFUR_idx(ctx, outIdx, partitionRes);
	// Sync context, cleanup
	//futhark_context_sync(ctx);
	futhark_free_u8_2d(ctx, inKeys_ft);
	futhark_free_opaque_partitionedSet_GFUR(ctx, partitionRes);
}

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
) {
	// Wrap keys
	struct futhark_u8_2d *inKeys_ft = futhark_new_u8_2d(ctx, inKeys, card, num_bytes);
	struct futhark_u8_2d *inPayloads_ft = futhark_new_u8_2d(ctx, inPayloads, card, num_bytes);
	// Perform the partitioning
	struct futhark_opaque_partitionedSet_GFTR *partitionRes;
	futhark_entry_partition_and_deepen_GFTR(
		ctx, &partitionRes, (int16_t)scatter_psize, scatter_psize, radix_bits, inKeys_ft, inPayloads_ft, size_thresh, max_depth
	);
	// Project fields
	futhark_project_opaque_partitionedSet_GFTR_ks(ctx, outKeys, partitionRes);
	futhark_project_opaque_partitionedSet_GFTR_pL(ctx, outPayloads, partitionRes);
	// Sync context, cleanup
	//futhark_context_sync(ctx);
	futhark_free_u8_2d(ctx, inKeys_ft);
	futhark_free_u8_2d(ctx, inPayloads_ft);
	futhark_free_opaque_partitionedSet_GFTR(ctx, partitionRes);
}

void getPartitionInfo(
	struct futhark_context *ctx,
	struct futhark_opaque_partitionInfo **outInfo,
	struct futhark_u8_2d *keys,
	int32_t radix_bits,
	idx_t size_thresh,
	int32_t max_depth,
	idx_t offset
) {
	futhark_entry_calc_partitions_from_partitioned_set(
		ctx, outInfo, radix_bits, keys, offset, size_thresh, max_depth
	);
}

void getRadixHashTable(
	struct futhark_context *ctx,
	struct futhark_opaque_radix_hashTable **outTable,
	struct futhark_u8_2d *keys,
	struct futhark_opaque_partitionInfo *pInfo,
	int32_t radix_bits,
	idx_t scatter_psize
) {
	futhark_entry_create_hash_table_from_partitioned_set(
		ctx, outTable, radix_bits, keys, pInfo, scatter_psize
	);
}

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
	struct futhark_opaque_partitionInfo *info1,
	struct futhark_opaque_partitionInfo *info2,
	struct futhark_opaque_radix_hashTable *hash_tbl2,
	idx_t card1,
	idx_t card2,
	idx_t scatter_psize
) {
	// Prepare output array
	struct futhark_opaque_joinPairs_bsq *joinPairs;
	// Do the join
	futhark_entry_Inner_Radix_Hash_Join(
		ctx, &joinPairs, radix_bits, keys1, keys2, info1, info2, hash_tbl2, scatter_psize
	);
	struct futhark_u8_2d *outVs_ft;
	futhark_project_opaque_joinPairs_bsq_vs(ctx, &outVs_ft, joinPairs);
	// Sync
	futhark_context_sync(ctx);
	// Project joinPair
	futhark_project_opaque_joinPairs_bsq_ix(ctx, outIdx1, joinPairs);
	futhark_project_opaque_joinPairs_bsq_iy(ctx, outIdx2, joinPairs);
	// Obtain number of pairs
	*numPairs = futhark_shape_u8_2d(ctx, outVs_ft)[0];
	// Unwrap vs
	*outVs_dptr = malloc((*numPairs)*num_bytes);
	futhark_values_u8_2d(ctx, outVs_ft, *outVs_dptr);
	// Sync
	futhark_context_sync(ctx);
	///*
	printf("Join Phase\n");
	for(idx_t i=0; i<*numPairs; i++) {
		idx_t l1, l2;
		futhark_index_i64_1d(ctx, &l1, *outIdx1, i);
		futhark_index_i64_1d(ctx, &l2, *outIdx2, i);
		printf("Current indices : %ld & %ld\n ", l1, l2);
	}
	//*/
	// Cleanup
	futhark_free_u8_2d(ctx, outVs_ft);
	futhark_free_opaque_joinPairs_bsq(ctx, joinPairs);
	return;
}

///*
void gatherPayloads_GFTR(
	struct futhark_context *ctx,
	char *outCol,
	idx_t payloadBytes,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	struct futhark_u8_2d *inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Gather
	struct futhark_u8_2d *outCol_ft;
	futhark_entry_gather_payloads_GFTR(ctx, &outCol_ft, incr, block_size, gatherIs, payloadBytes, inCol);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_u8_2d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_u8_2d(ctx, outCol_ft);
}
//*/

void gatherPayloads_GFUR_inFuthark(
	struct futhark_context *ctx,
	char *outCol,
	idx_t payloadBytes,
	idx_t sort_incr,
	idx_t join_incr,
	const int16_t block_size,
	struct futhark_i64_1d *sort_gatherIs,
	struct futhark_i64_1d *join_gatherIs,
	char *inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	// Gather indices
	struct futhark_i64_1d *true_gatherIs;
	futhark_entry_gather_payloads_long(ctx, &true_gatherIs, join_incr, block_size, join_gatherIs, sort_gatherIs);
	// Gather
	struct futhark_u8_2d *inCol_ft = futhark_new_u8_2d(ctx, inCol, card_columns, payloadBytes);
	struct futhark_u8_2d *outCol_ft;
	futhark_entry_gather_payloads_GFTR(ctx, &outCol_ft, sort_incr, block_size, true_gatherIs, payloadBytes, inCol_ft);
	// Sync
	futhark_context_sync(ctx);
	// Unwrap
	futhark_values_u8_2d(ctx, outCol_ft, outCol);
	// Sync
	futhark_context_sync(ctx);
	// Cleanup
	futhark_free_u8_2d(ctx, outCol_ft);
	futhark_free_i64_1d(ctx, true_gatherIs);
}

/*
void gatherPayloads_GFTR(
	struct futhark_context *ctx,
	char *outCol,
	idx_t payloadBytes,
	idx_t incr,
	const int16_t block_size,
	struct futhark_i64_1d *gatherIs,
	struct futhark_u8_2d *inCol,
	idx_t card_columns,
	idx_t numPairs
) {
	idx_t dummyIdx[card_columns];
	for(idx_t i=0; i<card_columns; i++) {
		dummyIdx[i] = i;
	}
	struct futhark_i64_1d *dummyIdx_ft = futhark_new_i64_1d(ctx, dummyIdx, card_columns);
	char dumb[card_columns*payloadBytes];
	futhark_values_u8_2d(ctx, inCol, dumb);
	gatherPayloads_GFUR_inFuthark(
		ctx,
		outCol,
		payloadBytes,
		0,
		incr,
		block_size,
		dummyIdx_ft,
		gatherIs,
		dumb,
		card_columns,
		numPairs
	);
	futhark_free_i64_1d(ctx, dummyIdx_ft);
}
*/