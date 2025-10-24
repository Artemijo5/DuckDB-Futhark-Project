import "ftbasics"
import "ftSMJ"
import "ftHashJoin"

-- TODO make bit_step (++) accessible from entry points

-- Misc

entry argmin_short [n] (ks: [n]i16) : idx_t.t =
  argmin (<) (==) (i16.highest) ks
entry argmin_int [n] (ks: [n]i32) : idx_t.t =
  argmin (<) (==) (i32.highest) ks
entry argmin_long [n] (ks: [n]i64) : idx_t.t =
  argmin (<) (==) (i64.highest) ks
entry argmin_float [n] (ks: [n]f32) : idx_t.t =
  argmin (<) (==) (f32.highest) ks
entry argmin_double [n] (ks: [n]f64) : idx_t.t =
  argmin (<) (==) (f64.highest) ks

entry min_idx = idx_t.minimum
entry max_idx = idx_t.maximum

-- Gather Operations

entry orderByIndices_short [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) : [n]i16 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
entry orderByIndices_int [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) : [n]i32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
entry orderByIndices_long [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) : [n]i64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
entry orderByIndices_float [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) : [n]f32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
entry orderByIndices_double [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) : [n]f64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys

entry gather_payloads_short (incr) (psize) (is) (ys: []i16)
  = gather_payloads incr psize (0) is ys
entry gather_payloads_int (incr) (psize) (is) (ys: []i32)
  = gather_payloads incr psize (0) is ys
entry gather_payloads_long (incr) (psize) (is) (ys: []i64)
  = gather_payloads incr psize (0) is ys
entry gather_payloads_float (incr) (psize) (is) (ys: []f32)
  = gather_payloads incr psize (0) is ys
entry gather_payloads_double (incr) (psize) (is) (ys: []f64)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_GFTR (incr) (psize) (is) (pL_bytes: idx_t.t) (ys: [][pL_bytes]u8)
  = gather_payloads incr psize (replicate pL_bytes (u8.i32 0)) is ys

entry gather_payloads_short_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i16) (is: [ni]idx_t.t) (ys: [n]i16)
  = gather_payloads_GFUR incr psize preVals is ys
entry gather_payloads_int_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i32) (is: [ni]idx_t.t) (ys: [n]i32)
  = gather_payloads_GFUR incr psize preVals is ys
entry gather_payloads_long_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i64) (is: [ni]idx_t.t) (ys: [n]i64)
  = gather_payloads_GFUR incr psize preVals is ys
entry gather_payloads_float_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f32) (is: [ni]idx_t.t) (ys: [n]f32)
  = gather_payloads_GFUR incr psize preVals is ys
entry gather_payloads_double_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f64) (is: [ni]idx_t.t) (ys: [n]f64)
  = gather_payloads_GFUR incr psize preVals is ys

