import "ftbasics"

type distType = #euclidean | #haversine

-- Assume angles are all in radians

type dbcBase_t 'f = {
	mins : [2]f,
	maxs : [2]f,
	step_per_dim : [2]f,
	part_per_dim : [2]i64,
	total_partitions : i64,
	dist_t : distType,
	eps : f,
	minPts : i64,
	m_size : i64
}
type dbcPartition_t [n] 'f = {
	part_id : i64,
	min_coord : [2]f,
	max_coord: [2]f,
	neighbours : [4]i64,
	isNeighbourVisited : [4]bool,
	dat : [n](f,f),
	isCore : [n]bool,
	isMargin : [n]bool,
	chain_ids : [n]i64
}
type~ dbcProcessor_t [part_no] 'f = {
	cur_part_id : i64,
	cur_part_order : i64,
	isPartVisited : [part_no]bool,
	isPartRelevant : [part_no]bool,
	buffered_pts : [](f,f),
	buff_isCore : []bool,
	buffered_ids : []i64,
	offs : i64,
	next_offs : i64,
	chainOffs : i64,
	next_chainOffs : i64
}

type~ dbcHandler_t [n] [part_no] 'f = {
	clBase : dbcBase_t f,
	clProc : dbcProcessor_t [part_no] f,
	current_partition : dbcPartition_t [n] f
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
		def map_to_rad = map deg_to_rad

	-- Distance Ops
		-- TODO cite properly
		local def earth_radius : t = F.f64 6161.6 -- average earth radius

		local def d_euclidean (pt1 : (f,f)) (pt2 : (f,f))
		: t = hypot (minus pt1.0 pt2.0) (minus pt1.1 pt2.1)

		local def d_haversine (pt1 : (f,f)) (pt2 : (f,f))
		: t =
			let h_theta = F.min (hav_theta pt1 pt2) one
			let theta = times h_theta two |> minus one |> acos
			in times theta earth_radius

		local def dist (dType : distType) (pt1 : (f,f)) (pt2 : (f,f))
		: t =
			match dType
			case #euclidean -> d_euclidean pt1 pt2
			case #haversine -> d_haversine pt1 pt2

	-- Initializations & Continuation Info
		local def mk_dbcBase
			(min_x : t) (min_y : t) (max_x : t) (max_y : t)
			(parts_x : i64) (parts_y : i64)
			(dist_type : distType)
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
				isPartRelevant = replicate clBase.total_partitions false,
				buffered_pts = [],
				buff_isCore = [],
				buffered_ids = [],
				offs = -1,
				next_offs = 0,
				chainOffs = -1,
				next_chainOffs = 0,
			}

		def mk_dbcHandler 
			(min_x : t) (min_y : t) (max_x : t) (max_y : t)
			(parts_x : i64) (parts_y : i64)
			(dist_type : distType)
			(eps : t) (minPts : i64)
			(m_size : i64)
		: dbcHandler [0] [parts_x * parts_y] =
			let clBase = mk_dbcBase min_x min_y max_x max_y parts_x parts_y dist_type eps m_size
			let clProc = mk_dbcProc clBase
			let dummy_partition : dbcPartition [0] =
				{
					part_id = -1,
					min_coord = replicate 2 highest,
					max_coord = replicate 2 lowest,
					neighbours = replicate 4 (-1),
					isNeighbourVisited = replicate 4 false,
					dat = [],
					isCore = [],
					isMargin = [],
					chain_ids = []
				}
			in {clBase = clBase, clProc = clProc, current_partition = dummy_partition}

		def next_partition_to_read [n] [part_no]
			(clHandler : dbcHandler [n] [part_no])
		: i64 =
			let isUnvisited = clHandler.clProc.isPartVisited |> map (!)
			let isRelevant = clHandler.clProc.isPartRelevant
			let num_relevant = isRelevant |> countFor (id)
			let parts_to_check =
				if num_relevant == 0
				then iota n |> zip isUnvisited |> filter (.0) |> map (.1)
				else iota n |> zip (map2 (&&) isUnvisited isRelevant) |> filter (.0) |> map (.1)
			in
				if (length parts_to_check) == 0
				then -1
				else parts_to_check[0]

		def add_next_partition [n_old] [n_new] [part_no]
			(clHandler : dbcHandler [n_old] [part_no])
			(new_id : i64)
			(new_xs : [n_new]f)
			(new_ys : [n_new]f)
		-- TODO
		-- : clHandler [n_new] [part_no] =
		: i64 = 0



}