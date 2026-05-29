-- Re-occuring routines for basic data-handling
-- Row indices are assumed to be i64

-- Gather Operations

	-- | Gather operation (based on futhark example).
	-- Use dummy value for invalid indices.
	def gather 't [ni] [n] (dummy_elem: t) (xs: [n]t) (is: [ni]i64) =
		is |> map (\i -> if (i>=0 && i<n) then xs[i] else dummy_elem)

	-- | Gather operation with destination array (based on futhark example).
	-- Consumes destination array.
	def gather_overArray 't [ni] [n] (dest: *[ni]t) (xs: [n]t) (is: [ni]i64) =
		dest
		|> zip is
		|> map (\(i,v) -> if (i>=0 && i<n) then xs[i] else v)

-- Aggregates

	-- | Exclusive scan operation (from Futhark by Example).
	def exscan f ne xs =
		map2
			(\i x -> if i==0 then ne else x)
			(indices xs)
			(rotate (-1) (scan f ne xs))

	-- | Function to count elements that satisfy a property.
	def countFor 't (p: t -> bool) (xs: []t) : i64 =
		i64.sum (xs |> map (p >-> i64.bool))

	-- | Index of minimum element - smallest index in case of draw.
	-- Based on Futhark by Example.
	-- NOTE: if lt and highest are given gt and lowest values, this becomes argmax.
	def argmin [n] 't
		(lt : t -> t -> bool)
		(eq : t -> t -> bool)
		(highest : t)
		(ks : [n]t)
	: i64 =
		let ne = (n, highest)
		let min_ik = ks
			|> zip (indices ks)
			|> reduce_comm(\(ix, vx) (iy, vy) ->
			    if (vx `lt` vy) || ((vx `eq` vy) && (ix < iy))
			      then (ix, vx)
			      else (iy, vy)
			  ) ne
		in min_ik.0

	-- | Sequential map function, for some cases of small static parallelism.
	def seqmap [n] 't 'ot
		(dummy_out : ot)
		(f : t -> ot)
		(xs: [n]t)
	: [n]ot =
		if n==0 then ([] :> [n]ot) else
		loop buff = (replicate n dummy_out) for j in (0..<n) do
			buff with [j] = f xs[j]

	-- | Sequential map2 function, for some cases of small static parallelism.
	def seqmap2 [n] 't1 't2 'ot
		(dummy_out : ot)
		(f : t1 -> t2 -> ot)
		(xs1: [n]t1)
		(xs2: [n]t2)
	: [n]ot =
		if n==0 then ([] :> [n]ot) else
		loop buff = (replicate n dummy_out) for j in (0..<n) do
			buff with [j] = f xs1[j] xs2[j]

