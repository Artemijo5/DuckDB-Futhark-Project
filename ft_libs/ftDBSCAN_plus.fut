import "ftbasics"

type distType = #euclidean | #haversine -- TODO hsv distance ? for Mahalanobis transform data externally
type anglType = #degrees | #radians

type dbcBase_t 'f = {
	mins : [2]f,
	maxs : [2]f,
	step_per_dim : [2]f,
	part_per_dim : [2]i64,
	total_partitions : i64,
	dist_t : distType,
	angle_t : anglType,
	radius : f,
	eps : f,
	minPts : i64,
	m_size : i64
}
type dbcPartition_t [n] 'f = {
	part_id : i64,
	min_coord : [2]f,
	max_coord: [2]f,
	inner_min_coord : [2]f, -- bottom left corner within 2*eps
	inner_max_coord : [2]f, -- top right corner within 2*eps
	neighbours : [8]i64, -- stored in counter-clockwise order, starting from left
	isNeighbourVisited : [8]bool,
	dat : [n](f,f),
	isCore : [n]bool,
	isMargin : [n]bool,
	isTightMargin : [n]bool,
	chain_ids : [n]i64
}
type~ dbcProcessor_t [part_no] 'f = {
	cur_part_id : i64,
	cur_part_order : i64,
	isPartVisited : [part_no]bool,
	relevantParts : []i64,
	buffered_pts : [](f,f),
	buff_isCore : []bool,
	buffered_ids : []i64,
	offs : i64,
	next_offs : i64,
	chainOffs : i64
}

type~ dbcHandler_t [n] [part_no] 'f = {
	clBase : dbcBase_t f,
	clProc : dbcProcessor_t [part_no] f,
	current_partition : dbcPartition_t [n] f,
	toFlush_dat : [](f,f),
	toFlush_ids : []i64,
	chain_collisions : [](i64,i64) -- TODO figure out exactly how to handle, prob in 2nd pass
}

module dbscan_plus (F : real) = {
	-- Types and Basic Ops
		type t = F.t

		type dbcBase = dbcBase_t t
		type dbcPartition [n] = dbcPartition_t [n] t
		type~ dbcProcessor [part_no] = dbcProcessor_t [part_no] t
		type~ dbcHandler [n] [part_no] = dbcHandler_t [n] [part_no] t

		local def gt = (F.>)
		local def lt = (F.<)
		local def geq = (F.>=)
		local def leq = (F.<=)
		local def eq = (F.==)

		local def plus = (F.+)
		local def minus = (F.-)
		local def times = (F.*)
		local def over = (F./)
		local def ceil = (F.ceil)

		local def from_i64 = (F.i64)
		local def to_i64 = (F.to_i64)

		local def zero = F.i32 0
		local def one = F.i32 1
		local def two = F.i32 2
		local def lowest = F.lowest
		local def highest = F.highest

		local def sqrt = (F.sqrt)
		local def hypot = (F.hypot)

	-- Angular Ops
		local def pi = (F.pi)
		local def oneEighty = F.i32 180

		local def cos = (F.cos)
		local def sin = (F.sin)
		local def acos = (F.acos)

		local def mid_angle (x1 : t) (x2 : t) = over (plus x1 x2) two

		-- formula from Widipedia
		-- TODO cite properly
		local def hav_theta (pt1 : (f,f)) (pt2: (f,f))
		: t =
			let n01 = cos (minus pt1.0 pt2.0) |> minus one
			let n2_ = times (cos pt1.0) (cos pt2.0)
			let n2 = minus pt1.1 pt2.1 |> minus one |> times n2_ 
			in mid_angle n01 n2

		local def deg_to_rad (x:t) : t = over (times x pi) oneEighty
		local def rad_to_deg (x:t) : t = over (times x oneEighty) pi

	-- Distance Ops
		-- TODO cite properly
		--local def earth_radius : t = F.f64 6161.6 -- average earth radius

