-- TODO routines for 2-pass sorting

-- Data is stored using typedCol.
-- And we use pointers to process that buffer.

-- | Stuct that holds information on sorting buffer.
-- Assumes the buffer size is a multiple of chunkSize.
type~ tps_bufferInfo [numChunks] = {
	chunkSize : i64,
	fetchSize : i64, -- how many chunks do we fetch each iteration
	startsAt : [chunks_No]i64,
	rowsUsed: [chunks_No]i64
}

def create_tpsInfo chunk_size fetch_size chunks_No
: tps_bufferInfo [chunks_No] = {
	chunkSize = chunk_size,
	fetchSize = fetch_size,
	startsAt = replicate chunks_No 0,
	rowsUsed = replicate chunks_No 0
}