-- Bulk Binary Search

	-- | Bulk binary search to locate the first matching element.
	--
	-- Outputs (-1) if no match exists, or if the initial index < 0.
	--
	-- Note: vs are on the left side of all comparisons.
	def bsearch_first [nvs] [n] 't
		(eq : t -> t -> bool)
		(gt : t -> t -> bool)
		(min_is : [nvs]i64)
		(max_is : [nvs]i64) -- exclusive
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs]i64 = vs |> map3 (\i_min i_max v ->
		let (found_at,_) = loop (i,last_step) = (i_min, i_max-i_min)
		while i>=0 && i>=i_min && i<i_max &&
			! ( (v `eq` xs[i]) && (i==i_min || (v `gt` xs[i-1])) )
		do
			-- check for v==xs[i] && v>xs[i-1] is done in loop conditions
			-- so inside loop assume that isn't the case
			let this_step = (last_step+1)/2 in
			if (v `gt` xs[i]) then
				if i==(i_max-1)
				then (-1,0)
				else (i64.min (i_max-1) (i+this_step), this_step)
			else
				if (i==i_min || (v `gt` xs[i-1]))
				then (-1,0)
				else (i64.max i_min (i - this_step), this_step)
		in found_at
	) min_is max_is

	-- | Bulk binary search to locate the last matching element.
	--
	-- If no match exists, outputs index of largest element smaller than v.
	-- If no smaller element exists, outputs (-1).
	-- Also outputs (-1) if the initial index < 0.
	--
	-- Note: vs are on the left side of all comparisons.
	def bsearch_last [nvs] [n] 't
		(geq: t -> t -> bool)
		(lt : t -> t -> bool)
		(min_is : [nvs]i64)
		(max_is : [nvs]i64) -- exclusive
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs]i64 = vs |> map3 (\i_min i_max v ->
		if i_min<0 || (i_min<n && (v `lt` xs[i_min])) then (-1) else
		let (found_at,_) = loop (i, last_step) = (i_min, i_max-i_min)
		while i>=0 && i>=i_min && i<i_max &&
			!( (v `geq` xs[i]) && ( i==(i_max-1) || (v `lt` xs[i+1]) ) )
		do
			-- check for kv>=cv && kv<nv is done in loop conditions
			-- so inside loop assume that isn't the case
			let this_step = (last_step+1)/2 in
			if (v `lt` xs[i]) then
				(i64.max i_min (i-this_step), this_step)
			else
				(i64.min (i_max-1) (i+this_step), this_step)
		in found_at
	) min_is max_is
		|> map (i64.min (n-1))

	-- | Bulk binary search to locate the index range of matching values.
	--
	-- Outputs (i0,c) the index of the first match and the count of matches.
	-- If no match is found, outputs (-1,0).
	--
	-- Note: vs are on the left side of all comparisons.
	def bsearch_range [nvs] [n] 't
		(eq : t -> t -> bool)
		(geq: t -> t -> bool)
		(gt : t -> t -> bool)
		(lt : t -> t -> bool)
		(min_is : [nvs]i64)
		(max_is : [nvs]i64) -- exclusive
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs](i64,i64) =
		-- Find the first match
		let diyige = bsearch_first
			(eq) (gt) min_is max_is xs vs
		-- Find the last match
		let zuihoude = bsearch_last
			(geq) (lt) diyige max_is xs vs
		-- Calculate the counts
		let duome = map2
			(\fm lm -> if fm<0 then 0 else lm-fm+1)
			diyige
			zuihoude
		in zip diyige duome

-- Radix Sorting/Partitioning

	-- | Represents a contiguous sequence of bytes.
	type byteSeq [bytes] = [bytes]u8

	import "lib/github.com/diku-dk/sorts/radix_sort"

	-- | Radix-based bucket-sort for i64 data.
	-- Meant for a 'small' number of compactly numbered buckets.
	def bucket_sort [n] 't
		(_: i32)
		(num_buckets : i64)
		(ks : [n]i64)
		(xs : [n]t)
	=
		let msb = num_buckets - 1 |> i64.clz |> (i32.-) i64.num_bits
		in zip ks xs
	    	|> radix_sort msb (\i (k,_) -> i64.get_bit i k)
			|> unzip

-- Grouping & Dictionary Encoding

	-- | Function to identify the group boundaries in an array of grouped keys.
	-- Returns a boolean array, with the first index of each group being true.
	-- NOTE : the previous element is on the left side of the neq comparator.
	def group_boundaries [n] 't (neq : t -> t -> bool) (xs : [n]t)
	: [n]bool = iota n
		|> map (\i -> if i==0 then true else (xs[i-1] `neq` xs[i]))

	-- | Dictionary encoding: assign compact i64 ids to grouped keys, using the group boundaries.
	def dict_encoding [n] (gbs : [n]bool)
	: [n]i64 = gbs
		|> map (i64.bool)
		|> scan (+) 0
		|> map (\i -> i-1)

-- Types used for joins.

	-- | Type used to store the information of a join (between x&y).
	-- vs : the values of x
	-- ix : the corresponding index of the value in x
	-- iy : the first match in y
	-- cm : number of matches found in y
	-- NOTE - each tuple corresponds to an individual x column, containing info about all its matches (or no matches) in y.
	-- Pure pairs are expressed with type joinPairs.
	type~ joinTup [n] 't = {vs : [n]t, ix: [n]i64, iy: [n]i64, cm: [n]i64}

	-- | The pairs obtained from joining x&y.
	-- vs : the values of each pair
	-- ix : the respective index in x
	-- iy : the respective index in y
	-- NOTE - unlike type joinTup, each tuple here corresponds to an individual match.
	type~ joinPairs 't = {vs: []t, ix: []i64, iy: []i64}

	-- | joinPairs for byteSeq [b]
	type~ joinPairs_bsq [b] = joinPairs (byteSeq [b])
		