-- Sorting

	-- ------------------------------------------------------
	-- GFTR
	-- ------------------------------------------------------

	entry radixSortRelation_short [n] [b]
	  (block_size: i16)
	  (xs: sortStruct_short [n] [b])
	 : sortStruct_short [n] [b]
	  = radixSortRelation_signed_integral (block_size) (xs) (i16.num_bits) (i16.get_bit)
	entry radixSortRelation_int [n] [b]
	  (block_size: i16)
	  (xs: sortStruct_int [n] [b])
	 : sortStruct_int [n] [b]
	  = radixSortRelation_signed_integral (block_size) (xs) (i32.num_bits) (i32.get_bit)
	entry radixSortRelation_long [n] [b]
	  (block_size: i16)
	  (xs: sortStruct_long [n] [b])
	 : sortStruct_long [n] [b]
	  = radixSortRelation_signed_integral (block_size) (xs) (i64.num_bits) (i64.get_bit)
	entry radixSortRelation_float [n] [b]
	  (block_size: i16)
	  (xs: sortStruct_float [n] [b])
	 : sortStruct_float [n] [b]
	  = radixSortRelation_signed_float (block_size) (xs) (f32.num_bits) (f32.get_bit)
	entry radixSortRelation_double [n] [b]
	  (block_size: i16)
	  (xs: sortStruct_double [n] [b])
	 : sortStruct_double [n] [b]
	  = radixSortRelation_signed_float (block_size) (xs) (f64.num_bits) (f64.get_bit)

	entry mergeSortRelation_short [n] [b]
	  (xs: sortStruct_short [n] [b])
	: sortStruct_short [n] [b]
	  = mergeSortRelation (xs) (<=)
	entry mergeSortRelation_int [n] [b]
	  (xs: sortStruct_int [n] [b])
	: sortStruct_int [n] [b]
	  = mergeSortRelation (xs) (<=)
	entry mergeSortRelation_long [n] [b]
	  (xs: sortStruct_long [n] [b])
	: sortStruct_long [n] [b]
	  = mergeSortRelation (xs) (<=)
	entry mergeSortRelation_float [n] [b]
	  (xs: sortStruct_float [n] [b])
	: sortStruct_float [n] [b]
	  = mergeSortRelation (xs) (<=)
	entry mergeSortRelation_double [n] [b]
	  (xs: sortStruct_double [n] [b])
	: sortStruct_double [n] [b]
	  = mergeSortRelation (xs) (<=)

	-- ------------------------------------------------------
	-- GFUR
	-- ------------------------------------------------------

	entry radixSortColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) : sortInfo_short [n] =
	  radixSortColumn_signed_integral incr block_size xs (i16.num_bits) (i16.get_bit)
	entry radixSortColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) : sortInfo_int [n] =
	  radixSortColumn_signed_integral incr block_size xs (i32.num_bits) (i32.get_bit)
	entry radixSortColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) : sortInfo_long [n] =
	  radixSortColumn_signed_integral incr block_size xs (i64.num_bits) (i64.get_bit)
	entry radixSortColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) : sortInfo_float [n] =
	  radixSortColumn_signed_float incr block_size xs (f32.num_bits) (f32.get_bit)
	entry radixSortColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) : sortInfo_double [n] =
	  radixSortColumn_signed_float incr block_size xs (f64.num_bits) (f64.get_bit)

	entry mergeSortColumn_short [n] (incr: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
	  mergeSortColumn incr xs (<=)
	entry mergeSortColumn_int [n] (incr: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
	  mergeSortColumn incr xs (<=)
	entry mergeSortColumn_long [n] (incr: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
	  mergeSortColumn incr xs (<=)
	entry mergeSortColumn_float [n] (incr: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
	  mergeSortColumn incr xs (<=)
	entry mergeSortColumn_double [n] (incr: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
	  mergeSortColumn incr xs (<=)

-- SMJ

entry inner_SMJ_short [nR] [nS]
  (tR: [nR]i16)
  (tS: [nS]i16)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_short =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>)

entry inner_SMJ_int [nR] [nS]
  (tR: [nR]i32)
  (tS: [nS]i32)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_int =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>)

entry inner_SMJ_long [nR] [nS]
  (tR: [nR]i64)
  (tS: [nS]i64)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_long =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>)

entry inner_SMJ_float [nR] [nS]
  (tR: [nR]f32)
  (tS: [nS]f32)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_float =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>)

entry inner_SMJ_double [nR] [nS]
  (tR: [nR]f64)
  (tS: [nS]f64)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_double =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>)


-- Radix-Partitioning

