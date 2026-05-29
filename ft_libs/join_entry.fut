-- Entry Points for Joins and related routines.

import "ftbasics"
import "ftColumns"
import "ftsort"
import "joins/ftSMJ"
import "tp_sort"

-- Wrapper types for GFTR & GFUR

	-- | Sorting information type (short)(GFTR).
	type sortStruct_i16 [n] [b] = sortStruct [n] [b] i16
	-- | Sorting information type (integer)(GFTR).
	type sortStruct_i32 [n] [b] = sortStruct [n] [b] i32
	-- | Sorting information type (long)(GFTR).
	type sortStruct_i64 [n] [b] = sortStruct [n] [b] i64
	-- | Sorting information type (float)(GFTR).
	type sortStruct_f32 [n] [b] = sortStruct [n] [b] f32
	-- | Sorting information type (double)(GFTR).
	type sortStruct_f64 [n] [b] = sortStruct [n] [b] f64

	-- | Sorting information type (short)(GFUR).
	type sortInfo_i16 [n] = sortInfo [n] i16
	-- | Sorting information type (integer)(GFUR).
	type sortInfo_i32 [n] = sortInfo [n] i32
	-- | Sorting information type (long)(GFUR).
	type sortInfo_i64 [n] = sortInfo [n] i64
	-- | Sorting information type (float)(GFUR).
	type sortInfo_f32 [n] = sortInfo [n] f32
	-- | Sorting information type (double)(GFUR).
	type sortInfo_f64 [n] = sortInfo [n] f64

	-- | Inner Equi-Join output for i32.
	type~ joinPairs_i32 = joinPairs i32
	-- | Inner Equi-Join output for i64.
	type~ joinPairs_i64 = joinPairs i64

-- 0. Column functions.

	module col_i32 = col_numeric i32
	module col_i64 = col_numeric i64

	-- | Initialize empty column for i32 data.
	entry mk_col_i32 = col_i32.mk_keyCol
	-- | Write i32 data to a column.
	entry update_col_i32 = col_i32.update_keyCol
	-- | Crop an i32 column.
	entry crop_col_i32 = col_i32.crop_keyCol
	-- | Read a segment from an i32 column.
	entry read_col_i32 = col_i32.read_keyCol

	-- | Initialize empty column for i64 data.
	entry mk_col_i64 = col_i64.mk_keyCol
	-- | Write i64 data to a column.
	entry update_col_i64 = col_i64.update_keyCol
	-- | Crop an i64 column.
	entry crop_col_i64 = col_i64.crop_keyCol
	-- | Read a segment from an i64 column.
	entry read_col_i64 = col_i64.read_keyCol

-- 1. Sorting

	-- | Sorting function for i32, also returns row indices.
	entry radix_sort_i32_GFUR [n] (xs : [n]i32) : sortInfo_i32 [n]
	= radixSort_int_GFUR 2 i32.num_bits i32.clz i32.get_bit xs

	-- | Sorting function for i64, also returns row indices.
	entry radix_sort_i64_GFUR [n] (xs : [n]i64) : sortInfo_i64 [n]
	= radixSort_int_GFUR 2 i64.num_bits i64.clz i64.get_bit xs


-- 2. Gathering

	-- | Perform a gather operation on an i32 payload column.
	entry gather_i32 pL is = gather 0i32 pL is
	-- | Perform a gather operation on an i64 payload column (or a column of row indices).
	entry gather_i64 pL is = gather (-1i64) pL is

-- 3. SMJ

	-- | Merge-Join routine for SMJ for i32 data.
	entry innerSMJ_i32 [nR] [nS]
		(tR : [nR]i32)
		(tS : [nS]i32)
	: joinPairs_i32 =
		do_InnerSMJ (==) (>=) (<=) (>) (<)
			((nR + nS + 16383)/16384)
			tR tS

	-- | Merge-Join routine for SMJ for i64 data.
	entry innerSMJ_i64 [nR] [nS]
		(tR : [nR]i64)
		(tS : [nS]i64)
	: joinPairs_i64 =
		do_InnerSMJ (==) (>=) (<=) (>) (<)
			((nR + nS + 16383)/16384)
			tR tS

