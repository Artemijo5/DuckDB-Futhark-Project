import "ftbasics"

type distType = #euclidean | #haversine
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
	chainOffs : i64 -- TODO could just use min point id as chain id, not needing this...
}

type~ dbcHandler_t [n] [part_no] 'f = {
	clBase : dbcBase_t f,
	clProc : dbcProcessor_t [part_no] f,
	current_partition : dbcPartition_t [n] f,
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
		local def leq = (F.>=)
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
				case #degrees -> (pt1, pt2)
				case #radians -> (
					(rad_to_deg pt1.0, rad_to_deg pt1.1),
					(rad_to_deg pt2.0, rad_to_deg pt2.1)
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
					chain_ids = []
				}
			in {clBase = clBase, clProc = clProc, current_partition = dummy_partition, chain_collisions = []}

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
			let eps2 = times two eps
			let clBase = clHandler.clBase
			let clProc = clHandler.clProc
			let old_part = clHandler.current_partition
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
			let isNeighVisited = neighs |> map (\n -> n<0 || !(clProc.isPartVisited[n]))
			let new_isPartVisited = (copy clProc.isPartVisited) with [new_id] = true
			-- new partition's margin points
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
			let new_isMargin = new_pts |> map (\(x,y) ->
				(x `leq` min_inner.0) || (x `geq` max_inner.0)
				|| (y `leq` min_inner.1) || (y `geq` max_inner.1)
			)
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
			-- update buffered points
			-- first add margin points of previous partition
			-- then filter
			-- only keep ones within 2*eps of an unvisited partition
			-- TODO see if filters will make compiler complain...
			let (new_buffered_pts, new_buffered_isCore new_buffered_cids) =
				-- get last part's margin points & their info
				let (last_pts, last_isC, last_cid) =
					zip4 old_part.dat old_part.isCore old_part.chain_ids old_part.isMargin
					|> filter (.3)
					|> map (\(dat, isCore, cid, _) -> (dat, isCore, cid))
					|> unzip3
				-- append them
				let (added_pts, added_isC, added_cid) = (
					dbcProc.buffered_pts ++ last_pts,
					dbcProc.buff_isCore ++ last_isC,
					dbcProc.buffered_ids ++ last_cid
				)
				-- filter irrelevant
				in indices added_pts
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
					|> filter (.1)
					|> map (\(i,_) -> (added_pts[i], added_isC[i], added_cid[i]))
					|> unzip3
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
				chain_ids = replicate n_new (-1)
			}
			in {
				clBase = clBase,
				clProc = new_proc,
				current_partition = new_part,
				chain_collisions = clHandler.chain_collisions
			}
	
	-- TODO
	-- implement DBSCAN algorithm from here on
	-- test that this stuff works...

	-- DBSCAN algorithm per partition

}