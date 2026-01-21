import "ftbasics"
import "lib/github.com/diku-dk/sorts/merge_sort"

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
	chain_collisions : [](i64,i64)
}

type~ flushedData_t 'f = {n : i64, xs : []f, ys : []f, chain_ids : []i64}

type~ collisionTable = {ncc : i64, chain_id: []i64, replaceWith: []i64}

module dbscan_plus (F : float) = {
	-- Types and Basic Ops
		type t = F.t

		type dbcBase = dbcBase_t t
		type dbcPartition [n] = dbcPartition_t [n] t
		type~ dbcProcessor [part_no] = dbcProcessor_t [part_no] t
		type~ dbcHandler [n] [part_no] = dbcHandler_t [n] [part_no] t

		type~ flushedData = flushedData_t t

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
		local def abs = (F.abs)

		local def num_bits : i32 = F.num_bits

	-- Angular Ops
		local def pi = (F.pi)
		local def oneEighty = F.i32 180

		local def cos = (F.cos)
		local def sin = (F.sin)
		local def acos = (F.acos)

		local def mid_angle (x1 : t) (x2 : t) = over (plus x1 x2) two

		-- formula from Widipedia
		-- TODO cite properly
		local def hav_theta (pt1 : (t,t)) (pt2: (t,t))
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

		local def d_euclidean (pt1 : (t,t)) (pt2 : (t,t))
		: t = 
			if (pt1.0 `eq` pt2.0) then abs (pt1.1 `minus` pt2.1)
			else if (pt1.1 `eq` pt2.1) then abs (pt1.0 `minus` pt2.0)
			else hypot (minus pt1.0 pt2.0) (minus pt1.1 pt2.1)

