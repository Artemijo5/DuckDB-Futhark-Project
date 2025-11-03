#include "sortstages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftRelational.h"
#include "mylogger.h"
#include "sort_util.h"
#include "db_util.h"

#include <limits.h> // for maximum values of integral types
#include <float.h> // for maximum values of floating-point types

// returns numIntermediate
// incr_idx is also updated
idx_t sort_Stage1_with_payloads(
	idx_t chunk_size,
	size_t buffer_size,
  const int16_t block_size,
  const char* intermName,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	duckdb_result res,
	idx_t col_count,
	duckdb_type* type_ids,
  char** colNames,
  idx_t keyCol_idx,
	idx_t *incr_idx,
  int blocked
) {
  idx_t numIntermediate = 0;

  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    void *keyBuffer = colType_malloc(type_ids[keyCol_idx], buffer_size);
    char* payloadBuffer;
    mylog(logfile, "Initalising buffers for each column...");
    idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
    idx_t pL_byteSizes[col_count-1]; // bytes taken by each column
    idx_t pL_prefixSizes[col_count-1]; // bytes taken up to each column
    duckdb_type payload_types[col_count-1]; // types of payload columns only
    for(idx_t col=0; col<col_count; col++) {
      if(col == keyCol_idx) continue; // payloads only
      idx_t accIdx = (col < keyCol_idx)? col: col-1; // adjust index
      pL_byteSizes[accIdx] = colType_bytes(type_ids[col]);
      pL_prefixSizes[accIdx] = pL_bytes;
      pL_bytes += pL_byteSizes[accIdx];
      payload_types[accIdx] = type_ids[col];
    }
    
    int exhaustedRes;
    idx_t cur_rows = bulk_load_chunks_GFTR(
      chunk_size,
      res,
      60, // TODO make argument
      col_count,
      keyCol_idx,
      type_ids,
      keyBuffer,
      &payloadBuffer,
      buffer_size,
      &exhaustedRes
    );
    flag = !exhaustedRes;

    if(!flag && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      free(keyBuffer);
      free(payloadBuffer);
      break;
    }

    char msgbuffer[100];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // Sort
    sortRelationByKey(
      ctx,
      keyBuffer,
      payloadBuffer,
      type_ids[keyCol_idx],
      blocked,
      block_size,
      keyBuffer,
      payloadBuffer,
      pL_bytes,
      cur_rows
    );
    mylog(logfile, "Performed sorting.");

    numIntermediate = store_intermediate_GFTR(
      numIntermediate,
      intermName,
      con,
      chunk_size,
      col_count,
      cur_rows,
      type_ids,
      colNames,
      keyBuffer,
      payloadBuffer,
      keyCol_idx
    );
    if(numIntermediate == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
    free(keyBuffer);
    free(payloadBuffer);
    mylog(logfile, "Freed this page's buffers.");

    *incr_idx += cur_rows;
  }

  return numIntermediate;
}

