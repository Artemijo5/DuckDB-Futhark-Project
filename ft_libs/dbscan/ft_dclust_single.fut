import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../merge_path"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

-- DBSCAN implementation based on cuda-DClust+

-- Original implementation uses a seed-list to expand clusters,
-- while using a tree index for neighbourhood search.
--
-- Here, I use nested parallelism to find neighbouring partitions,
-- and instead of traditionally expanding clusters via a seed-list,
-- I instead process points by their partition,
-- using the segmented extension.
--
-- Ultimately, the same pairwise point comparisons are made,
-- just in different order.
-- Could do traditional expansion using a relevant point queue
-- similar to the relevant partitions queue in dbscan.fut,
-- but that would be superfluous for futhark's logic.
module ft_dclust
	(V : vector)
	(F : real)
	(D : distance with vector 'a = V.vector a with t = F.t)
= {
	type t = F.t
	type vector 'a = V.vector a

	module I = grid_index V F

	local def zero = F.i32 0i32

	local def over  = (F./)
	local def times = (F.*)
	local def minus = (F.-)
	local def plus  = (F.+)

	local def slightly_bigger = times (F.f64 1.001)

	local def leq = (F.<=)

	local def to_i64 = (F.to_i64)
	local def from_i64 = (F.i64)

	local def min = F.min
	local def minimum = F.minimum
	local def maximum = F.maximum

	-- | Partition dataset with a regular grid using given subdivisions per dimension,
	-- also ensuring that in no dimension is the grid thinner than eps.
	def partition_dataset
		(eps : t)
		(sdv : [V.length]i64) -- subdivisions per dimension
		(pts : [](vector t))
	=
		-- get min & max values per dimension
		let perDim = iota (V.length) |> map (\i -> pts |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		let ranges = V.map2 (minus) maxs mins
		let sdv_alt = V.map (\r -> r `over` (slightly_bigger eps)) ranges
		let sdv' = V.map2 (min) (sdv |> V.from_array |> V.map (from_i64)) sdv_alt
			|> V.map (to_i64)
			|> V.map2 (i64.max) (V.replicate 1i64)
			|> V.to_array
		in (sdv', I.index_dataset sdv' pts)

	-- | Get pairs of adjacent partitions
	-- Specifically returns pairs (i,j) with i<=j.
	-- Unless bidir is set to true.
	-- TODO can do more efficiently by using grid properties.
	def get_adj_partitions [np]
		(_ : i64) -- extPar - vestigial parameter (...)
		(_ : t) -- eps - vestigial parameter (...)
		(subdiv : [V.length]i64)
		(part_sz : [np]i64)
	: [](i64, i64) =
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		let adj_cube_increments = iota (3**V.length)
			|> map (\i -> V.iota
				|> V.map (\d -> (-1) + (i/(3**d))%3)
			)
		let part_pairs = iota np
			|> filter (\i -> part_sz[i] > 0)
			-- convert pid into a vector of subdivision steps
			|> map (\cur_pid ->
				let as_vector = prefix_v
					|> V.map (\pref -> cur_pid / pref)
					|> V.map2 (\sdv pid_suffix -> pid_suffix%sdv) subdiv_v
				in (cur_pid,as_vector)
			)
			-- add adj_cube_increments
			|> map (\(cur_pid, as_vector) -> adj_cube_increments
				|> map (V.map2 (+) as_vector)
				|> zip (replicate (3**V.length) cur_pid)
			) |> flatten
			-- filter invalid
			|> filter (\(_,vec) -> 
				let all_positive = vec |> V.map (\v -> v>=0)
					|> V.reduce (&&) true
				let none_exceeding = vec |> V.map2 (\sdv v -> v<sdv) subdiv_v
					|> V.reduce (&&) true
				in all_positive && none_exceeding
			)
			-- convert vector back to numerical pid
			|> map (\(cur_pid,vec) ->
				let neigh_pid = vec |> V.map2 (*) prefix_v
					|> V.reduce (+) 0
				in (cur_pid, neigh_pid)
			)
			|> filter (\(pid1,pid2) -> part_sz[pid1]>0 && part_sz[pid2]>0)
		in part_pairs
		
	-- | Get partition information
	-- 1. pid per point
	-- 2. #points per partition
	-- 3. pairs of neighbouring partitions
	-- 4. #pairs per partition
	-- 5. index of each partition's segment in 3
	def partition_information [np] [n]
		(extPar : i64)
		(eps : t)
		(subdiv : [V.length]i64)
		(part_is     : [np]i64)
		(_ : [n](vector t)) -- indexed dataset pts
	=
		let pids = bsearch_last_merge_path
			(>=) (<)
			((np+2047)/2048)
			(iota n)
			part_is
		let pts_per_part = iota np
			|> map (\i -> if i==np-1 then n-part_is[i] else part_is[i+1]-part_is[i])
		let part_neigh_pairs = get_adj_partitions extPar eps subdiv pts_per_part
		let pairs_per_part = hist (+) 0 np
			(part_neigh_pairs |> map (.0)) (part_neigh_pairs |> map (\_ -> 1i64))
		let pairs_index_per_part = pairs_per_part |> exscan (+) 0
		in (pids, pts_per_part, part_neigh_pairs, pairs_per_part, pairs_index_per_part)

	def mk_clusters [n] [np]
		(seed_count : i64)
		(eps : t)
		(minPts : i64)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_is : [np]i64)
		(part_sz : [np]i64)
		(part_pairs_index : [np]i64)
		(part_pairs_count : [np]i64)
	: ([n]bool, [n]i64) =
		-- Initialize output buffers
		let init_neigh_count : [n]i64 = replicate n 0
		let init_cids : [n]i64 = replicate n (-1)
		-- Set up loop
		let num_iter = (n + seed_count - 1) / seed_count
		let (final_neigh_count, final_cids)
		= loop (neigh_count, cids)
		= (init_neigh_count, init_cids)
		for j<num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- will find 1 instance of each pair
			let cur_neigh_pairs = (inf..<sup)
				|> map (\i1 -> (i1,pids[i1]))
				-- expand to partitions it is compared with
				|> expand
					(\(_,pid1) -> part_pairs_count[pid1])
					(\(i1,pid1) ind ->
						let index_in_pairs = part_pairs_index[pid1] + ind
						let pid2 = (part_pairs[index_in_pairs]).1
						in (i1,pid2)
					)
				|> expand
					(\(_,pid2) -> part_sz[pid2])
					(\(i1,pid2) ind ->
						let i2 = part_is[pid2] + ind
						in (i1,i2)
					)
				|> filter (\(i1,i2) -> i1==i2 || D.check_neighbourhood eps pts[i1] pts[i2])
			-- find #neighbours of current window points
			let neigh_count' = reduce_by_index neigh_count (+) 0
					(cur_neigh_pairs |> map (.0))
					(cur_neigh_pairs |> map (\_ -> 1i64))
			-- find core points
			let is_core = neigh_count' |> map (\c -> c >= minPts)
			-- find clusters between current core points
			let cur_core_clusts = cur_neigh_pairs
				|> filter (\(i1,i2) -> is_core[i1] && is_core[i2] && (i1<i2 || i2<inf))
				|> map (\(i1,i2) -> (i64.min i1 i2, i64.max i1 i2))
				|> get_connected_subgraph_ids n
				|> map2 (\isC cur_id -> if isC then cur_id else (-1)) is_core
			-- find & correct collisions with previous clusters
			let collisions = zip3 cids cur_core_clusts is_core
				|> filter (.2)
				|> map (\(alt,neu,_) -> if alt<0 || neu<0 || alt==neu then (-1,-1)
					else (i64.min alt neu, i64.max alt neu))
				|> filter (\(alt,_) -> alt>=0)
			let rect_by_collisions = collisions |> get_connected_subgraph_ids n
			let cur_core_clusts' = cur_core_clusts
				|> map (\i -> if i<0 then i else rect_by_collisions[i])
			-- assign clusters to border points as well
			let total_cids = reduce_by_index cids (i64.max) (-1)
				(cur_neigh_pairs |> map (.1))
				(cur_neigh_pairs |> map (.0) |> map (\i -> cur_core_clusts'[i]))
				|> map (\i -> if i<0 then i else rect_by_collisions[i])
			in (neigh_count', copy total_cids)
		let final_is_core = final_neigh_count |> map (\c -> c>=minPts)
		let final_cids' = final_cids |> encode_subgraph_ids
		in (final_is_core, final_cids')

	-- | Index dataset & perform DBSCAN algorithm.
	def do_dclust [n]
		(extPar : i64)
		(seed_count : i64)
		(subdiv : [V.length]i64)
		(eps : t)
		(minPts : i64)
		(pts : [n](vector t))
	: ([n]bool, [n]i64) =
		let (subdiv', (pts',_,part_is,is))
			= partition_dataset eps subdiv pts
		let (pids, part_sz, part_pairs, part_pairs_count, part_pairs_is)
			= partition_information extPar eps subdiv'
				part_is
				pts'
		let (is_core, cluster_id) = mk_clusters
			seed_count
			eps
			minPts
			pts'
			pids
			part_pairs
			part_is
			part_sz
			part_pairs_is
			part_pairs_count
		let is_core'    = scatter (replicate n false) is is_core
		let cluster_id' = scatter (replicate n (-1))  is cluster_id
		in (is_core', cluster_id')

}
