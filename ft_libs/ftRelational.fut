import "ftbasics"
import "ft_SortGroupBy"
import "ft_StrUtil"
import "joins/ftSMJ"
import "joins/ftHashJoin_old"
import "joins/ftSMJ_str"
import "joins/ftHashJoin_str"

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

  entry minimum_short = i16.minimum
  entry maximum_short = i16.maximum

  entry minimum_int = i32.minimum
  entry maximum_int = i32.maximum

  entry minimum_long = i64.minimum
  entry maximum_long = i64.maximum

  entry minimum_float = f32.minimum
  entry maximum_float = f32.maximum

  entry minimum_double = f64.minimum
  entry maximum_double = f64.maximum

  entry min_idx = idx_t.minimum
  entry max_idx = idx_t.maximum

-- Gather Operations

  entry orderByIndices_short [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) : [n]i16 =
    let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
    in orderByIndices i16.num_bits block_size (0) offset_is ys
  entry orderByIndices_int [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) : [n]i32 =
    let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
    in orderByIndices i32.num_bits block_size (0) offset_is ys
  entry orderByIndices_long [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) : [n]i64 =
    let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
    in orderByIndices i64.num_bits block_size (0) offset_is ys
  entry orderByIndices_float [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) : [n]f32 =
    let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
    in orderByIndices f32.num_bits block_size (0) offset_is ys
  entry orderByIndices_double [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) : [n]f64 =
    let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
    in orderByIndices f64.num_bits block_size (0) offset_is ys

  entry gather_payloads_short (incr) (psize) (is) (ys: []i16)
    = gather_payloads i16.num_bits incr psize (0) is ys
  entry gather_payloads_int (incr) (psize) (is) (ys: []i32)
    = gather_payloads i32.num_bits incr psize (0) is ys
  entry gather_payloads_long (incr) (psize) (is) (ys: []i64)
    = gather_payloads i64.num_bits incr psize (0) is ys
  entry gather_payloads_float (incr) (psize) (is) (ys: []f32)
    = gather_payloads f32.num_bits incr psize (0) is ys
  entry gather_payloads_double (incr) (psize) (is) (ys: []f64)
    = gather_payloads f64.num_bits incr psize (0) is ys

  entry gather_payloads_GFTR (incr) (psize) (is) (pL_bytes: idx_t.t) (ys: [][pL_bytes]u8)
    = gather_payloads ((i32.i64 pL_bytes)*u8.num_bits) incr psize (replicate pL_bytes (u8.i32 0)) is ys

  entry gather_payloads_short_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i16) (is: [ni]idx_t.t) (ys: [n]i16)
    = gather_payloads_GFUR i16.num_bits incr psize preVals is ys
  entry gather_payloads_int_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i32) (is: [ni]idx_t.t) (ys: [n]i32)
    = gather_payloads_GFUR i32.num_bits incr psize preVals is ys
  entry gather_payloads_long_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i64) (is: [ni]idx_t.t) (ys: [n]i64)
    = gather_payloads_GFUR i64.num_bits incr psize preVals is ys
  entry gather_payloads_float_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f32) (is: [ni]idx_t.t) (ys: [n]f32)
    = gather_payloads_GFUR f32.num_bits incr psize preVals is ys
  entry gather_payloads_double_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f64) (is: [ni]idx_t.t) (ys: [n]f64)
    = gather_payloads_GFUR f64.num_bits incr psize preVals is ys

