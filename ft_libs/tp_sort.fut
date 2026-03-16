import "ftbasics"
import "ftsort"

-- TODO routines for 2-pass sorting

type tps_bufferStats 't 'pL_t = {
	chunkSize : i64, -- elements per chunk
	chunks_No : i64, -- #chunks in the buffer (total)	
	srcs_No   : i64, -- #intermediate (sorted) relations are used
	fetchCnks : i64, -- #chunks fetched each iteration
	highest  : t,
	lowest   : t,
	dummy_pL : pL_t
}

type tps_bufferInfo [chunks_No] [srcs_No] = {
	isSrcExhausted  : [srcs_No]bool,   -- for each source, mark when it's exhausted
	isChunkTaken    : [chunks_No]bool, -- for each chunk in the buffer, mark if it's been written
	free_positions  : [chunks_No]i64,   -- for each chunk in the buffer, mark number of unused positions
	freePos_waiting : [srcs_No]i64 -- free positions in waiting buffers
}

type tps_buffer [chunkSize] [chunks_No] 't 'pL_t = {
	ks : [chunkSize*chunks_No]t,
	pL : [chunkSize*chunks_No]pL_t
}

type tps_waiting_buffer [srcs_No] [chunkSize] 't 'pL_t = {
	ks : [srcs_No][chunkSize]t,
	pL : [srcs_No][chunkSize]pL_t
}

type tps_proc [chunkSize] [chunks_No] [srcs_No] 't 'pL_t = {
	tps_stats: tps_bufferStats t pL_t,
	tps_info : tps_bufferInfo [chunks_No] [srcs_No],
	buffs : tps_buffer [chunkSize] [chunks_No] t pL_t,
	waiting_buffs : tps_waiting_buffer [srcs_No] [chunkSize] t pL_t,
	chunk_cursor : i64
}

def init_TwoPassSort 't 'pL_t
	(highest : t) (lowest : t) (dummy_pL : pL_t)
	(chunkSize : i64)
	(chunks_No : i64)
	(srcs_No   : i64)
	(fetchCnks : i64)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	let init_bStats = {
		chunkSize = chunkSize,
		chunks_No = chunks_No,
		srcs_No   = srcs_No,
		fetchCnks = fetchCnks,
		highest  = highest,
		lowest   = lowest,
		dummy_pL = dummy_pL
	}
	let init_bInfo = {
		isSrcExhausted  = replicate srcs_No false,
		isChunkTaken    = replicate chunks_No false,
		free_positions  = replicate chunks_No chunkSize,
		freePos_waiting = replicate srcs_No chunkSize
	}
	let init_bs = {
		ks = replicate (chunkSize*chunks_No) highest,
		pL = replicate (chunkSize*chunks_No) dummy_pL
	}
	let init_wbs = {
		ks = replicate srcs_No (replicate chunkSize highest),
		pL = replicate srcs_No (replicate chunkSize dummy_pL)
	}
	in {
		tps_stats = init_bStats,
		tps_info  = init_bInfo,
		buffs = init_bs,
		waiting_buffs = init_wbs,
		chunk_cursor = 0
	}

def write_src [n] [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(at_src : i64)
	(src_ks : [n]t)
	(src_pL : [n]pL_t)
	(sort_proc : *tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	let pad_ks = replicate (chunkSize-n) sort_proc.tps_stats.highest
	let pad_pL = replicate (chunkSize-n) sort_proc.tps_stats.dummy_pL
	let new_waitingBuffs = {
		ks = (copy sort_proc.waiting_buffs.ks) with [at_src] =
			(copy src_ks ++ pad_ks) |> sized chunkSize,
		pL = (copy sort_proc.waiting_buffs.pL) with [at_src] =
			(copy src_pL ++ pad_pL) |> sized chunkSize
	}
	let new_freePosWaiting = (copy sort_proc.tps_info.freePos_waiting)
		with [at_src] = chunkSize-n
	in (sort_proc with waiting_buffs = new_waitingBuffs)
		with tps_info.freePos_waiting = new_freePosWaiting

def mark_src_exhausted [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(at_src : i64)
	(sort_proc : *tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	let new_isSrcExhausted = (copy sort_proc.tps_info.isSrcExhausted)
		with [at_src] = true
	in sort_proc with tps_info.isSrcExhausted = new_isSrcExhausted

def whichSrcNext [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(lt : t -> t -> bool)
	(eq : t -> t -> bool)
	(sort_proc : tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t)
: i64 =
	sort_proc.waiting_buffs.ks
		|> map (head)
		|> zip sort_proc.tps_info.isSrcExhausted
		|> argmin
			(\(isEx1,h1) (isEx2,h2) ->
				isEx2 || (!isEx1 && (h1 `lt` h2))
			)
			(\(isEx1,h1) (isEx2,h2) -> (isEx1 == isEx2) && (h1 `eq` h2))
			(false, sort_proc.tps_stats.highest)

def resetCursor [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(sort_proc : *tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	sort_proc with chunk_cursor = 0

def do_fromWaitingToBuff [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(from_src : i64)
	(ks : *[chunkSize*chunks_No]t)
	(pL : *[chunkSize*chunks_No]pL_t)
	(sort_Info  : *tps_bufferInfo [chunks_No] [srcs_No])
	(sort_stats : *tps_bufferStats t pL_t)
	(waiting_buffs: *tps_waiting_buffer [srcs_No] [chunkSize] t pL_t)
	(chunk_cursor : i64)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	let new_buffs = {
		ks = ks with [chunk_cursor*chunkSize:(chunk_cursor+1)*chunkSize]
			= waiting_buffs.ks[from_src],
		pL = pL with [chunk_cursor*chunkSize:(chunk_cursor+1)*chunkSize]
			= waiting_buffs.pL[from_src]
	}
	let new_FreePos = (copy sort_Info.free_positions)
		with [chunk_cursor] = sort_Info.freePos_waiting[from_src]
	let new_sortInfo = sort_Info with free_positions = new_FreePos
	in {
		tps_stats = sort_stats,
		tps_info  = new_sortInfo,
		buffs = new_buffs,
		waiting_buffs = waiting_buffs,
		chunk_cursor = (chunk_cursor+1)
	}

-- TODO do I really have to copy everything?
-- alternative is probably to call do_fromWaitingToBuff as entry if possible?
def fromWaitingToBuff [chunkSize] [chunks_No] [srcs_No] 't 'pL_t
	(from_src : i64)
	(sort_proc : *tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t)
: tps_proc [chunkSize] [chunks_No] [srcs_No] t pL_t =
	do_fromWaitingToBuff
		from_src
		(copy sort_proc.buffs.ks)
		(copy sort_proc.buffs.pL)
		(copy sort_proc.tps_info)
		(copy sort_proc.tps_stats)
		(copy sort_proc.waiting_buffs)
		(sort_proc.chunk_cursor)

-- TODO
-- 0. mark used chunks above
-- 1. sorting funcs (they also note free positions at the end to mark unused chunks)
-- 1+. partitioning funcs (?)
-- 2. fetching funcs
-- + comments
-- + entry points

