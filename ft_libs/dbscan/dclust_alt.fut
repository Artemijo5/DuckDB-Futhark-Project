import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

-- DBSCAN implementation based on cuda-DClust+

-- Alternative to see if I can get it working without segmented parallelism.

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

	local def slightly_over = times (F.f64 1.001)

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
		-- get value range per dimension
		let ranges = V.map2 (minus) maxs mins
		-- minimum cell width >= eps in every dimension
		let sdv_alt = V.map (\r -> r `over` (slightly_over eps)) ranges
		let sdv' = V.map2 (min) sdv_alt
			(sdv |> V.from_array |> V.map (from_i64))
			|> V.map (to_i64)
			-- at least 1 subdivision per dimension
			|> V.map2 (i64.max) (V.replicate 1i64)
			|> V.to_array
		-- return final subdivisions & indexed dataset
		in (sdv', I.index_dataset sdv' pts)

	-- | Get pairs of adjacent cells,
	-- as tuples of pids (pid1,pid2),
	-- in a contiguous array grouped by pid1.
	-- 
	-- If bidir is true, returns all pairs of adjacent cells.
	-- Otherwise, returns only (pid1,pid2) where pid1<=pid2.
	--
	-- If either pid1 or pid2 does not have any points in the dataset,
	-- then that pair is omitted from the output.
	def get_adj_partitions [np]
		(bidir : bool)
		(subdiv : [V.length]i64)
		(part_sz : [np]i64)
	: [](i64, i64) =
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		-- pid1's adjacent partitions are
		-- those located +-1 subdivisions of it
		-- in any dimension(s).
		-- This can be represented by a d-cube of width 3 subdivisions.
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
	-- 2. pairs of neighbouring cells
	-- 3. index of each cell's segment in 2
	def partition_information [np] [n]
		(bidir : bool)
		(subdiv : [V.length]i64)
		(part_is     : [np]i64)
		(_ : [n](vector t)) -- indexed dataset pts
	=
		-- Since pts are sorted by pid, can simply scatter & scan part_is
		let pids = scatter (replicate n (-1)) part_is (iota np)
			|> scan (i64.max) (-1)
		-- count of pts per pid
		let part_sz = hist (+) 0 np pids (replicate n 1i64)
		-- partition pairs
		let part_pairs = get_adj_partitions bidir subdiv part_sz
		-- part_pairs_is
		let part_pairs_is = hist (+) 0 np (part_pairs |> map (.0)) (part_pairs |> map (\_ -> 1i64))
			|> exscan (+) 0
		in (pids, part_pairs, part_pairs_is)

	-- | Get neighbour counts per point.
	def get_neighbour_counts [n] [np]
		(seed_count : i64)
		(eps : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_is : [np]i64)
		(part_pairs_is : [np]i64)
	: [n]i64 =
		let init_neigh_count : [n]i64 = replicate n 1i64
		let num_iter = (n + seed_count - 1) / seed_count
		let final_neigh_count = loop neigh_count = init_neigh_count
		for j<num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- get all relevant points to the current window:
			-- ie points in partitions adjacent to any partition present in the window
			let inf_pid = pids[inf]
			let sup_pid = pids[sup-1]
			let inf_pairs = part_pairs_is[inf_pid]
			let sup_pairs =if sup_pid==np-1
				then (length part_pairs)
				else part_pairs_is[sup_pid+1]
			let relevant_parts = part_pairs[inf_pairs:sup_pairs]
				|> map (.1)
				|> (\rels -> hist (||) false np
						rels (rels |> map (\_ -> true))
					|> zip (iota np)
					|> filter (.1)
					|> map (.0)
				)
			let relevant_inf = part_is[head relevant_parts]
			let relevant_sup = if (last relevant_parts) == (np-1)
				then n
				else part_is[(last relevant_parts) + 1]
			let n_rel = relevant_sup - relevant_inf
			-- has 1 instance of a point's index for every neighbour that point has found
			-- separated into 2 equi-sized arrays
			let (cur_mins,cur_maxs,_) = (inf..<sup)
				|> map (\i -> zip (replicate n_rel i) ((relevant_inf..<relevant_sup) |> sized n_rel))
				|> flatten
				|> filter (\(i1,i2) -> i1<i2)
				|> map (\(i1,i2) -> 
					let is_neigh = D.check_neighbourhood eps pts[i1] pts[i2]
					in (i1,i2,is_neigh)
				)
				|> filter (.2)
				|> unzip3
			-- add neighbour counts found now to those found previously
			in if (length cur_mins)==0 then neigh_count else
			hist (+) 0 n cur_mins (cur_mins |> map (\_ -> 1i64))
				|> map2 (+) (hist (+) 0 n cur_maxs (cur_maxs |> map (\_ -> 1i64)))
				|> map2 (+) neigh_count
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
	-- (index of first core point per partition)
	def part_get_core_info [n] [np]
		(core_pid : [n]i64)
		(_ : [np]i64) -- part_is
	: [np]i64 =
		hist (+) 0 np core_pid (replicate n 1i64)
			|> exscan (+) 0

	-- | Find clusters among core points.
	def find_clusters [n] [np]
		(seed_count : i64)
		(eps : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_core_is : [np]i64)
		(part_pairs_is : [np]i64)
	=
		let num_iter = (n + seed_count - 1) / seed_count
		let init_cid = iota n
		let final_cid = loop cid = init_cid
		for j < num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			let inf_pid = pids[inf]
			let sup_pid = pids[sup-1]
			let inf_pairs = part_pairs_is[inf_pid]
			let sup_pairs =if sup_pid==np-1
				then (length part_pairs)
				else part_pairs_is[sup_pid+1]
			let relevant_parts = part_pairs[inf_pairs:sup_pairs]
				|> map (.1)
				|> (\rels -> hist (||) false np
						rels (rels |> map (\_ -> true))
					|> zip (iota np)
					|> filter (.1)
					|> map (.0)
				)
			let relevant_inf = part_core_is[head relevant_parts]
			let relevant_sup = if (last relevant_parts) == (np-1)
				then n
				else part_core_is[(last relevant_parts) + 1]
			let n_rel = relevant_sup - relevant_inf
			-- finds every pair of adjacent core points (i1,i2), i1<i2
			let cur_neigh = (inf..<sup)
				|> map (\i -> zip (replicate n_rel i) ((relevant_inf..<relevant_sup) |> sized n_rel))
				|> flatten
				|> filter (\(i1,i2) -> i1<i2)
				|> map (\(i1,i2) -> 
					let is_neigh = D.check_neighbourhood eps pts[i1] pts[i2]
					in (i1,i2,is_neigh)
				)
				|> filter (.2)
				|> map (\(i1,i2,_) -> (i1,i2))
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
	-- as such, part_pairs and after need to be bidirectional
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
		(part_pairs_is : [np]i64)
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
			let inf_ = j*seed_count
			let sup_ = i64.min n (inf_+seed_count)
			let cur_is = (inf_..<sup_) |> filter (\i -> !is_core[i])
			in if (length cur_is) == 0 then cid else
			let inf = head cur_is
			let sup = (last cur_is) + 1
			let inf_pid = pids[inf]
			let sup_pid = pids[sup-1]
			let inf_pairs = part_pairs_is[inf_pid]
			let sup_pairs =if sup_pid==np-1
				then (length part_pairs)
				else part_pairs_is[sup_pid+1]
			let relevant_parts = part_pairs[inf_pairs:sup_pairs]
				|> map (.1)
			let relevant_inf = relevant_parts
				|> map (\i -> part_core_is[i])
				|> i64.minimum
			let relevant_sup = relevant_parts
				|> map (\i -> if i==np-1 then nc else part_core_is[i+1])
				|> i64.maximum
			let n_rel = relevant_sup - relevant_inf
			-- has every min-max pair of neighbours found
			let (cur_xs,cur_ys) = (inf..<sup)
				|> map (\i -> zip (replicate n_rel i) ((relevant_inf..<relevant_sup) |> sized n_rel))
				|> map (map (\(i1,i2) -> 
					let is_neigh = D.check_neighbourhood eps pts[i1] core_pts[i2]
					in (i1,i2,is_neigh)
				))
				|> flatten
				|> filter (.2)
				-- subtract inf from i1 to use in histogram
				-- and take the cluster id of i2
				|> map (\(i1,i2,_) -> (i1-inf,core_cids[i2]))
				|> unzip
			let cur_cid = hist (\c1 c2 -> if c1>=0 && (c2<0 || c2>c1) then c1 else c2) (-1)
				(sup-inf) cur_xs cur_ys
			let cur_cid' = cur_is |> map (\i -> i-inf) |> map (\i -> cur_cid[i]) -- only update border points
			in scatter (copy cid) cur_is cur_cid'
		in final_cid

	-- | Index dataset & perform DBSCAN algorithm.
	def do_dclust [n]
		(_ : i64) -- extPar - vestigial parameter
		(seed_count : i64)
		(subdiv : [V.length]i64)
		(eps : t)
		(minPts : i64)
		(pts : [n](vector t))
	: ([n]bool, [n]i64) =
		let (subdiv', (pts',_,part_is,is))
			= partition_dataset eps subdiv pts
		let (pids, part_pairs, part_pairs_is)
			= partition_information
				false
				subdiv'
				part_is
				pts'
		let neigh_count = get_neighbour_counts
			seed_count
			eps
			pts'
			pids
			part_pairs
			part_is
			part_pairs_is
		let is_core = get_is_core
			neigh_count
			minPts
		let (core_pts, core_pids) = isolate_core_pts
			pts'
			pids
			is_core
		let part_core_is = part_get_core_info
			core_pids
			part_is
		let core_cids = find_clusters
			seed_count
			eps
			core_pts
			core_pids
			part_pairs
			part_core_is
			part_pairs_is
		-- Get bidirectional partition pairs & core info
		let (_, part_pairs_bd, part_pairs_is_bd)
			= partition_information
				true
				subdiv'
				part_is
				pts'
		let cluster_id = assign_cluster_ids
			seed_count
			eps
			pts'
			pids
			is_core
			core_pts
			core_cids
			part_pairs_bd
			part_core_is
			part_pairs_is_bd
		let is_core'    = scatter (replicate n false) is is_core
		let cluster_id' = scatter (replicate n (-1))  is cluster_id
		in (is_core', cluster_id')


}