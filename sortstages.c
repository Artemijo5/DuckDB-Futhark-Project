#include "sortstages.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftsort.h"
#include "mylogger.h"
#include "myutil.h"

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
	idx_t *incr_idx
) {
  idx_t numIntermediate = 0;

  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    void *Buffers[col_count];
    mylog(logfile, "Initalising buffers for each column...");
    for(idx_t col=0; col<col_count; col++) {
      Buffers[col] = colType_malloc(type_ids[col], buffer_size);
      if(Buffers[col] == NULL) {
        mylog(logfile, "ERROR -- failed to allocate memory.");
        logclose(logfile);
        return -1;
      }
    }
    mylog(logfile, "Successfully initialised buffers.");

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
  		duckdb_vector res_col[col_count];
  		void *vector_data[col_count];
  		for (idx_t col = 0; col < col_count; col++) {
  			res_col[col] = duckdb_data_chunk_get_vector(result, col);
  			vector_data[col] = duckdb_vector_get_data(res_col[col]);
        memcpy(Buffers[col] + cur_rows*colType_bytes(type_ids[col]),
          vector_data[col],
          row_count * colType_bytes(type_ids[col]));
  		}
      
      cur_rows += row_count;

      duckdb_destroy_data_chunk(&result);
    }
    if(!flag && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      for(idx_t col=0; col<col_count; col++) {
        free(Buffers[col]); // free buffers
      }
      break;
    } // TODO flag is probably superfluous...
    char msgbuffer[50];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // sort key column
    struct futhark_i64_1d *sorted_idx_ft;
    mylog(logfile, "Passing key column for sorting...");
    sortKeyColumn(ctx, Buffers[0], type_ids[0], *incr_idx, block_size, &sorted_idx_ft, Buffers[0], cur_rows);
    mylog(logfile, "Sorted key column and obtained reordered indices.");
    
    // Next do the payload columns
    mylog(logfile, "Reordering payload columns...");
    for(idx_t col=1; col<col_count; col++) {
      orderPayloadColumn(ctx, Buffers[col], type_ids[col], *incr_idx, sorted_idx_ft, Buffers[col], cur_rows);
    }

    //mylog(logfile, "Now testing storage & retrieval.");
    numIntermediate = store_intermediate(numIntermediate, intermName, con, chunk_size, col_count, cur_rows, type_ids, Buffers);
    if(numIntermediate == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
    for(idx_t col=0; col<col_count; col++) {
      free(Buffers[col]);
    }
    mylog(logfile, "Freed this page's buffers.");
    futhark_free_i64_1d(ctx, sorted_idx_ft);
    mylog(logfile, "Freed futhark objects for this page.");

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
	idx_t *incr_idx
) {
	idx_t numIntermediate = 0;

  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    mylog(logfile, "Initalising buffer for the key column...");
    void *Buffer = colType_malloc(type_id, buffer_size);
    if(Buffer == NULL) {
      mylog(logfile, "ERROR -- failed to allocate memory.");
      logclose(logfile);
      return -1;
    }
    mylog(logfile, "Successfully initialised buffers.");

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
    if(!flag && cur_rows == 0) { // if table exhausted, break out of outer loop as well
      free(Buffer);
      break;
    }
    char msgbuffer[50];
    int msglen = sprintf(msgbuffer, "Finished scanning 'page' -- total of ");
    msglen += sprintf(msgbuffer + msglen, "%ld", cur_rows);
    msglen += sprintf(msgbuffer + msglen, " rows.");
    mylog(logfile, msgbuffer);

    // sort key column
    struct futhark_i64_1d *sorted_idx_ft;
    mylog(logfile, "Passing key column for sorting...");
    sortKeyColumn(ctx, Buffer, type_id, *incr_idx, block_size, &sorted_idx_ft, Buffer, cur_rows);
    mylog(logfile, "Sorted key column and obtained reordered indices.");
    
    int64_t orderedIndices[cur_rows];
    futhark_values_i64_1d(ctx, sorted_idx_ft, orderedIndices);

    void *Buffers[2] = {Buffer, (void*)orderedIndices};
    duckdb_type type_ids[2] = {type_id, DUCKDB_TYPE_BIGINT};
    idx_t col_count = 2;

    numIntermediate = store_intermediate(numIntermediate, intermName, con, chunk_size, col_count, cur_rows, type_ids, Buffers);
    if(numIntermediate == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    // clean-up
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
	idx_t numIntermediate,
  int quicksaves
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
  void* maxima = colType_malloc(type_ids[0], numIntermediate);
  // initialised as all false, a slot is set to true when its buffer is exhausted
  int intermIsExhausted[numIntermediate];
  // The buffers where the sorting takes place
  // one buffer of BUFFER_SIZE elements of the appropriate type, for each column
  // each iteration, the key buffer is sorted, and the payload buffers are reordered
  // the first pickup bytes of all buffers are stored
  // then, they are replaced by the next scan
  // if there is an entirely padded chunk at the end, that is also replaced
  // then, pickup is updated for the next iteration
  void** Buffers2 = malloc(col_count*sizeof(void*));
  for(idx_t c=0; c<col_count; c++) {
    Buffers2[c] = colType_malloc(type_ids[c], BUFFER_SIZE);
  }
  // max-pad key column to get rid of trash values - not needed
  /*
  max_padding(
    Buffers2[0],
    type_ids[0],
    BUFFER_SIZE
  );
  */
  // counter of full empty chunks at the end of buffer
  // set to 0 when they are replaced
  idx_t replaceChunksAtTheEnd = 0;
  mylog(logfile, "Initialised buffers for Stage 2.");

  // ---------- 2. Prepare the table where results will be stored.
  char type_strs[col_count][25];
  duckdb_logical_type ltypes[col_count];
   for(idx_t i=0; i<col_count; i++) {
    ltypes[i] = duckdb_create_logical_type(type_ids[i]);
    switch(type_ids[i]){
      case DUCKDB_TYPE_SMALLINT:
        sprintf( type_strs[i], "SMALLINT" );
        break;
      case DUCKDB_TYPE_INTEGER:
        sprintf( type_strs[i], "INTEGER" );
        break;
      case DUCKDB_TYPE_BIGINT:
        sprintf( type_strs[i], "BIGINT" );
        break;
      case DUCKDB_TYPE_FLOAT:
        sprintf( type_strs[i], "FLOAT" );
        break;
      case DUCKDB_TYPE_DOUBLE:
        sprintf( type_strs[i], "DOUBLE" );
        break;
      default:
        perror("Invalid type.");
        return;
    }
  }
  char resultQueryStr[100 + strlen(finalName) + 35*col_count];
  int resultQueryStr_len = sprintf(resultQueryStr, "CREATE OR REPLACE TABLE %s (", finalName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "x%ld %s, ", i, type_strs[i]);
    }
    else {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "x%ld %s);", i, type_strs[i]);
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
      // max-pad a chunk from the end
      for(idx_t c=0; c<col_count; c++) {
        max_padding(
          Buffers2[c] + (BUFFER_CHUNK_CAPACITY - 1 - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[c]),
          type_ids[c],
          CHUNK_SIZE
        );
      }
      replaceChunksAtTheEnd += 1;
      // also at the maximum
      max_padding(maxima + i*colType_bytes(type_ids[0]), type_ids[0], 1);
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
      void* dest_ptr = Buffers2[c] + (pickup + i - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[c]);
      // for key column: note maximum
      if(c == 0) {
        memcpy(
          maxima + i*colType_bytes(type_ids[0]),
          dat + (rcount-1)*colType_bytes(type_ids[0]),
          colType_bytes(type_ids[0])
        );
      }
      // copy data to buffer
      memcpy(
        dest_ptr,
        dat,
        rcount*colType_bytes(type_ids[c])
      );
      // max-pad remaining space within the chunk
      if(rcount < CHUNK_SIZE) {
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
  //logarray_long(logfile, "After first scan:\n", Buffers2[0], BUFFER_SIZE);

  // ---------- 5. Scan & Sort - loop.
  while(still_interm) {
    // 5.i scan the next pickup chunks, each time from the intermediate that is argmin of maxima
    idx_t i = 0; // chunk counter
    while(still_interm && i < pickup) {
      idx_t interm = argmin(ctx, type_ids[0], maxima, numIntermediate);
      // TODO address corner-case of valid max values...

      duckdb_data_chunk cnk = duckdb_fetch_chunk(interms[interm]);
      if(!cnk) {
        intermIsExhausted[interm] = true;
        still_interm -= 1;
        // Update maxima
        max_padding(maxima + interm*colType_bytes(type_ids[0]), type_ids[0], 1);
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
        void* dest_ptr = Buffers2[c] + i*colType_bytes(type_ids[0])*CHUNK_SIZE; // it's that easy...
        // for key column: note maximum
        if(c == 0) {
          memcpy(
            maxima + interm*colType_bytes(type_ids[0]),
            dat + (rcount-1)*colType_bytes(type_ids[0]),
            colType_bytes(type_ids[0])
          );
        }
        // copy data to buffer
        memcpy(
          dest_ptr,
          dat,
          rcount*colType_bytes(type_ids[c])
        );
        // max-pad remaining space within the chunk
        if(rcount < CHUNK_SIZE) {
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
    //logarray_long(logfile, "Before correct-scanning:\n", Buffers2[0], BUFFER_SIZE);
    // 5.i+ if i<pickup, max-pad remaining pickup space
    if(i<pickup) {
      max_padding(
        Buffers2[0] + i*CHUNK_SIZE*colType_bytes(type_ids[0]),
        type_ids[0],
        CHUNK_SIZE*(pickup-i)
      );
      ignore_rows += CHUNK_SIZE*(pickup-i);
    }
    // 5.i++ scan more chunks to fill empty pages at the end, if any
    while(still_interm && replaceChunksAtTheEnd) {
      idx_t interm = argmin(ctx, type_ids[0], maxima, numIntermediate);
      // TODO address corner-case of valid max values...
      duckdb_data_chunk cnk = duckdb_fetch_chunk(interms[interm]);
      if(!cnk) {
        intermIsExhausted[interm] = true;
        still_interm -= 1;
        // Update maxima
        max_padding(maxima + interm*colType_bytes(type_ids[0]), type_ids[0], 1);
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
        void* dest_ptr = Buffers2[c] + (BUFFER_CHUNK_CAPACITY - replaceChunksAtTheEnd)*CHUNK_SIZE*colType_bytes(type_ids[c]);
        // for key column: note maximum
        if(c == 0) {
          memcpy(
            maxima + interm*colType_bytes(type_ids[0]),
            dat + (rcount-1)*colType_bytes(type_ids[0]),
            colType_bytes(type_ids[0])
          );
        }
        // copy data to buffer
        memcpy(
          dest_ptr,
          dat,
          rcount*colType_bytes(type_ids[c])
        );
        // max-pad remaining space within the chunk
        if(rcount < CHUNK_SIZE) {
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
    //logarray_long(logfile, "Before sorting:\n", Buffers2[0], BUFFER_SIZE);

    // Update loop parametres here
    replaceChunksAtTheEnd = ignore_rows / CHUNK_SIZE;
    ignore_rows %= CHUNK_SIZE;
    pickup = BUFFER_CHUNK_CAPACITY - still_interm - replaceChunksAtTheEnd;

    // 5.ii sort key column, reorder payload columns
    struct futhark_i64_1d *stg2_sorted_idx_ft;
    for(idx_t c=0; c<col_count; c++) {
      if(c==0) {
        sortKeyColumn(ctx, Buffers2[0], type_ids[0], 0, block_size, &stg2_sorted_idx_ft, Buffers2[0], BUFFER_SIZE);
        continue;
      }
      orderPayloadColumn(ctx, Buffers2[c], type_ids[c], 0, stg2_sorted_idx_ft, Buffers2[c], BUFFER_SIZE);
    }
    futhark_free_i64_1d(ctx, stg2_sorted_idx_ft);
    mylog(logfile, "Sorted this scan.");
    //printf("%ld, %ld\n", ((long*)Buffers2[0])[0], ((long*)Buffers2[0])[BUFFER_SIZE-1]);
    //logarray_long(logfile, "After sorting:\n", Buffers2[0], BUFFER_SIZE);

    // 5.iii pick up & store chunks
    for(idx_t j=0; j<pickup; j++) {
      idx_t rowsToWrite = CHUNK_SIZE;
      if(!still_interm && j==pickup-1) rowsToWrite = CHUNK_SIZE - ignore_rows; // last chunk that is read at all

      duckdb_data_chunk cnk = duckdb_create_data_chunk(ltypes, col_count);
      duckdb_data_chunk_set_size(cnk, rowsToWrite);
      for(idx_t c=0; c<col_count; c++) {
        duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, c);
        void *dat = (void*)duckdb_vector_get_data(vec);
        memcpy(dat, &((char*)Buffers2[c])[j*CHUNK_SIZE*colType_bytes(type_ids[c])], rowsToWrite*colType_bytes(type_ids[c]));
      }
      if(duckdb_append_data_chunk(resultApp, cnk) == DuckDBError) {
        perror("Failed to append data chunk to result table.\n");
        return;
      }
      if (quicksaves) duckdb_appender_flush(resultApp);
      duckdb_destroy_data_chunk(&cnk);
    }
    mylog(logfile, "Appended data to result table.");
  }

  //Clean-up
  for(idx_t i=0; i<numIntermediate; i++) {
    duckdb_destroy_result(&interms[i]);
  }
  duckdb_appender_close(resultApp);
  duckdb_appender_destroy(&resultApp);
  for(idx_t c=0; c<col_count; c++) {
    free(Buffers2[c]);
  }
  free(Buffers2);
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
	idx_t numIntermediate,
  int quicksaves
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
		numIntermediate,
    quicksaves
	);
}

// ----------------------------------------------------------------------
// Consice gathered forms
// ----------------------------------------------------------------------

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
  const char* intermName,
  const char* finalName,
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
    mylog(logfile, "Obtained column's type.");
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
    &incr_idx
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
      numIntermediate,
      quicksaves
    );
  }
  else if(numIntermediate<1) {
    logdbg(logfile, (numIntermediate==0), "Note ------- table is empty.", "####### Error saving the intermediates.");
    return;
  }
  else { // if only one intermediate
    char finalStr[100 + strlen(intermName) + strlen(finalName)];
    sprintf(finalStr, "CREATE OR REPLACE TABLE %s AS (SELECT * FROM %s0);", finalName, intermName);
    duckdb_query(con, finalStr, NULL);
    mylog(logfile, "Stage 2 has been skipped, as the table fit within one buffer.");
  }
}

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
  const char* intermName,
  const char* finalName,
  int quicksaves
) {
  duckdb_result res;
  char queryStr[strlen(tblName) + 50];
  sprintf(queryStr, "SELECT #1 FROM %s;", tblName);
  duckdb_query(con, queryStr, &res);
  mylog(logfile, "Obtained result from initial table.");

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
    &incr_idx
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
      numIntermediate,
      quicksaves
    );
  }
  else if(numIntermediate<1) {
    logdbg(logfile, (numIntermediate==0), "Note ------- table is empty.", "####### Error saving the intermediates.");
    return;
  }
  else { // if only one intermediate
    char finalStr[100 + strlen(intermName) + strlen(finalName)];
    sprintf(finalStr, "CREATE OR REPLACE TABLE %s AS (SELECT * FROM %s0);", finalName, intermName);
    duckdb_query(con, finalStr, NULL);
    mylog(logfile, "Stage 2 has been skipped, as the table fit within one buffer.");
  }
}