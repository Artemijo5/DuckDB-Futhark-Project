import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"

type~ flushed_t [dim] 't = {
	n : i64,
	pts : [dim][]t,
	isCore : []bool,
	chain_id : []i64
}

module ft_dbscan
	(V : vector)
	(F : real)
	(I : spatial_index with vector 'a = V.vector a with t = F.t)
	(D : distance with vector 'a = V.vector a with t = F.t)
= {
	type t = F.t
	type vector 'a = V.vector a

	-- Auxilliary Types & Values

		def lowest = F.lowest
		def highest = F.highest

		type~ dbc_partition = {
			minmax : (vector t, vector t),
			pts : [](vector t),
			isCore : []bool,
			isMargin : []bool,
			isTightMargin : []bool,
			chain_id : []i64
		}

		type~ dbc_state [part_no] = {
			cur_part_id : i64,
			num_visited : i64,
			is_part_visited : [part_no]bool,
			relevant_parts : []i64,
			offs : i64,
			next_offs : i64,
			cid_offs : i64
		}

		type~ dbc_buffer = {
			pts : [](vector t),
			isCore : []bool,
			chain_id : []i64
		}

		-- chain collisions are kept as [](i64, i64)
		-- first is the id to be rectified, second is to be rectified to

		type~ flushed_vectors = flushed_t [1] (vector t)
		type~ flushed_data = flushed_t [V.length] t

	-- Initialization

		def times2 : t -> t = (F.*) (F.i32 2)

		def init_dbc_buffer : dbc_buffer = {pts=[],isCore=[],chain_id=[]}

		def init_dbc_state (part_no : i64) : dbc_state [part_no] = {
			cur_part_id = -1,
			num_visited = 0,
			is_part_visited = replicate part_no false,
			relevant_parts = [],
			offs = 0,
			next_offs = 0,
			cid_offs = 0
		}

	-- Read next partition

		def next_partition [part_no] (state : dbc_state [part_no]) : i64 =
			if state.num_visited == part_no then (-1) else
			if length state.relevant_parts > 0
			then state.relevant_parts |> head
			else state.is_part_visited
				|> argmin (\iv1 iv2 -> iv2 && !iv1) (==) (true)

		-- get margins & tight margins
		def mark_margins [n]
			(part_minmax : (vector t, vector t))
			(eps : t)
			(part_pts : [n](vector t))
		: ([n]bool, [n]bool) =
			let eps2 = times2 eps
			let isMargin = part_pts |> map (D.is_marginal part_minmax eps2)
			let isTightMargin = part_pts |> map (D.is_marginal part_minmax eps)
			in (isMargin, isTightMargin)

		-- next relevant partitions:
		-- partitions adjacent to the current one
		-- that are not already relevant
		-- and are not already visited
		def next_relevant_partitions [part_no]
			(parts_minmax : [part_no](vector t, vector t))
			(eps : t)
			(cur_i : i64)
			(cur_pts : [](vector t))
			(state : dbc_state [part_no])
		: []i64 =
			let cur_i_neighs = cur_pts
				|> D.get_adj_partitions parts_minmax eps cur_i
				|> filter (\i -> i!=cur_i && (state.relevant_parts |> all (!= i)))
			let old_rels = state.relevant_parts
				|> filter (\i -> i!=cur_i)
			in old_rels ++ cur_i_neighs

		def postfinal_read_next_partition [part_no]
			(total_num_pts : i64)
			(state : dbc_state [part_no])
			(buffer: dbc_buffer)
		: (dbc_state [part_no], dbc_partition, dbc_buffer, dbc_buffer) =
			let next_state = state
				with cur_part_id = (-1)
				with relevant_parts = []
				with offs = total_num_pts
				with next_offs = (-1)
			let next_part = {
				minmax = (V.replicate highest, V.replicate lowest),
				pts = [],
				isCore = [],
				isMargin = [],
				isTightMargin = [],
				chain_id = []
			}
			let next_buffer : dbc_buffer = {
				pts = [], isCore = [], chain_id = []
			}
			let flushed = buffer
			in (next_state, next_part, next_buffer, flushed)

		def read_next_partition [part_no]
			(parts_minmax : [part_no](vector t, vector t))
			(parts_is : [part_no]i64)
			(pts : [](vector t))
			(eps : t)
			(state : dbc_state [part_no])
			(buffer : dbc_buffer)
		: (dbc_state [part_no], dbc_partition, dbc_buffer, dbc_buffer) =
			-- TODO handle case of last partition (next_pid==part_no)
			let next_pid = next_partition state
			in if (next_pid<0 || next_pid>=part_no)
				then postfinal_read_next_partition (length pts) state buffer
				else
			let next_pts = I.fetch_partition parts_is pts next_pid
			let n = length next_pts
			let (isM, isTM) = next_pts |> mark_margins parts_minmax[next_pid] eps
			let next_part : dbc_partition = {
				minmax = parts_minmax[next_pid],
				pts = next_pts,
				isCore = replicate n false, -- this gets assigned later
				isMargin = isM,
				isTightMargin = isTM,
				chain_id = replicate n (-1)
			}
			-- only use marginal points to find next relevant parts
			let margin_pts = next_pts |> zip isTM
				|> filter (.0) |> map (.1)
			let new_relevants = state
				|> next_relevant_partitions parts_minmax eps next_pid margin_pts
			let next_state : dbc_state [part_no] = {
				cur_part_id = next_pid,
				num_visited = state.num_visited+1,
				is_part_visited = (copy state.is_part_visited) with [next_pid] = true,
				relevant_parts = new_relevants,
				offs = state.next_offs,
				next_offs = state.next_offs + n,
				cid_offs = state.cid_offs
			}
			-- update buffer
			-- only keep points that are close to some relevant partition
			let eps2 = times2 eps
			let (buff_pts, flush_pts) =
				zip3 buffer.pts buffer.isCore buffer.chain_id
				|> partition (\(pt,_,_) -> 
					let rel_parts = new_relevants |> map (\i -> parts_minmax[i])
					let count_close = pt |> D.get_num_adj_partitions rel_parts eps2 (-1)
					in count_close>0
				)
			let next_buffer : dbc_buffer = {
				pts = buff_pts |> map (.0),
				isCore = buff_pts |> map (.1),
				chain_id = buff_pts |> map (.2)
			}
			let flushed_pts : dbc_buffer = {
				pts = flush_pts |> map (.0),
				isCore = flush_pts |> map (.1),
				chain_id = buff_pts |> map (.2)
			}
			in (next_state, next_part, next_buffer, flushed_pts)

	-- dbscan steps

		-- find & count neighbours

		def num_neighbours_in (eps : t) (pts : [](vector t)) (pt : vector t) : i64 =
			pts |> countFor (D.check_neighbourhood eps pt)

		-- Implicit neighbourhood graph via contiguous array
		-- neighbours of the same pts1 point in pts2 are grouped together
		-- a second list hold the first index matching to each pts1 entry
		-- NOTE: this allocates quadratic space, even if the final graph is small
		-- to prevent this from crashing, need to ensure #points is capped 
		def mk_neighbourhood_graph [n1] (eps : t) (pts1 : [n1](vector t)) (pts2 : [](vector t))
		: ([]i64, [n1]i64) =
			let neighs = pts1 |> map (\pt -> pts2
				|> map (D.check_neighbourhood eps pt)
				|> zip (indices pts2)
			) |> flatten |> filter (.1) |> map (.0)
			let is = pts1 |> map (num_neighbours_in eps pts2)
				|> exscan (+) 0
			in (neighs, is)

		def mk_neighbourhood_graph_iterative [n]
			(extPar : i64)
			(eps : t)
			(core_pts : [n](vector t))
		: ([]i64, [n]i64) =
			let num_iter = (n + extPar - 1) / extPar
			let (neigh, neigh_is) : ([]i64, []i64)
			= loop (ns, nis) = ([],[]) for j < num_iter do
				let inf = j*extPar
				let sup = i64.min n (inf+extPar)
				let this_pts = core_pts[inf:sup]
				let (this_ns, this_nis) = mk_neighbourhood_graph
					eps this_pts core_pts
				in (ns ++ this_ns, nis ++ (this_nis |> map (\i -> i+(length ns))))
			in (neigh, neigh_is :> [n]i64)

		-- find chains

		def find_chains_compact [n]
			(extPar : i64)
			(eps : t)
			(core_pts : [n](vector t))
		: [n]i64 =
			let (neigh, n_is) = mk_neighbourhood_graph_iterative
				extPar eps core_pts
			let ndn = length neigh
			let segment_flags = scatter (replicate n false) n_is (replicate n true)
			-- Use adjacency list to identify connected subgraphs
			-- pivot used is the smallest-index member of each subgraph
			let chs : [n]i64 =
				-- Iterate over neigh
				-- assign each neigh to its smallest neighbour
				-- until convergence
				let (_,itered_neighlist) : ([ndn]i64, [ndn]i64) =
				loop (old_list,new_list) = (replicate ndn (-1), neigh :> [ndn]i64)
				while any (id) (map2 (\alt neu -> alt != neu) old_list new_list) do
					let current_min_neighbours = segmented_reduce
						(\n1 n2 -> if n1<n2 then n1 else n2) (i64.highest)
						segment_flags new_list
					let newer_list = new_list
						|> map (\i -> current_min_neighbours[i])
					in (new_list, newer_list)
				in segmented_reduce (\n1 n2 -> if n1<n2 then n1 else n2) (i64.highest)
					segment_flags (itered_neighlist :> [ndn]i64)
					|> sized n
			in chs


		def connect_chains [np] [nf]
			(extPar : i64)
			(eps : t)
			(chain_offs: i64)
			(part_cpts : [np](vector t))
			(buff_cpts : [nf](vector t))
			(buff_cids : [nf]i64)
		-- returns cids + chain collisions
		: ([np]i64, [nf]i64, [](i64,i64)) =
			-- 0. put into same list
			let nc = np+nf
			let pts = part_cpts ++ buff_cpts |> sized nc
			let pre_cids = (replicate np (-1)) ++ buff_cids |> sized nc
			-- 1. find cluster heads, ignoring pre_cids for now
			let chs : [nc]i64 = find_chains_compact extPar eps pts
			-- 2. Dictionary Encoding
			let ch_ids_ = chs |> zip (indices chs)
				|> map (\(ch,i) -> ch==i)
				|> map (i64.bool)
				|> exscan (+) 0
			let local_cids_unoffset = chs
				|> map (\hi -> ch_ids_[hi])
			let num_cids = (i64.maximum local_cids) + 1
			let local_cids : [n]i64 = local_cids_unoffset
				|> map (\i -> i + chain_offs)
			-- 3. Collide with pre-cids
			let cols_withMult = pre_cids |> zip local_cids_unoffset
				|> filter (\(_,pre_cid) -> pre_cid>=0)
				|> unzip
				|> (\(loc,pre) -> bucket_sort 2 num_cids loc pre)
				|> (\(loc,pre) -> zip loc pre)
				|> map (\(loc,pre) -> (loc + chain_offs, pre))
			let cols_distinct = cols_withMult
				|> group_boundaries (\(loc1,_) (loc2,_) -> loc1!=loc2)
				|> zip cols_withMult
				|> filter (.1)
				|> map (.0)
			let rectified_cids = local_cids
				|> bsearch_last (>=) (<) (replicate nc 0) cols_distinct
				|> zip local_cids
				|> map (\(loc,ri) -> if ri<0 || cols_distinct[ri].0 != loc
					then loc else cols_distinct[ri].1
				)
			let cids_part = rectified_cids[0:np] |> sized np
			let cids_buff = rectified_cids[np:nc] |> sized nf
			-- 4. save collisions between pre-cids
			let cols_prev = cols_distinct
				|> filter (\(loc,_) -> loc<chain_offs)
			in (cids_part, cids_buff, cols_prev)








		-- assign chain ids

		-- rectify collisions


	-- dbscan processing


		

}