-- 4. Two-Pass Sort

	entry init_bufferInfo cnk_sz cnk_No src_No
	: tps_bufferInfo = init_bufferInfo cnk_sz cnk_No src_No

	entry init_bufferProc (bufferInfo : tps_bufferInfo)
	: tps_bufferProc [bufferInfo.chunks_No] [bufferInfo.srcs_No]
	= init_bufferProc bufferInfo


	entry writeToWaiting_proc [chunks_No] [srcs_No]
		(at_src : i64)
		(chunk_size : i64)
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No]
	= writeToWaiting_proc at_src chunk_size bInfo bProc

	entry markSrcExhausted_proc [chunks_No] [srcs_No]
		(at_src : i64)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No]
	= markSrcExhausted at_src bProc

	entry next_unusedChunk_proc [chunks_No] [srcs_No]
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: i64
	= next_unusedChunk bProc
	
	entry writeToBuffer_proc [chunks_No] [srcs_No]
		(from_src : i64)
		(at_cnk : i64)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No]
	= writeToBuffer_proc from_src at_cnk bProc

	entry fetchSorted_proc [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No]
	= fetchSorted_proc bInfo bProc


	module colTps_i32 = mk_keyTps_numeric i32
	module colTps_i64 = mk_keyTps_numeric i64


	entry tps_next_waiting_i32 [n] [chunks_No] [srcs_No]
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_waiting : [srcs_No][n]i32)
	: i64 = colTps_i32.next_waiting bProc ks_waiting

	entry init_tps_buffer_i32 (bInfo : tps_bufferInfo)
	= colTps_i32.init_ks_waiting bInfo

	entry writeToWaiting_i32
	= colTps_i32.writeToWaiting_ks

	entry writeToBuffer_i32 [chunkSize] [buff_size] [srcs_No]
		(from_src : i64)
		(at_cnk : i64)
		(ks_waiting: [srcs_No][chunkSize]i32)
		(ks_buffer : *[buff_size]i32)
	= ks_buffer with [at_cnk*chunkSize:(at_cnk+1)*chunkSize] = ks_waiting[from_src]

	entry fetchSorted_i32 [n] [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_buffer : [n]i32)
	= colTps_i32.fetchSorted_ks bInfo bProc ks_buffer

	entry afterFetching_i32 [n] [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_buffer : *[n]i32)
	= colTps_i32.fetchSorted_ks bInfo bProc ks_buffer


	entry tps_next_waiting_i64 [n] [chunks_No] [srcs_No]
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_waiting : [srcs_No][n]i64)
	: i64 = colTps_i64.next_waiting bProc ks_waiting

	entry init_tps_buffer_i64 (bInfo : tps_bufferInfo)
	= colTps_i64.init_ks_waiting bInfo

	entry writeToWaiting_i64
	= colTps_i64.writeToWaiting_ks

	entry writeToBuffer_i64 [chunkSize] [buff_size] [srcs_No]
		(from_src : i64)
		(at_cnk : i64)
		(ks_waiting: [srcs_No][chunkSize]i64)
		(ks_buffer : *[buff_size]i64)
	= ks_buffer with [at_cnk*chunkSize:(at_cnk+1)*chunkSize] = ks_waiting[from_src]

	entry fetchSorted_i64 [n] [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_buffer : [n]i64)
	= colTps_i64.fetchSorted_ks bInfo bProc ks_buffer

	entry afterFetching_i64 [n] [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_buffer : *[n]i64)
	= colTps_i64.fetchSorted_ks bInfo bProc ks_buffer


-- 5. String Processing


-- 6. String Sorting & Hashing


-- 7. String Equi-Joins