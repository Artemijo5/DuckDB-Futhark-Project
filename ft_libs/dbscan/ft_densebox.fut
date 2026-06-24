-- Implementation taking ideas from FDBSCAN_denseBox.

import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../merge_path"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

-- Wrapper for expand_outer_reduce
-- handling the case of 1 point.
local def expand_outer_red [n] 't
	sz get op ne (xs : [n]t)
= if n>1
	then xs |> expand_outer_reduce sz get op ne
	else xs |> expand (sz >-> (i64.max 1))
		(\x ind -> if (sz x)==0 then ne else get x ind)
		|> reduce op ne
		|> replicate n

module ft_densebox
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
	local def sqrt = (F.sqrt)

	local def leq = (F.<=)

	local def to_i64 = F.to_i64
	local def from_i64 = F.i64

	local def ceil = F.ceil

	local def min = F.min
	local def minimum = F.minimum
	local def maximum = F.maximum

	-- | Partition dataset with a regular grid using given subdivisions per dimension,
	-- also ensuring that in no dimension is the grid thinner than eps.
	def partition_dataset [n]
		(eps : t)
		(pts : [](vector t))
	=
		-- get min & max values per dimension
		let cell_width = eps `over` (V.length |> from_i64 |> sqrt)
		let perDim = iota (V.length) |> map (\i -> pts |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		let ranges = V.map2 (minus) maxs mins
		let sdv = V.map (\r -> r `over` cell_width) ranges
		let sdv' = sdv
			|> V.map (ceil >-> to_i64)
			|> V.map2 (i64.max) (V.replicate 1i64)
			|> V.to_array
		-- Auxilliary point so that cell_width is correct
		-- is appended to the end of the dataset
		-- and is removed after partitioning
		let maxs' = V.map2 (\mi sub ->
			mi `plus` (sub `times` cell_width)
		) mins sdv
		let (pts', bounds, part_is, cell_ids, og_is) = I.index_dataset sdv' (pts ++ [maxs'])
		let np = length part_is
		in (sdv', pts'[0:n],
			bounds |> sized np, part_is |> sized np, cell_ids |> sized np,
			og_is[0:n]
		)

	def get_part_info [n] [np]
		(minPts : i64)
		(part_is : [np]i64)
		(_ : [n](vector t))
	: ([np]i64, [np]bool, [n]i64) =
		let part_sz = iota np
			|> map (\i -> if i<np-1 then part_is[i+1] else n)
			|> map2 (\cur_i next_i -> next_i-cur_i) part_is
		let isDense = part_sz |> map (>=minPts)
		let part_ids = scatter (replicate n (-1)) part_is (iota np)
			|> scan (i64.max) (-1)
		in (part_sz, isDense, part_ids)

	local def get_most_populated_dim [np]
		(cell_ids : [np]i64)
		(subdiv : [V.length]i64)
	: i64 =
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		let as_vectors = cell_ids |> map (\cur_pid ->
			prefix_v
				|> V.map (\pref -> cur_pid / pref)
				|> V.map2 (\sdv pid_suffix -> pid_suffix%sdv) subdiv_v
		)
		let distinct_per_dim = replicate V.length 0
			|> seqmap 0 (\dim ->
				let d_vals = as_vectors
					|> map (V.get dim)
				let d_min = i64.minimum d_vals
				let d_vals' = d_vals |> map (\d_v -> d_v-d_min)
				let d_max' = i64.maximum d_vals'
				let num_vals = scatter (replicate (d_max' + 1) false)
					d_vals' (replicate np true)
					|> countFor (id)
				in num_vals
			)
		in argmin (>) (==) 0 distinct_per_dim

	def get_box_neighbourhoods [np]
		(subdiv : [V.length]i64)
		(cell_ids : [np]i64)
	: ([](i64,i64),[np]i64,[np]i64) =
		let adj_range = V.length |> from_i64 |> sqrt
			|> ceil |> to_i64
		let most_popd_dim = get_most_populated_dim cell_ids subdiv
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		let as_vectors = cell_ids |> map (\cur_pid ->
			prefix_v
				|> V.map (\pref -> cur_pid / pref)
				|> V.map2 (\sdv pid_suffix -> pid_suffix%sdv) subdiv_v
		)
		-- Sort by the most populated dim
		let xs = as_vectors |> map (V.get most_popd_dim)
		let num_buckets = 1 + (i64.maximum xs)	
		let (xs', is') = bucket_sort 2 num_buckets xs (iota np)
		-- 1st stage: SMJ xs' with xs'
		-- check for neighbourhood only in one dimension
		let x_matches = xs' |> bsearch_range
			(\x1 x2 -> i64.abs (x1-x2) <= adj_range)
			(\x1 x2 -> x2 - x1 <= adj_range)
			(\x1 x2 -> x1 - x2 > adj_range)
			(\x1 x2 -> x2 - x1 > adj_range)
			(replicate np 0)
			(replicate np np)
			xs'
			-- return to original ordering
			|> scatter (replicate np (-1,0)) is'
		-- to avoid exploding memory in intermediate materialization
		-- do sequential loop over candidate matches
		let part_pairs = x_matches
			|> zip is'
			-- 1st loop to find 1st true match & number of true matches
			|> map (\(i1,(fm,cm)) ->
				let vec1 = as_vectors[i1]
				let (_,first_match,num_matches)
					= loop (j,tfm,tcm)
					= (fm,-1,0)
				while j<(fm+cm) do
					let i2 = is'[j]
					let vec2 = as_vectors[i2]
					let is_neigh = V.map2 (-) vec1 vec2
						|> V.map (\diff -> diff**2)
						|> V.reduce (+) 0
						|> (\meas -> meas <= adj_range**2)
					in 
						if !is_neigh
							then (j+1,tfm,tcm)
						else if tcm==0
							then (j+1,i2,tcm+1)
						else
							(j+1,tfm,tcm+1)
				in (i1, first_match, num_matches)
			)
			-- materialize
			|> expand
				(\(_,_,cm) -> cm)
				(\(i1,fm,_) k ->
					let vec1 = as_vectors[i1]
					let (foundAt,_)
						= loop (j,matches_so_far)
						= (fm-1,0)
					while matches_so_far<=k do
						let i2 = is'[j+1]
						let vec2 = as_vectors[i2]
						let is_neigh = V.map2 (-) vec1 vec2
							|> V.map (\diff -> diff**2)
							|> V.reduce (+) 0
							|> (\meas -> meas <= adj_range**2)
						in if is_neigh
							then (j+1,matches_so_far+1)
							else (j+1,matches_so_far)
					in (i1,foundAt)
				)
		-- Sort part_pairs by their distance
		-- so points will first check their closest neighbours
		let part_pairs_dists = part_pairs
			|> map (\(i1,i2) ->
				let vec1 = as_vectors[i1]
				let vec2 = as_vectors[i2]
				in V.map2 (-) vec1 vec2
					|> V.map (\diff -> diff**2)
					|> V.reduce (+) 0
			)
		let part_pairs' = part_pairs
			|> bucket_sort 2 np part_pairs_dists
			|> (.1) |> unzip
			|> (\(pp0,pp1) -> bucket_sort 2 np pp0 pp1)
			|> (\(pp0,pp1) -> zip pp0 pp1)
		let part_pairs_sz = hist_lean (+) 0 np
			(part_pairs' |> map (.0))
			(part_pairs' |> map (\_ -> 1i64))
		let part_pairs_is = part_pairs_sz
			|> exscan (+) 0
		in (part_pairs', part_pairs_is, part_pairs_sz)

	def find_core_pts [n] [np]
		(eps  : t)
		(minPts : i64)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(part_is : [np]i64)
		(part_sz : [np]i64)
		(part_pairs : [](i64,i64))
		(part_pairs_is : [np]i64)
		(part_pairs_sz : [np]i64)
	: [n]bool = 
		-- Some cells may have less than minPts pts in their entire neighbourhood
		-- dismiss those
		let dismiss_pid = iota np
			|> expand_outer_red
				(\pid -> 1 + part_pairs_sz[pid])
				(\pid ind ->
					if ind==0 then part_sz[pid] else
					let ind2 = ind-1
					let pid2 = part_pairs[part_pairs_is[pid]+ind2].1
					in part_sz[pid2]
				)
				(+) 0
			|> map ( < minPts)
		in map2 (\pid1 pt1 ->
				if dismiss_pid[pid1] then false else
				-- Points in the same cell are automatically neighbours
				-- if in a dense cell, already core
				-- otherwise, traverse all neighbouring points
				-- until is core
				let pairs_sz1 = part_pairs_sz[pid1]
				let (_,num_neigh)
					= loop (j, cur_neigh)
					= (0,part_sz[pid1])
				while j<pairs_sz1 && cur_neigh<minPts do
					let pid2 = part_pairs[part_pairs_is[pid1]+j].1
					let pt_count2 = part_sz[pid2]
					let (_,num_neigh2)
						= loop (k, cur_neigh2)
						= (0,cur_neigh)
					while k<pt_count2 && cur_neigh2<minPts do
						let pt2 = pts[part_is[pid2]+k]
						let is_neigh = D.check_neighbourhood eps pt1 pt2
						in if is_neigh
							then (k+1,cur_neigh2+1)
							else (k+1,cur_neigh2)
					in (j+1,num_neigh2)
				in num_neigh>=minPts
			) pids pts

	-- Clusters are made on the basis of CELLS rather than pts
	def mk_clusters [n] [np]
		(eps  : t)
		(pts  : [n](vector t))
		(pids : [n]i64)
		(is_core : [n]bool)
		(part_pairs : [](i64,i64))
		(_ : [np]i64) -- sth to contain np
	: [np]i64 =
		-- Get core pt info
		let (core_pts, core_pids, _) = zip3 pts pids is_core
			|> filter (.2)
			|> unzip3
		let part_core_sz = hist_lean (+) 0 np
			core_pids
			(core_pids |> map (\_ -> 1i64))
		let part_core_is = part_core_sz
			|> exscan (+) 0
		let has_cores = part_core_sz
			|> map (>0)
		-- Core pairs: both cells have at least one core-pt neighbourhood
		let core_pairs = part_pairs
			-- each pair only needs to be checked once
			-- use the cell with most cores left
			-- so that it's the outer part of the nested loop
			|> filter (\(pid1,pid2) -> pid1<pid2
				&& part_core_sz[pid1]>0
				&& part_core_sz[pid2]>0
			)
			|> map (\(pid1,pid2) ->
				if part_core_sz[pid1]<=part_core_sz[pid2]
				then (pid1,pid2)
				else (pid2,pid1)
			)
			|> map (\(pid1,pid2) ->
				let (_,has_neigh)
					= loop (j1, has_neigh1)
					= (0,false)
				while j1<part_core_sz[pid1] && !has_neigh1 do
					let i1 = part_core_is[pid1]+j1
					let pt1 = core_pts[i1]
					let (_,inner_has_neigh)
						= loop (j2,has_neigh2)
						= (0,false)
					while j2<part_core_sz[pid2] && !has_neigh2 do
						let i2 = part_core_is[pid2]+j2
						let pt2 = core_pts[i2]
						let is_neigh = D.check_neighbourhood eps pt1 pt2
						in (j2+1,is_neigh)
					in (j1+1, inner_has_neigh)
				in (pid1,pid2,has_neigh)
			)
			|> filter (.2)
			|> map (\(pid1,pid2,_) -> (i64.min pid1 pid2, i64.max pid1 pid2))
		in core_pairs
			|> get_connected_subgraph_ids np
			|> map2 (\hc i -> if hc then i else (-1)) has_cores
			|> encode_subgraph_ids

	def assign_cluster_ids [n] [np]
		(eps  : t)
		(pts  : [n](vector t))
		(is_core : [n]bool)
		(pids : [n]i64)
		(part_pairs : [](i64,i64))
		(part_cids : [np]i64)
	: [n]i64 =
		-- Get core pt info
		let (core_pts, core_pids, _) = zip3 pts pids is_core
			|> filter (.2)
			|> unzip3
		let part_core_sz = hist_lean (+) 0 np
			core_pids
			(core_pids |> map (\_ -> 1i64))
		let part_core_is = part_core_sz
			|> exscan (+) 0
		-- 1. Assign id's to pts in core parts
		let init_cid = pids
			|> map (\i -> part_cids[i])
		-- 2. Assign to the rest
		let candidate_borders = iota n
			|> filter (\i ->
				part_cids[pids[i]] < 0
			)
		-- keep only border parts' pairs (ie 1 part not core, 1 part core)
		let border_pairs = part_pairs
			|> filter (\(pid1,pid2) ->
				part_cids[pid1]<0 && part_cids[pid2]>=0
			)
		let border_pairs_sz = hist_lean (+) 0 np
			(border_pairs |> map (.0))
			(border_pairs |> map (\_ -> 1i64))
		let border_pairs_is = border_pairs_sz
			|> exscan (+) 0
		-- for each candidate border point
		-- do a loop until 1 core neighbour is found
		-- and take its cid
		let border_ids = candidate_borders
			|> map (\i1 ->
				let pid1 = pids[i1]
				let pt1 = pts[i1]
				let (_,has_neigh)
					= loop (j, cur_neigh)
					= (0,-1)
				while j<border_pairs_sz[pid1] && cur_neigh<0 do
					let pid2 = border_pairs[border_pairs_is[pid1]+j].1
					let (_,has_neigh2)
						= loop (j2, cur_neigh2)
						= (0, -1)
					while j2<part_core_sz[pid2] && cur_neigh2<0 do
						let i2 = part_core_is[pid2]+j2
						let pt2 = core_pts[i2]
						let is_neigh = D.check_neighbourhood eps pt1 pt2
						in if is_neigh
							then (j2+1,part_cids[pid2])
							else (j2+1,-1)
					in (j+1,has_neigh2)
				in has_neigh
			)
		in scatter (copy init_cid) candidate_borders border_ids

}