		local def d_haversine (aType : anglType) (r: t) (pt1 : (t,t)) (pt2 : (t,t))
		: t =
			let (p1,p2) : ((t,t),(t,t)) =
				match aType
				case #radians -> (pt1, pt2)
				case #degrees -> (
					(deg_to_rad pt1.0, deg_to_rad pt1.1),
					(deg_to_rad pt2.0, deg_to_rad pt2.1)
				)
			let h_theta = F.min (hav_theta p1 p2) one
			let theta = times h_theta two |> minus one |> acos
			in times theta r

		local def dist (dType : distType) (aType : anglType) (r : t) (pt1 : (t,t)) (pt2 : (t,t))
		: t =
			match dType
			case #euclidean -> d_euclidean pt1 pt2
			case #haversine -> d_haversine aType r pt1 pt2

		local def is_pt_marginal -- see if a point is within a partition's margins or frontier
			(dType : distType) (aType : anglType) (r : t)
			(minX : t) (minY : t) (maxX : t) (maxY : t)
			(pt : (t,t)) (thresh : t)
		: bool =
			let (x,y) = pt
			let dfunc : (t,t) -> t = dist dType aType r pt
			in
			(
				(y `geq` minY) && (y `leq` (maxY)) && 
				(((dfunc (minX,y)) `leq` thresh) || ((dfunc (maxX,y)) `leq` thresh))
			)
			||
			(
				(x `geq` minX) && (x `leq` (maxX)) && 
				(((dfunc (x,minY)) `leq` thresh) || ((dfunc (x,maxY)) `leq` thresh))
			)
			||
			(
				let corn_x = if (x `leq` minX) then minX else maxX
				let corn_y = if (y `leq` minY) then minY else maxY
				in ((dfunc (corn_x, corn_y)) `leq` thresh)
			)

	-- Initializations & Continuation Info
		local def mk_dbcBase
			(min_x : t) (min_y : t) (max_x : t) (max_y : t)
			(parts_x : i64) (parts_y : i64)
			(dist_type : distType) (angle_type : anglType) (radius : t)
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
				minPts = minPts,
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
			(dist_type : distType) (angle_type : anglType) (radius : t)
			(eps : t) (minPts : i64)
			(m_size : i64)
		: dbcHandler [0] [parts_x * parts_y] =
			let clBase = mk_dbcBase
				min_x min_y max_x max_y
				parts_x parts_y
				dist_type angle_type radius
				eps minPts
				m_size
			let clProc = (mk_dbcProc clBase) :> (dbcProcessor [parts_x * parts_y])
			let dummy_partition : dbcPartition [0] =
				{
					part_id = -1,
					min_coord = replicate 2 highest,
					max_coord = replicate 2 lowest,
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
			let n_unvisited = clHandler.clProc.isPartVisited |> countFor (\b -> !b)
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
			(new_xs : [n_new]t)
			(new_ys : [n_new]t)
		: dbcHandler [n_new] [part_no] =
			let clBase = clHandler.clBase
			let clProc = clHandler.clProc
			let old_part = clHandler.current_partition
			let eps = clBase.eps
			let eps2 = times two eps
			let isWithinMargins : t -> t -> t -> t -> (t,t) -> t -> bool
				= is_pt_marginal clBase.dist_t clBase.angle_t clBase.radius 
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
			let (min_coords, max_coords) =
				let minC = (
					if x_id==0 then lowest else
						plus clBase.mins[0] (times (from_i64 x_id) clBase.step_per_dim[0]),
					if y_id==0 then lowest else
						plus clBase.mins[1] (times (from_i64 y_id) clBase.step_per_dim[1])
				)
				let maxC = (
					if x_id==(clBase.part_per_dim[0]-1)
						then highest else
							plus clBase.mins[0] (times (from_i64 (x_id+1)) clBase.step_per_dim[0]),
					if y_id==(clBase.part_per_dim[1]-1)
						then highest else 
							plus clBase.mins[1] (times (from_i64 (y_id+1)) clBase.step_per_dim[1])
				)
				in (minC, maxC)
			let new_pts = zip new_xs new_ys
			-- update relevant partitions
			let new_relevants = neighs
				|> map (\n -> if clProc.relevantParts |> any (== n) then (-1) else n) -- avoid multiplicity
				|> map (\nid ->
					if nid<0 then (-1,false) else
					let nxid = nid % clBase.part_per_dim[0]
					let nyid = nid / clBase.part_per_dim[0]
					let min_x = plus clBase.mins[0] (times (from_i64 nxid) clBase.step_per_dim[0])
					let min_y = plus clBase.mins[1] (times (from_i64 nyid) clBase.step_per_dim[1])
					let max_x = plus min_x clBase.step_per_dim[0]
					let max_y = plus min_y clBase.step_per_dim[1]
					let isRel = new_pts |> any (\pt ->
						isWithinMargins min_x min_y max_x max_y pt eps
					)
					in (nid, isRel)
				)
				|> filter (.1) -- keep only partitions with pts within eps of their boundaries
				-- TODO could better shave corner points
				|> map (.0)
			let added_relevants = (clProc.relevantParts ++ new_relevants)
				|> filter (\pid -> !new_isPartVisited[pid])-- append for breadth-first
			-- new partition's margin points
			-- don't mind those that are not near any neighbours
			let new_isMargin = new_pts |> map (\pt ->
				neighs |> any (\nid ->
					if nid<0 then false else
					let nxid = nid % clBase.part_per_dim[0]
					let nyid = nid / clBase.part_per_dim[0]
					let min_x = plus clBase.mins[0] (times (from_i64 nxid) clBase.step_per_dim[0])
					let min_y = plus clBase.mins[1] (times (from_i64 nyid) clBase.step_per_dim[1])
					let max_x = plus min_x clBase.step_per_dim[0]
					let max_y = plus min_y clBase.step_per_dim[1]
					in
						isWithinMargins min_x min_y max_x max_y pt eps2
				)
			)
			let new_isTightMargin = new_pts |> zip new_isMargin
				|> map (\(isM, pt) ->
					isM &&
					(neighs |> any (\nid ->
						if nid<0 then false else
						let nxid = nid % clBase.part_per_dim[0]
						let nyid = nid / clBase.part_per_dim[0]
						let min_x = (plus clBase.mins[0] (times (from_i64 nxid) clBase.step_per_dim[0]))
						let min_y = (plus clBase.mins[1] (times (from_i64 nyid) clBase.step_per_dim[1]))
						let max_x = plus min_x clBase.step_per_dim[0]
						let max_y = plus min_y clBase.step_per_dim[1]
						in
							isWithinMargins min_x min_y max_x max_y pt eps
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
					clProc.buffered_pts ++ last_pts,
					clProc.buff_isCore ++ last_isC,
					clProc.buffered_ids ++ last_cid
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
								let cur_minX = (plus clBase.mins[0]
									(times (from_i64 cur_xid) clBase.step_per_dim[0]))
								let cur_minY = (plus clBase.mins[1]
									(times (from_i64 cur_yid) clBase.step_per_dim[1]))
								let cur_maxX = plus cur_minX clBase.step_per_dim[0]
								let cur_maxY = plus cur_minY clBase.step_per_dim[1]
								let isN = 
									isWithinMargins cur_minX cur_minY cur_maxX cur_maxY pt_i eps2
								in (curPart_i+1, isN)
						in (i, isNearAnyRelevant)
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

		def flush_dat [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
			(total : bool)
		: flushedData =
			let (xdat, ydat) =
				if !total
				then clHandler.toFlush_dat |> unzip
				else (clHandler.toFlush_dat ++ clHandler.clProc.buffered_pts ++ clHandler.current_partition.dat)
					|> unzip
			let cids =
				if !total
				then clHandler.toFlush_ids
				else clHandler.toFlush_ids ++ clHandler.clProc.buffered_ids ++ clHandler.current_partition.chain_ids
			in {n = length xdat, xs = xdat, ys = ydat, chain_ids = cids}
	
	-- DBSCAN functions
	-- NOTE : deletes the flushData from clHandler
	-- It is assumed that they will be flushed before DBSCAN

		local def get_num_neighbours_against [n1] [n2]
			(dat1 : [n1](t,t))
			(dat2 : [n2](t,t))
			(eps : t)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : t)
			(m_size : i64)
		: [n1]i64 =
			let extPar = i64.max 1 (m_size/(i64.max n2 1))
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

		-- TODO make compact like in ftDBSCAN
		local def find_chain_ids
			(core_pts_ : [](t,t))
			(pre_cids_ : []i64)
			(eps : t)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : t)
			(m_size : i64)
			(gather_psize : i64)
			(chain_offs : i64)
		-- returns cids + collided pairs
		: ([]i64, [](i64, i64)) =
			-- 0 - parameters
				let n1 = length pre_cids_
				let core_pts = core_pts_ :> [n1](t,t) -- dumb size-checker
				let pre_cids = pre_cids_ :> [n1]i64 -- dumb size-checker
				let extPar = i64.max 1 (m_size/(i64.max 1 n1))
				let num_iter = (extPar + n1 - 1) / extPar
			-- 1 - find current cluster IDs irrespective of preCids
				let (cluster_heads,_) =
				loop (chBuff,conv) = (iota n1, false) while (!conv) do
					let next_chBuff = 
					loop chBuff_ = chBuff for j in (iota num_iter) do
						let inf = j*extPar
						let sup = i64.min n1 (inf + extPar)
						let this_pts = core_pts[inf:sup]
						let this_chs = chBuff_[inf:sup]
						let this_min_ch = map2 (\ch pt ->
								let coreNeigh = core_pts
									|> map (\other_pt -> dist dist_t angle_t radius pt other_pt)
									|> map (\d -> d `leq` eps)
								let neighChs = map2 (\other_ch isN ->
									if isN then other_ch else ch
								) chBuff_ coreNeigh -- using chBuff_ here could bring faster convergence
								in i64.minimum neighChs
							) this_chs this_pts
						in (copy chBuff_) with [inf:sup] = this_min_ch
					let next_conv = (map2 (==) chBuff next_chBuff) |> all (id)
					in (next_chBuff :> [n1]i64, next_conv)
			-- 2 - List - Ranking & offset
				let is_cluster_head = map2 (==) (iota n1) (cluster_heads :> [n1]i64)
				let ch_ids = is_cluster_head
					|> map (\isCh -> if isCh then 1 else 0)
					|> exscan (+) 0
				let local_clustering = partitioned_gather (i64.num_bits) (gather_psize) (-1) ch_ids cluster_heads
					|> map (\cid -> cid + chain_offs)
			-- 3 - Collide with preCids
				-- make a compact list of collisions
				let prior_collisions_multiplicity = (pre_cids :> [n1]i64)
					|> zip local_clustering
					|> filter (\(_, preCid) -> preCid>=0)
				-- distinct
				-- TODO apply m_size here?
				let numCol_mult = length prior_collisions_multiplicity
				let prior_collisions_flags = iota numCol_mult
					|> map (\i ->
						let pc = prior_collisions_multiplicity[i]
						let toFlag = iota numCol_mult
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
							if (cur1!=cc || pre1<0 || (pre1>pre2 && cur2==cc))
							then (cur2,pre2)
							else (cur1,pre1) 
						) (cc, cc)
					)
					|> map (.1)
			-- 4 - Store preCid collisions
				let new_collisions =
					-- find where previous ids collide
					let previous_collisions = rectified_clustering
						|> zip pre_cids
						|> filter (\(pc,rc) -> rc<chain_offs && pc>=0)
					-- distinct by prevCid (keep the smallest per each one)
					-- TODO use m_size here?
					let n_prevCol = length previous_collisions
					let prcol_flag = indices previous_collisions
						|> map (\i ->
							let (prv,cur) = previous_collisions[i]
							let res = previous_collisions
								|> zip (indices previous_collisions)
								|> reduce_comm (\(i1, (prv1,cur1)) (i2, (prv2,cur2)) ->
									if (cur1!=cur || cur1==prv1 ||
									(cur2==cur && ((prv2<prv1 && prv2!=cur2) || prv2==prv1 && i2<i1)))
									then (i2, (cur2,prv2))
									else (i1, (cur1,prv1))
								) (i,(prv,cur))
							in i == res.0 && (res.1.0 != res.1.1)
						)
					in (previous_collisions :> [n_prevCol](i64,i64))
						|> zip (prcol_flag :> [n_prevCol]bool)
						|> filter(.0)
						|> map (.1)
			-- Return
				in (rectified_clustering, new_collisions)

		local def assign_chain_ids [fn] [pn]
			(isMarginal_func : (t,t) -> t -> bool)
			(frontier_pts : [fn](t,t))
			(partition_pts : [pn](t,t))
			(core_pts : [](t,t))
			(cids : []i64)
			(eps : t)
			(dist_t : distType)
			(angle_t : anglType)
			(radius : t)
			(m_size : i64)
		: ([fn]i64, [pn]i64) =
			let extPar = i64.max 1 (m_size/(i64.max 1 (length core_pts)))
			let cluster_head = indices core_pts
				|> map (\i -> (cids[i], core_pts[i]))
			-- assign cids to frontier points
			-- only need to be compared to tight marginal points
			let cluster_head_tm = cluster_head
				|> filter (\(_,pt) -> isMarginal_func pt eps)
			let extPar_tm = i64.max 1 (m_size/(i64.max 1 (length cluster_head_tm)))
			let numIter1 = (extPar_tm + fn - 1)/extPar_tm
			let f_cids =
				loop buff = (replicate fn (-1)) for j in (iota numIter1) do
				let inf = j*extPar_tm
				let sup = i64.min fn (inf+extPar_tm)
				let this_pts = frontier_pts[inf:sup]
				let cur_upd = this_pts |> map (\this_x ->
					cluster_head_tm
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
				loop buff = (replicate pn (-1)) for j in (iota numIter2) do
				let inf = j*extPar
				let sup = i64.min pn (inf+extPar)
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

		def do_DBSCAN [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
			(gather_psize : i64)
		: dbcHandler [n] [part_no] =
			let clBase = clHandler.clBase
			let clProc = clHandler.clProc
			let cur_part = clHandler.current_partition
			let eps = clBase.eps
			let eps2 = times two eps
			let minPts = clBase.minPts
			let isWithinMargins : t -> t -> t -> t -> (t,t) -> t -> bool
				= is_pt_marginal clBase.dist_t clBase.angle_t clBase.radius
			-- compute *tight* margin pts from partition + their indices - used twice later
			let (margin_is, margin_dat) = cur_part.dat
				|> zip3 (cur_part.isTightMargin) (iota n)
				|> filter (.0)
				|> map (\(_,is,dat) -> (is,dat))
				|> unzip				
			-- find tight and loose frontier from margin points
			let (tight_frontier, loose_frontier) =
				let frontier =
					indices clProc.buffered_pts
					|> map (\i -> (clProc.buffered_pts[i], clProc.buff_isCore[i], clProc.buffered_ids[i], i))
					|> filter (\(pt,_,_,_) ->
						isWithinMargins
							cur_part.min_coord[0] cur_part.min_coord[1] 
							cur_part.max_coord[0] cur_part.max_coord[1]
							pt eps2
					)
				in frontier
					|> partition (\(pt,_,_,_) ->
						isWithinMargins
							cur_part.min_coord[0] cur_part.min_coord[1] 
							cur_part.max_coord[0] cur_part.max_coord[1]
							pt eps
					)
			let (tf_pts, tf_cid, tf_is) = tight_frontier |> map (\(pt,_,cid,is) -> (pt,cid,is)) |> unzip3
			let (lf_pts, lf_isC, lf_cid, lf_is) = loose_frontier |> unzip4
			-- find core points from tight margins
			let tf_isC =
				-- get neighcount from tight & loose frontiers
				let tf_neighCount1 = get_num_neighbours_against
					tf_pts tf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
					|> map (\c -> c-1)
				let tf_neighCount2 = get_num_neighbours_against
					tf_pts lf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				-- get neighcount from partition's tight margin
				let tf_neighCount3 = get_num_neighbours_against
					tf_pts margin_dat eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
				-- sum
				in (map3 (\c1 c2 c3 -> c1+c2+c3)
						tf_neighCount1 tf_neighCount2 tf_neighCount3)
					|> map (>= minPts)
			-- find core points from partition
			let part_isC : [n]bool =
				let dat = cur_part.dat
				-- from self
				let part_neighCount1 = get_num_neighbours_against
					dat dat eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
					|> map (\c -> c-1)
				-- from tight frontier
				let part_neighCount2 =
					let part_neighCount2_ = get_num_neighbours_against
						margin_dat tf_pts eps clBase.dist_t clBase.angle_t clBase.radius clBase.m_size
					in scatter (replicate n 0) margin_is part_neighCount2_
				-- sum
				in (map2 (+) part_neighCount1 part_neighCount2)
					|> map (>= minPts)
			-- isolate core pts && cids
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
				-- loose frontier only needed if minPts >= 3
				let (lf_core_pts, lf_precids) =
					if minPts<3 then ([],[]) else
					zip3 lf_pts lf_cid lf_isC
					|> filter (.2)
					|> map (\(pts,cid,_) -> (pts,cid))
					|> unzip
				in (part_core_pts ++ tf_core_pts ++ lf_core_pts,
					part_precids ++ tf_precids ++ lf_precids)
			-- iteratively group them in chains
			let (cids, new_collisions) = find_chain_ids
				core_pts preCids
				eps clBase.dist_t clBase.angle_t clBase.radius
				clBase.m_size gather_psize
				clProc.chainOffs
			-- get frontier points together for assignment
			-- to address a tight margin point becoming core with new partition
			-- and having border neighbours in loose margin (with minPts>=2)
			let (f_pts, f_is) =
				if minPts<2
				then (tf_pts,tf_is)
				else (tf_pts++lf_pts, tf_is++lf_is)
			let nf = length f_is
			-- assign chains to all points
			let (f_cids, p_cids) = assign_chain_ids
				(isWithinMargins
					cur_part.min_coord[0] cur_part.min_coord[1]
					cur_part.max_coord[0] cur_part.max_coord[1]
				)
				f_pts cur_part.dat core_pts
				cids eps clBase.dist_t clBase.angle_t clBase.radius
				clBase.m_size
			let new_relevant_cids =
				scatter (copy clProc.buffered_ids) (f_is :> [nf]i64) (f_cids :> [nf]i64)
			let new_relevant_isC =
				scatter (copy clProc.buff_isCore) tf_is tf_isC
			let max_cid = i64.max (i64.maximum f_cids) (i64.maximum p_cids)
			-- return updated clHandler
			let new_part = {
				part_id = cur_part.part_id,
				min_coord = cur_part.min_coord,
				max_coord = cur_part.max_coord,
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
				chainOffs = i64.max (clProc.chainOffs) (max_cid+1)
			}
			in {
				clBase = clBase,
				clProc = new_proc,
				current_partition = new_part,
				toFlush_dat = [],
				toFlush_ids = [],
				chain_collisions = clHandler.chain_collisions ++ new_collisions
			}
			

	-- functions to rectify chain collisions (2nd pass)
		def old_make_collisions_compact [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
		: collisionTable =
			let m_size = clHandler.clBase.m_size
			let cc = clHandler.chain_collisions
			let nc = length cc
			in if nc == 0 then {ncc = 0, chain_id = [], replaceWith = []} else
			let extPar = i64.max 1 (m_size / nc)
			let num_iter = (extPar + nc - 1) / extPar
			let cc_flags =
				loop iter_flags = (replicate nc true) for j in (iota num_iter) do
					let inf = j*extPar
					let sup = i64.min (inf + extPar) nc
					let cur_is = (inf..<sup)
					let cur_flags = cur_is |> map (\i ->
							let pc = cc[i]
							let toFlag = cc
								|> reduce_comm (\cc1 cc2 ->
									if (cc1.0!=pc.0 || (cc2.0==pc.0 && cc2.1<cc1.1))
									then cc2
									else cc1
								) pc
							in toFlag.1 == pc.1
						)
					in iter_flags with [inf:sup] = cur_flags
			let cc_distinct = (iota nc)
				|> map (\i -> (cc_flags[i], cc[i]))
				|> filter (.0)
				|> map (.1)
			let (cur_chain, prev_chain) = cc_distinct |> unzip
			in {ncc = length cur_chain, chain_id = cur_chain, replaceWith = prev_chain}

		def make_collisions_compact [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
		: collisionTable =
			let cc_sorted = clHandler.chain_collisions
				|> merge_sort (\(c1,p1) (c2,p2) -> (c1<c2 || (c1==c2 && p1<=p2)))
			let cc_distinct = indices cc_sorted
				|> map (\i -> if i==0 then true else (cc_sorted[i].0 != cc_sorted[i-1].0))
				|> zip cc_sorted
				|> filter (.1)
				|> map (.0)
			let (cur_chain, prev_chain) = cc_distinct |> unzip
			in {ncc = length cur_chain, chain_id = cur_chain, replaceWith = prev_chain}

		def rectify_cluster_ids [n]
			(chain_ids : [n]i64)
			(colTbl : collisionTable)
			(gather_psize : i64)
		: [n]i64 =
			-- Binary search to find match
			let cid = colTbl.chain_id
			let ncc = colTbl.ncc
			in if ncc==0 then chain_ids else
			let num_iter = 1 + (ncc |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
			let (bsearch,_) =
				loop (search_is, last_step) = (replicate n 0, ncc) for _ in iota (num_iter) do
					let this_step = (last_step+1) / 2
					let cmps_ = search_is
			          |> map (\i ->
			            let prev_elem = if i<=0 then cid[0] else cid[i-1]
			            let cur_elem = if i<0 then cid[0] else cid[i]
			            in (i, prev_elem, cur_elem)
			          )
			        let cmps = map2 (\kv (i, pv, cv) ->
			            if i<0 then (-1) else
			            if (kv == cv) && (i==0 || (kv > pv))
			              then i
			            else if (kv == cv)
			              then i64.max 0 (i-this_step)
			            else if (kv > cv) then
			              if (i == ncc-1) -- ommitted nv to make tuple leaner
			              then -1
			              else i64.min (ncc-1) (i+this_step)
			            else -- kv < cv
			              if (i == 0 || (kv > pv))
			              then -1
			              else i64.max 0 (i-this_step)
			          ) chain_ids cmps_
			        in (cmps, this_step)
			in partitioned_gather_over_array (num_bits * 2) gather_psize
				chain_ids colTbl.replaceWith bsearch
}

-- Double Entry Points
	type  dbcBase_double = dbcBase_t f64
	type  dbcPartition_double [n] = dbcPartition_t [n] f64
	type~ dbcProcessor_double [part_no] = dbcProcessor_t [part_no] f64
	type~ dbcHandler_double [n] [part_no] = dbcHandler_t [n] [part_no] f64

	type~ flushedData_double = flushedData_t f64

	module dbscanPlus_double = dbscan_plus f64

	entry mk_dbcHandler_double
		(min_x : f64) (min_y : f64) (max_x : f64) (max_y : f64)
		(parts_x : i64) (parts_y : i64)
		(isEuclidean : bool) (isRadians : bool) (radius : f64)
		(eps : f64) (minPts : i64)
		(m_size : i64)
	: dbcHandler_double [0] [parts_x * parts_y] =
		let dist_type  : distType = if isEuclidean then #euclidean else #haversine
		let angle_type : anglType = if isRadians then #radians else #degrees
		in dbscanPlus_double.mk_dbcHandler
			min_x min_y max_x max_y parts_x parts_y
			dist_type angle_type radius
			eps minPts m_size

	entry next_partition_to_read_double [n] [part_no]
		(clHandler : dbcHandler_double [n] [part_no])
	: i64 =
		dbscanPlus_double.next_partition_to_read clHandler

	entry add_next_partition_double [n_old] [n_new] [part_no]
		(clHandler : dbcHandler_double [n_old] [part_no])
		(new_id : i64)
		(new_xs : [n_new]f64)
		(new_ys : [n_new]f64)
	: dbcHandler_double [n_new] [part_no] =
		dbscanPlus_double.add_next_partition clHandler new_id new_xs new_ys

	entry flush_dat_double [n] [part_no]
		(clHandler : dbcHandler_double [n] [part_no])
		(total : bool)
	: flushedData_double =
		dbscanPlus_double.flush_dat clHandler total

	entry do_DBSCAN_double [n] [part_no]
		(clHandler : dbcHandler_double [n] [part_no])
		(gather_psize : i64)
	: dbcHandler_double [n] [part_no] =
		dbscanPlus_double.do_DBSCAN clHandler gather_psize

	entry make_collisions_compact_double [n] [part_no]
		(clHandler : dbcHandler_double [n] [part_no])
	: collisionTable =
		dbscanPlus_double.make_collisions_compact clHandler

	entry rectify_cluster_ids_double [n]
		(chain_ids : [n]i64)
		(colTbl : collisionTable)
		(gather_psize : i64)
	: [n]i64 =
		dbscanPlus_double.rectify_cluster_ids chain_ids colTbl gather_psize

-- Tests
	def test_handler = mk_dbcHandler_double
		0.0 0.0 30.0 30.0
		3 3
		true true 0.0
		1.0 1
		4096

	def make_test_part [n]
		(pid) (xys : [n](f64,f64))
	: ([n]f64, [n]f64) =
		let x_id = pid % 3
		let y_id = pid / 3
		let x_offs = (f64.i64 x_id)*10.0
		let y_offs = (f64.i64 y_id)*10.0
		in xys |> map (\(x,y) -> (x+x_offs,y+y_offs)) |> unzip

	def test_1 (stop_point : i64) =
		let pts : [](f64,f64) = [
				(0.4,2.5),
				(8.5,9.5),
				(7.5,7.5),
				(9.4,2.1),
				(9.4,2.2),
				(9.4,3.5),
				(9.4,3.6),
				(8.5,3.6),
				(7.5,3.6),
				(3,0.2),
				(3,1.2),
				(2.5,2.5),
				(2.5,3.4),
				(3.4,3.4)]
		let (xs,ys) = make_test_part 4 pts
		let clHandler = add_next_partition_double (copy test_handler) 4 xs ys
		in if (stop_point<=0) then clHandler else
		let clHandler1 = do_DBSCAN_double clHandler 1024
		in if (stop_point==1) then clHandler1 else
		let next = next_partition_to_read_double clHandler1
		let pts : [](f64,f64) = [(4.0, 1.1),(0.0,1.9),(9.9,2.5),(4.0,9.7),(4.5,9.7)]
		let (xs,ys) = make_test_part next pts
		let clHandler = add_next_partition_double clHandler1 next xs ys
		in if (stop_point==2) then clHandler else
		let clHandler1 = do_DBSCAN_double clHandler 1024
		in if (stop_point==3) then clHandler1 else
		let next = next_partition_to_read_double clHandler1
		let clHandler = add_next_partition_double clHandler1 next [13] [9.7]
		in if (stop_point==4) then clHandler else
		let clHandler1 = do_DBSCAN_double clHandler 1024
		in if (stop_point==5) then clHandler1 else
		let next = next_partition_to_read_double clHandler1
		let pts : [](f64,f64) = [(0.1,2.2), (0.2,2.8), (0.1, 3.5)]
		let (xs,ys) = make_test_part next pts
		let clHandler = add_next_partition_double clHandler1 next xs ys
		in if (stop_point==6) then clHandler else
		let clHandler1 = do_DBSCAN_double clHandler 1024
		in clHandler1

	def test_2 (rect : bool) =
		let flushedCids : []i64 = []
		let pts : [](f64,f64) = [
				(0.4,2.5),
				(8.5,9.5),
				(7.5,7.5),
				(9.4,2.1),
				(9.4,2.2),
				(9.4,3.5),
				(9.4,3.6),
				(8.5,3.6),
				(7.5,3.6),
				(3,0.2),
				(3,1.2),
				(2.5,2.5),
				(2.5,3.4),
				(3.4,3.4)]
		let (xs,ys) = make_test_part 4 pts
		let clHandler = add_next_partition_double (copy test_handler) 4 xs ys
		let clHandler1 = do_DBSCAN_double clHandler 1024
		let next = next_partition_to_read_double clHandler1
		let pts : [](f64,f64) = [(4.0, 1.1),(0.0,1.9),(9.9,2.5),(4.0,9.7),(4.5,9.7)]
		let (xs,ys) = make_test_part next pts
		let clHandler = add_next_partition_double clHandler1 next xs ys
		let flushedCids = flushedCids ++ clHandler.toFlush_ids
		let clHandler1 = do_DBSCAN_double clHandler 1024
		let next = next_partition_to_read_double clHandler1
		let clHandler = add_next_partition_double clHandler1 next [13] [9.7]
		let flushedCids = flushedCids ++ clHandler.toFlush_ids
		let clHandler1 = do_DBSCAN_double clHandler 1024
		let next = next_partition_to_read_double clHandler1
		let pts : [](f64,f64) = [(0.1,2.2), (0.2,2.8), (0.1, 3.5)]
		let (xs,ys) = make_test_part next pts
		let clHandler = add_next_partition_double clHandler1 next xs ys
		let flushedCids = flushedCids ++ clHandler.toFlush_ids
		let clHandler1 = do_DBSCAN_double clHandler 1024
		let flushedCids = flushedCids ++ clHandler1.clProc.buffered_ids ++ clHandler1.current_partition.chain_ids
		in if !rect then flushedCids else
		let cc = make_collisions_compact_double clHandler1
		in rectify_cluster_ids_double flushedCids cc 1024


