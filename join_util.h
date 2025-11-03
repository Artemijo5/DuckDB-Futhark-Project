#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include "ftRelational.h"

#ifndef JOIN_UTIL_H
#define JOIN_UTIL_H

int join_preparation(
	FILE *logfile,
	duckdb_connection con,
	const char* R_tbl_name,
	const char* S_tbl_name,
	const char* Join_tbl_name,
	int is_R_semisorted,
	int is_S_semisorted,
	const char* R_keyName,
	const char* S_keyName,
	duckdb_type *key_type,
	duckdb_result *out_res_R,
	duckdb_appender *join_appender,
	idx_t *R_keyCol_Idx,
	idx_t *S_keyCol_Idx,
	idx_t *R_col_count,
	idx_t *S_col_count,
	idx_t *R_pL_bytesPerRow,
	idx_t *S_pL_bytesPerRow,
	duckdb_type **R_type_ids,
	duckdb_type **S_type_ids,
	duckdb_type **R_payloadTypes,
	duckdb_type **S_payloadTypes,
	duckdb_logical_type **join_type_ids,
	int is_in_GFUR,
	int saveAsTempTable
);

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
);

#endif