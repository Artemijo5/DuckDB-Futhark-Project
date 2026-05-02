import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
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

module ft_dbscan
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
			|> V.map (to_i64) |> V.to_array
		in I.index_dataset sdv' pts

	-- | Get pairs of adjacent partitions
	-- Specifically returns pairs (i,j) with i<=j.
	def get_adj_partitions [np]
		(extPar : i64)
		(eps : t)
		(parts : [np](vector t, vector t))
	: [](i64, i64) =
		let num_iter = (np + extPar - 1) / extPar
		in loop pairs : [](i64,i64) = []
		for j < num_iter do
			let inf = j*extPar
			let sup = i64.min np (inf+extPar)
			let this_pairs = (inf..<sup) |> map (\i1 ->
				(inf..<np) |> map (\i2 ->
					if i1>i2 then (-1,-1) else
					let d = D.get_rectangle_dist parts[i1] parts[i2]
					in if (d `leq` eps) then (i1,i2) else (-1,-1)
				)
			) |> flatten |> filter (\(i,_) -> i>=0)
			in pairs ++ this_pairs


	-- | Get partition information
	-- 1. pid per point
	-- 2. #points per partition
	-- 3. pairs of neighbouring partitions
	-- 4. #pairs per partition
	-- 5. index of each partition's segment in 3
	-- 6. #comparisons to be made per partition
	def partition_information [np] [n]
		(extPar : i64)
		(eps : t)
		(part_minmax : [np](vector t, vector t))
		(part_is     : [np]i64)
		(_ : [n](vector t)) -- indexed dataset pts
	=
		let pids = scatter (replicate n (-1)) part_is (iota np)
			|> scan (i64.max) (-1)
		let pts_per_part = iota np
			|> map (\i -> if i==np-1 then n-part_is[i] else part_is[i+1]-part_is[i])
		let part_neigh_pairs = get_adj_partitions extPar eps part_minmax
		let pairs_per_part = hist (+) 0 np
			(part_neigh_pairs |> map (.0)) (part_neigh_pairs |> map (\_ -> 1))
		let pairs_index_per_part = pairs_per_part |> exscan (+) 0
		let cmps_per_part = iota np |> expand_outer_reduce
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
						let is_neigh = D.check_neighbourhood eps pt pts[i2]
						in if is_neigh then (i1,i2) else (-1,-1)
					)
				|> expand (\(i1,_) -> if i1<0 then 0 else 2)
					(\(i1,i2) ind -> if ind==0 then i1 else i2)
			-- add neighbour counts found now to those found previously
			in reduce_by_index neigh_count (+) 0 cur_neigh (cur_neigh |> map (\_ -> 1))
		in final_neigh_count

	-- | Determine whether a point is core based on neighbourhood points count.
	def is_core
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
		(core_pts : [n](vector t))
		(core_pid : [n]i64)
		(part_pairs : [](i64,i64))
		(part_pairs_count : [np]i64)
		(part_pairs_is : [np]i64)
	=
		let count_per_part = hist (+) 0 np core_pid (replicate n 1)
		let first_per_part = count_per_part |> exscan (+) 0
		let cmps_per_part = iota np |> expand_outer_reduce
			(\i -> part_pair_count[i])
			(\i ind -> count_per_part[part_pairs[part_pair_is[i] + ind].1])
			(+) 0
		in (count_per_part, first_per_part, cmps_per_part)

	-- Find clusters among core points.
	-- TODO continue with this
	def find_clusters [n]
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
		let init_cid = replicate n i64.highest
		let final_cid = loop cid = init_cid
		for j < num_iter do
			let inf = j*seed_count
			let sup = i64.min n (inf+seed_count)
			-- has every min-max pair of neighbours found
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
						let is_neigh = D.check_neighbourhood eps pt pts[i2]
						in if is_neigh then (i1,i2) else (-1,-1)
					)
				|> expand (\(i1,_) -> if i1<0 then 0 else 2)
					(\(i1,i2) ind -> if ind==0 then i1 else i2)




}