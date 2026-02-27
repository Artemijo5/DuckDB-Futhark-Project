-- Radix-partitioning, used for Partitioned Hash Join.
-- Data is sorted on a radix, and grouped in contiguously stored partitions.
-- Skewed partitions are re-partitioned using additional bits ('depth').
--
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39

-- NOTE: bytes in a byteseq are indexed in decreasing order.
-- ie in byteSeq [b], byte 0 is the last, byte b-1 is the first

import "ftbasics"

-- Structs

	-- | Compact list giving information for all partitions present.
	--
	-- For PHJ, binary search is performed on this list to find corresponding partitions.
	type~ partitionInfo = {maxDepth: i32, bounds: []i64, depths: []i32}

	-- | Non-compact table providing information on the 1st-level radix.
	-- Specifically, the first and last partition in partitionInfo with that radix.
	-- For partitions that aren't present, both are set to (-1).
	--
	-- For PHJ, allows to skip first radix_bits iterations of the binary search.
	-- In the case all partitions are of depth 1: entirely skips the binary search.
	type radix_hashTable [rb] = {
		first_info_idx : [2**rb]i64,
		last_info_idx  : [2**rb]i64
	}

-- Basic Operations

	-- | Create a b-sized byteSeq of all-0 bytes.
	def dummy_byteSeq b = replicate b 0u8

	-- | Get a specific bit of a byteseq.
	def byteSeq_getBit [b] (i: i32) (x: byteSeq [b])
	: i32 =
		let whichByte = (i32.i64 b) - (i/u8.num_bits) - 1
		let whichBit = i % u8.num_bits
		in x[whichByte]
			|> u8.get_bit whichBit

	-- | Obtain the first (leftmost) and last (rightmost) bytes used by the radix.
	-- Note: last <= first
	def radix_first_last_bytes (i : i32) (j_ : i32) (b : i64)
	: (i32, i32) =
		let bi = i32.i64 b
		let j = i32.min j_ (bi*u8.num_bits - 1)
		let firstByte = i64.i32 (bi - (i/u8.num_bits) - 1)
		let lastByte = i64.i32 (bi - (j/u8.num_bits) - 1)
		in (firstByte, lastByte)

	-- | Create the bitmask for radix extraction.
	def mk_radix_bitmask (i : i32) (j_ : i32) (b : i64)
	: byteSeq [b] =
		let bi = i32.i64 b
		let j = i32.min j_ (bi*u8.num_bits - 1)
		let (firstByte, lastByte) = radix_first_last_bytes i j b
		let firstBit = i%u8.num_bits
		let lastBit = j%u8.num_bits
		let first_bitMask = u8.highest << (u8.i32 firstBit)
		let last_bitMask = (u8.>>>) u8.highest (u8.i32 (u8.num_bits - lastBit - 1))
		in iota b
			|> map (\i ->
				if i<lastByte || i>firstByte then 0u8 else
				if i==firstByte && i==lastByte then first_bitMask & last_bitMask else
				if i==firstByte then first_bitMask else
				if i==lastByte  then last_bitMask  else
					u8.highest
			)

	-- | Obtain the radix of a byteseq.
	def getRadix [b] (bitmask : byteSeq [b]) (x: byteSeq [b])
	: byteSeq [b] =
	 	map2 (&) bitmask x

	-- | Obtain the i64 value represented by the radix's bits.
	def radix_to_idx [b] (radix_bits : i32) (bitmask: i32) (bsq: byteSeq [b])
	: i64 =
		let rdx = getRadix bitmask bsq
		let radix_bytes = i64.i32 ((radix_bits + u8.num_bits - 1)/(u8.num_bits))
		in loop y : idx_t.t = 0
			for j in (0..<radix_bytes) do
			let r = (i64.u8 rdx[b-j-1]) << (j*(i64.i32 u8.num_bits))

-- byteSeq & radix comparisons.

-- | Equality between 2 byteseq by radix.
def byteSeq_eq [b] (fb : i32) (lb : i32) (bitmask : byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix bitmask x1
  let r2 = getRadix bitmask x2
  in foldl (&&) (true) (map2 (==) r1[fb:lb+1] r2[fb:lb+1])

-- | Non-equality between 2 byteseq by radix.
def byteSeq_neq [b] (fb : i32) (lb : i32) (bitmask : byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix bitmask x1
  let r2 = getRadix bitmask x2
  in foldl (||) (false) (map2 (!=) r1[lb:fb+1] r2[lb:fb+1])

-- | < between 2 byteseq by radix.
def byteSeq_lt [b]
	(fb : i32)
	(lb : i32)
	(bitmask : byteSeq [b])
	(x1: byteSeq [b])
	(x2: byteSeq [b])
: bool =
	let r1 = getRadix bitmask x1
	let r2 = getRadix bitmask x2
	let (lt, _) =
		loop (def_lt, byte) = (false, lb)
		while (!def_lt && byte<=fb) do
			let dlt = r1[byte] < r2[byte]
			in (dlt, byte+1)
	in lt

-- | > between 2 byteseq by radix.
def byteSeq_gt [b]
	(fb : i32)
	(lb : i32)
	(bitmask : byteSeq [b])
	(x1: byteSeq [b])
	(x2: byteSeq [b])
: bool =
	let r1 = getRadix bitmask x1
	let r2 = getRadix bitmask x2
	let (gt, _) =
		loop (def_gt, byte) = (false, lb)
		while (!def_gt && byte<=fb) do
			let dlt = r1[byte] < r2[byte]
			in (dlt, byte+1)
	in gt

-- TODO for PHJ will also need multi-level radix comparators (but primitive-based ig)

-- Radix-Partitioning


-- Struct Construction

