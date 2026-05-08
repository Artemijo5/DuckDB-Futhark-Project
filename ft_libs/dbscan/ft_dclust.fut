import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

-- | expand_outer_reduce wrapper for n==1
-- Because flag generation seemingly fails in that case...
let expand_outer_red [n] 'a 'b
	(sz  : a -> i64)
	(get : a -> i64 -> b)
	(op  : b -> b -> b)
	(ne  : b)
	(arr : [n]a)
: [n]b =
	if n==0 then (replicate n ne)
	else if n==1 then iota (i64.max 1 (sz arr[0]))
		|> map (\i -> if (sz arr[0]) == 0 then ne else get arr[0] i)
		|> reduce op ne
		|> replicate n
	else arr |> expand_outer_reduce
		sz get op ne

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
		(bidir : bool)
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
			-- filter out pid1 > pid2
			-- as well as with count<0
			|> filter (\(pid1,pid2) -> (pid1<=pid2 || bidir) && part_sz[pid1]>0 && part_sz[pid2]>0)
		in part_pairs
		
	-- | Get partition information
	-- 1. pid per point
	-- 2. #points per partition
	-- 3. pairs of neighbouring partitions
	-- 4. #pairs per partition
	-- 5. index of each partition's segment in 3
	-- 6. #comparisons to be made per partition (for each of its points)
	def partition_information [np] [n]
		(bidir : bool)
		(extPar : i64)
		(eps : t)
		(subdiv : [V.length]i64)
		(part_is     : [np]i64)
		(_ : [n](vector t)) -- indexed dataset pts
	=
		let pids = scatter (replicate n (-1)) part_is (iota np)
			|> scan (i64.max) (-1)
		let pts_per_part = iota np
			|> map (\i -> if i==np-1 then n-part_is[i] else part_is[i+1]-part_is[i])
		let part_neigh_pairs = get_adj_partitions bidir extPar eps subdiv pts_per_part
		let pairs_per_part = hist (+) 0 np
			(part_neigh_pairs |> map (.0)) (part_neigh_pairs |> map (\_ -> 1))
		let pairs_index_per_part = pairs_per_part |> exscan (+) 0
		-- TODO found expand_reduce & expand_outer_reduce fail when only 1 element
		-- probably report as bug
		let cmps_per_part : [np]i64 = iota np |> expand_outer_red
			(\i -> pairs_per_part[i])
			(\i ind -> pts_per_part[part_neigh_pairs[pairs_index_per_part[i] + ind].1])
			(+) 0
		in (pids, pts_per_part, part_neigh_pairs, pairs_per_part, pairs_index_per_part, cmps_per_part)

	-- | Get neighbour counts per point.
	def get_neighbour_counts [n] [np]
		(seed_count : i64)
		(eps : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_is : [np]i64)
		(part_sz : [np]i64)
		(part_pairs_index : [np]i64)
		(part_cmps_count  : [np]i64)
	: [n]i64 =
		let init_neigh_count : [n]i64 = replicate n 0
		let num_iter = (n + seed_count - 1) / seed_count
		let final_neigh_count = loop neigh_count = init_neigh_count
		for j<num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- has 1 instance of a point's index for every neighbour that point has found
			let cur_neigh = (inf..<sup) |> map (\i -> (i,pts[i],pids[i]))
				|> expand
					(\(_,_,pid) -> part_cmps_count[pid])
					(\(i1,pt,pid) ind ->
						-- Find the ind'th point to be compared with this partition
						-- Doing a sequential search across neighbouring partition
						let (i2,_,_) : (i64,i64,i64) =
						loop (i_against, part_i_against, pts_so_far) = (-1,part_pairs_index[pid], 0)
						while i_against < 0 do
							let part_against = part_pairs[part_i_against].1
							let count_against = part_sz[part_against]
							in if pts_so_far + count_against > ind
								then (part_is[part_against] + (ind - pts_so_far),-1,-1)
								else (-1,part_i_against+1,pts_so_far+count_against)
						in if i2<i1 then (-1,-1) else
						let is_neigh = D.check_neighbourhood eps pt pts[i2]
						in if is_neigh then (i1,i2) else (-1,-1)
					)
				|> expand (\(i1,i2) -> if i1<0 then 0 else if i1==i2 then 1 else 2)
					(\(i1,i2) ind -> if ind==0 then i1 else i2)
			-- add neighbour counts found now to those found previously
			in reduce_by_index neigh_count (+) 0 cur_neigh (cur_neigh |> map (\_ -> 1))
			--	|> trace
		in final_neigh_count

	-- | Determine whether a point is core based on neighbourhood points count.
	def get_is_core
		(neigh_count : []i64)
		(minPts : i64)
	= neigh_count |> map (\c -> c >= minPts)

	-- | Isolate core points.
	def isolate_core_pts [n]
		(pts : [n](vector t))
		(pid : [n]i64)
		(is_core : [n]bool)
	= zip3 pts pid is_core
		|> filter (.2)
		|> unzip3
		|> (\(pts',pid',_) -> (pts',pid'))

	-- | Get information on partition core pts.
	-- 1. #core points per partition
	-- 2. index of first core point per partition
	-- 3. #comparisons to be made per partition (going by core points)
	def part_get_core_info [n] [np]
		(core_pid : [n]i64)
		(part_pairs : [](i64,i64))
		(part_pairs_count : [np]i64)
		(part_pairs_is : [np]i64)
	=
		let count_per_part : []i64 = hist (+) 0 np core_pid (replicate n 1)
		let first_per_part = count_per_part |> exscan (+) 0
		let cmps_per_part : [np]i64 = if np == 1 then [n] |> sized np else
			iota np |> expand_outer_red
			(\i -> part_pairs_count[i])
			(\i ind -> count_per_part[part_pairs[part_pairs_is[i] + ind].1])
			(+) 0
			|> sized np
		in (count_per_part, first_per_part, cmps_per_part)

	-- | Find clusters among core points.
	def find_clusters [n] [np]
		(seed_count : i64)
		(eps : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_core_is : [np]i64)
		(part_core_sz : [np]i64)
		(part_pairs_index : [np]i64)
		(part_core_cmps_count  : [np]i64)
	=
		let num_iter = (n + seed_count - 1) / seed_count
		let init_cid = iota n
		let final_cid = loop cid = init_cid
		for j < num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- has every min-max pair of neighbours found
			let cur_neigh = (inf..<sup) |> map (\i -> (i,pts[i],pids[i]))
				|> expand
					(\(_,_,pid) -> part_core_cmps_count[pid])
					(\(i1,pt,pid) ind ->
						-- Find the ind'th point to be compared with this partition
						-- Doing a sequential search across neighbouring partition
						let (i2,_,_) : (i64,i64,i64) =
						loop (i_against, part_i_against, pts_so_far) = (-1,part_pairs_index[pid], 0)
						while i_against < 0 do
							let part_against = part_pairs[part_i_against].1
							let count_against = part_core_sz[part_against]
							in if pts_so_far + count_against > ind
								then (part_core_is[part_against] + (ind - pts_so_far),-1,-1)
								else (-1,part_i_against+1,pts_so_far+count_against)
						in if i2<=i1 then (-1,-1) else -- no need to record self-neighbourhood
						let is_neigh = D.check_neighbourhood eps pt pts[i2]
						in if is_neigh then (i1,i2) else (-1,-1)
					)
				|> filter (\(i1,_) -> i1>=0)
			let cur_node_no = if ((length cur_neigh) == 0) then 0 else
				1 + (cur_neigh |> map (.1) |> i64.maximum)
			let cur_connections = get_connected_subgraph_ids_unencoded cur_node_no cur_neigh
				|> map (\i -> cid[i])
			let cur_collisions = if cur_node_no==0 then [] else
				map3 (\cur_cid pre_cid (i : i64) ->
					if cur_cid != pre_cid && pre_cid != i
					then (i64.min cur_cid pre_cid,i64.max cur_cid pre_cid)
					else (-1,-1)
				) cur_connections[inf:cur_node_no] cid[inf:cur_node_no] (inf..<cur_node_no)
				|> filter (\(c1,_) -> c1 >= 0)
			let rect_list = get_connected_subgraph_ids_unencoded cur_node_no cur_collisions
			in scatter (copy cid) (iota cur_node_no) cur_connections
				|> map2 (i64.min) cid
				|> map (\i -> if i<cur_node_no then rect_list[i] else i)
		-- apply dictionary encoding
		let group_encoding = map2 (==) final_cid (iota n)
			|> dict_encoding
		in final_cid |> map (\i -> group_encoding[i])

	-- | Assign id's to border pts & noises
	-- NOTE we need to check core pts of smaller id partitions as well
	def assign_cluster_ids [n] [nc] [np]
		(seed_count : i64)
		(eps : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(is_core : [n]bool)
		(core_pts  : [nc](vector t))
		(core_cids : [nc]i64)
		(part_pairs : [](i64,i64))
		(part_core_is : [np]i64)
		(part_core_sz : [np]i64)
		(part_pairs_index : [np]i64)
		(part_core_cmps_count  : [np]i64)
	=
		let num_iter = (n + seed_count - 1) / seed_count
		-- For core points, gather their cid directly
		let init_cid = is_core |> map (i64.bool)
			|> exscan (+) 0
			|> zip is_core
			|> map (\(is_c,i) -> if is_c then core_cids[i] else (-1))
		-- For each non-core point, find its core pts within eps & assign their min cluster id
		-- if no core point within eps, assign (-1) as its cluster id (noise)
		let final_cid = loop cid = copy init_cid
		for j < num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- finds minimum cid neighbouring each point
			let cur_cid = (inf..<sup)
				|> expand_outer_red
					(\i -> if is_core[i] then 1 else part_core_cmps_count[pids[i]])
					(\i ind ->
						if is_core[i] then init_cid[i] else
						let pt = pts[i]
						let pid = pids[i]
						-- Find the ind'th point to be compared with this partition
						-- Doing a sequential search across neighbouring partition
						let (i2,_,_) : (i64,i64,i64) =
						loop (i_against, part_i_against, pts_so_far) = (-1,part_pairs_index[pid], 0)
						while i_against < 0 do
							let part_against = part_pairs[part_i_against].1
							let count_against = part_core_sz[part_against]
							in if pts_so_far + count_against > ind
								then (part_core_is[part_against] + (ind - pts_so_far),-1,-1)
								else (-1,part_i_against+1,pts_so_far+count_against)
						let is_neigh = D.check_neighbourhood eps pt core_pts[i2]
						in if is_neigh then core_cids[i2] else (-1)
					)
					(\cid1 cid2 -> if cid2<0 || (cid1>=0 && cid1<cid2) then cid1 else cid2) (-1)
			in scatter cid (inf..<sup) cur_cid
		in final_cid

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
		let (pids, part_sz, part_pairs, part_pairs_count, part_pairs_is, part_cmps)
			= partition_information false extPar eps subdiv'
				part_is
				pts'
		let neigh_count = get_neighbour_counts
			seed_count
			eps
			pts'
			pids
			part_pairs
			part_is
			part_sz
			part_pairs_is
			part_cmps
		let is_core = get_is_core
			neigh_count
			minPts
		let (core_pts, core_pids) = isolate_core_pts
			pts'
			pids
			is_core
		let (part_core_sz, part_core_is, part_core_cmps) = part_get_core_info
			core_pids
			part_pairs
			part_pairs_count
			part_pairs_is
		let core_cids = find_clusters
			seed_count
			eps
			core_pts
			core_pids
			part_pairs
			part_core_is
			part_core_sz
			part_pairs_is
			part_core_cmps
		-- Get bidirectional partition pairs & core info
		let (_, _, part_pairs_bd, part_pairs_count_bd, part_pairs_is_bd,_)
			= partition_information true extPar eps subdiv'
				part_is
				pts'
		let (part_core_sz_bd, part_core_is_bd, part_core_cmps_bd) = part_get_core_info
			core_pids
			part_pairs_bd
			part_pairs_count_bd
			part_pairs_is_bd
		let cluster_id = assign_cluster_ids
			seed_count
			eps
			pts'
			pids
			is_core
			core_pts
			core_cids
			part_pairs_bd
			part_core_is_bd
			part_core_sz_bd
			part_pairs_is_bd
			part_core_cmps_bd
		let is_core'    = scatter (replicate n false) is is_core
		let cluster_id' = scatter (replicate n (-1))  is cluster_id
		in (is_core', cluster_id')

}
