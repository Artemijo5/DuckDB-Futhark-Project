import "ftbasics"
import "ftsort"

-- Essentially using 6 objects
-- bInfo : holds constants
-- bProc : holds states of each chunk for each buffer
-- key & payload sorting buffers
-- key & payload waiting buffers
-- excepting bInfo, all other 5 objects are updated separately
-- as such need to coordinate their updating functions

-- always call the buffer update functions first, then the bProc function
-- this is to ensure the buffers see the unupdated bProc
-- except markSrcExhausted (no updates to buffers)
-- also nextUnusedChunk is called before writing to sorting buffers
-- & next_waiting is called before writing to waiting buffers (or marking exhausted)

-- Sorting funcs are just called separately on key * pL sorting buffers

-- Can possibly extend this to radix-partitioning
-- but how would re-partitionin be done?

-- Info-holding records & initialization

	type tps_bufferInfo = {
		chunkSize : i64, -- elements per chunk
		chunks_No : i64, -- #chunks in the buffer (total)	
		srcs_No   : i64, -- #intermediate (sorted) relations are used
	}

	type tps_bufferProc [chunks_No] [srcs_No] = {
		isChunkTaken    : [chunks_No]bool, -- for each chunk in the buffer, mark if it's been written
		free_positions  : [chunks_No]i64,   -- for each chunk in the buffer, mark number of unused positions
		isSrcExhausted  : [srcs_No]bool,   -- for each source, mark when it's exhausted
		freePos_waiting : [srcs_No]i64 -- free positions in waiting buffers
	}

	def init_bufferInfo chunkSize chunks_No srcs_No : tps_bufferInfo =
		{chunkSize=chunkSize,chunks_No=chunks_No,srcs_No=srcs_No}

	def init_bufferProc (bufferInfo : tps_bufferInfo)
	: tps_bufferProc [bufferInfo.chunks_No] [bufferInfo.srcs_No]
	= {
		isChunkTaken    = replicate bufferInfo.chunks_No false,
		free_positions  = replicate bufferInfo.chunks_No bufferInfo.chunkSize,
		isSrcExhausted  = replicate bufferInfo.srcs_No false,
		freePos_waiting = replicate bufferInfo.srcs_No bufferInfo.chunkSize
	}

-- Processing Info

	-- Next to get out of waiting buffer (done in module type mk_keyTps)

	def writeToWaiting_proc [chunks_No] [srcs_No]
		(at_src : i64)
		(chunk_size : i64)
		(bInfo : tps_bufferInfo)
		(bProc : *tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No] =
		let freePos_waiting' = (copy bProc.freePos_waiting)
			with [at_src] = bInfo.chunkSize - chunk_size
		in bProc with freePos_waiting = freePos_waiting'

	def markSrcExhausted [chunks_No] [srcs_No]
		(at_src : i64)
		(bProc : *tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No] =
		let isSrcExhausted' = (copy bProc.isSrcExhausted)
			with [at_src] = true
		in bProc with isSrcExhausted = isSrcExhausted'

	def next_unusedChunk [chunks_No] [srcs_No]
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: i64 =
		let at_cnk = bProc.isChunkTaken
			|> argmin (\c1 c2 -> !c1 || c2) (==) (true)
		in if (at_cnk>=chunks_No || bProc.isChunkTaken[at_cnk]) then (-1) else at_cnk

	def writeToBuffer_proc [chunks_No] [srcs_No]
		(from_src : i64)
		(at_cnk : i64)
		(bProc : *tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No] =
		let isChunkTaken' = (copy bProc.isChunkTaken)
			with [at_cnk] = true
		let free_positions' = (copy bProc.free_positions)
			with [at_cnk] = bProc.freePos_waiting[from_src]
		in (bProc with isChunkTaken = isChunkTaken')
			with free_positions = free_positions'

	def fetchSorted_proc [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : *tps_bufferProc [chunks_No] [srcs_No])
	: tps_bufferProc [chunks_No] [srcs_No] =
		let active_srcs = bProc.isSrcExhausted |> countFor (not)
		let total_free_pos = bProc.free_positions |> i64.sum
		let freeChunksAtTheEnd = total_free_pos/bInfo.chunkSize
		let chunks_toFetch = i64.min (chunks_No - active_srcs) (chunks_No - freeChunksAtTheEnd - 1)
		-- TODO am I calculating it right?
		let isChunkTaken' = ((copy bProc.isChunkTaken)
			with [0:chunks_toFetch] = (replicate chunks_toFetch false))
			with [chunks_No-freeChunksAtTheEnd : chunks_No] = (replicate freeChunksAtTheEnd false)
		in bProc with isChunkTaken = isChunkTaken'

-- Processing key buffers

module type mk_keyTps = {
	type t

	val highest : t
	val lt : t -> t -> bool
	val eq : t -> t -> bool

	val next_waiting [n] [chunks_No] [srcs_No]
	: tps_bufferProc [chunks_No] [srcs_No] -> [srcs_No][n]t -> i64

	val init_ks_buffer [chunks_No] [srcs_No]
	: tps_bufferInfo -> tps_bufferProc [chunks_No] [srcs_No] -> []t

