import "../../../ft_SortGroupBy"

-- Group-by maximum on 2 i32 columns.
-- ==
-- entry: max_i32
-- input @data/data_i32.in

entry max_i32 [n]
	(keys : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i32)
	(_ : [n]i32)
=
	let (k_idx, k_size) = sortgroup_find_known_key_counts (iota maxV[0]) keys (==) (>) (<)
	--let (k_idx,k_size) = sortgroup_find_unknown_key_counts keys (==)
	in SortGroupedAggregator_int.maximum k_idx k_size col1

-- Group-by covariance on 2 i32 columns.
-- ==
-- entry: cov_i32
-- input @data/data_i32.in

entry cov_i32 [n]
	(keys : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i32)
	(col2 : [n]i32)
=
	let (k_idx, k_size) = sortgroup_find_known_key_counts (iota maxV[0]) keys (==) (>) (<)
	--let (k_idx,k_size) = sortgroup_find_unknown_key_counts keys (==)
	in SortGroupedAggregator_int.cov k_idx k_size col1 col2

-- Group-by maximum on 2 i64 columns.
-- ==
-- entry: max_i64
-- input @data/data_i64.in

entry max_i64 [n]
	(keys : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i64)
	(_ : [n]i64)
=
	let (k_idx, k_size) = sortgroup_find_known_key_counts (iota maxV[0]) keys (==) (>) (<)
	--let (k_idx,k_size) = sortgroup_find_unknown_key_counts keys (==)
	in SortGroupedAggregator_long.maximum k_idx k_size col1

-- Group-by covariance on 2 i64 columns.
-- ==
-- entry: cov_i64
-- input @data/data_i64.in

entry cov_i64 [n]
	(keys : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i64)
	(col2 : [n]i64)
=
	let (k_idx, k_size) = sortgroup_find_known_key_counts (iota maxV[0]) keys (==) (>) (<)
	--let (k_idx,k_size) = sortgroup_find_unknown_key_counts keys (==)
	in SortGroupedAggregator_long.cov k_idx k_size col1 col2