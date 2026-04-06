import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "skyline_base"

-- | Module for numeric skyline utilising regular grid subdivisions.
-- This allows to avoid certain pairwise comparisons among the points that reach output.
--
-- For high-dimensional data, should probably avoid using a lot of subdivisions.
-- As this will cause the sequential loop used to increase exponentially.
--
-- Based on traditional MR-Grid.
module skyline_grid
	(V : vector)
	(N : numeric)
	(S : skyline_base with t = N.t with tup = V.vector N.t)
= {
	type vector 'a = V.vector a
	type t = N.t
	type tup = vector t

	module skyline = mk_skyline V N S

	-- | Get the abs difference of pt2 from pt1 for all fields.
	def grid_dist (pt1 : tup) (pt2 : tup) : tup =
		pt2 |> V.map2 (\x1 x2 -> (N.-) x1 x2 |> N.abs) pt1

	-- | Get the size of a grid partition per dimension.
	def get_grid_step
		(subdiv_per_dim : vector i64)
		(minmax : (tup, tup))
	: tup = minmax.1
		|> grid_dist minmax.0
		|> V.map2 (\sub d -> (N./) d sub) (subdiv_per_dim |> V.map (N.i64))

	-- | Get the grid placement of a particular point.
	-- Smaller id = closer to the pivot
	def get_grid_id
		(subdiv_per_dim : vector i64)
		(grid_step : tup)
		(pt : tup)
	: vector i64 = pt
		|> V.map2 (\gstep x -> (N./) x gstep) grid_step
		|> V.map (N.to_i64)
		|> V.map2 (i64.min) (subdiv_per_dim |> V.map (\s -> s-1))

	-- | Get the grid placement of all points in the grid.
	def mk_grid
		(subdiv_per_dim : vector i64)
		(grid_step : tup)
		(minmax : (tup,tup))
		(pts : skyData tup)
	: [](vector i64) = pts
		|> map ((.0) >-> grid_dist minmax.0 >-> get_grid_id subdiv_per_dim grid_step)

	-- | Filter pts2 by domination from pts1, using grid subdivisions.
	def filter_against
		(subdiv_per_dim : vector i64)
		(minmax : (tup, tup))
		(pts1 : skyData tup)
		(pts2 : skyData tup)
	: skyData tup =
		let subdiv_prefix = subdiv_per_dim |> V.to_array
			|> exscan (*) 1 |> V.from_array
		let part_no = subdiv_per_dim |> V.reduce (*) 1
		in if part_no == 1
			then pts2 |> skyline.filter_against pts1
			else
		let n1 = length pts1
		let n2 = length pts2
		let gstep = minmax |> get_grid_step subdiv_per_dim
		let grid1 = pts1 |> mk_grid subdiv_per_dim gstep minmax
		let grid2 = pts2 |> mk_grid subdiv_per_dim gstep minmax
		in loop ret : skyData tup = [] for j<part_no do
			let cur_gid = subdiv_prefix |> V.map (\pref -> j / pref)
				|> V.map2 (\spd jOverPref -> jOverPref % spd) subdiv_per_dim
			let cur_pts1 = pts1 |> sized n1
				|> zip (grid1 |> sized n1)
				|> filter (\(gid,_) -> V.map2 (<=) gid cur_gid |> V.reduce (&&) true)
				|> map (.1)
			let cur_pts2 = pts2 |> sized n2
				|> zip (grid2 |> sized n2)
				|> filter (\(gid,_) -> V.map2 (==) gid cur_gid |> V.reduce (&&) true)
				|> map (.1)
			in ret ++ (cur_pts2 |> skyline.filter_against cur_pts1)

	-- Filter pts by domination from amongst themselves, using grid subdivisions.
	def filter_self
		(subdiv_per_dim : vector i64)
		(minmax : (tup, tup))
		(pts : skyData tup)
	: skyData tup =
		let subdiv_prefix = subdiv_per_dim |> V.to_array
			|> exscan (*) 1 |> V.from_array
		let part_no = subdiv_per_dim |> V.reduce (*) 1
		in if part_no == 1
			then pts |> skyline.filter_self
			else
		let n = length pts
		let gstep = minmax |> get_grid_step subdiv_per_dim
		let grid = pts |> mk_grid subdiv_per_dim gstep minmax
		in loop ret : skyData tup = [] for j<part_no do
			let cur_gid = subdiv_prefix |> V.map (\pref -> j / pref)
				|> V.map2 (\spd jOverPref -> jOverPref % spd) subdiv_per_dim
			let cur_pts1 = pts |> sized n
				|> zip (grid |> sized n)
				|> filter (\(gid,_) -> V.map2 (<=) gid cur_gid |> V.reduce (&&) true)
				-- keep the same subdivision out
				|> filter (\(gid,_) -> V.map2 (!=) gid cur_gid |> V.reduce (||) false)
				|> map (.1)
			let cur_pts2 = pts |> sized n
				|> zip (grid |> sized n)
				|> filter (\(gid,_) -> V.map2 (==) gid cur_gid |> V.reduce (&&) true)
				|> map (.1)
			let cur_pts2' = cur_pts2 |> skyline.filter_self
				|> skyline.filter_against cur_pts1
			in ret ++ cur_pts2'

	-- | Merge 2 skyData by filtering against each other, using grid subdivisions.
	def merge
		(subdiv_per_dim : vector i64)
		(minmax : (tup, tup))
		(pts1 : skyData tup)
		(pts2 : skyData tup)
	: skyData tup =
		let pts1' = pts1 |> filter_against subdiv_per_dim minmax pts2
		let pts2' = pts2 |> filter_against subdiv_per_dim minmax pts1'
		in pts1' ++ pts2'
}

