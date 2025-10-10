#include "db_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include <string.h>

#include <limits.h> // for maximum values of integral types
#include <float.h> // for maximum values of floating-point types

size_t colType_bytes(duckdb_type type) {
  switch (type){
    case DUCKDB_TYPE_SMALLINT:
      return sizeof(int16_t);
    case DUCKDB_TYPE_INTEGER:
      return sizeof(int32_t);
    case DUCKDB_TYPE_BIGINT:
      return sizeof(int64_t);
    case DUCKDB_TYPE_FLOAT:
      return sizeof(float);
    case DUCKDB_TYPE_DOUBLE:
      return sizeof(double);
    default:
      perror("Invalid type.");
      return 0;
  }
}

void* colType_malloc(duckdb_type type, idx_t card) {
  size_t ms = colType_bytes(type);
  if (ms == 0) return NULL;
  return malloc(card * ms);
}

int colType_name(duckdb_type type, char *dest_str) {
	switch(type) {
    case DUCKDB_TYPE_SMALLINT:
      return sprintf(dest_str, "SMALLINT" );
    case DUCKDB_TYPE_INTEGER:
      return sprintf(dest_str, "INTEGER" );
    case DUCKDB_TYPE_BIGINT:
      return sprintf(dest_str, "BIGINT" );
    case DUCKDB_TYPE_FLOAT:
      return sprintf(dest_str, "FLOAT" );
    case DUCKDB_TYPE_DOUBLE:
      return sprintf(dest_str, "DOUBLE" );
    default:
      perror("Invalid type.");
      return -1;
	}
}

void max_padding(void* dest, duckdb_type type, idx_t n) {
  for(idx_t i=0; i<n; i++) {
    switch(type) {
      case DUCKDB_TYPE_SMALLINT:
        ((short*)dest)[i] = SHRT_MAX;
        break;
      case DUCKDB_TYPE_INTEGER:
        ((int*)dest)[i] = INT_MAX;
        break;
      case DUCKDB_TYPE_BIGINT:
        ((long*)dest)[i] = LONG_MAX;
        break;
      case DUCKDB_TYPE_FLOAT:
        ((float*)dest)[i] = FLT_MAX;
        break;
      case DUCKDB_TYPE_DOUBLE:
        ((double*)dest)[i] = DBL_MAX;
        break;
      default:
        perror("Invalid type.");
        return;
    }
  }
}

void payloadColumnsToByteArray(
  char** outBytes,
  idx_t* pL_rowBytes,
  duckdb_type* payload_types,
  void** inPayloads,
  idx_t pL_col_count,
  idx_t row_count
) {
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[pL_col_count]; // bytes taken by each column
  idx_t pL_prefixSizes[pL_col_count]; // bytes taken up to each column
  for(idx_t col=0; col<pL_col_count; col++) {
    pL_byteSizes[col] = colType_bytes(payload_types[col]);
    pL_prefixSizes[col] = pL_bytes;
    pL_bytes += pL_byteSizes[col];
  }
  // Allocate
  *outBytes = malloc(row_count*pL_bytes);
  // Transform - strided access pattern
  for(idx_t r=0; r<row_count; r++) {
    for(idx_t col=0; col<pL_col_count; col++) {
      memcpy(
        (*outBytes) + r*pL_bytes + pL_prefixSizes[col],
        &((char*)inPayloads[col])[r*pL_byteSizes[col]],
        pL_byteSizes[col]
      );
    }
  }
  *pL_rowBytes = pL_bytes;
}

void payloadColumnsFromByteArray(
  void** outPayloads,
  duckdb_type* payload_types,
  char* inBytes,
  idx_t pL_col_count,
  idx_t row_count
) {
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[pL_col_count]; // bytes taken by each column
  idx_t pL_prefixSizes[pL_col_count]; // bytes taken up to each column
  for(idx_t col=0; col<pL_col_count; col++) {
    pL_byteSizes[col] = colType_bytes(payload_types[col]);
    pL_prefixSizes[col] = pL_bytes;
    pL_bytes += pL_byteSizes[col];
  }
  for(idx_t col=0; col<pL_col_count; col++) {
    outPayloads[col] = colType_malloc(payload_types[col], row_count);
  }
  for(idx_t r=0; r<row_count; r++) {
    for(idx_t col=0; col<pL_col_count; col++) {
      memcpy(
        outPayloads[col] + r*pL_byteSizes[col],
        inBytes + r*pL_bytes + pL_prefixSizes[col],
        pL_byteSizes[col]
      );
    }
  }
}

