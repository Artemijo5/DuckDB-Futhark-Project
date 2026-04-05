import "../ftbasics"
import "../lib/github.com/athas/vector/vector"

-- | Array of tuples holding
-- 1. elements of type a (performing skyline on)
-- 2. i64 row indices
type~ skyData 'a = [](a,i64)

-- | Module type with basic functions for Pareto Optimality.
-- Further developed in numeric modules.
-- Assume tup holds multiple fields of the same type,
-- and the selection criterion treats all fields the same.
module type skyline_base = {
	type t -- type of each field of the datapoints
	type tup -- type of datapoints as tuples

	-- | Is the 1st value better than or as good as the 2nd wrt the selection criterion?
	val beq : t -> t -> bool
	-- | Is the 1st value strictly better than the 2nd wrt the selection criterion?
	val bt : t -> t -> bool
	-- | Select the better out of 2 values.
	val better : t -> t -> t
	-- | ne for better
	val ne : t
	-- | Select the worse out of 2 values.
	val worse : t -> t -> t
	-- | ne for worse
	val wne : t
	
	-- | Is the 2nd point dominated by the first?
	val pt_dominated : tup -> tup -> bool

	-- | Select a pivot point among points,
	-- using the better values for each field.
	-- This is meant to be passed as an aggregate function to reduce_comm or hist.
	-- This pivot can be used for optimization techniques like angular subdivisions.
	val mk_pivot : tup -> tup -> tup

	-- | Opposite of pivot selection.
	-- Return the worse values for each field among points.
	val mk_antipivot : tup -> tup -> tup
}

-- | Skyline for numeric tuples, where x dominates y
-- when for all dimensions, x<=y
-- and for at least one, x<y
module mk_skyline_minwise (V : vector) (N : numeric)
: skyline_base with t = N.t with tup = (V.vector N.t) = {
	type t = N.t
	type tup = V.vector N.t

	def beq = (N.<=)
	def bt = (N.<)
	def better = N.min
	def ne = N.highest
	def worse = N.max
	def wne = N.lowest

	def pt_dominated pt1 pt2 : bool =
		(pt2 |> V.map2 (beq) pt1 |> V.reduce (&&) true)
			&&
		(pt2 |> V.map2 (bt) pt1 |> V.reduce (||) false)

	def mk_pivot pt1 pt2 : tup =
		V.map2 (better) pt1 pt2

	def mk_antipivot pt1 pt2 : tup =
		V.map2 (worse) pt1 pt2
}

-- | Skyline for numeric tuples, where x dominates y
-- when for all dimensions, x>=y
-- and for at least one, x>y
module mk_skyline_maxwise (V : vector) (N : numeric)
: skyline_base with t = N.t with tup = (V.vector N.t) = {
	type t = N.t
	type tup = V.vector N.t

	def beq = (N.>=)
	def bt = (N.>)
	def better = N.max
	def ne = N.lowest
	def worse = N.min
	def wne = N.highest

	def pt_dominated pt1 pt2 : bool =
		(pt2 |> V.map2 (beq) pt1 |> V.reduce (&&) true)
			&&
		(pt2 |> V.map2 (bt) pt1 |> V.reduce (||) false)

	def mk_pivot pt1 pt2 : tup =
		V.map2 (better) pt1 pt2

	def mk_antipivot pt1 pt2 : tup =
		V.map2 (worse) pt1 pt2
}


-- | Parametric module implementing simple skyline routines for numeric tuples.
-- No optimization techniques are applied here yet.
-- Points are held in (skyData tup) arrays, and those are filtered using domination criterion.
module mk_skyline
	(V : vector)
	(N : numeric)
	(S : skyline_base with t = N.t with tup = V.vector N.t)
= {
	type vector 'a = V.vector a
	type t = N.t
	type tup = vector t

	-- | Get the minimum bounding rectangle of the dataset.
	def get_minmax (pts : skyData tup) : (tup, tup) =
		let mi = pts |> map (.0)
			|> reduce_comm (S.mk_pivot) (V.replicate S.ne)
		let ma = pts |> map (.0)
			|> reduce_comm (S.mk_antipivot) (V.replicate S.wne)
		in (mi,ma)

	-- | Is pt dominated by any in pts?
	def pt_dominated_against
		(pts : skyData tup)
		(pt : tup)
	: bool =
		pts |> map (.0) |> any (\x -> pt |> S.pt_dominated x)

	-- | Filter pts2 that are dominated by any in pts1.
	def filter_against
		(pts1 : skyData tup)
		(pts2 : skyData tup)
	: skyData tup = pts2
		|> map ((.0) >-> (pt_dominated_against pts1))
		|> zip pts2
		|> filter ((.1) >-> not)
		|> map (.0)

	-- | Filter pts based on domination from amongst themselves.
	def filter_self (pts : skyData tup) : skyData tup
		= pts |> filter_against pts

	-- | Merge 2 skyData by filtering against each other.
	def merge (pts1 : skyData tup) (pts2 : skyData tup) : skyData tup =
		let pts1' = pts1 |> filter_against pts2
		let pts2' = pts2 |> filter_against pts1'
		in pts1' ++ pts2'
}