// returns numIntermediate
// incr_idx is also updated
idx_t sort_Stage1_without_payloads(
	idx_t chunk_size,
	size_t buffer_size,
  const int16_t block_size,
  const char* intermName,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	duckdb_result res,
	duckdb_type type_id,
  char** colNames,
	idx_t *incr_idx,
  int blocked
) {
	idx_t numIntermediate = 0;

  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    mylog(logfile, "Initalising buffer for the key column...");
    void *Buffer = colType_malloc(type_id, buffer_size);
    if(Buffer == NULL) {
      mylog(logfile, "ERROR -- failed to allocate memory.");
      return -1;
    }
    mylog(logfile, "Successfully initialised buffers.");

    /*
    idx_t cur_rows = 0;

    // iterate until result is exhausted
    while (buffer_size - cur_rows >= chunk_size) {
      duckdb_data_chunk result = duckdb_fetch_chunk(res);
      if (!result) {
        mylog(logfile, "Table has been fully processed.");
        flag = false; // last iteration of the outer loop
        break;
      }
      // get the number of rows & columns from the data chunk
      idx_t row_count = duckdb_data_chunk_get_size(result);

      // obtain the column vectors
      duckdb_vector res_col = duckdb_data_chunk_get_vector(result, 0);
      void *vector_data = duckdb_vector_get_data(res_col);
      memcpy(Buffer + cur_rows*colType_bytes(type_id),
        vector_data,
        row_count * colType_bytes(type_id)
      );
      
      cur_rows += row_count;

      duckdb_destroy_data_chunk(&result);
    }
    */
    int exhaustedRes;
    idx_t cur_rows = bulk_load_chunks_GFTR(
      chunk_size,
      res,
      60, // TODO make argument
      1,
      0,
      &type_id,
      Buffer,
      NULL,
      buffer_size,
      &exhaustedRes
    );
    flag = !exhaustedRes;

    if(!flag && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      free(Buffer);
      break;
    }
    char msgbuffer[100];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // sort key column
    struct futhark_i64_1d *sorted_idx_ft;
    mylog(logfile, "Passing key column for sorting...");
    sortKeyColumn(ctx, Buffer, type_id, *incr_idx, blocked, block_size, &sorted_idx_ft, Buffer, cur_rows);
    mylog(logfile, "Sorted key column and obtained reordered indices.");
    
    int64_t *orderedIndices = colType_malloc(DUCKDB_TYPE_BIGINT, cur_rows);
    futhark_values_i64_1d(ctx, sorted_idx_ft, orderedIndices);

    void *Buffers[2] = {Buffer, (void*)orderedIndices};
    duckdb_type type_ids[2] = {type_id, DUCKDB_TYPE_BIGINT};

    idx_t col_count = 2;

    numIntermediate = store_intermediate(numIntermediate, intermName, con, chunk_size, col_count, cur_rows, type_ids, colNames, Buffers);
    if(numIntermediate == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
    free(orderedIndices);
    free(Buffer);
    mylog(logfile, "Freed this page's buffer.");
    futhark_free_i64_1d(ctx, sorted_idx_ft);
    mylog(logfile, "Freed futhark objects for this page.");

    *incr_idx += cur_rows;
  }

  return numIntermediate;
}