	val init_ks_waiting [chunks_No] [srcs_No]
	: tps_bufferInfo -> tps_bufferProc [chunks_No] [srcs_No] -> [srcs_No][]t

	val writeToWaiting_ks [n] [chunkSize] [srcs_No]
	: i64 -> [n]t -> *[srcs_No][chunkSize]t -> [srcs_No][chunkSize]t

	val writeToBuffer_ks [chunkSize] [chunks_No] [srcs_No]
	: i64 -> i64 -> [srcs_No][chunkSize]t -> *[chunks_No*chunkSize]t -> [chunks_No*chunkSize]t

	val fetchSorted_ks [n] [chunks_No] [srcs_No]
		: tps_bufferInfo -> tps_bufferProc [chunks_No] [srcs_No] -> [n]t -> []t
}

module mk_keyTps_numeric (N : numeric) : mk_keyTps with t = N.t = {
	type t = N.t

	def highest = N.highest
	def lt = (N.<)
	def eq = (N.==)

	def next_waiting [n] [chunks_No] [srcs_No]
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_waiting : [srcs_No][n]t)
	: i64 = ks_waiting
		|> map (head)
		|> zip (bProc.isSrcExhausted)
		|> argmin
			(\(ex1,v1) (ex2,v2) -> ex2 || (!ex1 && (v1 `lt` v2)))
			(\(ex1,v1) (ex2,v2) -> (ex1==ex2) && (v1 `eq` v2))
			(false,highest)

	def init_ks_buffer [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: []t =
		replicate (chunks_No*bInfo.chunkSize) highest

	def init_ks_waiting [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
	: [srcs_No][]t =
		replicate srcs_No (replicate bInfo.chunkSize highest)
	
	def writeToWaiting_ks [n] [chunkSize] [srcs_No]
		(at_src : i64)
		(ks_dat: [n]t)
		(ks_waiting : *[srcs_No][chunkSize]t)
	: [srcs_No][chunkSize]t =
		(ks_waiting with [at_src,0:n] = ks_dat)
			with [at_src,n:chunkSize] = (replicate (chunkSize-n) highest)

	def writeToBuffer_ks [chunkSize] [chunks_No] [srcs_No]
		(from_src : i64)
		(at_cnk : i64)
		(ks_waiting: [srcs_No][chunkSize]t)
		(ks_buffer : *[chunks_No*chunkSize]t)
	: [chunks_No*chunkSize]t =
		ks_buffer with [at_cnk*chunkSize:(at_cnk+1)*chunkSize] = ks_waiting[from_src]

	def fetchSorted_ks [n] [chunks_No] [srcs_No]
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(ks_buffer : [n]t)
	: []t =
		let active_srcs = bProc.isSrcExhausted |> countFor (not)
		let total_free_pos = bProc.free_positions |> i64.sum
		let freeChunksAtTheEnd = total_free_pos/bInfo.chunkSize
		let chunks_toFetch = i64.min (chunks_No - active_srcs) (chunks_No - freeChunksAtTheEnd - 1)
		in ks_buffer[0:chunks_toFetch*bInfo.chunkSize]
}

-- Processing payload buffers

	def init_pL_buffer 't (dummy : t)
		(bInfo : tps_bufferInfo)
	: [bInfo.chunks_No*bInfo.chunkSize]t =
		replicate (bInfo.chunks_No * bInfo.chunkSize) dummy

	def init_pL_waiting 't (dummy : t)
		(bInfo : tps_bufferInfo)
	: [bInfo.srcs_No][bInfo.chunkSize]t =
		replicate bInfo.srcs_No (replicate bInfo.chunkSize dummy)

	def writeToWaiting_pL [n] 't
		(at_src : i64)
		(bInfo : tps_bufferInfo)
		(pL_dat: [n]t)
		(pL_waiting : *[bInfo.srcs_No][bInfo.chunkSize]t)
	: [bInfo.srcs_No][bInfo.chunkSize]t =
		pL_waiting with [at_src,0:n] = pL_dat

	def writeToBuffer_pL 't
		(from_src : i64)
		(at_cnk : i64)
		(bInfo : tps_bufferInfo)
		(pL_waiting: [bInfo.srcs_No][bInfo.chunkSize]t)
		(pL_buffer : *[bInfo.chunks_No*bInfo.chunkSize]t)
	: [bInfo.chunks_No*bInfo.chunkSize]t =
		pL_buffer with [at_cnk*bInfo.chunkSize:(at_cnk+1)*bInfo.chunkSize]
			= pL_waiting[from_src]

	def fetchSorted_pL [chunks_No] [srcs_No] 't
		(bInfo : tps_bufferInfo)
		(bProc : tps_bufferProc [chunks_No] [srcs_No])
		(pL_buffer : [bInfo.chunks_No*bInfo.chunkSize]t)
	: []t =
		let active_srcs = bProc.isSrcExhausted |> countFor (not)
		let total_free_pos = bProc.free_positions |> i64.sum
		let freeChunksAtTheEnd = total_free_pos/bInfo.chunkSize
		let chunks_toFetch = i64.min (chunks_No - active_srcs) (chunks_No - freeChunksAtTheEnd - 1)
		in pL_buffer[0:chunks_toFetch*bInfo.chunkSize]
		