idx_t bulk_fetch_chunks(duckdb_result res, idx_t num_chunks, duckdb_data_chunk* out_chunks) {
	idx_t total_read = 0;
	for(idx_t j=0; j<num_chunks; j++) {
		duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
		if(!cnk) {
			// Result is exhausted.
			break;
		}
		total_read++;
		out_chunks[j] = cnk;
	}
	return total_read;
}

// TODO optimise as needed
idx_t bulk_buffer_chunks(
	duckdb_data_chunk* chunks,
	idx_t num_chunks,
	idx_t col_count,
	idx_t excludeCol,
	duckdb_type* types,
	void** dests,
	void* excludeDest,
	idx_t offset
) {
	idx_t row_count = 0;
	for(idx_t j=0; j<num_chunks; j++) {
		duckdb_data_chunk cnk = chunks[j];
		idx_t cnk_rows = duckdb_data_chunk_get_size(cnk);
		for(idx_t c=0; c<col_count; c++) { // buffer non-excluded columns
			if(c == excludeCol) continue;
			idx_t accIdx = (c < excludeCol || (excludeCol<0))? c: c-1;

			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
			void *dat = duckdb_vector_get_data(vec);

			memcpy(
				dests[accIdx] + (offset+row_count)*colType_bytes(types[c]),
				dat,
				cnk_rows*colType_bytes(types[c])
			);
		}
		if(excludeDest && excludeCol>=0) { // buffer excluded column, if exists and dest is not NULL
			duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, excludeCol);
			void *dat = duckdb_vector_get_data(vec);
			memcpy(
				excludeDest + (offset+row_count)*colType_bytes(types[excludeCol]),
				dat,
				cnk_rows*colType_bytes(types[excludeCol])
			);
		}
		row_count += cnk_rows;

		duckdb_destroy_data_chunk(&cnk); // TODO here or all in the end?
	}
	return row_count;
}
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
) {
	idx_t row_count = 0;
	*exhaustedRes = false;

	while(!(*exhaustedRes)) {
		idx_t remaining_space_in_buffer = (capacity - row_count)/CHUNK_SIZE;
		idx_t chunks_to_read = (remaining_space_in_buffer > num_chunks)? num_chunks: remaining_space_in_buffer;
		if(chunks_to_read==0) break;

		duckdb_data_chunk chunks[chunks_to_read];
		idx_t chunks_read = bulk_fetch_chunks(res, chunks_to_read, chunks);
		if(chunks_read < chunks_to_read) *exhaustedRes = true; // Exhausted result TODO log?
		if(chunks_read == 0) break; // no chunks left
		idx_t rows_buffd = bulk_buffer_chunks(
			chunks, chunks_read, col_count, excludeCol, types, dests, excludeDest, row_count
		);
		//if(rows_buffd % CHUNK_SIZE != 0) *exhaustedRes = true; // Apparently this is not the case when using the WHERE clause...
		row_count += rows_buffd;
	}
	return row_count;
}

