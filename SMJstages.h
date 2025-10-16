#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "smjutil.h"

#ifndef SMJSTAGES_H
#define SMJSTAGES_H

void Inner_MergeJoin_GFTR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *sorted_R_tbl_name,
	const char *sorted_S_tbl_name,
	const char *R_keyName,
	const char *S_keyName,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
);

void Inner_MergeJoin_GFUR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	idx_t PAYLOAD_INDEX_BLOCK,
	idx_t PAYLOAD_GATHER_BLOCK,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *R_tbl_name,
	const char *S_tbl_name,
	const char *sorted_R_tbl_name,
	const char *sorted_S_tbl_name,
	const char *R_keyName,
	const char *S_keyName,
	const char *Join_temp_tbl_name,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
);

// TODO add more parametres for the sorting part
void SortMergeJoin_GFTR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *R_tbl_name,
	const char *S_tbl_name,
	int is_R_sorted,
	int is_S_sorted,
	const char *R_keyName,
	const char *S_keyName,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
);

// TODO add more parametres for the sorting part
void SortMergeJoin_GFUR(
	idx_t CHUNK_SIZE,
	idx_t R_JOIN_BUFFER,
	idx_t S_JOIN_BUFFER,
	int16_t BLOCK_SIZE,
	idx_t EXT_PARALLELISM,
	idx_t MERGE_PARTITION_SIZE,
	idx_t RESCALE_FACTOR,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	const char *R_tbl_name,
	const char *S_tbl_name,
	int is_R_sorted,
	int is_S_sorted,
	const char *R_keyName,
	const char *S_keyName,
	const char *Join_tbl_name,
	int quicksaves,
	int saveAsTempTable
);


#endif