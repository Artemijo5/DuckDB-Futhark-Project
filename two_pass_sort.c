#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "duckdb.h"

#include "ftsort.h"
#include "mylogger.h"
#include "myutil.h"

#define LOGFILE "two_pass_sort.log.txt"

#define CHUNK_SIZE duckdb_vector_size()
#define BUFFER_SIZE 128*CHUNK_SIZE
#define TABLE_SIZE 64*BUFFER_SIZE

#define BUFFER_CHUNK_CAPACITY BUFFER_SIZE/CHUNK_SIZE

#define DBFILE "testdb.db"
#define DDB_MEMSIZE "2GB"
#define DDB_TEMPDIR "tps_tempdir"

/* ------------------------------------------------------------------------------------------------------------------------------
  // TODO
  // save to chunks & new table "sortedTbl" (for now via appenders...)
  // -> 2-PASS
  // gpu runs...
  */

int main() {
  // Initialise logger
  FILE* logfile = loginit(LOGFILE, "Two-Pass Sort : Starting test program.");
  if(!logfile) {
    perror("Failed to initialise the logger.");
    return 1;
  }

  // DuckDB initialisation
  duckdb_database db;
	duckdb_connection con;
  duckdb_config config;

  // set config
  if (duckdb_create_config(&config) == DuckDBError) {
    perror("Failed to create config.");
    return -1;
  }
  duckdb_set_config(config, "max_memory", DDB_MEMSIZE);
  duckdb_set_config(config, "memory_limit", DDB_MEMSIZE);
  duckdb_set_config(config, "temp_directory", DDB_TEMPDIR);
	//duckdb_open(NULL, &db);
  if(duckdb_open_ext(DBFILE, &db, config, NULL) == DuckDBError) {
    perror("Failed to open database with configuration options.");
    return -1;
  }
  duckdb_destroy_config(&config);

	duckdb_connect(db, &con);

  // Create the table tbl on which the testing will be done.
  duckdb_result res;
	
  duckdb_query(con, "CREATE OR REPLACE TABLE tbl (k BIGINT, payload1 BIGINT, payload2 DOUBLE);", NULL);
  duckdb_query(con, "setseed(0.42);", NULL);

  duckdb_prepared_statement init_stmt;
  if (duckdb_prepare(con,
    "INSERT INTO tbl (SELECT ($1 - i), 10000*random(), 10000*random() FROM range($1) t(i));",
    &init_stmt) == DuckDBError)
  {
    perror("Failed to initialise the table.");
    return 1;
  }
  duckdb_bind_int32(init_stmt, 1, TABLE_SIZE);
  duckdb_execute_prepared(init_stmt, NULL);
  duckdb_destroy_prepare(&init_stmt);
  
  duckdb_query(con, "SELECT * FROM tbl;", &res);
  
  idx_t incr_idx = 0;
  mylog(logfile, "Initialised increment at 0.");
  idx_t col_count = duckdb_column_count(&res);
  // TODO current strat is to sort key column and reorder payload within the same pass, holding them all together
  // ultimate strat would be:
  // have a fixed buffer size (eg 64 bits x 65535 items -> 4 MB)
  // on each scan, process as many columns as can fit
  // the first scan sorts the key and produces the reordered indices that following scans use
  duckdb_type type_ids[col_count];
  mylog(logfile, "Initalising info for each column...");
  for(idx_t col=0; col<col_count; col++) {
    type_ids[col] = duckdb_column_type(&res, col);
    mylog(logfile, "Obtained column's type.");
  }

  // Set up futhark core
  struct futhark_context_config *cfg = futhark_context_config_new();
  struct futhark_context *ctx = futhark_context_new(cfg);
  mylog(logfile, "Set up futhark context & config.");

  idx_t numIntermediate = 0;

  mylog(logfile, "Beginning to process pages...");
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  // STAGE 1 - SCAN TABLE, SAVE INTO TEMPORARY TABLES
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  int flag = true;
  while(flag) {
    mylog(logfile, "Next page...");

    void *Buffers[col_count];
    mylog(logfile, "Initalising buffers for each column...");
    for(idx_t col=0; col<col_count; col++) {
      Buffers[col] = colType_malloc(type_ids[col], BUFFER_SIZE);
      if(Buffers[col] == NULL) {
        mylog(logfile, "ERROR -- failed to allocate memory.");
        logclose(logfile);
        return 1;
      }
    }
    mylog(logfile, "Successfully initialised buffers.");

    idx_t cur_rows = 0;

    // iterate until result is exhausted
  	while (BUFFER_SIZE - cur_rows >= CHUNK_SIZE) {
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
    void *sorted_cols[col_count];
    sorted_cols[0] = colType_malloc(type_ids[0], cur_rows);
    void *sorted_x = sorted_cols[0];
    mylog(logfile, "Passing key column for sorting...");
    sortKeyColumn(ctx, sorted_x, type_ids[0], incr_idx, &sorted_idx_ft, Buffers[0], cur_rows);
    mylog(logfile, "Sorted key column and obtained reordered indices.");
    //logarray_long(logfile, "Sorted x: ", sorted_x, cur_rows);
    // test that sorting & reordering was done correctly
    long sorted_idx[cur_rows];
    futhark_values_i64_1d(ctx, sorted_idx_ft, sorted_idx);
    futhark_context_sync(ctx);
    /*
    int isSorted = true;
    int indexIsCorrect = true;
    for(idx_t i=0; i<cur_rows-1; i++) {
      switch(type_ids[0]) {
        case DUCKDB_TYPE_SMALLINT:
          isSorted =  isSorted && (((short*)sorted_x)[i] <= ((short*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_INTEGER:
          isSorted =  isSorted && (((int*)sorted_x)[i] <= ((int*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_BIGINT:
          isSorted =  isSorted && (((long*)sorted_x)[i] <= ((long*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_FLOAT:
          isSorted =  isSorted && (((float*)sorted_x)[i] <= ((float*)sorted_x)[i+1]);
          break;
        case DUCKDB_TYPE_DOUBLE:
          isSorted =  isSorted && (((double*)sorted_x)[i] <= ((double*)sorted_x)[i+1]);
          break;
        default:
          perror("Invalid Type.");
          return 1;
      }
    }
    size_t x_bytes = colType_bytes(type_ids[0]);
    for(idx_t i=0; i<cur_rows; i++) {
      for(size_t b=0; b<x_bytes; b++) {
        indexIsCorrect = indexIsCorrect && (((char*)sorted_x)[i*x_bytes + b]
          == ((char*)Buffers[0])[sorted_idx[i]*x_bytes + b - incr_idx*x_bytes]);
      }
    }
    logdbg(logfile,
      isSorted,
      "x was sorted correctly.",
      "!!!!!!!!!!!!!!! Error: x was not sorted correctly. !!!!!!!!!!!!!!!");
    logdbg(logfile,
      indexIsCorrect,
      "Indices were ordered correctly.",
      "!!!!!!!!!!!!!!! Error: indices were not ordered correctly. !!!!!!!!!!!!!!!");
    */
    
    // Next do the payload columns
    mylog(logfile, "Reordering payload columns...");
    for(idx_t col=1; col<col_count; col++) {
      sorted_cols[col] = colType_malloc(type_ids[col], cur_rows);
      orderPayloadColumn(ctx, sorted_cols[col], type_ids[col], incr_idx, sorted_idx_ft, Buffers[col], cur_rows);
      //mylog(logfile, "Reordered the next payload column.");
      // Test whether the reordering was done correctly...
      /*
      int yIsCorrect = true;
      size_t y_bytes = colType_bytes(type_ids[col]);
      for (idx_t i=0; i<cur_rows; i++) {
        for(size_t b=0; b<y_bytes; b++) {
          // Byte-wise comparison, rather than switch-case per type...
          yIsCorrect = yIsCorrect && (((char*)sorted_cols[col])[i*y_bytes + b]
            == ((char*)Buffers[col])[sorted_idx[i]*y_bytes + b - incr_idx*y_bytes]);
        }
      }
      logdbg(logfile,
        yIsCorrect,
        "Payload column was reordered correctly.",
        "!!!!!!!!!!!!!!! Error: payload column was not reordered correctly. !!!!!!!!!!!!!!!");
      */
    }

    //mylog(logfile, "Now testing storage & retrieval.");
    numIntermediate = store_intermediate(numIntermediate, con, CHUNK_SIZE, col_count, cur_rows, type_ids, sorted_cols);
    if(numIntermediate == -1) {
      perror("Failed to store intermediate.\n");
      return -1;
    }
    mylog(logfile, "Stored buffer as intermediate.");
    /*
    void* testBuffers[col_count];
    for(idx_t c=0; c<col_count; c++) {
      testBuffers[c] = colType_malloc(type_ids[c], cur_rows);
    }
    idx_t scanned_rows = 0;
    duckdb_result testRes;
    prepareToFetch_intermediate(numIntermediate-1, con, &testRes);
    int retrievedPageIsCorrect = true;
    while(true) {
      idx_t new_rows = fetch_intermediate(testRes, col_count, type_ids, (void**)testBuffers, scanned_rows);
      if(new_rows == 0) break;

      for(idx_t c=0; c<col_count; c++) {
        for(idx_t b=0; b<new_rows*colType_bytes(type_ids[c]); b++) {
          idx_t B = b + scanned_rows*colType_bytes(type_ids[c]);
          if(((char*)sorted_cols[c])[B] != ((char*)testBuffers[c])[B]) {
            //printf("Mistake at col %ld at row %ld !\n", c, B);
            retrievedPageIsCorrect = false;
            break;
          }
        }
        if(!retrievedPageIsCorrect) break;
      }

      scanned_rows += new_rows;
    }
    logdbg(logfile,
      retrievedPageIsCorrect,
      "Page was stored & retrieved correctly in its entirety.",
      "!!!!!!!!!!!!!!! Error: retrieved data does not match original. !!!!!!!!!!!!!!!");
    */
    
    // clean-up
    for(idx_t col=0; col<col_count; col++) {
      free(Buffers[col]);
      free(sorted_cols[col]);
    }
    mylog(logfile, "Freed this page's buffers.");
    futhark_free_i64_1d(ctx, sorted_idx_ft);
    mylog(logfile, "Freed futhark objects for this page.");

    incr_idx += cur_rows;
  }

  duckdb_destroy_result(&res);
  mylog(logfile, "Freed initial duckdb result from the 1st stage.");

  // TODO currently Stg2 handles only key column - need to also reorder & store the payloads...
  // Will probably need to put each of those into their own buffers & then reorder, like in Stg1...

  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  // STAGE 2 - RETRIEVE DATA, SORTING THE FIRST BLOCK OF EACH FILE EACH TIME
  // #########################################################################################################################################
  // #########################################################################################################################################
  // #########################################################################################################################################
  mylog(logfile, "Now entering the second stage of processing...");
  // The plan:
  // 1. initialise a buffer of BUFFER_SIZE
  // half of it is used for new chunks, the other half for leftover data from the last sort
  // 2. read new chunks
  // the first time, read 1 chunk from each intermediate
  // from then on, use priorities (the newly read chunk with the smallest last element takes priority)
  // exhausted intermediates are exempt from priorities
  // 3. sort
  // when all are exhausted, dump the buffer's leftovers into the result
  // TODO figure out how to handle fragmentation gaps
  // current idea is a rowsToIgnore counter, & those are filled with max values & not copied, + the ones at the end are not sorted...
  // ...
  // TODO organise
  // TODO use max_padding for the long witch statements I was doing before...

  // 2nd stage requires all intermediates + at least 1 empty chunk to fit in the buffer
  if(numIntermediate > (BUFFER_CHUNK_CAPACITY - 1)) {
    perror("Insufficient buffer size.");
    return -1;
  }
  // Number of chunks that are picked up from the buffer into storage each iteration
  // Can increase if an intermediate is exhausted before the others (expected only for the last intermediate)
  idx_t pickup = BUFFER_CHUNK_CAPACITY - numIntermediate;
  // Number of rows to ignore, caused by not-full chunks
  // the slots of those rows in the buffer are padded with the type's maximum value, and neither read nor sorted
  // expectedly, only the last of all chunks can be not-full
  // so ignore_rows will be increased at most once, and not again
  // ALSO, a not-full chunk means that it's the last of that intermediate (assumption for duckdb's logic)
  // ergo, ignore_rows will be set to 0, and pickup increased by 1 at the end of that iteration
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
        return -1;
    }
  }
  const char *resultTblName = "TPSResult";
  char resultQueryStr[100 + 35*col_count];
  int resultQueryStr_len = sprintf(resultQueryStr, "CREATE OR REPLACE TEMP TABLE %s (", resultTblName);
  for(idx_t i=0; i<col_count; i++) {
    if(i<col_count-1) {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "x%ld %s, ", i, type_strs[i]);
    }
    else {
      resultQueryStr_len += sprintf(resultQueryStr + resultQueryStr_len, "x%ld %s);", i, type_strs[i]);
    }
  }
  // TODO for testing
  printf("%s\n", resultQueryStr);
  // Execute the query
  if( duckdb_query(con, resultQueryStr, NULL) == DuckDBError ) {
    perror("Failed to create result table.\n");
    return -1;
  }
  mylog(logfile, "Created table where the sorted results will be stored.");
  // Create appender
  duckdb_appender resultApp;
  if( duckdb_appender_create(con, NULL, resultTblName, &resultApp) == DuckDBError ) {
    perror("Failed to create appender for the result table.\n");
    return -1;
  }
  mylog(logfile, "Created appender for the result table.");

  // ---------- 3. Prepare the intermediates for scanning.
  duckdb_result interms[numIntermediate];
  for(idx_t i=0; i<numIntermediate; i++) {
    prepareToFetch_intermediate(i, con, &interms[i]);
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
        sortKeyColumn(ctx, Buffers2[0], type_ids[0], 0, &stg2_sorted_idx_ft, Buffers2[0], BUFFER_SIZE);
        continue;
      }
      orderPayloadColumn(ctx, Buffers2[c], type_ids[c], 0, stg2_sorted_idx_ft, Buffers2[c], BUFFER_SIZE);
    }
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
        return -1;
      }
      duckdb_appender_flush(resultApp);
      duckdb_destroy_data_chunk(&cnk);
    }
    mylog(logfile, "Appended data to result table.");
  }

  // review result
  printf("\n\nResults\n");
  duckdb_result finalRes;
  if( duckdb_query(con, "SELECT * FROM TPSResult;", &res) == DuckDBError ) {
    perror("Failed final query...\n");
    return -1;
  }
  idx_t cc = 0; // chunk counter
  while(true) {
    duckdb_data_chunk cnk = duckdb_fetch_chunk(res);
    if(!cnk) break;

    duckdb_vector vec = duckdb_data_chunk_get_vector(cnk, 0);
    long* dat = (long*)duckdb_vector_get_data(vec);

    idx_t r = duckdb_data_chunk_get_size(cnk);
    //for(idx_t j=0; j<r; j++) {
    //  printf("%ld, ", dat[j]);
    //}
      printf(
        "Chunk %ld: first element %ld, last element %ld, number of elements %ld.\n",
        cc,
        dat[0],
        dat[r-1],
        r
      );
    cc++;

    duckdb_destroy_data_chunk(&cnk);
  }
  duckdb_destroy_result(&finalRes);
  printf("\n\n");

  //Clean-up
  for(idx_t i=0; i<numIntermediate; i++) {
    duckdb_destroy_result(&interms[i]);
  }
  duckdb_appender_destroy(&resultApp);
  for(idx_t c=0; c<col_count; c++) {
    free(Buffers2[c]);
  }
  free(Buffers2);
  free(maxima);
  mylog(logfile, "Freed buffers and objects from Stage 2.");


  futhark_context_free(ctx);
  futhark_context_config_free(cfg);
  mylog(logfile, "Freed futhark core.");

	duckdb_disconnect(&con);
	duckdb_close(&db);
  mylog(logfile, "Disconnected duckdb and freed its memory.");

  logclose(logfile);
  
  return 0;
}