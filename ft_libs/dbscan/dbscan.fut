import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"

type~ flushed_t [dim] 't = {
	n : i64,
	pts : [dim][]t,
	is : []i64,
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

		def leq = (F.<=)
		def lt  = (F.<)

		def lowest = F.lowest
		def highest = F.highest

		def zero = F.i32 0
		def minus_one = F.i32 (-1)

		type~ dbc_partition = {
			minmax : (vector t, vector t),
			pts : [](vector t),
			is : []i64,
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
			is : []i64,
			isCore : []bool,
			chain_id : []i64
		}

		-- chain collisions are kept as [](i64, i64)
		-- first is the id to be rectified, second is to be rectified to

		type~ flushed_vectors = flushed_t [1] (vector t)
		type~ flushed_data = flushed_t [V.length] t

	-- Initialization

		def times2 : t -> t = (F.*) (F.i32 2)

		def init_dbc_buffer : dbc_buffer = {pts=[],is=[],isCore=[],chain_id=[]}

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
				is = [],
				isCore = [],
				isMargin = [],
				isTightMargin = [],
				chain_id = []
			}
			let next_buffer : dbc_buffer = {
				pts = [], is = [], isCore = [], chain_id = []
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
				is = indices pts |> map (\i -> i + parts_is[next_pid]),
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
				zip4 buffer.pts buffer.is buffer.isCore buffer.chain_id
				|> partition (\(pt,_,_,_) -> 
					let rel_parts = new_relevants |> map (\i -> parts_minmax[i])
					let count_close = pt |> D.get_num_adj_partitions rel_parts eps2 (-1)
					in count_close>0
				)
			let next_buffer : dbc_buffer = {
				pts = buff_pts |> map (.0),
				is = buff_pts |> map (.1),
				isCore = buff_pts |> map (.2),
				chain_id = buff_pts |> map (.3)
			}
			let flushed_pts : dbc_buffer = {
				pts = flush_pts |> map (.0),
				is = flush_pts |> map (.1),
				isCore = flush_pts |> map (.2),
				chain_id = buff_pts |> map (.3)
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
			let segment_flags = scatter (replicate ndn false) n_is (replicate n true)
			-- Use adjacency list to identify connected subgraphs
			-- pivot used is the smallest-index member of each subgraph
			--
			-- Iterate over neigh
			-- assign each neigh to its smallest neighbour
			-- until convergence
			let (_,itered_neighlist) : ([ndn]i64, [ndn]i64) =
				loop (old_list,new_list) = (replicate ndn (-1), neigh :> [ndn]i64)
				while any (id) (map2 (!=) old_list new_list) do
					let current_min_neighbours = segmented_reduce
						(\n1 n2 -> i64.min n1 n2) (i64.highest)
						segment_flags new_list
					let newer_list = new_list
						|> map (\i -> current_min_neighbours[i])
					in (new_list, newer_list)
			let chs = segmented_reduce (\n1 n2 -> i64.min n1 n2) (i64.highest)
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
			-- If eps<0, just assign all 0 cid
			if (eps `lt` zero)
				then (replicate np 0, replicate nf 0, [])
				else
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
			let num_cids = (i64.maximum local_cids_unoffset) + 1
			let local_cids : [nc]i64 = local_cids_unoffset
				|> map (\i -> i + chain_offs)
			-- 3. Collide with pre-cids
			let cols_withMult = pre_cids |> zip local_cids_unoffset
				|> filter (\(_,pre_cid) -> pre_cid>=0)
				|> unzip
				|> (\(loc,pre) -> bucket_sort 2 num_cids pre loc)
				|> (\(pre,loc) -> bucket_sort 2 num_cids loc pre)
				|> (\(loc,pre) -> zip loc pre)
				|> map (\(loc,pre) -> (loc + chain_offs, pre))
			let cols_distinct = cols_withMult
				|> group_boundaries (\(loc1,_) (loc2,_) -> loc1!=loc2)
				|> zip cols_withMult
				|> filter (.1)
				|> map (.0)
			let rectified_cids = local_cids
				|> bsearch_last (>=) (<) (replicate nc 0) (cols_distinct |> map (.0))
				|> zip local_cids
				|> map (\(loc,ri) -> if ri<0 || cols_distinct[ri].0 != loc
					then loc else cols_distinct[ri].1
				)
			let cids_part = rectified_cids[0:np] |> sized np
			let cids_buff = rectified_cids[np:nc] |> sized nf
			-- 4. save collisions between pre-cids
			let cols_prev_mult = cols_withMult
				|> map (\(loc,pre) -> (rectified_cids[loc],pre))
				|> filter (\(loc,pre) -> loc<chain_offs && loc!=pre)
			let cols_prev = cols_prev_mult
				|> group_boundaries (\(l1,p1) (l2,p2) -> l1!=l2 || p1!=p2)
				|> zip cols_prev_mult |> filter (.1) |> map (.0)
				-- TODO this might be necessary to ensure both collided chains see each other
				|> expand (\_ -> 2)
					(\(loc,pre) ind -> if ind==0 then (loc,pre) else (pre,loc))
			--	|> map (\(loc,pre) -> (pre,loc)) -- rectified goes 2nd
			-- Return
			in (cids_part, cids_buff, cols_prev)

		-- assign chain ids

		def assign_chain_ids [n] [nc]
			(eps : t)
			(pts : [n](vector t))
			(pre_cids : [n]i64)
			(core_pts : [nc](vector t))
			(core_cids : [nc]i64)
		: [n]i64 = pts
			|> map (D.find_closest_within eps core_pts)
			|> map2 (\alt i -> if i>=nc || i<0 then alt else core_cids[i])
				pre_cids

	-- process a partition

		def process_partition_dbscan [part_no]
			(extPar : i64)
			(part : dbc_partition)
			(state: dbc_state [part_no])
			(buffs: dbc_buffer)
			(chain_cols : [](i64,i64))
			(eps : t)
			(minPts : i64)
		-- returns updated part, state, buffs, and chain_cols
		: (dbc_partition, dbc_state [part_no], dbc_buffer, [](i64,i64)) =
				let eps2 = times2 eps
				let n = length part.pts
			-- 1. Get the frontier from buffered pts
			-- Separate into tight & loose frontier
				let (tf_info, lf_info) = zip4 buffs.pts buffs.isCore buffs.chain_id (indices buffs.pts)
					|> filter (\(pt,_,_,_) -> (D.dist_from_partition part.minmax pt) `leq` eps2)
					|> partition (\(pt,_,_,_) -> (D.dist_from_partition part.minmax pt) `leq` eps)
				-- 1+. Also separate marginal points
				let (margin_info, internal_info) = zip4
					(part.pts |> sized n)
					(part.isMargin |> sized n)
					(part.isTightMargin |> sized n)
					(iota n)
				|> partition (.1)
				|> (\(mgs,ins) -> (
					mgs |> map (\(pt,_,isTM,i) -> (pt,isTM,i)),
					ins |> map (\(pt,_,_,i) -> (pt,i))
				))
				let (tm_info, lm_info) = margin_info
					|> partition (.1)
					|> (\(tms,lms) -> (
						tms |> map (\(pt,_,i)->(pt,i)),
						lms |> map (\(pt,_,i)->(pt,i))
					))
			-- separate pts
				let internal_pts = internal_info |> map (.0)
				let lm_pts = lm_info |> map (.0)
				let tm_pts = tm_info |> map (.0)
				let tf_pts = tf_info |> map (.0)
				let lf_pts = lf_info |> map (.0)
			-- 2. Get core points from different areas
			-- i. From Internal Points
				let internal_nc1 = internal_pts |> map (num_neighbours_in eps internal_pts)
				let internal_nc2 = internal_pts |> map (num_neighbours_in eps lm_pts)
				let internal_nc = map2 (+) internal_nc1 internal_nc2
				let internal_isCore = internal_nc |> map (\nc -> nc>=minPts)
				let internal_core_pts = zip internal_pts internal_isCore
				|> filter (.1) |> map (.0)
			-- ii. From Loose Margin
				let lm_nc1 = lm_pts |> map (num_neighbours_in eps internal_pts)
				let lm_nc2 = lm_pts |> map (num_neighbours_in eps lm_pts)
				let lm_nc3 = lm_pts |> map (num_neighbours_in eps tm_pts)
				let lm_nc = map3 (\nc1 nc2 nc3 -> nc1+nc2+nc3) lm_nc1 lm_nc2 lm_nc3
				let lm_isCore = lm_nc |> map (\nc -> nc>=minPts)
				let lm_core_pts = zip lm_pts lm_isCore
				|> filter (.1) |> map (.0)
			-- iii. From Tight Margin
				let tm_nc1 = tm_pts |> map (num_neighbours_in eps lm_pts)
				let tm_nc2 = tm_pts |> map (num_neighbours_in eps tm_pts)
				let tm_nc3 = tm_pts |> map (num_neighbours_in eps tf_pts)
				let tm_nc = map3 (\nc1 nc2 nc3 -> nc1+nc2+nc3) tm_nc1 tm_nc2 tm_nc3
				let tm_isCore = tm_nc |> map (\nc -> nc>=minPts)
				let tm_core_pts = zip tm_pts tm_isCore
					|> filter (.1) |> map (.0)
			-- iv. From Tight Frontier
				let tf_nc1 = tf_pts |> map (num_neighbours_in eps tm_pts)
				let tf_nc2 = tf_pts |> map (num_neighbours_in eps tf_pts)
				let tf_nc3 = tf_pts |> map (num_neighbours_in eps lf_pts)
				let tf_nc = map3 (\nc1 nc2 nc3 -> nc1+nc2+nc3) tf_nc1 tf_nc2 tf_nc3
				let tf_isCore = tf_nc |> map (\nc -> nc>=minPts)
				let (tf_core_pts, tf_core_precids) = zip3 tf_pts tf_isCore (tf_info |> map (.2))
					|> filter (.1) |> map (\(pt,_,precid) -> (pt,precid)) |> unzip
			-- v. From Loose Frontier (only needed if numPts >= 4)
			-- If numPts<=3, any core point in lf will have given its id to a tf, or be irrelevant
			-- If numPts>=4 && a point in tf has 2 core neighbours in lf,
			-- but becomes core with this new partition, it might be needed to
			-- bridge/collide their chains.
				let lf_isCore =
					let pre_isCore = lf_info |> map (.1)
					in if minPts>=4 then pre_isCore
						else pre_isCore |> map (\_ -> false)
				let (lf_core_pts, lf_core_precids) = zip3 lf_pts lf_isCore (lf_info |> map (.2))
					|> filter (.1) |> map (\(pt,_,precid) -> (pt,precid)) |> unzip
			-- connect partition core pts
			-- first internal, then loose margin, then tight margin
				let n_ipc = length internal_core_pts
				let n_lmc = length lm_core_pts
				let n_tmc = length tm_core_pts
				let partition_core_pts = internal_core_pts
					++ lm_core_pts ++ tm_core_pts
			-- connect frontier core pts
			-- first tight, then loose
				let n_tfc = length tf_core_pts
				let n_lfc = length lf_core_pts
				let (frontier_core_pts, frontier_precids) =
					(zip tf_core_pts tf_core_precids) ++ (zip lf_core_pts lf_core_precids)
					|> unzip
			-- 3. Find chain id's for those pts
				let (partition_core_cids, frontier_core_cids, new_ccs) = connect_chains
					extPar eps state.cid_offs partition_core_pts frontier_core_pts frontier_precids
			-- Separate chain id's by area
				let internal_core_cids = partition_core_cids[0:n_ipc]
				let lm_core_cids = partition_core_cids[n_ipc:n_ipc+n_lmc]
				let tm_core_cids = partition_core_cids[n_ipc+n_lmc:n_ipc+n_lmc+n_tmc]
				let tf_core_cids = frontier_core_cids[0:n_tfc]
				let lf_core_cids = frontier_core_cids[n_tfc:n_tfc+n_lfc]
			-- 4. Assign chain id's to partition's own points
			-- i. For internal pts
				let internal_cids = assign_chain_ids eps internal_pts (internal_pts |> map (\_ -> (-1)))
					(internal_core_pts  ++ lm_core_pts  |> sized (n_ipc+n_lmc))
					(internal_core_cids ++ lm_core_cids |> sized (n_ipc+n_lmc))
			-- ii. For loose margins
				let lm_cids = assign_chain_ids eps lm_pts (lm_pts |> map (\_ -> (-1)))
					partition_core_pts partition_core_cids
			-- iii. For tight margins
				let tm_cids = assign_chain_ids eps tm_pts (tm_pts |> map (\_ -> (-1)))
					(lm_core_pts  ++ tm_core_pts  ++ tf_core_pts  |> sized (n_lmc+n_tmc+n_tfc))
					(lm_core_cids ++ tm_core_cids ++ tf_core_cids |> sized (n_lmc+n_tmc+n_tfc))
			-- iv. For tight frontier
				let tf_cids = assign_chain_ids eps tf_pts (tf_pts |> map (\_ -> (-1)))
					(tm_core_pts  ++ tf_core_pts  ++ lf_core_pts  |> sized (n_tmc+n_tfc+n_lfc))
					(tm_core_cids ++ tf_core_cids ++ lf_core_cids |> sized (n_tmc+n_tfc+n_lfc))
			-- v. For loose frontier
				let lf_cids = assign_chain_ids eps lf_pts (lf_pts |> map (\_ -> (-1)))
					frontier_core_pts frontier_core_cids
			-- 5. Update & return
				let new_part_cids =
					let withInternals = scatter (replicate n (-1)) (internal_info |> map (.1)) internal_cids
					let withLMs = scatter withInternals (lm_info |> map (.1)) lm_cids
					let withTMs = scatter withLMs (tm_info |> map (.1)) tm_cids
					in withTMs
				let new_part_isCore =
					let withInternals = scatter (replicate n false) (internal_info |> map (.1)) internal_isCore
					let withLMs = scatter withInternals (lm_info |> map (.1)) lm_isCore
					let withTMs = scatter withLMs (tm_info |> map (.1)) tm_isCore
					in withTMs
				let new_buff_isCore = scatter (copy buffs.isCore) (tf_info |> map (.3)) tf_isCore
				let new_buff_cids =
					let withTFs = scatter (copy buffs.chain_id) (tf_info |> map (.3)) tf_cids
					let withLFs = scatter withTFs (lf_info |> map (.3)) lf_cids
					in withLFs
				let new_part = (copy part)
					with isCore = new_part_isCore
					with chain_id = new_part_cids
				let new_buff = (copy buffs)
					with isCore = new_buff_isCore
					with chain_id = new_buff_cids
				let new_state = (copy state)
					with cid_offs = i64.max state.cid_offs
						((i64.maximum (partition_core_cids ++ frontier_core_cids))+1)
			in (new_part, new_state, new_buff, chain_cols ++ new_ccs)

		-- Append all of the points in the partition to the buffer.
		-- Non-frontier points will be flushed when the next partition is read.
		def flush_part_pts
			(part : dbc_partition)
			(buff : dbc_buffer)
		: dbc_buffer = {
			pts = buff.pts ++ part.pts,
			is  = buff.is  ++ part.is,
			isCore = buff.isCore ++ part.isCore,
			chain_id = buff.chain_id ++ part.chain_id
		}

	-- rectify collisions
	-- these are meant to be done at the end

		-- returns, for each current chain id, the pivot to replace it with
		def mk_rectification_list (chain_offs : i64) (chain_cols : [](i64,i64))
		: [chain_offs]i64 =
			-- Use histogram to perform BFS on collisions graph
			-- so as to identify connected subgraphs
			let cols_graph = chain_cols |> map (.1)
			let cols_his = chain_cols |> map (.0)
			let cols_graph_iter = let (_,converged) =
				loop (old_list,new_list) = (cols_graph |> map (\_ -> (-1)), cols_graph)
				while any (id) (map2 (!=) old_list new_list) do
					let current_min_collisions = reduce_by_index (iota chain_offs)
						(\c1 c2 -> i64.min c1 c2) i64.highest
						cols_his new_list
					let newer_list = new_list
						|> map (\i -> current_min_collisions[i])
					in (new_list, newer_list)
				in converged
			let cc_pivots = reduce_by_index (iota chain_offs)
				(\c1 c2 -> i64.min c1 c2) i64.highest
				cols_his cols_graph_iter
			in cc_pivots

		def rectify_chain_ids [n] (rect_list : []i64) (cur_ids : [n]i64)
		: [n]i64 = cur_ids |> map (\i -> rect_list[i])


	-- TODO pipeline, dbscan* & category separation (...)
			
		

}