-- Sorting

  -- ------------------------------------------------------
  -- GFTR
  -- ------------------------------------------------------

    entry radixSortRelation_short [n] [b]
      (block_size: idx_t.t)
      (xs: sortStruct_short [n] [b])
     : sortStruct_short [n] [b]
      = radixSortRelation_signed_integral (block_size) (xs) (i16.num_bits) (i16.get_bit)
    entry radixSortRelation_int [n] [b]
      (block_size: idx_t.t)
      (xs: sortStruct_int [n] [b])
     : sortStruct_int [n] [b]
      = radixSortRelation_signed_integral (block_size) (xs) (i32.num_bits) (i32.get_bit)
    entry radixSortRelation_long [n] [b]
      (block_size: idx_t.t)
      (xs: sortStruct_long [n] [b])
     : sortStruct_long [n] [b]
      = radixSortRelation_signed_integral (block_size) (xs) (i64.num_bits) (i64.get_bit)
    entry radixSortRelation_float [n] [b]
      (block_size: idx_t.t)
      (xs: sortStruct_float [n] [b])
     : sortStruct_float [n] [b]
      = radixSortRelation_signed_float (block_size) (xs) (f32.num_bits) (f32.get_bit)
    entry radixSortRelation_double [n] [b]
      (block_size: idx_t.t)
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

    entry radixSortColumn_short [n] (incr: idx_t.t) (block_size: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
      radixSortColumn_signed_integral incr block_size xs (i16.num_bits) (i16.get_bit)
    entry radixSortColumn_int [n] (incr: idx_t.t) (block_size: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
      radixSortColumn_signed_integral incr block_size xs (i32.num_bits) (i32.get_bit)
    entry radixSortColumn_long [n] (incr: idx_t.t) (block_size: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
      radixSortColumn_signed_integral incr block_size xs (i64.num_bits) (i64.get_bit)
    entry radixSortColumn_float [n] (incr: idx_t.t) (block_size: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
      radixSortColumn_signed_float incr block_size xs (f32.num_bits) (f32.get_bit)
    entry radixSortColumn_double [n] (incr: idx_t.t) (block_size: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
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
      (i16.num_bits) (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>) (<)

  entry inner_SMJ_int [nR] [nS]
    (tR: [nR]i32)
    (tS: [nS]i32)
    (offset_R: idx_t.t)
    (offset_S: idx_t.t)
    (partitionSize: idx_t.t)
    (scatter_psize: idx_t.t)
  : joinPairs_int =
    inner_SMJ
      (i32.num_bits) (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>) (<)

  entry inner_SMJ_long [nR] [nS]
    (tR: [nR]i64)
    (tS: [nS]i64)
    (offset_R: idx_t.t)
    (offset_S: idx_t.t)
    (partitionSize: idx_t.t)
    (scatter_psize: idx_t.t)
  : joinPairs_long =
    inner_SMJ
      (i64.num_bits) (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>) (<)

  entry inner_SMJ_float [nR] [nS]
    (tR: [nR]f32)
    (tS: [nS]f32)
    (offset_R: idx_t.t)
    (offset_S: idx_t.t)
    (partitionSize: idx_t.t)
    (scatter_psize: idx_t.t)
  : joinPairs_float =
    inner_SMJ
      (f32.num_bits) (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>) (<)

  entry inner_SMJ_double [nR] [nS]
    (tR: [nR]f64)
    (tS: [nS]f64)
    (offset_R: idx_t.t)
    (offset_S: idx_t.t)
    (partitionSize: idx_t.t)
    (scatter_psize: idx_t.t)
  : joinPairs_double =
    inner_SMJ
      (f64.num_bits) (0) (tR) (tS) (offset_R) (offset_S) (partitionSize) (scatter_psize) (==) (>) (<)


-- Radix-Partitioning

  entry partition_and_deepen_GFTR [n] [b] [pL_b]
    (block_size: i16)
    (gather_psize: idx_t.t)
    (radix_size: i32)
    (xs: [n](byteSeq [b]))
    (pL: [n](byteSeq [pL_b]))
    (size_thresh: idx_t.t)
    (max_depth: i32)
    (bit_step: i32)
  : partitionedSet_GFTR [n] [b] [pL_b] =
    let kps =
      partition_and_deepen block_size gather_psize radix_size xs pL size_thresh max_depth bit_step
    in {ks = kps.0, pL = kps.1}

  entry partition_and_deepen_GFUR [n] [b]
    (block_size: i16)
    (gather_psize: idx_t.t)
    (radix_size: i32)
    (xs: [n](byteSeq [b]))
    (offset: idx_t.t)
    (size_thresh: idx_t.t)
    (max_depth: i32)
    (bit_step: i32)
  : partitionedSet_GFUR [n] [b] =
    let is = (offset..<(offset+n)) :> [n]idx_t.t
    let kis =
      partition_and_deepen block_size gather_psize radix_size xs is size_thresh max_depth bit_step
    in {ks = kis.0, idx = kis.1}

  entry calc_partitions_from_partitioned_set [n] [b]
    (radix_size: i32)
    (pXs: [n](byteSeq [b]))
    (offset: idx_t.t)
    (size_thresh: idx_t.t)
    (max_depth: i32)
  : partitionInfo =
    calc_partInfo radix_size pXs offset size_thresh max_depth

  entry create_hash_table_from_partitioned_set [n] [b]
    (radix_size : i32)
    (pXs : [n](byteSeq [b]))
    (x_info : partitionInfo)
    (scatter_psize : idx_t.t)
  : radix_hashTable [i64.i32 radix_size] =
    calc_radixHashTab radix_size pXs x_info scatter_psize

-- Radix-Hash Partitioned Join

  entry Inner_Radix_Hash_Join [nR] [nS] [b]
    (radix_size : i32)
    (pR : [nR](byteSeq [b]))
    (pS : [nS](byteSeq [b]))
    (s_info : partitionInfo)
    (s_hashTable : radix_hashTable [i64.i32 radix_size])
   : joinPairs_bsq [b] =
    radix_hash_join radix_size pR pS s_info s_hashTable

  entry Inner_Radix_Hash_Join_with_right_keys_unique [nR] [nS] [b]
    (radix_size : i32)
    (pR : [nR](byteSeq [b]))
    (pS : [nS](byteSeq [b]))
    (s_info : partitionInfo)
    (s_hashTable : radix_hashTable [i64.i32 radix_size])
  : joinPairs_bsq [b] =
    radix_hash_join_with_S_keys_unique radix_size pR pS s_info s_hashTable

-- Group-By
    entry find_known_key_counts_long (key_no) (ks)
    = find_known_key_counts key_no ks

    type~ sortgroupInfo = {group_idx: []idx_t.t, group_count: []idx_t.t}

    entry sortgroup_find_known_key_counts_short (k_ids : []i16) (ks : []i16)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_known_key_counts k_ids ks (==) (>))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_known_key_counts_int (k_ids : []i32) (ks : []i32)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_known_key_counts k_ids ks (==) (>))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_known_key_counts_long (k_ids : []i64) (ks : []i64)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_known_key_counts k_ids ks (==) (>))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_known_key_counts_float (k_ids : []f32) (ks : []f32)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_known_key_counts k_ids ks (==) (>))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_known_key_counts_double (k_ids : []f64) (ks : []f64)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_known_key_counts k_ids ks (==) (>))
      in {group_idx = gidx, group_count = gsize}

    entry sortgroup_find_unknown_key_counts_short (ks : []i16)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_unknown_key_counts ks (==))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_unknown_key_counts_int (ks : []i32)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_unknown_key_counts ks (==))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_unknown_key_counts_long (ks : []i64)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_unknown_key_counts ks (==))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_unknown_key_counts_float (ks : []f32)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_unknown_key_counts ks (==))
      in {group_idx = gidx, group_count = gsize}
    entry sortgroup_find_unknown_key_counts_double (ks : []f64)
    : sortgroupInfo =
      let (gidx,gsize) = (sortgroup_find_unknown_key_counts ks (==))
      in {group_idx = gidx, group_count = gsize}

  -- TODO other aggregates...

    entry short_sum = GroupedAggregator_short.sum
    entry short_sum_xy = GroupedAggregator_short.sum_xy

    entry int_sum = GroupedAggregator_int.sum
    entry int_sum_xy = GroupedAggregator_int.sum_xy

    entry long_sum = GroupedAggregator_long.sum
    entry long_sum_xy = GroupedAggregator_long.sum_xy

    entry float_sum = GroupedAggregator_float.sum
    entry float_sum_xy = GroupedAggregator_float.sum_xy

    entry double_sum = GroupedAggregator_double.sum
    entry double_sum_xy = GroupedAggregator_double.sum_xy

    entry alt_short_sum = SortGroupedAggregator_short.sum
    entry alt_short_sum_xy = SortGroupedAggregator_short.sum_xy

    entry alt_int_sum = SortGroupedAggregator_int.sum
    entry alt_int_sum_xy = SortGroupedAggregator_int.sum_xy

    entry alt_long_sum = SortGroupedAggregator_long.sum
    entry alt_long_sum_xy = SortGroupedAggregator_long.sum_xy

    entry alt_float_sum = SortGroupedAggregator_float.sum
    entry alt_float_sum_xy = SortGroupedAggregator_float.sum_xy

    entry alt_double_sum = SortGroupedAggregator_double.sum
    entry alt_double_sum_xy = SortGroupedAggregator_double.sum_xy