entry partition_and_deepen_GFTR [n] [b] [pL_b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (pL: [n](byteSeq [pL_b]))
  (size_thresh: idx_t.t)
  (max_depth: i32)
: partitionedSet_GFTR [n] [b] [pL_b] =
  let kps =
    partition_and_deepen block_size gather_psize radix_size xs pL size_thresh max_depth
  in {ks = kps.0, pL = kps.1}

entry partition_and_deepen_GFUR [n] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: partitionedSet_GFUR [n] [b] =
  let is = (offset..<(offset+n)) :> [n]idx_t.t
  let kis =
    partition_and_deepen block_size gather_psize radix_size xs is size_thresh max_depth
  in {ks = kis.0, idx = kis.1}

entry calc_partitions_from_partitioned_set [n] [b]
  (radix_size: i32)
  (pXs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: partitionInfo =
  let recursive_info : (partitionInfo, bool)
  = loop p = (getPartitionBounds 1 pXs 0 (radix_size-1), true)
  while (p.0.maxDepth < max_depth && p.1) do
    let bs = p.0.bounds
    let taidade = indices bs
      |> map (\i -> (bs[i], if (i<(length bs)-1) then bs[i+1] else (n), i))
      |> filter (\(lb, ub, _) -> ub-lb > size_thresh)
    let (inner_info : partitionInfo, _: idx_t.t, _: idx_t.t)
    = loop (q, ad, ox) = (p.0, 0, 0)
    for bounds in taidade do
      let m = bounds.1-bounds.0
      let x_bufen = pXs[bounds.0:bounds.1] :> [m](byteSeq [b])
      let new_i = radix_size*(p.0.maxDepth)
      let new_j = radix_size*(p.0.maxDepth+1) - 1
      let deeper_info = getPartitionBounds (p.0.maxDepth+1) x_bufen new_i new_j
      let insert_len = length deeper_info.bounds
      let stitch = bounds.2 + ox - ad
      let new_q = {
        maxDepth = p.0.maxDepth + 1,
        bounds = q.bounds[0:stitch] ++ (deeper_info.bounds |> map (\b -> b+bounds.0)) ++ q.bounds[stitch+1:(length q.bounds)],
        depths = q.depths[0:stitch] ++ deeper_info.depths ++ q.depths[stitch+1:(length q.depths)]
      }
      in (new_q, ad+1, ox + insert_len)
    in (inner_info, (length taidade) > 0)
  let info_len = length recursive_info.0.bounds
  let recursive_info_with_offset = {
    maxDepth = recursive_info.0.maxDepth,
    bounds = recursive_info.0.bounds |> map (\b -> b + offset) :> [info_len]idx_t.t,
    depths = recursive_info.0.depths :> [info_len]i32
  }
  in recursive_info_with_offset

entry create_hash_table_from_partitioned_set [n] [b]
  (radix_size : i32)
  (pXs : [n](byteSeq [b]))
  (x_info : partitionInfo)
  (scatter_psize : idx_t.t)
: radix_hashTable [i64.i32 radix_size] =
  let m = length x_info.bounds
  let is_base = iota m
  let rs = i64.i32 radix_size
  let scatter_is_withMultiplicity = is_base
    |> map (\i ->  radix_to_idx (i32.i64 rs) pXs[x_info.bounds[i]])
  let is_first_last =
    if x_info.maxDepth == 1
    then replicate m (true, true)
    else is_base
      |> map (\i ->
        let cur_i = scatter_is_withMultiplicity[i]
        let pre_i = if i==0 then -1 else scatter_is_withMultiplicity[i-1]
        let is_first = (cur_i != pre_i)
        let pos_i = if i==(m-1) then -1 else scatter_is_withMultiplicity[i+1]
        let is_last = (cur_i != pos_i)
        in (is_first, is_last)
      )
  let scatter_is_first = map2 (\(is_first, _) i -> if is_first then scatter_is_withMultiplicity[i] else -1) is_first_last is_base
  let scatter_is_last = map2 (\(_, is_last) i -> if is_last then scatter_is_withMultiplicity[i] else -1) is_first_last is_base
  let first_partitionIndices = partitioned_scatter scatter_psize (replicate (2**rs) (-1)) scatter_is_first is_base
  let last_partitionIndices = partitioned_scatter scatter_psize (replicate (2**rs) (-1)) scatter_is_last is_base
  in {first_info_idx = first_partitionIndices, last_info_idx = last_partitionIndices}

-- Radix-Hash Partitioned Join

entry Inner_Radix_Hash_Join [nR] [nS] [b]
  (radix_size : i32)
  (pR : [nR](byteSeq [b]))
  (pS : [nS](byteSeq [b]))
  (r_info : partitionInfo)
  (s_info : partitionInfo)
  (s_hashTable : radix_hashTable [i64.i32 radix_size])
  (scatter_psize: idx_t.t)
: joinPairs_bsq [b] =
  let rp_matches = partitionMatchBounds
    radix_size
    pR
    pS
    r_info.bounds
    s_info.bounds
    r_info.depths
    s_info.depths
    s_hashTable
  in join_hashPartitions pR pS r_info s_info rp_matches scatter_psize