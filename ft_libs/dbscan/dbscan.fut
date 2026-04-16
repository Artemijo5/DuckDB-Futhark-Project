import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

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

		-- | Holds information about the partition currently being processed.
		type~ dbc_partition = {
			minmax : (vector t, vector t),
			pts : [](vector t),
			is : []i64,
			isCore : []bool,
			isMargin : []bool,
			isTightMargin : []bool,
			chain_id : []i64
		}

		-- | Holds information about the algorithm's progress across partitions.
		type~ dbc_state [part_no] = {
			cur_part_id : i64,
			num_visited : i64,
			is_part_visited : [part_no]bool,
			relevant_parts : []i64,
			offs : i64,
			next_offs : i64,
			cid_offs : i64
		}

		-- | Holds the information for each point.
		type~ dbc_buffer [n] = {
			is_pt_buffered : [n]bool,
			is_pt_flushed  : [n]bool,
			is_core  : [n]bool,
			chain_id : [n]i64
		}

		-- chain collisions are kept as [](i64, i64)
		-- first is the id to be rectified, second is to be rectified to

		type~ flushed_vectors = flushed_t [1] (vector t)
		type~ flushed_data = flushed_t [V.length] t

	-- Initialization

		def times2 : t -> t = (F.*) (F.i32 2)

		def init_dbc_state (part_no : i64) : dbc_state [part_no] = {
			cur_part_id = -1,
			num_visited = 0,
			is_part_visited = replicate part_no false,
			relevant_parts = [],
			offs = 0,
			next_offs = 0,
			cid_offs = 0
		}

		def init_dbc_buffer (pts_no : i64) : dbc_buffer [pts_no] = {
			is_pt_buffered = replicate pts_no false,
			is_pt_flushed  = replicate pts_no false,
			is_core  = replicate pts_no false,
			chain_id = replicate pts_no (-1)
		}

	-- Read next partition

		def next_partition [part_no] (state : dbc_state [part_no]) : i64 =
			if state.num_visited == part_no then (-1) else
			if length state.relevant_parts > 0
			then state.relevant_parts |> head
			else state.is_part_visited
				|> argmin (\iv1 iv2 -> iv2 && !iv1) (==) (true)

		-- get margins & tight margins
		-- assuming non-overlapping partitions, margins are within eps, 2eps, of partition ends
		-- otherwise, we would have to see if they are near another partition
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
			-- Find partitions in the epsilon neighbourhood of the current partition
			let cur_i_neighs = cur_i
				|> D.get_adj_partitions parts_minmax eps
			-- Isolate relevant neighbours
			-- by filtering those that actually have a point of the current partition within eps
			-- (can pass only tight margins here since we assume no overlapping partitions)
			let relevant_neighs = cur_i_neighs
				|> map (\i -> parts_minmax[i])
				|> (\n_parts -> cur_pts
					|> D.get_adj_partitions_withPts n_parts eps (-1)
				)
				|> map (\i -> cur_i_neighs[i])
				-- if partition is already visited, ignore it
				|> filter (\i -> !state.is_part_visited[i])
				-- if partition already relevant, ignore it to avoid multiplicity
				|> filter (\i -> state.relevant_parts |> all (!= i))
			let old_rels = state.relevant_parts
				|> filter (\i -> i!=cur_i)
			in old_rels ++ relevant_neighs

		def postfinal_read_next_partition [part_no] [n]
			(state : dbc_state [part_no])
			(buff  : dbc_buffer [n])
		: (dbc_state [part_no], dbc_partition, dbc_buffer [n]) =
			let next_state = state
				with cur_part_id = (-1)
				with relevant_parts = []
				with offs = n
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
			let next_buff = buff
				with is_pt_buffered = replicate n false
				with is_pt_flushed  = replicate n true
			in (next_state, next_part, next_buff)

		def read_next_partition [part_no] [pts_no]
			(parts_minmax : [part_no](vector t, vector t))
			(parts_is : [part_no]i64)
			(pts : [pts_no](vector t))
			(eps : t)
			(state  : dbc_state [part_no])
			(buffer : *dbc_buffer [pts_no])
		: (dbc_state [part_no], dbc_partition, dbc_buffer [pts_no]) =
			let next_pid = next_partition state
			in if (next_pid<0 || next_pid>=part_no)
				then postfinal_read_next_partition state buffer
				else
			let next_is = I.fetch_partition parts_is pts next_pid
			let next_pts = next_is |> map (\i -> pts[i])
			let n = length next_pts
			let (isM, isTM) = next_pts |> mark_margins parts_minmax[next_pid] eps
			let next_part : dbc_partition = {
				minmax = parts_minmax[next_pid],
				pts = next_pts,
				is = next_is,
				isCore = replicate n false, -- this gets assigned later
				isMargin = isM,
				isTightMargin = isTM,
				chain_id = replicate n (-1)
			}
			-- only use marginal points to find next relevant parts
			let margin_pis = indices next_pts |> zip isTM
				|> filter (.0) |> map (.1)
			let margin_pts = margin_pis |> map (\i -> next_pts[i])
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
			in (next_state, next_part, buffer)

	-- dbscan steps

		-- find & count neighbours

		def num_neighbours_in (eps : t) (pts : [](vector t)) (pt : vector t) : i64 =
			pts |> countFor (D.check_neighbourhood eps pt)

		-- find chains

		def find_chains [n]
			(extPar : i64)
			(eps : t)
			(core_pts : [n](vector t))
		: [n]i64 = core_pts
			|> D.get_neighbour_pairs extPar eps
			|> get_connected_subgraph_ids n

		-- Returns
		-- 1. collisions between precids themselves (these are rectified later)
		-- 2. rectified new cids, based on the precids they collided with
		def mark_chain_collisions [n]
			(old_cids : [n]i64)
			(new_cids : [n]i64)
		: ([](i64,i64), []i64) =
			let old_offs = 1 + (old_cids |> i64.maximum |> i64.max (-1))
			let new_offs = 1 + (new_cids |> i64.maximum |> i64.max (-1))
			let (old_cids', new_cids') = zip old_cids new_cids
				|> filter (\(alt,neu) -> alt >= 0 && neu >= 0) |> unzip
			let connections = get_connected_subgraph_ids_unencoded
				new_offs
				(zip old_cids' new_cids')
			let rectified_new_cids = new_cids |> map (\i -> if i<0 then i else connections[i])
			let old_ccs = iota (old_offs)
				|> zip connections[0 : old_offs]
				|> filter (\(precid,rect_with) -> precid != rect_with)
			in (old_ccs, rectified_new_cids)

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
			-- 1. find cluster ids, ignoring pre_cids for now
			-- these are returned dictionary-encoded
			let cids : [nc]i64 = find_chains extPar eps pts
				|> map (\cid -> if cid<0 then cid else cid + chain_offs)
			-- 2. Rectify cids based on collisions with pre-cids
			-- and record collisions among pre-cids
			let (old_ccs, rectified_cids) = mark_chain_collisions pre_cids cids
			-- 3. Separate cids by pt category and return
			in (rectified_cids[0:np], rectified_cids[np:nc] |> sized nf, old_ccs)

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

		def process_partition_dbscan [part_no] [pts_no]
			(extPar : i64)
			(part  : dbc_partition)
			(state : dbc_state  [part_no])
			(buff  : dbc_buffer [pts_no])
			(all_pts : [pts_no](vector t))
			(eps : t)
			(minPts : i64)
		-- returns updated part, state, buff, chain collisions
		: (dbc_partition, dbc_state [part_no], dbc_buffer [pts_no], [](i64,i64)) =
				let eps2 = times2 eps
				let n = length part.pts
			-- 1. Get the frontier from buffered pts
			-- Separate into tight & loose frontier
				let (tf_is, lf_is) = indices all_pts
					|> zip buff.is_pt_buffered
					|> filter (\(isBuffd,i) -> isBuffd && (
						(D.dist_from_partition part.minmax all_pts[i])
						`leq` eps2
					))
					|> map (.1)
					|> partition (\i -> (D.dist_from_partition part.minmax all_pts[i]) `leq` eps)
				-- 1+. Also separate internal & marginal points
				-- furhter into tight & loose margins
				--
				-- NOTE partition2 is a function that requires care apparently
				-- for partition f g xs
				-- .0 will be xs that satisfy f
				-- .1 will be xs that do not satisfy f, and satisfy g
				-- .2 will be xs that satisfy neither
				let (tm_pis, lm_pis, internal_pis) = zip3 (indices part.pts) (part.isTightMargin) (part.isMargin)
					|> partition2 (.1) (.2)
					|> (\(tm_stuffs, lm_stuffs, in_stuffs) ->
						(map (.0) tm_stuffs, map (.0) lm_stuffs, map (.0) in_stuffs)
					)
			-- gather pts
				let internal_pts = internal_pis |> map (\i -> part.pts[i])
				let lm_pts = lm_pis |> map (\i -> part.pts[i])
				let tm_pts = tm_pis |> map (\i -> part.pts[i])
				let tf_pts = tf_is  |> map (\i -> all_pts[i])
				let lf_pts = lf_is  |> map (\i -> all_pts[i])
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
				let (tf_core_pts, tf_core_precids) = zip3 tf_pts tf_isCore (tf_is |> map (\i -> buff.chain_id[i]))
					|> filter (.1) |> map (\(pt,_,precid) -> (pt,precid)) |> unzip
			-- v. From Loose Frontier (only needed if numPts >= 4)
			-- If numPts<=3, any core point in lf will have given its id to a tf, or be irrelevant
			-- If numPts>=4 && a point in tf has 2 core neighbours in lf,
			-- but becomes core with this new partition, it might be needed to
			-- bridge/collide their chains.
				let lf_isCore =
					let pre_isCore = lf_is |> map (\i -> buff.is_core[i])
					in if minPts>=4 then pre_isCore
						else pre_isCore |> map (\_ -> false)
				let (lf_core_pts, lf_core_precids) = zip3 lf_pts lf_isCore (lf_is |> map (\i -> buff.chain_id[i]))
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
				let tf_cids = assign_chain_ids eps tf_pts (tf_is |> map (\i -> buff.chain_id[i]))
					(tm_core_pts  ++ tf_core_pts  ++ lf_core_pts  |> sized (n_tmc+n_tfc+n_lfc))
					(tm_core_cids ++ tf_core_cids ++ lf_core_cids |> sized (n_tmc+n_tfc+n_lfc))
			-- v. For loose frontier
				let lf_cids = assign_chain_ids eps lf_pts (lf_is |> map (\i -> buff.chain_id[i]))
					frontier_core_pts frontier_core_cids
			-- 5. Update Partition
				let new_part_cids =
					let withInternals = scatter (replicate n (-1)) internal_pis internal_cids
					let withLMs = scatter withInternals lm_pis lm_cids
					let withTMs = scatter withLMs tm_pis tm_cids
					in withTMs
				let new_part_isCore =
					let withInternals = scatter (replicate n false) internal_pis internal_isCore
					let withLMs = scatter withInternals lm_pis lm_isCore
					let withTMs = scatter withLMs tm_pis tm_isCore
					in withTMs
				let new_buff_isCore = scatter (copy buff.is_core) tf_is tf_isCore
				let new_buff_cids =
					let withTFs = scatter (copy buff.chain_id) tf_is tf_cids
					let withLFs = scatter withTFs lf_is lf_cids
					in withLFs
				let new_part = (copy part)
					with isCore = new_part_isCore
					with chain_id = new_part_cids
				let new_buff = (copy buff)
					with is_core = new_buff_isCore
					with chain_id = new_buff_cids
				let new_state = (copy state)
					with cid_offs = i64.max state.cid_offs
						((i64.maximum (partition_core_cids ++ frontier_core_cids))+1)
			in (new_part, new_state, new_buff, new_ccs)

	-- rectify collisions
	-- these are meant to be done at the end

		def mk_rectification_list (chain_offs : i64) (ccs : [](i64,i64))
		: [chain_offs]i64 =
			let (rectify_to, rectify_by) = ccs |> unzip
			in reduce_by_index (iota chain_offs) (i64.min) i64.highest
				rectify_by
				rectify_to

		def rectify_cids [n] (rect_list : []i64) (cur_ids : [n]i64)
		: [n]i64 = cur_ids |> map (\i -> if i<0 then i else rect_list[i])

		def buffer_part_pts [part_no] [pts_no]
			(part  : dbc_partition)
			(state : dbc_state [part_no])
			(buff  : dbc_buffer [pts_no])
			(chain_collisions : [](i64,i64))
		: dbc_buffer [pts_no] =
			-- only flush internal pts of partition
			-- otherwise buffer them
			let n = length part.is
			let rect_list = mk_rectification_list state.cid_offs chain_collisions
			let new_part_cids = part.chain_id |> rectify_cids rect_list
			let new_buff_cids = (copy buff.chain_id) |> rectify_cids rect_list
			let new_is_core = scatter (copy buff.is_core)
				(part.is |> sized n) (part.isCore |> sized n)
			let new_buff_cids' = scatter (copy new_buff_cids)
				(part.is |> sized n) (new_part_cids |> sized n)
			let (is_to_buffer, is_to_flush) = indices part.is
				|> map (\i -> (part.is[i], part.isMargin[i]))
				|> partition (.1)
				|> (\(toBuff, toFlush) -> (toBuff |> map (.0), toFlush |> map (.0)))
			let new_is_buffered = scatter
				(copy buff.is_pt_buffered) is_to_buffer (is_to_buffer |> map (\_ -> true))
			let new_is_flushed = scatter
				(copy buff.is_pt_flushed) is_to_flush (is_to_flush |> map (\_ -> true))
			in {
				is_pt_buffered = new_is_buffered,
				is_pt_flushed  = new_is_flushed,
				chain_id = new_buff_cids',
				is_core = new_is_core
			}

	-- dbscan pipeline

		-- TODO interpreter doesn't like loop sized (...) why?
		-- 1 way to solve would be to keep both buffered & flushed chain_id, isCore in n-sized array (...)
		-- would require care to ensure only already read pts are going there
		-- ... might require to rewrite previous funcs to follow this logic as well...
		def internal_dbscan [part_no] [n]
			(extPar : i64)
			(eps : t)
			(minPts : i64)
			(partitions : [part_no](vector t, vector t))
			(part_is : [part_no]i64)
			(pts : [n](vector t))
		: dbc_buffer [n] =
			let state0 = init_dbc_state part_no
			let buffer0 = init_dbc_buffer n
			let (_,final_buff) = loop (state, buffer) = (state0, buffer0)
			for j<(part_no+1) do
				let (this_state, this_part, this_buff) = read_next_partition
						partitions part_is pts eps state (copy buffer)
				let (upd_part, upd_state, upd_buff_, ccs) = process_partition_dbscan
					extPar this_part this_state this_buff pts eps minPts
				let upd_buff = buffer_part_pts upd_part upd_state upd_buff_ ccs
				in (upd_state, upd_buff)
			in final_buff

}