void payloadColumnsToByteArray_preallocated(
  char* outBytes,
  idx_t pL_bytes,
  idx_t* pL_byteSizes,
  idx_t* pL_prefixSizes,
  void** inPayloads,
  idx_t pL_col_count,
  idx_t row_count
) {
	// TODO is this or reverse order faster?
  for(idx_t r=0; r<row_count; r++) {
    for(idx_t col=0; col<pL_col_count; col++) {
      memcpy(
        outBytes + r*pL_bytes + pL_prefixSizes[col],
        &((char*)inPayloads[col])[r*pL_byteSizes[col]],
        pL_byteSizes[col]
      );
    }
  }
}
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
) {
	idx_t row_count = 0;
	for(idx_t j=0; j<num_chunks; j++) {
		duckdb_data_chunk cnk = chunks[j];
		idx_t cnk_rows = duckdb_data_chunk_get_size(cnk);
		// buffer key column
		duckdb_vector kvec = duckdb_data_chunk_get_vector(cnk, keyCol);
		void *kdat = duckdb_vector_get_data(kvec);
		memcpy(
			keyDest + (offset+row_count)*colType_bytes(types[keyCol]),
			kdat,
			cnk_rows*colType_bytes(types[keyCol])
		);
		// buffer payload columns
		if(pL_bytes > 0 && plDest) {
			void* pL_dats[col_count-1];
			for(idx_t c=0; c<col_count; c++) {
				if(c == keyCol) continue;
				idx_t accIdx = (c < keyCol)? c: c-1;

				duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
				void *dat = duckdb_vector_get_data(vec);
				pL_dats[accIdx] = dat;
			}
			payloadColumnsToByteArray_preallocated(
			  plDest + (offset+row_count)*pL_bytes,
			  pL_bytes,
			  pL_byteSizes,
			  pL_prefixSizes,
			  pL_dats,
			  col_count-1,
			  cnk_rows
			);
		}
		row_count += cnk_rows;

		duckdb_destroy_data_chunk(&cnk); // TODO here or all in the end?
	}
	return row_count;
}

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
) {
	idx_t row_count = 0;
	*exhaustedRes = false;

	idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[col_count-1]; // bytes taken by each column
  idx_t pL_prefixSizes[col_count-1]; // bytes taken up to each column
  duckdb_type payload_types[col_count-1]; // types of payload columns only
  for(idx_t col=0; col<col_count; col++) {
    if(col == keyCol) continue; // payloads only
    idx_t accIdx = (col < keyCol)? col: col-1; // adjust index
    pL_byteSizes[accIdx] = colType_bytes(types[col]);
    pL_prefixSizes[accIdx] = pL_bytes;
    pL_bytes += pL_byteSizes[accIdx];
    payload_types[accIdx] = types[col];
  }
  if(pL_bytes>0 && plDest) *plDest = malloc(pL_bytes * capacity); // MUST BE FREED MANUALLY !!!

	while(!(*exhaustedRes)) {
		idx_t remaining_space_in_buffer = (capacity - row_count)/CHUNK_SIZE;
		idx_t chunks_to_read = (remaining_space_in_buffer > num_chunks)? num_chunks: remaining_space_in_buffer;
		if(chunks_to_read==0) break;

		duckdb_data_chunk chunks[chunks_to_read];
		idx_t chunks_read = bulk_fetch_chunks(res, chunks_to_read, chunks);
		if(chunks_read < chunks_to_read) *exhaustedRes = true; // Exhausted result TODO log?
		if(chunks_read == 0) break; // no chunks left
		char* actual_plDest = (pL_bytes>0 && plDest)? *plDest: NULL;
		idx_t rows_buffd = bulk_buffer_chunks_GFTR(
			chunks, chunks_read, col_count, keyCol, types, pL_bytes, pL_byteSizes, pL_prefixSizes, keyDest, actual_plDest, row_count
		);
		//if(rows_buffd % CHUNK_SIZE != 0) *exhaustedRes = true; // Apparently this is not the case when using the WHERE clause...
		row_count += rows_buffd;
	}
	return row_count;
}

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
) {
  // 0 turn types into logical_types & strings to create the table
  duckdb_logical_type ltypes[col_count];
  char type_strs[col_count][25];
  for(idx_t i=0; i<col_count; i++) {
    ltypes[i] = duckdb_create_logical_type(types[i]);
    colType_name(types[i], type_strs[i]);
  }

  // 1 create temporary table
  char tblName[strlen(intermName) + 25];
  sprintf(tblName, "%s%ld", intermName, numInter);
  char queryStr[100 + 35*col_count];
  int queryStr_len = sprintf(queryStr, "CREATE OR REPLACE TEMP TABLE %s (", tblName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s, ", colNames[i], type_strs[i]);
    }
    else {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s);", colNames[i], type_strs[i]);
    }
  }
  // TODO for testing
  //printf("%s\n", queryStr);
  if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
    perror("Failed to create temporary table.\n");
    return -1;
  }
  // TODO for testing
  //printf("Created temporary table #%ld.\n", numInter);
  // 2 create an appender for the table
  duckdb_appender tmp_appender;
  if( duckdb_appender_create(con, NULL, tblName, &tmp_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return -1;
  }
  //printf("Created appender #%ld.\n", numInter);

  // 3 insert the data into the table, 1 chunk at a time
  for(idx_t r=0; r<row_count; r+=chunkSize) {
    size_t this_size = (row_count-r > chunkSize)? chunkSize: row_count-r;
    duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, col_count);
    duckdb_data_chunk_set_size(cnk, this_size);
    for(idx_t c=0; c<col_count; c++) {
      duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
      void *dat = (void*)duckdb_vector_get_data(vec);
      memcpy(dat, &((char*)BuffersIn[c])[r*colType_bytes(types[c])], this_size*colType_bytes(types[c]));
    }
    if(duckdb_append_data_chunk(tmp_appender, cnk) == DuckDBError) {
      perror("Failed to append data chunk.\n");
      return -1;
    }
    // TODO
    // Flush every 60 (or multiple) chunks (row group size)
    //if((1+(r/chunkSize))%300 == 0) duckdb_appender_flush(tmp_appender);

    //printf("Appended %ld elements.\n", this_size);
    //duckdb_appender_flush(tmp_appender);
    duckdb_destroy_data_chunk(&cnk);
  }
  //duckdb_appender_flush(tmp_appender);
  // IF USING PARQUET STORAGE:
  /*
  char storagePart[100 + 2*strlen(tblName)];
  sprintf(storagePart, "COPY %s TO %s.parquet (FORMAT parquet);", tblName, tblName);
  char clearPart[100];
  sprintf(clearPart, "DROP TABLE %s;", tblName);
  if(duckdb_query(con, storagePart, NULL) == DuckDBError) {
    perror("Failed to spill intermediate result to disk.");
    return -1;
  }
  if(duckdb_query(con, clearPart, NULL) == DuckDBError) {
    perror("Failed to drop temp table.");
    return -1;
  }
  */

  // Cleanup
  duckdb_appender_destroy(&tmp_appender);
  for(int i=0; i<col_count; i++) {
    duckdb_destroy_logical_type(&ltypes[i]);
  }

  return numInter + 1;
}

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
) {
  // 0 turn types into logical_types & strings to create the table
  duckdb_logical_type ltypes[col_count];
  char type_strs[col_count][25];
  for(idx_t i=0; i<col_count; i++) {
    ltypes[i] = duckdb_create_logical_type(types[i]);
    colType_name(types[i], type_strs[i]);
  }

  // 1 create temporary table
  char tblName[strlen(intermName) + 25];
  sprintf(tblName, "%s%ld", intermName, numInter);
  char queryStr[100 + 35*col_count];
  int queryStr_len = sprintf(queryStr, "CREATE OR REPLACE TEMP TABLE %s (", tblName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s, ", colNames[i], type_strs[i]);
    }
    else {
      queryStr_len += sprintf(queryStr + queryStr_len, "%s %s);", colNames[i], type_strs[i]);
    }
  }
  if( duckdb_query(con, queryStr, NULL) == DuckDBError ) {
    perror("Failed to create temporary table.\n");
    return -1;
  }
  // 2 create an appender for the table
  duckdb_appender tmp_appender;
  if( duckdb_appender_create(con, NULL, tblName, &tmp_appender) == DuckDBError ) {
    perror("Failed to create appender.\n");
    return -1;
  }

  // pre-3 : calculate payload column stuff...
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_prefixSizes[col_count-1]; // bytes taken up to each column
  for(idx_t col=0; col<col_count; col++) {
    if(col == keyCol_idx) continue;
    idx_t accIdx = (col < keyCol_idx)? col: col-1;
    pL_prefixSizes[accIdx] = pL_bytes;
    pL_bytes += colType_bytes(types[col]);
  }

  // 3 insert the data into the table, 1 chunk at a time
  for(idx_t r=0; r<row_count; r+=chunkSize) {
    size_t this_size = (row_count-r > chunkSize)? chunkSize: row_count-r;
    duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, col_count);
    duckdb_data_chunk_set_size(cnk, this_size);
    for(idx_t c=0; c<col_count; c++) {
      duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
      void *dat = (void*)duckdb_vector_get_data(vec);
      idx_t col_size = colType_bytes(types[c]);
      if(c == keyCol_idx) {
        memcpy(dat, key_data + r*col_size, this_size*col_size);
      }
      else { // Strided access pattern for payloads
        idx_t accIdx = (c < keyCol_idx)? c: c-1;
        for(idx_t j=0; j<this_size; j++) {
          memcpy(
            dat + j*col_size,
            pL_data + j*pL_bytes + pL_prefixSizes[accIdx],
            col_size
          );
        }
      }
    }
    if(duckdb_append_data_chunk(tmp_appender, cnk) == DuckDBError) {
      perror("Failed to append data chunk.\n");
      return -1;
    }
    // TODO
    // Flush manually (?)
    //if((1+(r/chunkSize))%20 == 0) duckdb_appender_flush(tmp_appender);

    //printf("Appended %ld elements.\n", this_size);
    //duckdb_appender_flush(tmp_appender);
    duckdb_destroy_data_chunk(&cnk);
  }
  //duckdb_appender_flush(tmp_appender);
  // IF USING PARQUET STORAGE:
  /*
  char storagePart[100 + 2*strlen(tblName)];
  sprintf(storagePart, "COPY %s TO %s.parquet (FORMAT parquet);", tblName, tblName);
  char clearPart[100];
  sprintf(clearPart, "DROP TABLE %s;", tblName);
  if(duckdb_query(con, storagePart, NULL) == DuckDBError) {
    perror("Failed to spill intermediate result to disk.");
    return -1;
  }
  if(duckdb_query(con, clearPart, NULL) == DuckDBError) {
    perror("Failed to drop temp table.");
    return -1;
  }
  */

  // Cleanup
  duckdb_appender_destroy(&tmp_appender);
  for(int i=0; i<col_count; i++) {
    duckdb_destroy_logical_type(&ltypes[i]);
  }

  return numInter + 1;
}

