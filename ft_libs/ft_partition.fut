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

-- NOTE: only supports cases where at most 64 bits are used for partitioning

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
		let j = i32.max 0 (i32.min j_ (bi*u8.num_bits - 1))
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

	-- Convert an i64 value into a byteSeq
	def idx_to_radix (b : i64) (idx : i64)
	: byteSeq [b] =
		let u8_bitmask = i64.u8 u8.highest
		in iota b
			|> map (\i ->
				(i64.>>>) idx ((b - i - 1)*(i64.i32 u8.num_bits))
			)
			|> map (& u8_bitmask)
			|> map (u8.i64)

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

	-- Radix comparator
	-- comparison function must be eq, geq, leq, gt, or lt
	def radix_cmp [b]
		(cmp : u8 -> u8 -> bool)
		(radix_bits : i32)
		(p1 : (i32, byteSeq [b]))
		(p2 : (i32, byteSeq [b]))
	: bool =
		let (depth1, x1) = p1
		let (depth2, x2) = p2
		let depth = i32.min depth1 depth2
		let (final_res,_,_) =
		loop (res,terminated,j)=(false,false,0)
		while !terminated && j<depth do
			let bitmask = mk_radix_bitmask (j*radix_bits) ((j+1)*radix_bits-1) b
			let r1 = x1 |> getRadix bitmask
			let r2 = x2 |> getRadix bitmask
			let terminated' = map2 (!=) r1 r2 |> foldl (||) false
			let (this_cmp,_,_)
			= loop (this_res,decided,k) = (false,false,0)
			while !decided && k<b do
				if k<(b-1) && r1[k]==r2[k] then (this_res,false,k+1)
				else (cmp r1[k] r2[k], true, k+1)
			in (res || this_cmp, terminated', j+1)
		in final_res

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

	-- | Obtain the first index of each partition.
	def getPartitionBounds [n] [b]
		(i: i32)
		(j: i32)
		(pXs: [n](byteSeq [b]))
	: [n]bool =
		if j<0 then pXs |> group_boundaries (\_ _ -> false) else
		let bitmask = mk_radix_bitmask i j b
		let (fb,lb) = radix_first_last_bytes i j b
		in pXs
			|> map (getRadix bitmask)
			|> group_boundaries (byteSeq_neq fb lb bitmask)

	-- | Multi-level radix-partitioning with payload data.
	def partition_and_deepen 't [n] [b]
		(bit_step: i32)
		(radix_size: i32)
		(size_thresh: i64)
		(max_depth_: i32)
		(xs: [n](byteSeq [b]))
		(pLs: [n]t)
	: ([n](byteSeq [b]), [n]t) =
		let max_J = (i32.i64 b)*u8.num_bits - 1
		let max_depth = i32.min max_depth_ (((i32.i64 b)*u8.num_bits + radix_size - 1)/radix_size)
		let loop_over : ([n](byteSeq [b]), [n]t, [](i64, i64),  i32)
	    = loop (pXs, pPs, taidade, dp) = (xs, pLs, [(0,n)], 0)
	    while (length taidade)>0 && dp<max_depth do
	    	-- 'taidade' = 'too big' -> indicate partitions exceeding size thresh
	    	let nt = length taidade
		    let new_i = radix_size*dp
		    let new_j = i32.min max_J (new_i + radix_size - 1)
		    let (gather_groups, gather_is) = if dp==0
		    	then (replicate n 0, iota n)
		    	else taidade
			    	|> expand (.1) (\(inf, _) ind -> (inf, inf+ind))
			    	|> unzip
		    let gather_groups' = gather_groups |> group_boundaries (!=)
		    	|> dict_encoding
		    let (gather_xs, gather_pLs, gather_groups_) = if nt==1
		    	-- only sort gather_groups' if E more than one group
		    	then let (g_xs, g_ps) = gather_is |> map (\i -> (pXs[i], pPs[i]))
			    	|> unzip
			    	|> (\(gxs, gps) -> radix_part new_i new_j bit_step gxs gps)
			    	in (g_xs, g_ps, gather_groups')
			    else gather_is |> zip gather_groups' |> map (\(g,i) -> (pXs[i], pPs[i], g))
			    	|> unzip3
			    	|> (\(gxs, gps, ggs) -> radix_part new_i new_j bit_step gxs (zip gps ggs))
			    	|> (\(gxs, gpgs) -> (gxs, gpgs |> map (.0), gpgs |> map (.1)))
		    let (gather_xs', gather_pLs') = zip gather_xs gather_pLs
		    	|> bucket_sort bit_step nt gather_groups_
		    	|> (\(_, xps) -> unzip xps)
		    let old_part_bounds  = copy (getPartitionBounds 0 (new_i-1) pXs)
		    let this_part_bounds = getPartitionBounds 0 new_j gather_xs'
		    let new_part_bounds  = scatter old_part_bounds gather_is this_part_bounds
		    let new_pXs = scatter (copy pXs) gather_is gather_xs'
		    let new_pPs = scatter (copy pPs) gather_is gather_pLs'
		    let new_part_sizes = segmented_reduce (+) 0 new_part_bounds (replicate n 1i64)
		    let new_bound_is = indices new_part_bounds |> filter (\i -> new_part_bounds[i])
		    let np = length new_part_sizes
		    let new_taidade = zip (new_bound_is |> sized np) (new_part_sizes |> sized np)
		    	|> filter (\(_,size) -> size > size_thresh)
		    in (new_pXs, new_pPs, new_taidade, dp+1)
		in (loop_over.0, loop_over.1)

	-- | Multi-level radix-partitioning with payload data, using a pre-made partitioning scheme.
	-- This can be used eg on the left-side relation of PHJ, to ensure memory coalescion.
	-- 
	-- Implemented similar to partition_and_deepen, only taidade is got from use_info rather than calculated.
	def partition_preconfigured 't [n] [b]
		(bit_step: i32)
		(radix_size: i32)
		(use_prev  : [](byteSeq [b]))
		(use_info  : partitionInfo)
		(xs: [n](byteSeq [b]))
		(pLs: [n]t)
	: ([n](byteSeq [b]), [n]t) =
		let max_J = (i32.i64 b)*u8.num_bits - 1
		let max_depth = use_info.maxDepth
		let loop_over : ([n](byteSeq [b]), [n]t, [](i64, i64),  i32)
	    = loop (pXs, pPs, taidade, dp) = (xs, pLs, [(0,n)], 0)
	    while (length taidade)>0 && dp<max_depth do
	    	-- 'taidade' = 'too big' -> indicate partitions to be subdivided, based on use_info
	    	let nt = length taidade
		    let new_i = radix_size*dp
		    let new_j = i32.min max_J (new_i + radix_size - 1)
		    let (gather_groups, gather_is) = if dp==0
		    	then (replicate n 0, iota n)
		    	else taidade
			    	|> expand (.1) (\(inf, _) ind -> (inf, inf+ind))
			    	|> unzip
		    let gather_groups' = gather_groups |> group_boundaries (!=)
		    	|> dict_encoding
		    let (gather_xs, gather_pLs, gather_groups_) = if nt==1
		    	-- only sort gather_groups' if E more than one group
		    	then let (g_xs, g_ps) = gather_is |> map (\i -> (pXs[i], pPs[i]))
			    	|> unzip
			    	|> (\(gxs, gps) -> radix_part new_i new_j bit_step gxs gps)
			    	in (g_xs, g_ps, gather_groups')
			    else gather_is |> zip gather_groups' |> map (\(g,i) -> (pXs[i], pPs[i], g))
			    	|> unzip3
			    	|> (\(gxs, gps, ggs) -> radix_part new_i new_j bit_step gxs (zip gps ggs))
			    	|> (\(gxs, gpgs) -> (gxs, gpgs |> map (.0), gpgs |> map (.1)))
		    let (gather_xs', gather_pLs') = zip gather_xs gather_pLs
		    	|> bucket_sort bit_step nt gather_groups_
		    	|> (\(_, xps) -> unzip xps)
		    let old_part_bounds  = copy (getPartitionBounds 0 (new_i-1) pXs)
		    let this_part_bounds = getPartitionBounds 0 new_j gather_xs'
		    let new_part_bounds  = scatter old_part_bounds gather_is this_part_bounds
		    let new_pXs = scatter (copy pXs) gather_is gather_xs'
		    let new_pPs = scatter (copy pPs) gather_is gather_pLs'
		    let new_part_sizes = segmented_reduce (+) 0 new_part_bounds (replicate n 1i64)
		    let new_bound_is = indices new_part_bounds |> filter (\i -> new_part_bounds[i])
		    let np = length new_bound_is
		    -- new taidade are obtained by comparison with prev's partitions
		    -- using binary search
		    let relevant_part_is = indices use_info.depths
				|> filter (\i -> use_info.depths[i] > (dp+1))
			let cur_bitmask = mk_radix_bitmask 0 new_j b
			let (fb,lb) = radix_first_last_bytes 0 new_j b
			let new_taidade = new_bound_is |> sized np
				|> map (\i -> new_pXs[i])
				|> bsearch_first
					(byteSeq_eq fb lb cur_bitmask)
					(\r1 r2 -> radix_cmp (>) radix_size (dp+1,r1) (dp+1,r2))
					(replicate np 0)
					(relevant_part_is |> map (\i -> use_prev[use_info.bounds[i]]))
				|> zip (indices new_bound_is |> sized np)
				|> filter (\(_,match_i) -> match_i >=0 )
				|> map (\(i,_) -> (new_bound_is[i], new_part_sizes[i]))
			-- -- alternative with nested parallelism (might explode if too many partitions)
			--let new_taidade = zip (new_bound_is |> sized np) (new_part_sizes |> sized np) 
			--	|> filter (\(i,_) -> any
			--		(byteSeq_eq fb lb cur_bitmask pXs[i])
			--		(relevant_part_is |> map (\i2 -> use_prev[use_info.bounds[i2]]))
			--	)
		    in (new_pXs, new_pPs, new_taidade, dp+1)
		in (loop_over.0, loop_over.1)

-- Struct Construction

	-- | Construct partitionInfo for a partitioned set.
	def calc_partInfo [n] [b]
		(radix_size: i32)
		(offset: i64)
		(size_thresh: i64)
		(max_depth_: i32)
		(pXs: [n](byteSeq [b]))
	: partitionInfo =
		let bi = i32.i64 b
		let max_depth = i32.min
			max_depth_
			((bi*u8.num_bits + radix_size - 1)/radix_size)
		let (part_bounds, part_depths, _, part_depth) =
		loop (cur_bounds, cur_depths, part_sizes, cur_depth)
		= (pXs |> group_boundaries (\_ _ -> false), replicate n 0i32, [n], 0)
		while (cur_depth < max_depth && (i64.maximum part_sizes) > size_thresh) do
			let parts_to_subdivide = indices part_sizes
				|> filter (\i -> part_sizes[i]>size_thresh)
			let bound_is = part_sizes |> exscan (+) 0
			let gather_is = parts_to_subdivide |> map (\i ->
				(bound_is[i], if i==(length bound_is - 1) then n else bound_is[i+1])
			) |> expand (\(inf,sup) -> sup-inf) (\(inf,_) ind -> inf + ind)
			let gather_xs = gather_is |> map (\i -> pXs[i])
			let this_bounds = gather_xs
				|> getPartitionBounds 0 ((cur_depth+1)*radix_size - 1)
			let new_bounds = scatter (copy cur_bounds) gather_is this_bounds
			let new_depths = scatter cur_depths gather_is (gather_is |> map (\_ -> (cur_depth+1)))
			let new_sizes = segmented_reduce (+) 0 new_bounds (replicate n 1i64)
			in (new_bounds, new_depths, new_sizes, cur_depth+1)
		let (final_bounds, final_depths) = indices part_bounds
			|> filter (\i -> part_bounds[i])
			|> map (\i -> (i, part_depths[i]))
			|> unzip
		in {
			maxDepth = part_depth,
			bounds = final_bounds |> map (\i -> i+offset),
			depths = final_depths
		}

	-- | Construct partitionInfo for a partitioned set, using a pre-made partitioning scheme.
	def calc_partInfo_preconfigured [n] [b]
		(radix_size: i32)
		(offset: i64)
		(use_prev : [](byteSeq [b]))
		(use_info : partitionInfo)
		(pXs: [n](byteSeq [b]))
	: partitionInfo =
		let max_J = (i32.i64 b)*u8.num_bits - 1
		let max_depth = use_info.maxDepth
		let (part_bounds, part_depths, part_depth) =
		loop (cur_bounds, cur_depths, cur_depth)
		= (pXs |> group_boundaries (\_ _ -> false), replicate n 0i32, 0)
		while (cur_depth < max_depth) do
			let relevant_part_is = indices use_info.depths
				|> filter (\i -> use_info.depths[i] > cur_depth)
			let cur_bitmask = mk_radix_bitmask 0 (cur_depth*radix_size-1) b
			let (fb,lb) = radix_first_last_bytes 0 (i32.min max_J ((cur_depth)*radix_size-1)) b
			let bound_is = indices cur_bounds |> filter (\i -> cur_bounds[i])
			-- -- parts_to_subdivide : partitions in pXs having a match in prev with depth>cur_depth
			-- -- find via binary search
			let parts_to_subdivide = bound_is |> map (\i -> pXs[i])
				|> bsearch_first
					(byteSeq_eq fb lb cur_bitmask)
					(\r1 r2 -> radix_cmp (>) radix_size (cur_depth,r1) (cur_depth,r2))
					(bound_is |> map (\_ -> 0))
					(relevant_part_is |> map (\i -> use_prev[use_info.bounds[i]]))
				|> zip (indices bound_is)
				|> filter (\(_,match_i) -> match_i >= 0)
				|> map (.0)
			-- -- alternative with nested parallelism (might explode if too many partitions)
			--let parts_to_subdivide = indices bound_is |> filter (\i -> any
			--	(byteSeq_eq fb lb cur_bitmask pXs[bound_is[i]])
			--	(relevant_part_is |> map (\i2 -> use_prev[use_info.bounds[i2]]))
			--)
			let gather_is = parts_to_subdivide |> map (\i ->
				(bound_is[i], if i==(length bound_is - 1) then n else bound_is[i+1])
			) |> expand (\(inf,sup) -> sup-inf) (\(inf,_) ind -> inf + ind)
			let gather_xs = gather_is |> map (\i -> pXs[i])
			let this_bounds = gather_xs
				|> getPartitionBounds 0 ((cur_depth+1)*radix_size - 1)
			let new_bounds = scatter (copy cur_bounds) gather_is this_bounds
			let new_depths = scatter cur_depths gather_is (gather_is |> map (\_ -> (cur_depth+1)))
			in (new_bounds, new_depths, cur_depth+1)
		let (final_bounds, final_depths) = indices part_bounds
			|> filter (\i -> part_bounds[i])
			|> map (\i -> (i, part_depths[i]))
			|> unzip
		in {
			maxDepth = part_depth,
			bounds = final_bounds |> map (\i -> i+offset),
			depths = final_depths
		}

	-- | Construct radix_hashTable for a partitioned set.
	def calc_radixHashTab [n] [b]
		(radix_size : i32)
		(x_info : partitionInfo)
		(pXs : [n](byteSeq [b]))
	: radix_hashTable [i64.i32 radix_size] =
		let rs = i64.i32 radix_size
		let bitmask = mk_radix_bitmask 0 (radix_size-1) b
		let brs = x_info.bounds
			|> map (\i -> pXs[i])
			|> map (radix_to_idx radix_size bitmask)
		let is_first = brs
			|> group_boundaries (!=)
		let is_last = is_first
			|> rotate 1
		let scatter_isF = is_first
			|> zip (brs)
			|> map (\(br,isf) -> if isf then br else (-1))
		let scatter_isL = is_last
			|> zip (brs)
			|> map (\(br,isl) -> if isl then br else (-1))
		in {
			first_info_idx = scatter (replicate (2**rs) (-1)) scatter_isF (indices x_info.bounds),
			last_info_idx  = scatter (replicate (2**rs) (-1)) scatter_isL (indices x_info.bounds)
		}

-- some test vals
-- let xs1:[][]u8=[[4,1],[0,3],[1,4],[1,4],[1,4],[5,4],[2,3],[4,3],[1,2],[2,2],[4,2],[3,2],[0,1]]
-- let xs2:[][]u8=[[4,1],[5,1],[0,1],[0,1],[5,4],[5,4],[1,4],[2,4],[2,2],[2,2],[2,2],[2,2],[5,3],[7,3],[1,3],[3,3]]
