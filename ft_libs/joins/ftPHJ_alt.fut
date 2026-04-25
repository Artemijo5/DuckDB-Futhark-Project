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
	let s_partition_sizes = s_info.bounds
		|> indices
		|> map (\i -> if i==np_s-1 then nS-s_info.bounds[i] else s_info.bounds[i+1]-s_info.bounds[i])
	-- For each partition in R, find the corresponding partition(s) in S
	-- will only be one partition if S has been partitioned with partition_preconfigured
	let r_parts = zip (r_info.depths |> sized np_r) (r_info.bounds |> map (\i -> tR[i]) |> sized np_r)
	let s_parts = zip (s_info.depths |> sized np_s) (s_info.bounds |> map (\i -> tS[i]) |> sized np_s)
	let matching_partitions = r_parts |> bsearch_range
		(radix_cmp (==) radix_bits)
		(radix_cmp (>=) radix_bits)
		(radix_cmp (>)  radix_bits)
		(radix_cmp (<)  radix_bits)
		(replicate np_r 0)
		(replicate np_r np_s)
		s_parts
	-- get the maximum size of participating r partitions
	let r_max_part_size = r_partition_sizes |> sized np_r
		|> zip (matching_partitions |> map (.0) |> sized np_r)
		|> filter (\(fm,_) -> fm>=0)
		|> map (.1) |> i64.maximum
	-- get the region in tS corresponding to each tR partition
	let respective_region = matching_partitions
		|> map (\(first_match, count) ->
			if first_match<0 then (-1,-1) else
			let last_match = first_match+count-1
			let inf = s_info.bounds[first_match]
			let sup = s_info.bounds[last_match] + s_partition_sizes[last_match]
			in (inf,sup)
		)
	-- stream elements of each partition of R
	-- comparing in parallel with corresponding elements in S
	-- and concatenate to previous results
	let acc_pairs : [](byteSeq [b], i64, i64)
	= loop found_pairs = [] for j < r_max_part_size do
		let this_pairs = zip3
				(r_info.bounds |> sized np_r)
				(r_partition_sizes |> sized np_r)
				(respective_region |> sized np_r)
			|> expand
				-- for partitions that have no match or have been exhausted
				-- skip them
				-- for the rest, allocate as many places as their matches in S
				(\(_,size,(inf,sup)) ->
					if size <= j || inf < 0 then 0
					else sup-inf
				)
				-- compare the streamed value of each partition of R
				-- with all the values in its matching region in S
				(\(r_b, _, (inf,_)) ind ->
					let r_v = tR[r_b + j]
					let s_v = tS[inf + ind]
					in (r_v, r_b+j, inf+ind, all (id) (map2 (==) r_v s_v))
				)
			|> filter (.3)
			|> map (\(v,ix,iy,_) -> (v,ix,iy))
		in found_pairs ++ this_pairs
	let (vs,ix,iy) = acc_pairs |> unzip3
	in {vs = vs, ix = ix, iy = iy}