-- String Functions

  -- Gather
    entry gather_str [ni]
      (psize : i64)
      (gather_is : [ni]i64)
      (str_con : []u8)
      (str_is : []i64)
    : strInfo =
      gather_str psize gather_is {str_content = str_con, str_idx = str_is}

  -- Sort
    entry arith_sort_str_GFTR [n] [total_len] [pL_b]
      (str_content : [total_len]u8)
      (str_idx : [n]idx_t.t)
      (ys : [n][pL_b]u8)
      (psize : idx_t.t)
    : sortStruct_str [pL_b] =
      let (sort_con, sort_is, sort_pL) = do_sort_str arith_char_cmp str_content str_idx ys psize
      let sort_info : strInfo = {str_content = sort_con, str_idx = sort_is}
      in {str_info = sort_info, pL = sort_pL}
    entry arith_sort_str_GFUR [n] [total_len]
      (str_content : [total_len]u8)
      (str_idx : [n]idx_t.t)
      (offset : idx_t.t)
      (psize : idx_t.t)
    : sortInfo_str =
      let ys = indices str_idx |> map (\i -> i+offset)
      let (sort_con, sort_is, sort_pL) = do_sort_str arith_char_cmp str_content str_idx ys psize
      let sort_info : strInfo = {str_content = sort_con, str_idx = sort_is}
      in {str_info = sort_info, is = sort_pL}

    entry arith_sort_str_caseInsens_GFTR [n] [total_len] [pL_b]
      (str_content : [total_len]u8)
      (str_idx : [n]idx_t.t)
      (ys : [n][pL_b]u8)
      (psize : idx_t.t)
    : sortStruct_str [pL_b] =
      let ci_arith_char_cmp = case_insensitive_char_cmp arith_char_cmp
      let (sort_con, sort_is, sort_pL) = do_sort_str ci_arith_char_cmp str_content str_idx ys psize
      let sort_info : strInfo = {str_content = sort_con, str_idx = sort_is}
      in {str_info = sort_info, pL = sort_pL}
    entry arith_sort_str_caseInsens_GFUR [n] [total_len]
      (str_content : [total_len]u8)
      (str_idx : [n]idx_t.t)
      (offset : idx_t.t)
      (psize : idx_t.t)
    : sortInfo_str =
      let ci_arith_char_cmp = case_insensitive_char_cmp arith_char_cmp
      let ys = indices str_idx |> map (\i -> i+offset)
      let (sort_con, sort_is, sort_pL) = do_sort_str ci_arith_char_cmp str_content str_idx ys psize
      let sort_info : strInfo = {str_content = sort_con, str_idx = sort_is}
      in {str_info = sort_info, is = sort_pL}

  -- SMJ
    entry sortMergeJoin_str
      (str_info1 : strInfo)
      (str_info2 : strInfo)
      (offset_R : idx_t.t)
      (offset_S : idx_t.t)
      (partitionSize: idx_t.t)
      (scatter_psize: idx_t.t)
    : joinPairs_str =
      smj_str arith_char_cmp str_info1 str_info2 offset_R offset_S partitionSize scatter_psize

    entry sortMergeJoin_str_caseInsens
      (str_info1 : strInfo)
      (str_info2 : strInfo)
      (offset_R : idx_t.t)
      (offset_S : idx_t.t)
      (partitionSize: idx_t.t)
      (scatter_psize: idx_t.t)
    : joinPairs_str =
      let ci_arith_char_cmp = case_insensitive_char_cmp arith_char_cmp
      in smj_str ci_arith_char_cmp str_info1 str_info2 offset_R offset_S partitionSize scatter_psize

  -- Hashing
    entry hash_str_col
      (len_bytes : idx_t.t)
      (content_bytes : idx_t.t)
      (len_range_per_bucket : idx_t.t)
      (from_subdiv : idx_t.t)
      (num_subdiv : idx_t.t)
      (case_sens : bool)
      (merge_chars : idx_t.t)
      (str_info : strInfo)
    : str_hash_info [content_bytes + len_bytes] =
      get_hashed_strs len_bytes content_bytes len_range_per_bucket from_subdiv num_subdiv case_sens merge_chars str_info

  -- Hash-Sort
    entry sort_hashed_strs_GFUR [b]
      (xs : str_hash_info [b])
    : sortedHash_str_GFUR [b] =
      do_sort_hashed_strs_GFUR xs

  -- Hash-SMJ
    def str_HSMJ [b]
      (char_cmp : u8 -> u8 -> i32)
      (tR : sortedHash_str_GFUR [b])
      (tS : sortedHash_str_GFUR [b])
      (partition_size : i64)
      (scatter_psize : i64)
      (str_info_R : strInfo)
      (str_info_S : strInfo)
    : joinPairs_str =
      do_hashed_str_SMJ char_cmp tR tS partition_size scatter_psize str_info_R str_info_S