void sort_Stage2(
	idx_t CHUNK_SIZE,
	size_t BUFFER_SIZE,
  const int16_t block_size,
  const char* intermName,
  const char* finalName,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	idx_t col_count,
	duckdb_type* type_ids,
  char** colNames,
  idx_t keyCol_idx,
	idx_t numIntermediate,
  int blocked,
  int quicksaves,
  int saveAsTempTable
) {
  mylog(logfile, "Now entering the second stage of processing...");

  size_t BUFFER_CHUNK_CAPACITY = BUFFER_SIZE / CHUNK_SIZE;

  // 2nd stage requires all intermediates + at least 1 empty chunk to fit in the buffer
  if(numIntermediate > (BUFFER_CHUNK_CAPACITY - 1)) {
    perror("Insufficient buffer size.");
    return;
  }
  // Number of chunks that are picked up from the buffer into storage each iteration
  // Can increase if an intermediate is exhausted before the others (expected only for the last intermediate)
  idx_t pickup = BUFFER_CHUNK_CAPACITY - numIntermediate;
  // Number of rows to ignore, caused by not-full chunks
  // the slots of those rows in the buffer are padded with the type's maximum value, and neither read nor sorted
  // expectedly, only the last of all chunks can be not-full
  // so ignore_rows will be increased at most once, and not again
  // ALSO, a not-full chunk means that it's the last of that intermediate (assumption for duckdb's logic)
  idx_t ignore_rows = 0;
  // Number of intermediates that have not been exhausted yet
  // reduced by 1 when an intermediate is exhausted
  idx_t still_interm = numIntermediate;

  // ---------- 1. Initialise buffers.
  // maximum element of the last-read chunk of each intermediate
  // the argmin of these is the next chunk to be read
  // updated immediately when reading a new chunk
  // when an intermediate is exhausted, its maximum is set to the max of the key type
  // this creates a theoretical corner case of having eg INT_MAX in non-exhausted intermediates
  // TODO address this corner case (low priority)
  void* maxima = colType_malloc(type_ids[keyCol_idx], numIntermediate);
  // initialised as all false, a slot is set to true when its buffer is exhausted
  int intermIsExhausted[numIntermediate];
  // keyBuffer2 : holds key data
  void* keyBuffer2 = colType_malloc(type_ids[keyCol_idx], BUFFER_SIZE);
  char* payloadBuffer2;
  idx_t pL_bytes = 0; // total bytes taken by payload columns altogether
  idx_t pL_byteSizes[col_count-1]; // bytes taken by each column
  idx_t pL_prefixSizes[col_count-1]; // bytes taken up to each column
  duckdb_type pL_types[col_count-1];
  for(idx_t col=0; col<col_count; col++) {
    if(col == keyCol_idx) continue; // payloads only
    idx_t accIdx = (col < keyCol_idx)? col: col-1; // adjust index
    pL_byteSizes[accIdx] = colType_bytes(type_ids[col]);
    pL_prefixSizes[accIdx] = pL_bytes;
    pL_bytes += pL_byteSizes[accIdx];
    pL_types[accIdx] = type_ids[col];
  }
  payloadBuffer2 = malloc(pL_bytes*BUFFER_SIZE);
  // counter of full empty chunks at the end of buffer
  // set to 0 when they are replaced
  idx_t replaceChunksAtTheEnd = 0;
  mylog(logfile, "Initialised buffers for Stage 2.");

  // ---------- 2. Prepare the table where results will be stored.
  char type_strs[col_count][25];
  duckdb_logical_type ltypes[col_count];
   for(idx_t i=0; i<col_count; i++) {
    ltypes[i] = duckdb_create_logical_type(type_ids[i]);
    colType_name(type_ids[i], type_strs[i]);
  }
  char resultQueryStr[150 + strlen(finalName) + 35*col_count];
  int resultQueryStr_len = (saveAsTempTable)?
    sprintf(resultQueryStr, "CREATE OR REPLACE TEMP TABLE %s (", finalName):
    sprintf(resultQueryStr, "CREATE OR REPLACE TABLE %s (", finalName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "%s %s, ", colNames[i], type_strs[i]);
    }
    else {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "%s %s);", colNames[i], type_strs[i]);
    }
  }
  // TODO for testing
  //printf("%s\n", resultQueryStr);
  // Execute the query
  if( duckdb_query(con, resultQueryStr, NULL) == DuckDBError ) {
    perror("Failed to create result table.\n");
    return;
  }
  mylog(logfile, "Created table where the sorted results will be stored.");
  // Create appender
  duckdb_appender resultApp;
  if( duckdb_appender_create(con, NULL, finalName, &resultApp) == DuckDBError ) {
    perror("Failed to create appender for the result table.\n");
    return;
  }
  mylog(logfile, "Created appender for the result table.");

  // ---------- 3. Prepare the intermediates for scanning.
  duckdb_result interms[numIntermediate];
  for(idx_t i=0; i<numIntermediate; i++) {
    prepareToFetch_intermediate(i, intermName, con, &interms[i]);
  }
  mylog(logfile, "Prepared intermediates for scanning.");

  // ---------- 4. First scan
  // Scan the first chunk of all intermediates into the buffer
  for(idx_t i=0; i<numIntermediate; i++) {
    duckdb_data_chunk cnk = duckdb_fetch_chunk(interms[i]);
    if(!cnk) {
      mylog(logfile, "Intermediate is exhausted before scan (unexpected).");
      intermIsExhausted[i] = true;
      still_interm -= 1;
      // max-pad a chunk from the end (only the key column)
      max_padding(
        keyBuffer2 + (BUFFER_CHUNK_CAPACITY - 1 - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[keyCol_idx]),
        type_ids[keyCol_idx],
        CHUNK_SIZE
      );
      replaceChunksAtTheEnd += 1;
      // also at the maximum
      max_padding(maxima + i*colType_bytes(type_ids[keyCol_idx]), type_ids[keyCol_idx], 1);
      continue;
    }
    intermIsExhausted[i] = false;

    idx_t rcount = duckdb_data_chunk_get_size(cnk);
    if(rcount<CHUNK_SIZE) {
      ignore_rows += CHUNK_SIZE - rcount;
    }

    for(idx_t c=0; c<col_count; c++) {
      duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
      void* dat = duckdb_vector_get_data(vec);
      // for key column: note maximum
      if(c == keyCol_idx) {
        memcpy(
          maxima + i*colType_bytes(type_ids[keyCol_idx]),
          dat + (rcount-1)*colType_bytes(type_ids[keyCol_idx]),
          colType_bytes(type_ids[keyCol_idx])
        );
      }
      // copy data to buffer
      idx_t accIdx = (c<keyCol_idx)? c: c-1;
      void *dest_ptr = (c==keyCol_idx)?
       keyBuffer2 + (pickup + i - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[keyCol_idx]):
       payloadBuffer2 + (pickup + i - replaceChunksAtTheEnd)*CHUNK_SIZE*pL_bytes + pL_prefixSizes[accIdx];
      if(c==keyCol_idx) {  
        memcpy(
          dest_ptr,
          dat,
          rcount*colType_bytes(type_ids[c])
        );
      }
      else {
        for(idx_t r=0; r<rcount; r++) {
          memcpy(
            dest_ptr + r*pL_bytes,
            dat + r*pL_byteSizes[accIdx],
            pL_byteSizes[accIdx]
          );
        }
      }
      
      // max-pad remaining space within the chunk
      if(c==keyCol_idx && rcount < CHUNK_SIZE) {
        max_padding(
          dest_ptr + rcount*colType_bytes(type_ids[c]),
          type_ids[c],
          CHUNK_SIZE - rcount
        );
      }
    }
    duckdb_destroy_data_chunk(&cnk);
  }
  mylog(logfile, "Completed first scan.");
  //logarray_long(logfile, "After first scan:\n", keyBuffer2, BUFFER_SIZE);

  // ---------- 5. Scan & Sort - loop.
  while(still_interm) {
    // 5.i scan the next pickup chunks, each time from the intermediate that is argmin of maxima
    idx_t i = 0; // chunk counter
    while(still_interm && i < pickup) {
      idx_t interm = argmin(ctx, type_ids[keyCol_idx], maxima, numIntermediate);
      // address corner-case of valid max values
      int investigateCornerCase = false;
      switch(type_ids[keyCol_idx]) {
        case DUCKDB_TYPE_SMALLINT:
          if(((short*)maxima)[interm] == SHRT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_INTEGER:
          if(((int*)maxima)[interm] == INT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_BIGINT:
          if(((long*)maxima)[interm] == LONG_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_FLOAT:
          if(((float*)maxima)[interm] == FLT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_DOUBLE:
          if(((double*)maxima)[interm] == DBL_MAX) investigateCornerCase = true;
          break;
        default:
          perror("Invalid key type.");
          return;
      }
      if(investigateCornerCase) { // if all maxima, pick first valid intermediate
        for(idx_t j=0; j<numIntermediate; j++) {
          if(!intermIsExhausted[j]) {
            interm = j;
            break;
          }
        }
      }

      duckdb_data_chunk cnk = duckdb_fetch_chunk(interms[interm]);
      if(!cnk) {
        intermIsExhausted[interm] = true;
        still_interm -= 1;
        // Update maxima
        max_padding(maxima + interm*colType_bytes(type_ids[keyCol_idx]), type_ids[keyCol_idx], 1);
        // no max-padding needed in the buffer, just skip
        continue;
      }

      idx_t rcount = duckdb_data_chunk_get_size(cnk);
      if(rcount<CHUNK_SIZE) {
        ignore_rows += CHUNK_SIZE - rcount;
      }

      for(idx_t c=0; c<col_count; c++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
        void* dat = duckdb_vector_get_data(vec);
        idx_t accIdx = (c<keyCol_idx)? c: c-1;
        void* dest_ptr = (c==keyCol_idx)?
          keyBuffer2 + i*colType_bytes(type_ids[keyCol_idx])*CHUNK_SIZE:
          payloadBuffer2 + i*pL_bytes*CHUNK_SIZE + pL_prefixSizes[accIdx];
        // for key column: note maximum
        if(c == keyCol_idx) {
          memcpy(
            maxima + interm*colType_bytes(type_ids[keyCol_idx]),
            dat + (rcount-1)*colType_bytes(type_ids[keyCol_idx]),
            colType_bytes(type_ids[keyCol_idx])
          );
        }
        // copy data to buffer
        if(c==keyCol_idx){
          memcpy(
            dest_ptr,
            dat,
            rcount*colType_bytes(type_ids[c])
          );
        }
        else {
          for(idx_t r=0; r<rcount; r++) {
            memcpy(
              dest_ptr + r*pL_bytes,
              dat + r*pL_byteSizes[accIdx],
              pL_byteSizes[accIdx]
            );
          }
        }
       
        // max-pad remaining space within the chunk
        if(c == keyCol_idx  && rcount < CHUNK_SIZE) {
          max_padding(
            dest_ptr + rcount*colType_bytes(type_ids[c]),
            type_ids[c],
            CHUNK_SIZE - rcount
          );
        }
      }
      duckdb_destroy_data_chunk(&cnk);
      i += 1;
    }
    // 5.i+ if i<pickup, max-pad remaining pickup space
    if(i<pickup) {
      max_padding(
        keyBuffer2 + i*CHUNK_SIZE*colType_bytes(type_ids[keyCol_idx]),
        type_ids[keyCol_idx],
        CHUNK_SIZE*(pickup-i)
      );
      ignore_rows += CHUNK_SIZE*(pickup-i);
    }
    // 5.i++ scan more chunks to fill empty pages at the end, if any
    while(still_interm && replaceChunksAtTheEnd) {
      idx_t interm = argmin(ctx, type_ids[keyCol_idx], maxima, numIntermediate);
      // address corner-case of valid max values
      int investigateCornerCase = false;
      switch(type_ids[keyCol_idx]) {
        case DUCKDB_TYPE_SMALLINT:
          if(((short*)maxima)[interm] == SHRT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_INTEGER:
          if(((int*)maxima)[interm] == INT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_BIGINT:
          if(((long*)maxima)[interm] == LONG_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_FLOAT:
          if(((float*)maxima)[interm] == FLT_MAX) investigateCornerCase = true;
          break;
        case DUCKDB_TYPE_DOUBLE:
          if(((double*)maxima)[interm] == DBL_MAX) investigateCornerCase = true;
          break;
        default:
          perror("Invalid key type.");
          return;
      }
      if(investigateCornerCase) { // if all maxima, pick first valid intermediate
        for(idx_t j=0; j<numIntermediate; j++) {
          if(!intermIsExhausted[j]) {
            interm = j;
            break;
          }
        }
      }

      duckdb_data_chunk cnk = duckdb_fetch_chunk(interms[interm]);
      if(!cnk) {
        intermIsExhausted[interm] = true;
        still_interm -= 1;
        // Update maxima
        max_padding(maxima + interm*colType_bytes(type_ids[keyCol_idx]), type_ids[keyCol_idx], 1);
        // no max-padding needed in the buffer, just skip
        continue;
      }

      idx_t rcount = duckdb_data_chunk_get_size(cnk);
      if(rcount<CHUNK_SIZE) {
        ignore_rows += CHUNK_SIZE - rcount;
      }

      for(idx_t c=0; c<col_count; c++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
        void* dat = duckdb_vector_get_data(vec);
        idx_t accIdx = (c<keyCol_idx)? c: c-1;
        void* dest_ptr = (c==keyCol_idx)?
          keyBuffer2 + (BUFFER_CHUNK_CAPACITY - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[c]):
          payloadBuffer2 + (BUFFER_CHUNK_CAPACITY - replaceChunksAtTheEnd)*CHUNK_SIZE*pL_bytes + pL_prefixSizes[accIdx];
        // for key column: note maximum
        if(c == keyCol_idx) {
          memcpy(
            maxima + interm*colType_bytes(type_ids[keyCol_idx]),
            dat + (rcount-1)*colType_bytes(type_ids[keyCol_idx]),
            colType_bytes(type_ids[keyCol_idx])
          );
        }
        // copy data to buffer
        if(c==keyCol_idx) {
          memcpy(
            dest_ptr,
            dat,
            rcount*colType_bytes(type_ids[c])
          );
        }
        else {
          for(idx_t r=0; r<rcount; r++) {
            memcpy(
              dest_ptr + r*pL_bytes,
              dat + r*pL_byteSizes[accIdx],
              pL_byteSizes[accIdx]
            );
          }
        }
        
        // max-pad remaining space within the chunk
        if(c==keyCol_idx && rcount < CHUNK_SIZE) {
          max_padding(
            dest_ptr + rcount*colType_bytes(type_ids[c]),
            type_ids[c],
            CHUNK_SIZE - rcount
          );
        }
      }
      duckdb_destroy_data_chunk(&cnk);
      replaceChunksAtTheEnd -= 1;
    }
    mylog(logfile, "Completed a scan.");
    //logarray_long(logfile, "Before sorting:\n", keyBuffer2, BUFFER_SIZE);

    // Update loop parametres here
    replaceChunksAtTheEnd = ignore_rows / CHUNK_SIZE;
    ignore_rows %= CHUNK_SIZE;
    pickup = BUFFER_CHUNK_CAPACITY - still_interm - replaceChunksAtTheEnd;

    // 5.ii sort key column, reorder payload columns
    sortRelationByKey(
      ctx, keyBuffer2, payloadBuffer2, type_ids[keyCol_idx], blocked, block_size, 
      keyBuffer2, payloadBuffer2, pL_bytes, BUFFER_SIZE
    );
    mylog(logfile, "Sorted this scan.");
    // prepare combined buffers
    void* payloadCols[col_count-1];
    payloadColumnsFromByteArray(payloadCols, pL_types, payloadBuffer2, (col_count-1), BUFFER_SIZE);
    mylog(logfile, "Reobtained payload columns.");

    // 5.iii pick up & store chunks
    for(idx_t j=0; j<pickup; j++) {
      idx_t rowsToWrite = CHUNK_SIZE;
      if(!still_interm && j==pickup-1) rowsToWrite = CHUNK_SIZE - ignore_rows; // last chunk that is read at all

      duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, col_count);
      duckdb_data_chunk_set_size(cnk, rowsToWrite);
      for(idx_t c=0; c<col_count; c++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
        void *dat = (void*)duckdb_vector_get_data(vec);
        idx_t accIdx = (c<keyCol_idx)? c: c-1;
        void *srcBuff = (c==keyCol_idx)? keyBuffer2: payloadCols[accIdx];
        memcpy(dat, &((char*)srcBuff)[j*CHUNK_SIZE*colType_bytes(type_ids[c])], rowsToWrite*colType_bytes(type_ids[c]));
      }
      if(duckdb_append_data_chunk(resultApp, cnk) == DuckDBError) {
        perror("Failed to append data chunk to result table.\n");
        return;
      }
      if (quicksaves) duckdb_appender_flush(resultApp);
      duckdb_destroy_data_chunk(&cnk);
    }
    for(idx_t c=0; c<col_count-1; c++) {
      free(payloadCols[c]);
    }
    mylog(logfile, "Appended data to result table.");
  }

  //Clean-up
  for(idx_t i=0; i<numIntermediate; i++) {
    duckdb_destroy_result(&interms[i]);
  }
  duckdb_appender_close(resultApp);
  duckdb_appender_destroy(&resultApp);
  free(payloadBuffer2);
  free(keyBuffer2);
  free(maxima);
  mylog(logfile, "Freed buffers and objects from Stage 2.");
}

void sort_Stage2_without_payloads(
	idx_t CHUNK_SIZE,
	size_t BUFFER_SIZE,
  const int16_t block_size,
  const char* intermName,
  const char* finalName,
	FILE *logfile,
	struct futhark_context *ctx,
	duckdb_connection con,
	duckdb_type type_id,
  char** colNames,
	idx_t numIntermediate,
  int blocked,
  int quicksaves,
  int saveAsTempTable
) {
	duckdb_type type_ids[2] = {type_id, DUCKDB_TYPE_BIGINT};
	sort_Stage2(
		CHUNK_SIZE,
		BUFFER_SIZE,
    block_size,
    intermName,
    finalName,
		logfile,
		ctx,
		con,
		2,
		type_ids,
    colNames,
    0,
		numIntermediate,
    blocked,
    quicksaves,
    saveAsTempTable
	);
}

// ----------------------------------------------------------------------
// Consice gathered forms
// ----------------------------------------------------------------------

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
) {
  duckdb_result res;
  char queryStr[strlen(tblName) + 50];
  sprintf(queryStr, "SELECT * FROM %s;", tblName);
  duckdb_query(con, queryStr, &res);
  mylog(logfile, "Obtained result from initial table."); 

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");

  idx_t col_count = duckdb_column_count(&res);
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    //mylog(logfile, "Obtained column's type.");
  }
  idx_t kc = -1; // Column index of the key column
  char** colNames = malloc(col_count*sizeof(char*));
  for(idx_t i=0; i<col_count; i++) {
    colNames[i] = malloc(strlen(duckdb_column_name(&res, i)) + 1);
    sprintf(colNames[i], "%s", duckdb_column_name(&res, i));

    if(strcmp(colNames[i], keyName) == 0) kc = i;
  }

  if(kc < 0) {
    mylog(logfile, "Invalid column name for sorting.");
    for(idx_t i=0; i<col_count; i++) {
      free(colNames[i]);
    }
    free(colNames);
    duckdb_destroy_result(&res);
    return;
  }

  // STAGE 1 - SCAN TABLE, SAVE INTO LOCALLY SORTED TEMPORARY TABLES
  mylog(logfile, "Beginning to process pages...");
  idx_t numIntermediate = sort_Stage1_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    block_size,
    intermName,
    logfile,
    ctx,
    con,
    res,
    col_count,
    type_ids,
    colNames,
    kc,
    &incr_idx,
    blocked
  );

  duckdb_destroy_result(&res);
  mylog(logfile, "Freed initial duckdb result from the 1st stage.");

  // STAGE 2 - RETRIEVE DATA, SORTING THE FIRST BLOCK OF EACH FILE EACH TIME
  if(numIntermediate > 1) {
    sort_Stage2(
      CHUNK_SIZE,
      BUFFER_SIZE,
      block_size,
      intermName,
      finalName,
      logfile,
      ctx,
      con,
      col_count,
      type_ids,
      colNames,
      kc,
      numIntermediate,
      blocked,
      quicksaves,
      saveAsTempTable
    );
  }
  else if(numIntermediate<1) {
    logdbg(logfile, (numIntermediate==0), "Note ------- table is empty.", "####### Error saving the intermediates.");
  }
  else { // if only one intermediate
    char finalStr[100 + strlen(intermName) + strlen(finalName)];
    if(saveAsTempTable) {
      sprintf(finalStr, "ALTER TABLE %s0 RENAME TO %s;", intermName, finalName);
    }
    else {
      sprintf(finalStr, "CREATE OR REPLACE TABLE %s AS (SELECT * FROM %s0);", finalName, intermName);
    }
    duckdb_query(con, finalStr, NULL);
    mylog(logfile, "Stage 2 has been skipped, as the table fit within one buffer.");
  }

  for(idx_t i=0; i<col_count; i++) {
    free(colNames[i]);
  }
  free(colNames);
}

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
) {
  duckdb_result res;
  char queryStr[strlen(tblName) + 50];
  sprintf(queryStr, "SELECT %s FROM %s;", keyName, tblName);
  if(duckdb_query(con, queryStr, &res) == DuckDBError) {
    mylog(logfile, "Failed to obtain result from initial table.");
    return;
  }
  mylog(logfile, "Obtained result from initial table.");

  idx_t col_count = duckdb_column_count(&res);

  char **colNames = malloc(2*sizeof(char*));
  colNames[0] = malloc(1+strlen(keyName));
  colNames[1] = malloc(7);
  sprintf(colNames[0], "%s", keyName);
  sprintf(colNames[1], "rowIdx");

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");

  duckdb_type type_id = duckdb_column_type(&res, 0);
  mylog(logfile, "Obtained key column's type.");

  // STAGE 1 - SCAN TABLE, SAVE INTO LOCALLY SORTED TEMPORARY TABLES
  mylog(logfile, "Beginning to process pages...");
  idx_t numIntermediate = sort_Stage1_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    block_size,
    intermName,
    logfile,
    ctx,
    con,
    res,
    type_id,
    colNames,
    &incr_idx,
    blocked
  );

  duckdb_destroy_result(&res);
  mylog(logfile, "Freed initial duckdb result from the 1st stage.");

  // STAGE 2 - RETRIEVE DATA, SORTING THE FIRST BLOCK OF EACH FILE EACH TIME
  if(numIntermediate > 1) {
    sort_Stage2_without_payloads(
      CHUNK_SIZE,
      BUFFER_SIZE,
      block_size,
      intermName,
      finalName,
      logfile,
      ctx,
      con,
      type_id,
      colNames,
      numIntermediate,
      blocked,
      quicksaves,
      saveAsTempTable
    );
  }
  else if(numIntermediate<1) {
    logdbg(logfile, (numIntermediate==0), "Note ------- table is empty.", "####### Error saving the intermediates.");
  }
  else { // if only one intermediate
    char finalStr[200 + strlen(intermName) + strlen(finalName)];
    if(saveAsTempTable) {
      sprintf(finalStr, "ALTER TABLE %s0 RENAME TO %s;", intermName, finalName);
    }
    else {
      sprintf(finalStr, "CREATE OR REPLACE TABLE %s AS (SELECT * FROM %s0);", finalName, intermName);
    }
    duckdb_query(con, finalStr, NULL);
    mylog(logfile, "Stage 2 has been skipped, as the table fit within one buffer.");
  }

  for(idx_t i=0; i<2; i++) {
    free(colNames[i]);
  }
  free(colNames);
}