		local def d_euclidean (pt1 : (f,f)) (pt2 : (f,f))
		: t = hypot (minus pt1.0 pt2.0) (minus pt1.1 pt2.1)

		local def d_haversine (aType : anglType) (r: f) (pt1 : (f,f)) (pt2 : (f,f))
		: t =
			let (p1,p2) : ((f,f),(f,f)) =
				match aType
				case #radians -> (pt1, pt2)
				case #degrees -> (
					(deg_to_rad pt1.0, deg_to_rad pt1.1),
					(deg_to_rad pt2.0, deg_to_rad pt2.1)
				)
			let h_theta = F.min (hav_theta p1 p2) one
			let theta = times h_theta two |> minus one |> acos
			in times theta r

		local def dist (dType : distType) (aType : anglType) (r : f) (pt1 : (f,f)) (pt2 : (f,f))
		: t =
			match dType
			case #euclidean -> d_euclidean pt1 pt2
			case #haversine -> d_haversine aType r pt1 pt2

	-- Initializations & Continuation Info
		local def mk_dbcBase
			(min_x : t) (min_y : t) (max_x : t) (max_y : t)
			(parts_x : i64) (parts_y : i64)
			(dist_type : distType) (angle_type : anglType) (radius : f)
			(eps : t) (minPts : i64)
			(m_size : i64)
		: dbcBase =
			let min_pt = [min_x, min_y]
			let max_pt = [max_x, max_y]
			let steps_x = parts_x |> from_i64 |> over (minus max_x min_x)
			let steps_y = parts_y |> from_i64 |> over (minus max_y min_y)
			let steps_pD = [steps_x, steps_y]
			let parts_pD = [parts_x, parts_y]
			in {
				mins = min_pt,
				maxs = max_pt,
				step_per_dim = steps_pD,
				part_per_dim = parts_pD,
				total_partitions = parts_x * parts_y,
				dist_t = dist_type,
				angle_t = angle_type,
				radius = radius,
				eps = eps,
				mintPts = minPts,
				m_size = m_size
			}

		local def mk_dbcProc (clBase : dbcBase)
		: dbcProcessor [clBase.total_partitions] =
			{
				cur_part_id = -1,
				cur_part_order = -1,
				isPartVisited = replicate clBase.total_partitions false,
				relevantParts = [],
				buffered_pts = [],
				buff_isCore = [],
				buffered_ids = [],
				offs = -1,
				next_offs = 0,
				chainOffs = 0,
			}

		def mk_dbcHandler 
			(min_x : t) (min_y : t) (max_x : t) (max_y : t)
			(parts_x : i64) (parts_y : i64)
			(dist_type : distType) (angle_type : anglType) (radius : f)
			(eps : t) (minPts : i64)
			(m_size : i64)
		: dbcHandler [0] [parts_x * parts_y] =
			let clBase = mk_dbcBase
				min_x min_y max_x max_y
				parts_x parts_y
				dist_type angle_type radius
				eps minPts
				m_size
			let clProc = mk_dbcProc clBase
			let dummy_partition : dbcPartition [0] =
				{
					part_id = -1,
					min_coord = replicate 2 highest,
					max_coord = replicate 2 lowest,
					inner_min_coord = replicate 2 highest,
					inner_max_coord = replicate 2 lowest,
					neighbours = replicate 8 (-1),
					isNeighbourVisited = replicate 8 false,
					dat = [],
					isCore = [],
					isMargin = [],
					isTightMargin = [],
					chain_ids = []
				}
			in {
				clBase = clBase,
				clProc = clProc,
				current_partition = dummy_partition,
				toFlush_dat = [],
				toFlush_ids = [],
				chain_collisions = []
			}

		def next_partition_to_read [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
		: i64 =
			if clHandler.clProc.cur_part_order==part_no-1 then (-1) else
			-- first use relevant partitions for breadth-first traversal
			-- if no relevant partitions, go to first unvisited partition
			-- if no unvisited partition, return -1 (exhausted)
			let n_relevant = clHandler.clProc.relevantParts |> length
			in if n_relevant > 0 then clHandler.clProc.relevantParts[0] else
			let n_unvisited = clHandler.clProc.isPartVisited |> countFor (!)
			in if n_unvisited > 0
				then
					let unvisited = clHandler.clProc.isPartVisited
					|> zip (iota part_no)
					|> filter (\(_,isV) -> !isV)
					|> map (.0)
					in unvisited[0]
				else (-1)

		def add_next_partition [n_old] [n_new] [part_no]
			(clHandler : dbcHandler [n_old] [part_no])
			(new_id : i64)
			(new_xs : [n_new]f)
			(new_ys : [n_new]f)
		: clHandler [n_new] [part_no] =
			let clBase = clHandler.clBase
			let clProc = clHandler.clProc
			let old_part = clHandler.current_partition
			let eps = clBase.eps
			let eps2 = times two eps
			-- find adjacent partition ids
			let x_id = new_id % clBase.part_per_dim[0]
			let y_id = new_id / clBase.part_per_dim[0]
			let neighs : [8]i64 =
				let nleft = if x_id>0
					then (y_id*clBase.part_per_dim[0] + x_id - 1)
					else (-1)
				let nld = if x_id>0 && y_id>0
					then ((y_id-1)*clBase.part_per_dim[0] + x_id - 1)
					else (-1)
				let ndown = if y_id>0
					then ((y_id-1)*clBase.part_per_dim[0] + x_id)
					else (-1)
				let nrd = if x_id<(clBase.part_per_dim[0]-1) && y_id>0
					then ((y_id-1)*clBase.part_per_dim[0] + x_id + 1)
					else (-1)
				let nright = if x_id<(clBase.part_per_dim[0]-1)
					then (y_id*clBase.part_per_dim[0] + x_id + 1)
					else (-1)
				let nru = if x_id<(clBase.part_per_dim[0]-1) && y_id<(clBase.part_per_dim[1]-1)
					then ((y_id+1)*clBase.part_per_dim[0] + x_id + 1)
					else (-1)
				let nup = if y_id<(clBase.part_per_dim[1]-1)
					then ((y_id+1)*clBase.part_per_dim[0] + x_id)
					else (-1)
				let nlu = if x_id>0 && y_id<(clBase.part_per_dim[1]-1)
					then ((y_id+1)*clBase.part_per_dim[0] + x_id - 1)
					else (-1)
				in [nleft, nld, ndown, nrd, nright, nru, nup, nlu]
			let isNeighVisited = neighs |> map (\n -> n>=0 && (clProc.isPartVisited[n]))
			let new_isPartVisited = (copy clProc.isPartVisited) with [new_id] = true
			-- new partition coords
			let (min_coords, max_coords, min_inner, max_inner) =
				let minC = (
					if x_id==0 then lowest else clBase.mins[0] + x_id*clBase.step_per_dim[0],
					if y_id==0 then lowest else clBase.mins[1] + y_id*clBase.step_per_dim[1]
				)
				let maxC = (
					if x_id==(clBase.part_per_dim[0]-1)
						then highest else clBase.mins[0] + (x_id+1)*clBase.step_per_dim[0],
					if y_id==(clBase.part_per_dim[1]-1)
						then highest else clBase.mins[1] + (y_id+1)*clBase.step_per_dim[1]
				)
				let min_inner = (
					if (x_id==0 || isNeighVisited[0]) then minC.0 else plus minC.0 eps2,
					if (y_id==0 || isNeighVisited[1]) then minC.1 else plus minC.1 eps2
				)
				let max_inner = (
					if (x_id==(clBase.part_per_dim[0]-1) || isNeighVisited[2])
						then maxC.0 else minus maxC.0 eps2,
					if (y_id==(clBase.part_per_dim[1]-1) || isNeighVisited[3])
						then maxC.1 else minus maxC.1 eps2
				)
				in (minC, maxC, min_inner, max_inner)
			let new_pts = zip new_xs new_ys
			-- update relevant partitions
			let new_relevants = neighs
				|> map (\n -> if isNeighVisited[n] then (-1) else n) -- visited can't be relevant
				|> map (\n -> if clProc.relevantParts |> any (== n) then (-1) else n) -- avoid multiplicity
				|> map (\nid ->
					if nid<0 then (-1,false) else
					let nxid = nid % clBase.part_per_dim[0]
					let nyid = nid / clBase.part_per_dim[0]
					let min_x = clBase.mins[0] + nxid*clBase.step_per_dim[0]
					let min_y = clBase.mins[1] + nyid*clBase.step_per_dim[1]
					let max_x = clBase.mins[0] + (nxid+1)*clBase.step_per_dim[0]
					let max_y = clBase.mins[1] + (nyid+1)*clBase.step_per_dim[1]
					let isRel = new_pts |> any (\(x,y) ->
						((x `geq` (minus min_x eps)) && (x `leq` (plus max_x eps)))
						&&
						((y `geq` (minus min_y eps)) && (y `leq` (plus max_y eps)))
					)
					in (nid, isRel)
				)
				|> filter (.1) -- keep only partitions with pts within eps of their boundaries
				-- TODO could better shave corner points
				|> map (.0)
			let added_relevants = clProc ++ new_relevants -- append for breadth-first
			-- new partition's margin points
			-- don't mind those that are not near any neighbours
			let new_isMargin = new_pts |> map (\(x,y) ->
				neighs |> any (\nid ->
					if nid<0 then false else
					let nxid = nid % clBase.part_per_dim[0]
					let nyid = nid / clBase.part_per_dim[0]
					let min_x = clBase.mins[0] + nxid*clBase.step_per_dim[0]
					let min_y = clBase.mins[1] + nyid*clBase.step_per_dim[1]
					let max_x = clBase.mins[0] + (nxid+1)*clBase.step_per_dim[0]
					let max_y = clBase.mins[1] + (nyid+1)*clBase.step_per_dim[1]
					in
						((x `geq` (minus min_x eps2)) && (x `leq` (plus max_x eps2)))
						&&
						((y `geq` (minus min_y eps2)) && (y `leq` (plus max_y eps2)))
				)
			)
			let new_isTightMargin = new_pts |> zip new_isMargin
				|> map (\(isM, (x,y)) ->
					isM &&
					(neighs |> any (\nid ->
						if nid<0 then false else
						let nxid = nid % clBase.part_per_dim[0]
						let nyid = nid / clBase.part_per_dim[0]
						let min_x = clBase.mins[0] + nxid*clBase.step_per_dim[0]
						let min_y = clBase.mins[1] + nyid*clBase.step_per_dim[1]
						let max_x = clBase.mins[0] + (nxid+1)*clBase.step_per_dim[0]
						let max_y = clBase.mins[1] + (nyid+1)*clBase.step_per_dim[1]
						in
							((x `geq` (minus min_x eps)) && (x `leq` (plus max_x eps)))
							&&
							((y `geq` (minus min_y eps)) && (y `leq` (plus max_y eps)))
					))
				)
			-- update buffered points
			-- also flush points
			-- first add margin points of previous partition
			-- then filter
			-- only keep ones within 2*eps of an unvisited partition
			-- TODO see if filters will make compiler complain...
			let (new_buffered_pts, new_buffered_isCore, new_buffered_cids,
				flush_pts, flush_cids) =
				-- get last part's margin points & their info
				-- flush non-margin points
				let (lastTups_keep, lastTups_flush) =
					zip4 old_part.dat old_part.isCore old_part.chain_ids old_part.isMargin
					|> partition (.3)
				let (last_pts, last_isC, last_cid) =
					lastTups_keep
					|> map (\(dat, isCore, cid, _) -> (dat, isCore, cid))
					|> unzip3
				let (flush_pts_fromPart, flush_cids_fromPart) =
					lastTups_flush
					|> map (\(dat, _, cid, _) -> (dat, cid))
					|> unzip
				-- append them
				let (added_pts, added_isC, added_cid) = (
					dbcProc.buffered_pts ++ last_pts,
					dbcProc.buff_isCore ++ last_isC,
					dbcProc.buffered_ids ++ last_cid
				)
				-- flush irrelevant margin points
				let (margins_toKeep_is, margins_toFlush_is) =
					indices added_pts
					|> map (\i ->
						let pt_i = added_pts[i]
						let (_,isNearAnyRelevant) =
							loop (curPart_i, isNear) = (0, false)
							while (curPart_i < length clProc.relevantParts) && (!isNear) do
								let curPart = clProc.relevantParts[curPart_i]
								let cur_xid = curPart % clBase.part_per_dim[0]
								let cur_yid = curPart / clBase.part_per_dim[0]
								let cur_minX = clBase.mins[0] + cur_xid*clBase.step_per_dim[0]
								let cur_minY = clBase.mins[1] + cur_yid*clBase.step_per_dim[1]
								let cur_maxX = clBase.mins[0] + (cur_xid+1)*clBase.step_per_dim[0]
								let cur_maxY = clBase.mins[1] + (cur_yid+1)*clBase.step_per_dim[1]
								let isN = (
									((pt_i.0 `geq` (minus cur_minX eps2)) && (pt_i.0 `leq` (plus cur_maxX eps2)))
									&&
									((pt_i.1 `geq` (minus cur_minY eps2)) && (pt_i.1 `leq` (plus cur_maxY eps2)))
								)
								in (curPart_i+1, isN)
						in (curPart_i+1, isNearAnyRelevant)
					)
					|> partition (.1)
				let (nBuff_pts, nBuff_isCore, nBuff_cids) =
					margins_toKeep_is
					|> map (\(i,_) -> (added_pts[i], added_isC[i], added_cid[i]))
					|> unzip3
				let (margin_flush_pts, margin_flush_cids) =
					margins_toFlush_is
					|> map (\(i,_) -> (added_pts[i], added_cid[i]))
					|> unzip
				in (nBuff_pts, nBuff_isCore, nBuff_cids,
					flush_pts_fromPart ++ margin_flush_pts, flush_cids_fromPart ++ margin_flush_cids)
			-- lastly, create new handler objects
			let new_proc : dbcProcessor [part_no] = {
				cur_part_id = new_id,
				cur_part_order = clProc.cur_part_order + 1,
				isPartVisited = new_isPartVisited,
				relevantParts = added_relevants,
				buffered_pts = new_buffered_pts,
				buff_isCore = new_buffered_isCore,
				buffered_ids = new_buffered_cids,
				offs = clProc.next_offs,
				next_offs = clProc.next_offs + n_new,
				chainOffs = clProc.chainOffs
			}
			let new_part : dbcPartition [n_new] = {
				part_id = new_id,
				min_coord = [min_coords.0, min_coords.1],
				max_coord = [max_coords.0, max_coords.1],
				inner_min_coord = [min_inner.0, min_inner.1],
				inner_max_coord = [max_inner.0, max_inner.1],
				neighbours = neighs,
				isNeighbourVisited = isNeighVisited,
				dat = new_pts,
				isCore = replicate n_new false,
				isMargin = new_isMargin,
				isTightMargin = new_isTightMargin,
				chain_ids = replicate n_new (-1)
			}
			in {
				clBase = clBase,
				clProc = new_proc,
				current_partition = new_part,
				toFlush_dat = flush_pts,
				toFlush_ids = flush_cids,
				chain_collisions = clHandler.chain_collisions
			}
	
	-- DBSCAN functions
	-- NOTE : deletes the flushData from clHandler
	-- It is assumed that they will be flushed before DBSCAN

		local def get_num_neighbours_against [n1] [n2]
			(dat1 : [n1](f,f))
			(dat2 : [n2](f,f))
			(eps : f)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : f)
			(m_size : i64)
		: [n1]i64 =
			let extPar = i64.max 1 (m_size/n2)
			let num_iter = (extPar + n1 - 1)/extPar
			in loop nnBuff = (replicate n1 0) for j in (iota num_iter) do
				let inf = j*extPar
				let sup = i64.min n1 (inf+extPar)
				let this_xs = dat1[inf:sup]
				let this_neighCount = this_xs
					|> map (\this_x ->
						dat2
							|> map (\cmp_x -> dist dist_t angle_t radius this_x cmp_x)
							|> countFor (\d -> d `leq` eps)
					) |> map (\c -> c)
				in nnBuff with [inf:sup] = this_neighCount

		local def find_chain_ids
			(core_pts : [](f,f))
			(pre_cids : []i64)
			(eps : t)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : f)
			(m_size : i64)
			(gather_psize : i64)
			(chain_offs : i64)
		-- returns cids + collided pairs
		: ([]i64, [](i64, i64)) =
			-- 0 - parameters
				let n = length core_pts
				let extPar = i64.max 1 (m_size/n)
			-- 1 - find current cluster IDs irrespective of preCids
				let (cluster_heads,_) =
				loop (chBuff,conv) = (iota n, false) while (!conv) do
					let next_chBuff = 
					loop chBuff_ = chBuff for j in (iota inner_iter) do
						let inf = j*extPar
						let sup = i64.min n (inf + extPar)
						let this_pts = core_pts[inf:sup]
						let this_chs = chBuff_[inf:sup]
						let this_min_ch = map2 (\ch pt ->
								let coreNeigh = core_pts
									|> map (\other_pt -> dist pt other_pt)
									|> map (\d -> d `leq` eps)
								let neighChs = map2 (\other_ch isN ->
									if isN then other_ch else ch
								) chBuff_ coreNeigh -- using chBuff_ here could bring faster convergence
								in i64.minimum neighChs
							) this_chs this_pts
						in (copy chBuff_) with [inf:sup] = this_min_ch
					let next_conv = (map2 (==) chBuff next_chBuff) |> all (id)
					in (next_chBuff, next_conv)
			-- 2 - List - Ranking & offset
				let is_cluster_head = map2 (==) (iota n) (cluster_heads :> [n]i64)
				let ch_ids = is_cluster_head
					|> map (\isCh -> if isCh then 1 else 0)
					|> exscan (+) 0
				let local_clustering = partitioned_gather (i64.num_bits) (gather_psize) (-1) ch_ids cluster_heads
					|> map (\cid -> cid + chain_offs)
			-- 3 - Collide with preCids
				-- make a compact list of collisions
				let prior_collisions_multiplicity = (pre_cids :> [n]i64)
					|> zip local_clustering
					|> filter (\(_, preCid) -> preCid>=0)
				-- distinct
				-- TODO apply m_size here?
				let numCol_mult = length prior_collisions_multiplicity
				let prior_collisions_flags = numCol_mult
					|> map (\i ->
						let pc = prior_collisions_multiplicity[i]
						let toFlag = numCol_mult
							|> map (\j -> (j, prior_collisions_multiplicity[j]))
							|> reduce_comm (\(j1,pc1) (j2,pc2) ->
								if ((pc1.0 != pc.0 || pc1.1 != pc.1) ||
									(j1>j2 && pc2.0 == pc.0 && pc2.1 == pc.1))
								then (j2,pc2)
								else (j1,pc1)
							) (i,pc)
						in toFlag.0 == i
					)
				let prior_collisions = (prior_collisions_multiplicity :> [numCol_mult](i64,i64))
					|> zip (prior_collisions_flags :> [numCol_mult]bool)
					|> filter (.0)
					|> map (.1)
				-- all ids are replaced with smallest collision, if exists
				-- TODO apply m_size here?
				let rectified_clustering = local_clustering |> map (\cc ->
						prior_collisions |> reduce_comm (\(cur1,pre1) (cur2,pre2) ->
							if (cur1!=cc || pre1<0 || (pre1>pre2 && cur2==cc)) then (cur2,pre2)
							else (cur1,pre1) 
						) (cc, cc)
					)
			-- 4 - Store preCid collisions
				let new_collisions = prior_collisions
					|> filter (\(curCid,_) -> curCid < chain_offs)
			-- Return
				in (rectified_clustering, new_collisions)

		local def assign_chain_ids [fn] [pn]
			(frontier_pts : [fn](f,f))
			(partition_pts : [pn](f,f))
			(core_pts : [](f,f))
			(cids : []i64)
			(eps : t)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : f)
			(m_size : i64)
		: ([fn]i64, [pn]i64) =
			let extPar = i64.max 1 (m_size/(length core_pts))
			let cluster_head = indices core_pts
				|> map (\i -> (cids[i], core_pts[i]))
			-- assign cids to frontier points
			let numIter1 = (extPar + fn - 1)/extPar
			let f_cids =
				loop buff = (replicate n (-1)) for j in (iota num_iter) do
				let inf = j*extPar
				let sup = i64.min fn (inf+extPar)
				let this_pts = frontier_pts[inf:sup]
				let cur_upd = this_pts |> map (\this_x ->
					cluster_head
					|> map (\(i, cd) -> (i, dist dist_t angle_t radius this_x cd))
					|> reduce_comm (\(ch1,d1) (ch2,d2) ->
						if (d1 `gt` eps) && (d2 `gt` eps) then (-1,highest)
						else if (d1 `lt` d2) then (ch1,d1)
						else (ch2,d2) -- non-deterministic if d1==d2
					) (-1,highest)
				) |> map (.0)
				in buff with [inf:sup] = cur_upd
			-- assign cids to partition points
			let numIter2 = (extPar + pn - 1)/extPar
			let p_cids =
				loop buff = (replicate n (-1)) for j in (iota num_iter) do
				let inf = j*extPar
				let sup = i64.min fn (inf+extPar)
				let this_pts = partition_pts[inf:sup]
				let cur_upd = this_pts |> map (\this_x ->
					cluster_head
					|> map (\(i, cd) -> (i, dist dist_t angle_t radius this_x cd))
					|> reduce_comm (\(ch1,d1) (ch2,d2) ->
						if (d1 `gt` eps) && (d2 `gt` eps) then (-1,highest)
						else if (d1 `lt` d2) then (ch1,d1)
						else (ch2,d2) -- non-deterministic if d1==d2
					) (-1,highest)
				) |> map (.0)
				in buff with [inf:sup] = cur_upd
			in (f_cids, p_cids)

		def do_DBSCAN [n]
			(clHander : dbcHandler [n])
			(gather_psize : i64)
		: dbcHandler [n] =
			let clBase = clHandler.clBase
			let clProc = clHandler.clProc
			let cur_part = clHandler.current_partition
			let eps = clBase.eps
			let eps2 = times two eps
			let minPts = clBase.minPts
			-- compute *tight* margin pts from partition + their indices - used twice later
			let (margin_is, margin_dat) = cur_part.dat
				|> zip3 (isTightMargin) (iota n)
				|> filter (.0) ((.0) && !(.1)) --TODO
				|> map (\(_,is,dat) -> (is,dat))
				|> unzip				
			-- find tight and loose frontier from margin points
			let (tight_frontier, loose_frontier) =
				let frontier =
					indices clProc.buffered_pts
					|> map (\i -> (clProc.buffered_pts[i] clProc.buff_isCore[i] clProc.buffered_ids[i], i))
					|> filter (\((x,y),_,_,_) ->
						((x `geq` (minus cur_part.min_coord[0] eps2)) && (x `leq` (plus cur_part.max_coord[0] eps2)))
							&&
						((y `geq` (minus cur_part.min_coord[1] eps2)) && (y `leq` (plus cur_part.max_coord[1] eps2)))
					)
				in frontier
					|> partition (\((x,y),_,_,_) ->
						((x `geq` (minus cur_part.min_coord[0] eps)) && (x `leq` (plus cur_part.max_coord[0] eps)))
							&&
						((y `geq` (minus cur_part.min_coord[1] eps)) && (y `leq` (plus cur_part.max_coord[1] eps)))
					)
			let (tf_pts, tf_cid, tf_is) = tight_frontier |> map (\(pt,_,cid,is) -> (pt,cid,is)) |> unzip
			let lf_pts = loose_frontier |> map (.0)
			-- find core points from tight margins
			let tf_isC =
				-- get neighcount from tight & loose frontiers
				let tf_neighCount1 = get_num_neighbours_against
					tf_pts tf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				let tf_neighCount2 = get_num_neighbours_against
					tf_pts lf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				-- get neighcount from partition's tight margin
				let tf_neighCount3 = get_num_neighbours_against
					tf_pts margin_dat eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				-- sum
				in map3 (\c1 c2 c3 -> c1+c2+c3-1)
					tf_neighCount1 tf_neighCount2 tf_neighCount3
					|> map (>= minPts)
			-- find core points from partition
			let part_isC : [n]bool =
				let dat = cur_part.dat
				-- from self
				let part_neighCount1 = get_num_neighbours_against
					dat dat eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				-- from tight frontier
				let part_neighCount2 =
					let part_neighCount2_ = get_num_neighbours_against
						margin_dat tf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
					in scatter (replicate n 0) margin_is part_neighCount2_
				-- sum
				in map2 (+) part_neighCount1 part_neighCount2
					|> map (>= minPts)
			-- isolate core pts && cids from tfs
			let (core_pts, preCids) =
				let (tf_core_pts, tf_precids) =
					zip3 tf_pts tf_cid tf_isC
					|> filter (.2)
					|> map (\(pts,cid,_) -> (pts,cid))
					|> unzip
				let (part_core_pts, part_precids) =
					zip3 cur_part.dat (replicate n (-1)) part_isC
					|> filter (.2)
					|> map (\(pts,cid,_) -> (pts,cid))
					|> unzip
				in (tf_core_pts ++ part_core_pts, tf_precids ++ part_precids)
			-- iteratively group them in chains
			-- TODO write code for this func...
			let (cids, new_collisions) = find_chain_ids
				core_pts preCids
				eps clBase.dist_t clBase.angle_t clBase.radius
				clBase.m_size gather_psize
				clProc.chainOffs
			-- TODO assign chains to all points
			let (f_cids, p_cids) = assign_chain_ids
				tf_pts cur_part.dat core_pts
				cids eps clBase.dist_t clBase.angle_t clBase.radius
				clBase.m_size
			let new_relevant_cids = scatter (copy clProc.buffered_ids) tf_is f_cids
			let new_relevant_isC = scatter (copy clProc.buff_isCore) tf_is tf_isC
			-- TODO return updated clHandler
			let new_part = {
				part_id = cur_part.part_id,
				min_coord = cur_part.min_coord,
				max_coord = cur_part.max_coord,
				inner_min_coord = cur_part.inner_min_coord,
				inner_max_coord = cur_part.inner_max_coord,
				neighbours = cur_part.neighbours,
				isNeighbourVisited = cur_part.isNeighbourVisited,
				dat = cur_part.dat,
				isCore = part_isC,
				isMargin = cur_part.isMargin,
				isTightMargin = cur_part.isTightMargin,
				chain_ids = p_cids
			}
			let new_proc = {
				cur_part_id = clProc.cur_part_id,
				cur_part_order = clProc.cur_part_order,
				isPartVisited = clProc.isPartVisited,
				relevantParts = clProc.relevantParts,
				buffered_pts = clProc.buffered_pts,
				buff_isCore = new_relevant_isC,
				buffered_ids = new_relevant_cids,
				offs = clProc.offs,
				next_offs = clProc.next_offs,
				chainOffs = clProc.chainOffs
			}
			in {
				clBase = clBase,
				clProc = new_proc,
				cur_part = new_part,
				toFlush_dat = [],
				toFlush_ids = [],
				chain_collisions = clHandler.chain_collisions ++ new_collisions
			}
			

	-- TODO functions to rectify chain collisions (2nd pass)



}