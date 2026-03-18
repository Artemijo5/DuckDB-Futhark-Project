import "ftbasics"
import "lib/github.com/athas/vector/vector"
import "vector_cols"
import "lib/github.com/diku-dk/sorts/merge_sort"

-- Implementation of MR-Angle Skyline

type mrAngleInfo_t [d_minusOne] 't = {
	subdiv_per_dim : [d_minusOne]i64,
	subdiv_total   : i64,
	subdiv_prefix  : [d_minusOne]i64
}
type~ skylineBuffers_t [dim] 't 'pL_t = {
	len : i64,
	dat : [dim][]t,
	pL  : []pL_t
}

module mk_Skyline_real (V : vector) (F : real) = {
	type vector 'a = V.vector a
	type t = F.t

	module vcs = vec_cols V

	type~ skyData 'pL_t = []((vector t), pL_t)

	local def dim = V.length

	type mrAngleInfo = mrAngleInfo_t [dim - 1] t
	type~ skylineBuffers 'pL_t = skylineBuffers_t [dim] t pL_t

	local def gt = (F.>)
	local def geq = (F.>=)
	local def lt = (F.<)
	local def leq = (F.<=)
	local def eq = (F.==)

	local def plus = (F.+)
	local def minus = (F.-)
	local def times = (F.*)
	local def over = (F./)

	local def from_i64 = (F.i64)
	local def to_i64 = (F.to_i64)

	local def atan2 = (F.atan2)
	local def sqrt = (F.sqrt)

	local def zero = F.i32 0
	local def one = F.i32 1
	local def piSeconds = over (F.pi) (F.i32 2)
	local def highest = F.highest

	local def stdfoldl = foldl
	local def stdall = all
	local def stdany = any

	local def foldl 'a f ne (v : vector a) =
		loop acc = ne
		for j < dim do f acc (V.get j v)
	local def all = foldl (&&) true
	local def any = foldl (||) false

	-- Check if pt1 dominates pt2 (using Pareto Optimality criterion)
	def pt_dominated (pt1 : vector t) (pt2 : vector t) : bool =
		-- TODO if this won't compile, replace with foldl
		let all_leq = pt2 |> V.map2 (leq) pt1 |> all
		let any_lt  = pt2 |> V.map2 (lt)  pt1 |> any
		in all_leq && any_lt

	-- Check if pt is dominated by any point in pts
	def pt_dominatedAgainst 'pL_t (pts : skyData pL_t) (pt : vector t) : bool =
		pts |> map (.0) |> stdany (\x -> pt |> pt_dominated x)

	-- Flag points from pts2 that are dominated by any point in pts1
	def dominatedAgainst 'pL_t (pts1 : skyData pL_t) (pts2 : skyData pL_t) : []bool =
		pts2 |> map (.0) |> map (\pt -> pt |> pt_dominatedAgainst pts1)

	-- Filter pts2 that are dominated by any point in pts1
	def filterAgainst 'pL_t (pts1 : skyData pL_t) (pts2 : skyData pL_t) : skyData pL_t =
		pts2 |> dominatedAgainst pts1 |> zip pts2 |> filter ((.1) >-> not) |> map (.0)

	-- Filter points from pts that are dominated by any other points in pts
	def filterSelf 'pL_t (pts : skyData pL_t) : skyData pL_t =
		pts |> filterAgainst pts

	-- Make a scheme for angular subdivisions
	def mk_angularSubdivScheme (subdiv_per_dim : [dim - 1]i64)
	: mrAngleInfo = {
		subdiv_per_dim = subdiv_per_dim,
		subdiv_total   = subdiv_per_dim |> i64.sum,
		subdiv_prefix  = subdiv_per_dim |> exscan (+) 0
	}
	-- Same #subdivisions across dimensions
	def mk_angularSubdivScheme_uniform (subdiv : i64)
	: mrAngleInfo = subdiv
		|> replicate (dim-1)
		|> mk_angularSubdivScheme
	-- Only one dimension is subdivided
	def mk_angularSubdivScheme_singular (atDim : i64) (subdiv : i64)
	: mrAngleInfo = ((replicate (dim-1) 164) with [atDim] = subdiv)
		|> mk_angularSubdivScheme

	-- Functions to initialise data (from vector_cols)

	def init_cols n
		= vcs.init_cols highest n
	def create_cols
		: [][]t -> [](vector t)
		= vcs.create_cols
	def write_cols 
		: i64 -> [](vector t) -> *[](vector t) -> [](vector t)
		= vcs.write_cols
	def set_col
		: i64 -> []t -> *[](vector t) -> [](vector t)
		= vcs.set_col
	def get_col
		: i64 -> [](vector t) -> []t
		= vcs.get_col
	def read_col
		: i64 -> i64 -> i64 -> [](vector t) -> []t
		= vcs.read_col
	def crop_cols
		: i64 -> i64 -> *[](vector t) -> [](vector t)
		= vcs.crop_cols

	-- Functions to wrap & unwrap vector columns as skyData

	def cols_to_skyData
		(offs:i64) (cols : [](vector t)) : skyData i64
		= cols |> indices |> map (\i -> i+offs) |> zip cols
	def skyData_to_cols
		: skyData i64 -> [](vector t)
		= map (.0)
	def skyData_to_indices
		: skyData i64 -> []i64
		= map (.1)

	def cols_to_skyData_withPL [b]
		: [](vector t) -> [](byteSeq [b]) -> skyData (byteSeq [b])
		= zip
	def skyData_withPL_to_cols [b]
		: skyData (byteSeq [b]) -> [](vector t)
		= map (.0)
	def skyData_withPL_to_pL [b]
		: skyData (byteSeq [b]) -> [](byteSeq [b])
		= map (.1)

	-- TODO continue with skyline routines
}