idx_t semi_sort_with_payloads(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t block_size,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  int blocked,
  int quicksaves
) {
  duckdb_result res;
  char queryStr[strlen(tblName) + 50];
  sprintf(queryStr, "SELECT * FROM %s;", tblName);
  duckdb_query(con, queryStr, &res);
  mylog(logfile, "Obtained result from initial table."); 

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");

  idx_t col_count = duckdb_column_count(&res);
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    //mylog(logfile, "Obtained column's type.");
  }
  idx_t kc = -1; // Column index of the key column
  char** colNames = malloc(col_count*sizeof(char*));
  for(idx_t i=0; i<col_count; i++) {
    colNames[i] = malloc(strlen(duckdb_column_name(&res, i)) + 1);
    sprintf(colNames[i], "%s", duckdb_column_name(&res, i));

    if(strcmp(colNames[i], keyName) == 0) kc = i;
  }

  if(kc < 0) {
    mylog(logfile, "Invalid column name for sorting.");
    for(idx_t i=0; i<col_count; i++) {
      free(colNames[i]);
    }
    free(colNames);
    duckdb_destroy_result(&res);
    return 0;
  }

  // STAGE 1 - SCAN TABLE, SAVE INTO LOCALLY SORTED TEMPORARY TABLES
  mylog(logfile, "Beginning to process pages...");
  idx_t numIntermediate = sort_Stage1_with_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    block_size,
    intermName,
    logfile,
    ctx,
    con,
    res,
    col_count,
    type_ids,
    colNames,
    kc,
    &incr_idx,
    blocked
  );

  duckdb_destroy_result(&res);
  mylog(logfile, "Completed semi-sort.");

  for(idx_t i=0; i<col_count; i++) {
    free(colNames[i]);
  }
  free(colNames);

  return numIntermediate;
}

