-- TODO routines for 2-pass sorting
-- Can see some potential similarity to string sorting (...)

-- Implementation:
-- each chunk fully takes up its designated space
-- empty slots are replaced by type.highest
-- so that they go to the end

-- Data is stored using typedCol.
-- And we use pointers to process that buffer.

-- | Stuct that holds information on sorting buffer.
-- Assumes the buffer size is a multiple of chunkSize.
type tps_bufferInfo [numChunks] 't = {
	chunkSize : i64,
	fetchSize : i64, -- how many chunks do we fetch each iteration
	rowsUsed: [numChunks]i64, -- unused rows = chunkSize - rowsUsed, then sum
	currentChunks : i64,
	highest : t
}

type tps_buffer [numChunks] [n] 't 'pL_t = {
	bufferInfo : tps_bufferInfo [numChunks] t,
	buffer : [n]t,
	pL_buffer : [n]pL_t
}

def create_tpsInfo 't chunk_size fetch_size chunks_No (highest : t)
: tps_bufferInfo [chunks_No] t = {
	chunkSize = chunk_size,
	fetchSize = fetch_size,
	rowsUsed = replicate chunks_No 0,
	currentChunks = 0,
	highest = highest
}

def create_tpsBuffer 't 'pL_t chunk_size fetch_size chunks_No (highest : t) (dummy_pL : pL_t)
: tps_buffer [chunks_No] [chunk_size*chunks_No] t pL_t = {
	bufferInfo = create_tpsInfo chunk_size fetch_size chunks_No highest,
	buffer     = replicate (chunk_size*chunks_No) highest,
	pL_buffer  = replicate (chunk_size*chunks_No) dummy_pL
}