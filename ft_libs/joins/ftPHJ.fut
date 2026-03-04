import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"
import "../ft_partition"

-- Partitioned Hash Join
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39
-- & References

-- Method:
-- After applying partitioning, we transform the radices to unsigned integer primitives.
-- (this requires that the keys are no more than 64 bits)
-- This is done because
-- a. binary search operations & comparisons on primitives are faster than on nested arrays.
-- b. allows to shrink the data size to the minimum required.
-- Multi-level radices can also be processed as just one number via bit rearrangement.

-- | Parametric module to perform hash join after partitioning.
-- Radices processed as unsigned integers of appropriate size.
module prim_PHJ (U : integral) = {
	type t = U.t

	local def zero : t = U.i32 0
	local def one  : t = U.i32 1
	local def ones : t = U.highest

	local def num_bits : i32 = U.num_bits
	local def num_bytes: i64 = i64.i32 (num_bits / u8.num_bits)

	local def from_u8  : u8  -> t = (U.u8)
	local def from_i32 : i32 -> t = (U.i32)
	local def to_i64 = (U.to_i64)

	local def eq = (U.==)
	local def gt = (U.>)
	local def lt = (U.<)
	local def geq = (U.>=)
	local def leq = (U.<=)

	local def lshift : t -> i32 -> t = (\x bi -> bi |> from_i32 |> (U.<<) x)
	local def rshift : t -> i32 -> t = (\x bi -> bi |> from_i32 |> (U.>>>) x)
	local def bitwise_or  : t -> t -> t = (U.|)
	local def bitwise_and : t -> t -> t = (U.&)

	-- | Convert byteSeq into unsigned integer of type t
	local def from_byteSeq [b] (x : byteSeq [b]) : t =
		loop y : t = zero
		for j<(i64.min b num_bytes) do
			let r = (from_u8 x[b-j-1]) `lshift` ((i32.i64 j) * u8.num_bits)
			in y `bitwise_or` r

	-- | Gets a single level of the radix.
	local def get_simple_radix
		(radix_bits : i32)
		(cur_level  : i32)
		(x : t)
	: t =
		let lx = x `rshift` (radix_bits*cur_level)
		let mask = ones `rshift` (num_bits - radix_bits)
		in lx `bitwise_and` mask

	-- | Obtains a multi-level radix, with bits re-arranged by level.
	local def get_radix
		(radix_bits : i32)
		(depth : i32)
		(x : t)
	: t =
		loop y : t = zero
		for j<depth do
			let r = (x |> get_simple_radix radix_bits j)
				`lshift` (radix_bits*(depth-j-1))
			in y `bitwise_or` r

	-- | Radix comparisons (a single primitive comparison between re-arranged bits)
	local def radix_cmp
		(cmp : t -> t -> bool)
		(radix_bits : i32)
		(depth : i32)
		(x1 : t)
		(x2 : t)
	: bool =
		let r1 = get_radix radix_bits depth x1
		let r2 = get_radix radix_bits depth x2
		in r1 `cmp` r2

	-- | Radix ==
	local def radix_eq radix_bits depth x1 x2 = radix_cmp (eq) radix_bits depth x1 x2
	-- | Radix >
	local def radix_gt radix_bits depth x1 x2 = radix_cmp (gt) radix_bits depth x1 x2
	-- | Radix <
	local def radix_lt radix_bits depth x1 x2 = radix_cmp (lt) radix_bits depth x1 x2
	-- | Radix >=
	local def radix_geq radix_bits depth x1 x2 = radix_cmp (geq) radix_bits depth x1 x2
	-- | Radix <=
	local def radix_leq radix_bits depth x1 x2 = radix_cmp (leq) radix_bits depth x1 x2

