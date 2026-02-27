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
import "lib/github.com/diku-dk/segmented/segmented"

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
	: (i64, i64) =
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
	def radix_to_idx [b] (radix_bits : i32) (bitmask: byteSeq [b]) (bsq: byteSeq [b])
	: i64 =
		let rdx = getRadix bitmask bsq
		let radix_bytes = i64.i32 ((radix_bits + u8.num_bits - 1)/(u8.num_bits))
		in loop y : i64 = 0
			for j in (0..<radix_bytes) do
			let r = (i64.u8 rdx[b-j-1]) << (j*(i64.i32 u8.num_bits))
			in y | r

-- byteSeq & radix comparisons.

	-- | Equality between 2 byteseq by 1st-level radix.
	def byteSeq_eq [b]
		(fb : i64)
		(lb : i64)
		(bitmask : byteSeq [b])
		(x1: byteSeq [b])
		(x2: byteSeq [b])
	: bool =
	  let r1 = getRadix bitmask x1
	  let r2 = getRadix bitmask x2
	  in foldl (&&) (true) (map2 (==) r1[fb:lb+1] r2[fb:lb+1])

	-- | Non-equality between 2 byteseq by 1st-level radix.
	def byteSeq_neq [b]
		(fb : i64)
		(lb : i64)
		(bitmask : byteSeq [b])
		(x1: byteSeq [b])
		(x2: byteSeq [b])
	: bool =
	  let r1 = getRadix bitmask x1
	  let r2 = getRadix bitmask x2
	  in foldl (||) (false) (map2 (!=) r1[lb:fb+1] r2[lb:fb+1])

	-- | < between 2 byteseq by 1st-level radix.
	def byteSeq_lt [b]
		(fb : i64)
		(lb : i64)
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

	-- | > between 2 byteseq by 1st-level radix.
	def byteSeq_gt [b]
		(fb : i64)
		(lb : i64)
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

	-- TODO for PHJ will also need multi-level radix comparators (but primitive-based)

-- Radix-Partitioning

-- | Sorting step for radix-partition with payload data.
def radix_part_step [n][b] 't
	(i: i32)
	(j: i32)
	(xs: [n][b]u8)
	(pL: [n]t)
: ([n][b]u8, [n]t) =
	let xps = zip xs pL
	let xps' = radix_sort_multistep
		i j (\bit xp -> byteSeq_getBit bit xp.0) xps
	in unzip xps'

-- | Single-level radix-partition with payload data.
def radix_part [n][b] 't
	(i: i32)
	(j: i32)
	(bit_step: i32)
	(xs: [n][b]u8)
	(pL: [n]t)
: ([n][b]u8, [n]t) =
	let j_ = i32.min j ((i32.i64 b)*u8.num_bits - 1) in
	loop (xs, pL)
	for bit in (i..(i+bit_step)...(j_))
	do radix_part_step bit (i32.min j_ (bit+bit_step-1)) xs pL

def getPartitionBounds [n] [b]
	(curDepth: i32)
	(i: i32)
	(j: i32)
	(pXs: [n](byteSeq [b]))
: partitionInfo =
	let bitmask = mk_radix_bitmask i j b
	let (fb,lb) = radix_first_last_bytes i j b
	let boundIndices = pXs
		|> map (getRadix bitmask)
		|> group_boundaries (byteSeq_neq fb lb bitmask)
		|> zip (iota n)
		|> filter (.1)
		|> map (.0)
	in {
		maxDepth = curDepth,
		bounds = [0] ++ boundIndices,
		depths = replicate (1+ (length boundIndices)) curDepth
	}

-- | Multi-level radix-partitioning with payload data.
def partition_and_deepen 't [n] [b]
	(radix_size: i32)
	(size_thresh: i64)
	(max_depth_: i32)
	(bit_step: i32)
	(xs: [n](byteSeq [b]))
	(pLs: [n]t)
: ([n](byteSeq [b]), [n]t) =
	let max_J = (i32.i64 b)*u8.num_bits - 1
	let max_depth = i32.min max_depth_ (((i32.i64 b)*u8.num_bits + radix_size - 1)/radix_size)
	-- recursively subdivide partitions that are too large
	-- starting with the entire dataset as one too large partition
	let loop_over : ([n](byteSeq [b]), [n]t, [](i64, i64),  i32)
    	= loop (pXs, pPs, taidade, dp) = (xs, pLs, [(0,n)], 0)
    	while (length taidade)>0 && dp<max_depth do
    		let nt = length taidade
	        let new_i = radix_size*dp
	        let new_j = i32.min max_J (new_i + radix_size - 1)
	        -- get indices & lens of all taidade partitions
	        let pinds = taidade |> map (.0)
	        let plens = taidade |> map (.1)
	        -- create gather/scatter indices via segmented iota
			-- as well as replicated taidade partition ids
			let pids = plens |> replicated_iota -- compact
			let nr = length pids
			let gather_idx = pids
				|> group_boundaries (!=)
				|> segmented_iota
				|> sized nr
				|> map2 (+) (pids |> map (\i -> pinds[i]) |> sized nr)
			-- gather partitions & apply repartitioning
			-- Cases:
			-- 1. nt == 1, only process xs & pL
			-- 2. nt > 1, requires additional partition key info
			let (new_xs, new_pLs) =
				if nt == 1
				then
					let xps = indices gather_idx
						|> map (\i -> (pXs[gather_idx[i]], pPs[gather_idx[i]]))
						|> unzip
					in radix_part new_i new_j bit_step xps.0 xps.1
				else
					let xips = indices gather_idx
						|> map (\i -> (pXs[gather_idx[i]], (pids[i], pPs[gather_idx[i]])))
						|> unzip
					let new_xips = radix_part
						new_i new_j bit_step xips.0 xips.1
						|> (\ret -> zip ret.0 ret.1)
					-- sort by previous part id
					in new_xips
						|> map (\(x,(pid,pL)) -> (x,pid,pL))
						|> unzip3
						|> (\(xs, pids, pLs) -> bucket_sort bit_step nt pids (zip xs pLs))
						|> (\(_, xps) -> xps)
						|> unzip
			-- identify if there are any new taidade partitions
			let curBounds = (getPartitionBounds dp 0 new_j new_xs).bounds
			let n_xinBufen = length curBounds
			let xin_taidade = if dp==max_depth-1 then []
			else indices curBounds
				|> map (\i -> 
					if i==n_xinBufen-1
					then (curBounds[i],nr)
					else (curBounds[i],curBounds[i+1])
				)
				|> map (\(inf,sup) -> sup-inf)
				|> zip (indices curBounds)
				|> map (\(i,s) -> (gather_idx[curBounds[i]], s))
				|> filter (\(_,s) -> s>size_thresh)
			-- scatter & move on to the next iteration
			let xin_pXs = scatter (copy pXs) gather_idx new_xs
			let xin_pPs = scatter (copy pPs) gather_idx new_pLs
			in (xin_pXs, xin_pPs, xin_taidade, dp+1)
	in (loop_over.0, loop_over.1)

-- Struct Construction

