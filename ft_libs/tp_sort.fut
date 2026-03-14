-- TODO routines for 2-pass sorting
-- Can see some potential similarity to string sorting (...)

-- Data is stored using typedCol.
-- And we use pointers to process that buffer.

-- | Stuct that holds information on sorting buffer.
-- Assumes the buffer size is a multiple of chunkSize.
type tps_bufferInfo [numChunks] = {
	chunkSize : i64,
	fetchSize : i64, -- how many chunks do we fetch each iteration
	startsAt : [numChunks]i64,
	rowsUsed: [numChunks]i64,
	currentChunks : i64
}

type tps_buffer [numChunks] [n] 't 'pL_t = {
	bufferInfo : tps_bufferInfo [numChunks],
	buffer : [n]t,
	pL_buffer : [n]pL_t
}

def create_tpsInfo chunk_size fetch_size chunks_No
: tps_bufferInfo [chunks_No] = {
	chunkSize = chunk_size,
	fetchSize = fetch_size,
	startsAt = replicate chunks_No 0,
	rowsUsed = replicate chunks_No 0,
	currentChunks = 0
}

def create_tpsBuffer 't 'pL_t chunk_size fetch_size chunks_No (dummy : t) (dummy_pL : pL_t)
: tps_buffer [chunks_No] [chunk_size*chunks_No] t pL_t = {
	bufferInfo = create_tpsInfo chunk_size fetch_size chunks_No,
	buffer     = replicate (chunk_size*chunks_No) dummy,
	pL_buffer  = replicate (chunk_size*chunks_No) dummy_pL
}