	-- | Complex binary search for finding matching partitions.
	-- Finds the matching partition in xs for each v, if it exists.
	-- f_init, l_init, maxRange obtained from hash table.
	local def radix_bsearch [nv]
		(radix_bits : i32)
		(bounds : []i64)
		(depths : []i32)
		(maxRange : i64)
		(f_init : [nv]i64) -- first potential match
		(l_init : [nv]i64) -- last potential match
		(xs : []t)
		(vs : [nv]t)
	: [nv]i64 =
		let maxIter = 1 + (maxRange |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
		let max3 (i1 : i64) (i2 : i64) (i3 : i64) = i64.max i1 (i64.max i2 i3)
		in if maxIter<=1 then f_init else
		let (foundAt, _) =
			loop (is, last_steps) = (f_init, map2 (\fi li -> li - fi + 1) f_init l_init)
			for _ in iota maxIter do
				let this_steps = last_steps |> map (\ls -> (ls+1)/2)
				let searchAt = is
					|> map2 (\fi i ->
						let prev_elem = xs[bounds[max3 0 fi (i-1)]]
						let this_elem = xs[bounds[max3 0 fi i]]
						in (i, prev_elem, this_elem)
					) f_init
					|> map5 (\kv fi li step (i,pv,cv) ->
						if i<0 then (-1) else
						if radix_eq radix_bits depths[i] kv cv
							then i
						else if radix_gt radix_bits depths[i] kv cv then
							if i==li
							then (-1)
							else i64.min li (i+step)
						else
							if i==fi || radix_gt radix_bits depths[i-1] kv pv
							then (-1)
							else i64.max fi (i-step)
					) vs f_init l_init this_steps
				in (searchAt, this_steps)
		in foundAt

	-- | Locate the matching partition of xs for each radix in vs
	local def hash_bsearch [nv]
		(radix_bits : i32)
		(xInfo : partitionInfo)
		(xTbl  : radix_hashTable [i64.i32 radix_bits])
		(xs : []t)
		(vs : [nv]t)
	: [nv]i64 =
		-- Get the potentially matching partitions from partitionInfo
		let (f_init, l_init) = vs
			|> map (\v -> v |> get_radix radix_bits 1 |> to_i64)
			|> map (\i -> (xTbl.first_info_idx[i], xTbl.last_info_idx[i]))
			|> unzip
		-- Get the max partition range among radices present in values
		-- Use the fact that vs are grouped by 1st-level radix to take distinct
		let maxRange = if xInfo.maxDepth==1 then 1 else f_init
			|> group_boundaries (!=)
			|> zip3 f_init l_init
			|> filter (.2)
			|> map (\(fi,li,_) -> if fi<0 then 0 else li-fi+1)
			|> i64.maximum
		in radix_bsearch
			radix_bits xInfo.bounds xInfo.depths maxRange f_init l_init xs vs

	-- | Sequential search to find the matches of v in a partition of x.
	-- Returns the index of the first match (-1 if none) and number of matches.
	local def count_matches [n] [np] (partId : i64) (bounds: [np]i64) (xs : [n]t) (v : t)
	: (i64, i64) =
		if partId < 0 then (-1,0) else
		let inf = bounds[partId]
		let sup = if partId==(np-1) then n else bounds[partId+1]
		in loop (j,y) = (-1,0) for i in (inf..<sup) do
			let j' = if j<0 && (v `eq` xs[i]) then i else j
			let y' = if (v `eq` xs[i]) then y+1 else y
			in (j',y')

	-- | Sequential search to find the k-th match of v in x starting from a given position.
	-- Assuming the k-th match exists (unless startFrom < 0)
	-- Returns the index of the k-th match.
	local def find_kth_match (startFrom : i64) (k : i64) (xs : []t) (v : t)
	: i64 =
		if startFrom<0 then startFrom else
		let (foundAt_plusOne,_) =
			loop (i,found)=(startFrom,0) while found<k do
				if (v `eq` xs[i])
				then (i+1,found+1)
				else (i+1,found)
		in foundAt_plusOne - 1

	-- | PHJ match-finding phase.
	--
	-- First locates the matching partition in R for all values of S via the Hash Table.
	-- If there are deep partitions, a binary search over partitionInfo is also performed.
	-- Then it performs a sequential scan to get the number of matches.
	-- Returns the mapped-to-unsigned relations to pass to the expansion phase.
	def phj_matchFinding [nR] [nS] [b]
		(radix_bits : i32)
		(tR : [nR](byteSeq [b]))
		(tS : [nS](byteSeq [b]))
		(tR_info : partitionInfo)
		(tR_hashTbl : radix_hashTable [i64.i32 radix_bits])
	: ([nR]t, joinTup [nS] t) =
		-- Map tR, tS to t
		let uR = tR |> map (from_byteSeq)
		let uS = tS |> map (from_byteSeq)
		-- Find matching partitions
		let (iy,cm) = uS
			|> hash_bsearch radix_bits tR_info tR_hashTbl uR
		-- Find indices & number of matches
			|> zip uS
			|> map (\(v,pid) -> count_matches pid tR_info.bounds uR v)
			|> unzip
		-- Return
		in (uR, {vs = uS, ix = iota nS, iy = iy, cm = cm})

	-- | PHJ expansion phase (for Inner Join).
	-- Expansion repeats sequential scans, starting from index of first match.
	def phj_expand [nR] [nS] [b]
		(tR : [nR](byteSeq [b]))
		(uR : [nR]t)
		(matches : joinTup [nS] t)
	: joinPairs (byteSeq [b]) =
		let (exp_ix, exp_iy) = zip4 matches.vs matches.ix matches.iy matches.cm
			|> expand (.3) (\(v,ix,iy,_) ind -> (ix, find_kth_match iy (ind+1) uR v))
			|> unzip
		-- output pretends R was left-side
		let exp_vs = exp_iy |> map (\i -> tR[i])
		in {vs = exp_vs, ix = exp_iy, iy = exp_ix}

	-- | PHJ full join routine (for Inner Join).
	def do_InnerPHJ [nR] [nS] [b]
		(radix_bits : i32)
		(tR : [nR](byteSeq [b]))
		(tS : [nS](byteSeq [b]))
		(tR_info : partitionInfo)
		(tR_hashTbl : radix_hashTable [i64.i32 radix_bits])
	: joinPairs (byteSeq [b]) =
		let (uR, jTup) = phj_matchFinding radix_bits tR tS tR_info tR_hashTbl
		in jTup |> phj_expand tR uR
}

module u8_phj  = prim_PHJ u8
module u16_phj = prim_PHJ u16
module u32_phj = prim_PHJ u32
module u64_phj = prim_PHJ u64

type~ joinPairs_bsq [b] = joinPairs (byteSeq [b])

-- | PHJ full join routine (for Inner Join).
-- Uses primitives of the minimum needed size.
def innerPHJ [nR] [nS] [b]
	(radix_bits : i32)
	(tR : [nR](byteSeq [b]))
	(tS : [nS](byteSeq [b]))
	(tR_info : partitionInfo)
	(tR_hashTbl : radix_hashTable [i64.i32 radix_bits])
: joinPairs_bsq [b] =
	if b==1 then
		u8_phj.do_InnerPHJ radix_bits tR tS tR_info tR_hashTbl
	else if b==2 then
		u16_phj.do_InnerPHJ radix_bits tR tS tR_info tR_hashTbl
	else if b<=4 then
		u32_phj.do_InnerPHJ radix_bits tR tS tR_info tR_hashTbl
	else
		u64_phj.do_InnerPHJ radix_bits tR tS tR_info tR_hashTbl