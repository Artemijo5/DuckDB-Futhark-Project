import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"
import "../ft_partition"

-- Partitioned Hash Join
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39
-- & References

-- Alternative Method:
-- After partitioning both datasets and obtaining their partitionInfo,
-- stream keys from each partition of R,
-- and compare them in parallel to the corresponding partition in S.
-- This can be done in parallel with the expand operator.
-- Concatenate results.

-- | PHJ full join routine (for Inner Join).
def innerPHJ [nR] [nS] [b]
	(radix_bits : i32)
	(tR : [nR](byteSeq [b]))
	(tS : [nS](byteSeq [b]))
	(r_info : partitionInfo)
	(s_info : partitionInfo)
: joinPairs_bsq [b] =
	let np_r = length r_info.bounds
	let np_s = length s_info.bounds
	-- Get partition sizes
	let r_partition_sizes = r_info.bounds
		|> indices
		|> map (\i -> if i==np_r-1 then nR-r_info.bounds[i] else r_info.bounds[i+1]-r_info.bounds[i])
	-- For each partition in S, find the corresponding partition in R
	-- will only be one partition if have used partition_preconfigured
	let r_parts = zip (r_info.depths |> sized np_r) (r_info.bounds |> map (\i -> tR[i]) |> sized np_r)
	let s_parts = zip (s_info.depths |> sized np_s) (s_info.bounds |> map (\i -> tS[i]) |> sized np_s)
	let matching_partitions = s_parts |> bsearch_first
		(radix_cmp (==) radix_bits)
		(radix_cmp (>)  radix_bits)
		(replicate np_s 0)
		(replicate np_s np_r)
		r_parts
	-- get the maximum size of participating r partitions
	let r_max_part_size = r_partition_sizes |> sized np_r
		|> zip (matching_partitions |> sized np_r)
		|> filter (\(fm,_) -> fm>=0)
		|> map (.1) |> i64.maximum
	-- for each element in S, get its matching partition in R
	let matching_parts = scatter (replicate nS (-1)) s_info.bounds (indices s_info.bounds)
		|> scan (i64.max) (-1)
		|> map (\i -> matching_partitions[i])
	-- stream elements of each partition of R
	-- comparing in parallel with corresponding elements in S
	-- and concatenate to previous results
	let acc_pairs : [](byteSeq [b], i64, i64)
	= loop found_pairs = [] for j < r_max_part_size do
		let this_pairs = matching_parts
			|> map2 (\i_s mpart ->
				if mpart < 0 || j>=r_partition_sizes[mpart] then (i_s,-1) else 
				let i_r = r_info.bounds[mpart] + j
				in if all (id) (map2 (==) tR[i_r] tS[i_s])
					then (i_s,i_r)
					else (i_s,-1)
			) (indices tS)
			|> filter (\(_,i_r) -> i_r>=0)
			|> map (\(iy,ix) -> (tS[iy],ix,iy))
		in found_pairs ++ this_pairs
	let (vs,ix,iy) = acc_pairs |> unzip3
	in {vs = vs, ix = ix, iy = iy}