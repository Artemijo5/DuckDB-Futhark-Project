-- Re-occuring routines for basic data-handling
-- Row indices are assumed to be i64

-- Gather Operations

	-- | Gather operation (based on futhark example).
	-- Use dummy value for invalid indices.
	def gather 't [ni] [n] (dummy_elem: t) (xs: [n]t) (is: [ni]i64) =
		is |> map (\i -> if (i>=0 && i<n) then xs[i] else dummy_elem)

	-- | Gather operation with destination array (based on futhark example).
	-- Does NOT consume destination array.
	def gather_overArray 't [ni] [n] (dest: [ni]t) (xs: [n]t) (is: [ni]i64) =
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

	-- | Sequential map function, for some cases of nested parallelism.
	def seqmap [n] 't 'ot
		(dummy_out : ot)
		(f : t -> ot)
		(xs: [n]t)
	: [n]ot =
		if n==0 then ([] :> [n]ot) else
		loop buff = (replicate n dummy_out) for j in (0..<n) do
			buff with [j] = f xs[j]

-- Bulk Binary Search

	-- | Bulk binary search to locate the first matching element.
	--
	-- Outputs (-1) if no match exists, or if the initial index < 0.
	--
	-- Note: vs are on the left side of all comparisons.
	def bsearch_first [nvs] [n] 't
		(eq : t -> t -> bool)
		(gt : t -> t -> bool)
		(init_is : [nvs]i64)
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs]i64 =
		let num_iter = 1 + (n |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
		let (foundAt,_) = loop (is,last_step) = (init_is,n)
		for _ in iota num_iter do
			let this_step = (last_step + 1)/2
			let searchAt = is
				|> map (\i ->
					let prev_elem = xs[i64.max 0 (i-1)]
					let cur_elem = xs[i64.max 0 i]
					in (i, prev_elem, cur_elem)
				)
				|> zip vs
				|> map (\(kv, (i, pv, cv)) ->
					if i<0 then (-1) else
					if (kv `eq` cv) && (i==0 || (kv `gt` pv))
						then i
					else if (kv `eq` cv)
						then i64.max 0 (i-this_step)
					else if (kv `gt` cv) then
						if (i == n-1)
						then -1
						else i64.min (n-1) (i+this_step)
					else -- kv `lt` cv
						if (i == 0 || (kv `gt` pv))
						then -1
						else i64.max 0 (i-this_step)
				)
			in (searchAt, this_step)
		in foundAt

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
		(init_is : [nvs]i64)
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs]i64 =
		let num_iter = 1 + (n |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
		let (foundAt,_) = loop (is,last_step) = (init_is,n)
		for _ in iota num_iter do
			let this_step = (last_step + 1)/2
			let searchAt = is
				|> map (\i ->
					let cur_elem = xs[i64.max 0 i]
					let next_elem = xs[i64.min (i+1) (n-1)]
					in (i, cur_elem, next_elem)
				)
				|> zip vs
				|> map (\(kv, (i, cv, nv)) ->
					if i<0 then (-1) else
					if (kv `geq` cv) && (i==(n-1) || (kv `lt` nv)) then
						i
					else if (kv `geq` cv) then
						i64.min (n-1) (i+this_step)
					-- from hereon kv `lt` cv
					else if i==0 then
						-1
					else
						i64.max 0 (i-this_step)
				)
			in (searchAt, this_step)
		in foundAt

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
		(init_is : [nvs]i64)
		(xs : [n]t)
		(vs : [nvs]t)
	: [nvs](i64,i64) =
		-- Find the first match
		let diyige = bsearch_first
			(eq) (gt) init_is xs vs
		-- Find the last match
		let zuihoude = bsearch_last
			(geq) (lt) diyige xs vs
		-- Calculate the counts
		let duome = map2
			(\fm lm -> if fm<0 then 0 else lm-fm+1)
			diyige
			zuihoude
		in zip diyige duome

-- Radix Sorting/Partitioning

	-- | Represents a contiguous sequence of bytes.
	type byteSeq [bytes] = [bytes]u8

	-- | Obtain bits i-j (inclusive) from y.
	-- Can't obtain more than 1 byte's worth of bits.
	def get_radix 't (i : i32) (j : i32) (get_bit : i32 -> t -> i32) (x : t)
	: u8 =
		loop y=0
		for bit in (0...(j-i))
		do u8.set_bit bit y (get_bit (i+bit) x)

	-- | Perform a radix-sort step, using multiple bits at a time.
	-- Based on Futhark by Example.
	def radix_sort_multistep [n] 't
		(i : i32)
		(j : i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
	: [n]t =
		let ij_bits = j-i+1
		let up_to = (1 << ij_bits) |> i64.i32
		let rs = xs
			|> map (get_radix i j get_bit)
			|> map (i64.u8)
		let counts = hist (+) 0 up_to rs (replicate n 1)
		let offs = exscan (+) 0 counts
		let idxs = loop js = (iota n) for num < up_to do
			let guanzhong = rs
				|> map (\r -> r==num)
			let zuowei = guanzhong
				|> map (i64.bool)
				|> exscan (+) 0
				|> map (\i -> i+offs[num])
			in js
				|> zip guanzhong
				|> map (\(g, i) -> if g then zuowei[i] else i)
		in scatter (copy xs) idxs xs

	-- | Radix-based bucket-sort for i64 data.
	-- Meant for a 'small' number of compactly numbered buckets.
	def bucket_sort [n] 't
		(bit_step: i32)
		(num_buckets : i64)
		(ks : [n]i64)
		(xs : [n]t)
	=
		let bucket_bits = num_buckets |> f64.i64 |> f64.log2 |> f64.ceil |> i32.f64
		let num_iter = (bucket_bits + bit_step - 1) / bit_step
		in loop (ks, xs) for iter < num_iter do
			let i = iter*bit_step
			let j = i32.min (bucket_bits-1) (i + bit_step - 1)
			in radix_sort_multistep i j (\bi (k,_) -> i64.get_bit bi k) (zip ks xs)
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
	type joinTup [n] 't = {vs : [n]t, ix: [n]i64, iy: [n]i64, cm: [n]i64}

	-- | The pairs obtained from joining x&y.
	-- vs : the values of each pair
	-- ix : the respective index in x
	-- iy : the respective index in y
	-- NOTE - unlike type joinTup, each tuple here corresponds to an individual match.
	type~ joinPairs 't = {vs: []t, ix: []i64, iy: []i64}