idx_t semi_sort_without_payloads(
  idx_t CHUNK_SIZE,
  size_t BUFFER_SIZE,
  const int16_t block_size,
  FILE *logfile,
  struct futhark_context *ctx,
  duckdb_connection con,
  const char* tblName,
  const char* keyName,
  const char* intermName,
  int blocked,
  int quicksaves
) {
  duckdb_result res;
  char queryStr[strlen(tblName) + 50];
  sprintf(queryStr, "SELECT %s FROM %s;", keyName, tblName);
  if(duckdb_query(con, queryStr, &res) == DuckDBError) {
    mylog(logfile, "Failed to obtain result from initial table.");
    return 0;
  }
  mylog(logfile, "Obtained result from initial table.");

  idx_t col_count = duckdb_column_count(&res);

  char **colNames = malloc(2*sizeof(char*));
  colNames[0] = malloc(1+strlen(keyName));
  colNames[1] = malloc(7);
  sprintf(colNames[0], "%s", keyName);
  sprintf(colNames[1], "rowIdx");

  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");

  duckdb_type type_id = duckdb_column_type(&res, 0);
  mylog(logfile, "Obtained key column's type.");

  // STAGE 1 - SCAN TABLE, SAVE INTO LOCALLY SORTED TEMPORARY TABLES
  mylog(logfile, "Beginning to process pages...");
  idx_t numIntermediate = sort_Stage1_without_payloads(
    CHUNK_SIZE,
    BUFFER_SIZE,
    block_size,
    intermName,
    logfile,
    ctx,
    con,
    res,
    type_id,
    colNames,
    &incr_idx,
    blocked
  );

  duckdb_destroy_result(&res);
  mylog(logfile, "Completed semi-sort.");

  for(idx_t i=0; i<2; i++) {
    free(colNames[i]);
  }
  free(colNames);

  return numIntermediate;
}