void prepareToFetch_intermediate(
  idx_t numInter,
  const char *intermName,
  duckdb_connection con,
  duckdb_result *result_ptr
) {
  char tblName[strlen(intermName) + 25];
  // IF _NOT_ USING PARQUET STORAGE:
  sprintf(tblName, "%s%ld", intermName, numInter);
  // IF USING PARQUET STORAGE:
  //sprintf(tblName, "tempholder%ld.parquet", numInter);
  char queryStr[100];
  sprintf(queryStr, "SELECT * FROM %s;", tblName);
  // TODO for testing
  //printf("%s\n", queryStr);
  if( duckdb_query(con, queryStr, result_ptr) == DuckDBError ) {
    perror("Failed to retrieve intermediate.");
  }
  // TODO better error handling
}

idx_t fetch_intermediate(
  duckdb_result result,
  idx_t col_count,
  duckdb_type* types,
  void** BuffersOut,
  idx_t start_idx
) {
  duckdb_data_chunk cnk = duckdb_fetch_chunk(result);
  if(!cnk) {
    // Result is exhausted.
    return 0;
  }
  idx_t row_count = duckdb_data_chunk_get_size(cnk);
  // TODO bulk-buffer - also am I using this func rn?
  for(idx_t c=0; c<col_count; c++) {
    duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
    void* dat = duckdb_vector_get_data(vec);
    size_t colSize = colType_bytes(types[c]);
    char *curBuffer = (char*)BuffersOut[c];
    memcpy(curBuffer + start_idx*colSize, dat, row_count*colSize);
  }
  duckdb_destroy_data_chunk(&cnk);
  return row_count;
}
