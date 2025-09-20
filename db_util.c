#include "db_util.h"

#include <stdio.h>
#include <stdlib.h>
#include "duckdb.h"
#include <string.h>

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

idx_t bulk_fetch_chunks(duckdb_result res, idx_t num_chunks, dukdb_data_chunk* out_chunks) {
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
void bulk_buffer_chunks(
	duckdb_data_chunk* chunks,
	idx_t num_chunks,
	idx_t col_count,
	idx_t excludeCol,
	duckdb_type* types,
	void** dests,
	void* excludeDest
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

idx_t bulk_proc_chunks(
	idx_t CHUNK_SIZE,
	duckdb_result res,
	idx_t num_chunks,
	idx_t col_count,
	idx_t excludeCol,
	duckdb_type* types,
	void** dests,
	void* excludeDest,
	idx_t capacity,
	bool* exhaustedRes
) {
	idx_t row_count = 0;
	*exhaustedRes = false;

	while(!(*exhausted_res) && row_count < (capacity - CHUNK_SIZE*num_chunks)) {
		duckdb_data_chunk chunks[num_chunks];
		idx_t chunks_read = bulk_fetch_chunks(res, num_chunks, chunks);
		if(chunks_read < num_chunks) *exhaustedRes = true; // Exhausted result TODO log?
		if(chunks_read == 0) break; // no chunks left
		idx_t rows_buffd = bulk_buffer_chunks(
			chunks, chunks_read, col_count, excludeCol, types, dests, excludeDest, row_count
		);
		if(rows_buffd % CHUNK_SIZE != 0) *exhaustedRes = true; // Exhausted result
		row_count += rows_buffd;
	}
	return row_count;
}