-- | Module for numeric skyline utilising angular subdivisions.
-- This allows for cheap pre-filtering by pivot points.
--
-- Based on traditional MR-Angle.
module skyline_angle
	(V : vector)
	(F : real)
	(S : skyline_base with t = F.t with tup = V.vector F.t)
= {
	type vector 'a = V.vector a
	type t = F.t
	type tup = vector t

	module skyline = mk_skyline V F S

	local def from_i64 = F.i64
	local def to_i64 = F.to_i64

	local def zero = F.i32 0

	local def lt = (F.<)
	local def eq = (F.==)

	local def plus = (F.+)
	local def minus = (F.-)
	local def times = (F.*)
	local def over = (F./)
	local def abs = (F.abs)
	local def sqrt = F.sqrt
	local def atan2 = F.atan2

	-- | Get the square of the euclidean distance of pt2 from pt1.
	def dist (pt1 : tup) (pt2 : tup) : t =
		pt2 |> V.map2 (\x1 x2 -> (F.-) x1 x2) pt1
			|> V.map (\x -> x `times` x)
			|> V.reduce (plus) zero

	-- | Obtain spherical coordinates (without radius) from cartesian.
	-- This only returns the angular phi coordinates, radius is ignored.
	def cartesian_to_spherical (pivot : tup) (pt : tup) : [V.length-1]t =
		iota (V.length-1) |> seqmap zero (\d ->
				let this_x = minus (V.get d pt) (V.get d pivot) |> abs
				let this_y = V.iota
					|> V.map (\i ->
						if i<=d
						then zero
						else minus (V.get i pt) (V.get i pivot) |> abs
					)
					|> V.map (\x -> x `times` x)
					|> V.reduce (plus) zero
					|> sqrt
				in atan2 this_y this_x
			)

	-- | Get the prefix product of the angular subdivisions.
	def get_angle_prefix
		(subdiv : [V.length-1]i64)
	: [V.length-1]i64 = subdiv |> exscan (*) 1

	-- | Get the angle subdivision id of a single point.
	def get_angle_id
		(subdiv : [V.length-1]i64)
		(angle_prefix : [V.length-1]i64)
		(pivot : tup)
		(pt : tup)
	: i64 =
		let pt' = pt |> cartesian_to_spherical pivot
		let perDim = map2 (over) pt' (subdiv |> map (from_i64))
		in perDim |> map (to_i64)
			|> map2 (i64.min) (map (\s -> s-1) subdiv) -- truncate to subdiv
			|> map2 (*) angle_prefix
			|> reduce (+) 0

	-- | Get the angle subdivision id's for all pts.
	def mk_angle
		(subdiv : [V.length-1]i64)
		(angle_prefix : [V.length-1]i64)
		(pivot : tup)
		(pts : skyData tup)
	: []i64 =
		pts |> map ((.0) >-> get_angle_id subdiv angle_prefix pivot)

	-- | Get the closest pt to the pivot per partition.
	def get_closests
		(minmax : (tup,tup))
		(angle_ids : []i64)
		(pts : skyData tup)
	: []i64 =
		let n = length pts
		let num_subdiv = 1 + (i64.maximum angle_ids)
		let pts' = pts |> map ((.0) >-> dist minmax.0) |> sized n
			|> zip (iota n)
		in hist (\(i1,d1) (i2,d2) -> if (d1`lt`d2) || ((d1`eq`d2) && i1<i2) then (i1,d1) else (i2,d2))
			(n, dist minmax.1 minmax.0)
			num_subdiv
			(angle_ids |> sized n) pts'
		|> map (.0)

	-- | Get the pts with the best value for each dimension, per partition.
	def get_bests
		(minmax : (tup,tup))
		(angle_ids : []i64)
		(pts : skyData tup)
	: []i64 =
		let n = length pts
		let num_subdiv = 1 + (i64.maximum angle_ids)
		in iota (V.length) |> seqmap (replicate num_subdiv (-1))
			(\d ->
				let pts' = pts |> map (\(pt,_) -> V.get d pt) |> sized n |> zip (iota n)
				in hist (\(i1,v1) (i2,v2) -> if (S.bt v1 v2) || ((S.beq v1 v2) && i1<i2) then (i1,v1) else (i2,v2))
					(n, V.get d minmax.1)
					num_subdiv
					(angle_ids |> sized n) pts'
				|> map (.0)
			)
		|> flatten

	-- | Local filtering.
	def filter_self
		(use_many_pts : bool)
		(minmax : (tup,tup))
		(subdiv : [V.length-1]i64)
		(pts : skyData tup)
	: skyData tup =
		let angle_prefix = get_angle_prefix subdiv
		let angle_ids = pts |> mk_angle subdiv angle_prefix minmax.0
		let closests = pts |> get_closests minmax angle_ids
		let pts' = pts |> skyline.filter_against (closests |> map (\i -> pts[i]))
		let angle_ids' = pts' |> mk_angle subdiv angle_prefix minmax.0
		let bests = if use_many_pts then pts' |> get_bests minmax angle_ids'
			else []
		in pts' |> skyline.filter_against (bests |> map (\i -> pts'[i]))

	-- | Apply a sequence of local filters.
	def multi_filter_self
		(use_many_pts : bool)
		(minmax : (tup,tup))
		(subdiv_min : i64)
		(subdiv_max : i64)
		(subdiv_step : i64)
		(size_thresh : i64)
		(pts : skyData tup)
	: skyData tup =
		let (pts', _) = loop (xs,subdiv) = (pts, subdiv_min)
		while (length xs) > size_thresh && subdiv<=subdiv_max do
			let (xs',_) = loop (ys, d) = (xs, 0)
			while (length ys > size_thresh && d<V.length-1) do
				let cur_subdiv = (replicate (V.length-1) 1) with [d]=subdiv
				let ys' = ys |> filter_self
					use_many_pts minmax cur_subdiv
				in (ys', d+1)
			in (xs', subdiv + subdiv_step)
		in pts'
}
