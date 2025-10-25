#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "radixJoin_util.h"

#ifndef RADIXJOINSTAGES_H
#define RADIXJOINSTAGES_H

void RadixHashJoin_GFTR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  int32_t radix_bits,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_partitioned,
  int is_S_partitioned,
  int is_R_unique,
  int is_S_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
);

void RadixHashJoin_GFUR(
  idx_t CHUNK_SIZE,
  idx_t R_JOIN_BUFFER,
  idx_t S_JOIN_BUFFER,
  int16_t BLOCK_SIZE,
  idx_t SCATTER_PSIZE,
  idx_t MAX_PARTITION_SIZE,
  int32_t MAX_DEPTH,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  int32_t radix_bits,
  const char *R_tbl_name,
  const char *S_tbl_name,
  int is_R_partitioned,
  int is_S_partitioned,
  int is_R_unique,
  int is_S_unique,
  const char *R_keyName,
  const char *S_keyName,
  const char *Join_tbl_name,
  int quicksaves,
  int saveAsTempTable
);

#endif