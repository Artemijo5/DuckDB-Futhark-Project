#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"

#include "ftSMJ.h"
#include "mylogger.h"
#include "sort_util.h"

#ifndef SORTSTAGES_H

/**
 * A function to perform a (GPU-based) two-pass sort over a duckdb table,
 * storing the result into a new table of the name specified.
 * 
 * Params:
 * CHUNK_SIZE : the maximum number of rows per chunk read by duckdb (make sure it is always duckdb_vector_size())
 * BUFFER_SIZE : the number of rows per column the sorting buffer is to hold
 * block_size : used by the blocked gpu sorting function
 * logfile : the file used by the logger
 * ctx : the futhark context
 * con : the duckdb connection
 * tblName : name of the unsorted table
 * intermName : base for the names of intermediate tables that will be used
 * finalName : name of the final, sorted table
 * blocked : if true use blocked radix sort, if false use merge sort
 * quickSaves : if true, flush appenders chunk-by-chunk
 */
void two_pass_sort_with_payloads(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t block_size,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  const char* finalName,
  int blocked,
  int quicksaves,
  int saveAsTempTable
);

/**
 * A function to perform a (GPU-based) two-pass sort over the key column of a duckdb table,
 * storing the sorted key column (with the reordered indices) into a new table of the name specified.
 * 
 * Params:
 * CHUNK_SIZE : the maximum number of rows per chunk read by duckdb (make sure it is always duckdb_vector_size())
 * BUFFER_SIZE : the number of rows per column the sorting buffer is to hold
 * block_size : used by the blocked gpu sorting function
 * logfile : the file used by the logger
 * ctx : the futhark context
 * con : the duckdb connection
 * tblName : name of the unsorted table
 * intermName : base for the names of intermediate tables that will be used
 * finalName : name of the final, sorted table
 * blocked : if true use blocked radix sort, if false use merge sort
 * quickSaves : if true, flush appenders chunk-by-chunk
 */
void two_pass_sort_without_payloads(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t block_size,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  const char* finalName,
  int blocked,
  int quicksaves,
  int saveAsTempTable
);

#endif