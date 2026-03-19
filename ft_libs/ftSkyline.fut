import "ftbasics"
import "lib/github.com/athas/vector/vector"
import "vector_cols"

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

	local def minimum = F.minimum

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
	def mk_angularSubdivScheme (subdiv_per_dim : []i64)
	: mrAngleInfo = {
		subdiv_per_dim = subdiv_per_dim |> sized (dim-1),
		subdiv_total   = subdiv_per_dim |> sized (dim-1) |> reduce (*) 1,
		subdiv_prefix  = subdiv_per_dim |> sized (dim-1) |> exscan (*) 1
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


	-- Obtain spherical coordinates (without radius) from cartesian
	def cartesian_to_spherical (pt : vector t) : [dim-1]t =
		iota (dim-1) |> seqmap zero (\d ->
				let this_x = V.get d pt
				let this_y = V.iota
					|> V.map (\i -> if i<=d then zero else V.get i pt)
					|> V.map (\x -> x `times` x)
					|> foldl (plus) zero
					|> sqrt
				in atan2 this_y this_x
			)

	-- get angular partition id's
	def getSubdivId (angleSchema : mrAngleInfo) (pt : vector t) : i64 =
		let angle_per_dim = angleSchema.subdiv_per_dim
			|> seqmap zero (\s -> s |> from_i64 |> over piSeconds)
		let pt_angles = pt |> cartesian_to_spherical
		in seqmap2 zero (over) pt_angles angle_per_dim
			|> seqmap 0 (to_i64)
			|> seqmap2 0 (\perDim sub ->
					if sub<0 then 0
					else if sub>=perDim then perDim-1
					else sub
				)
				angleSchema.subdiv_per_dim
			|> seqmap2 0 (\pref sub -> sub*pref) angleSchema.subdiv_prefix
			|> stdfoldl (+) 0

	-- pre-filtering

	def prefilter 'pL_t
		(use_many_pts : bool)
		(pts : skyData pL_t)
	: skyData pL_t =
		if (length pts) == 0 then pts else
		-- Obtain the pt with the smallest radius
		let closest_pt = pts
			|> map (.0)
			|> vcs.mapAll (\x -> x `times` x)
			|> vcs.foldlAll (plus) (zero)
			|> argmin (lt) (eq) (highest) -- its index
			|> replicate 1 -- its index as 1-elem array
			|> gather (pts[0]) pts -- the point as 1-elem array
		-- if use_many_pts: obtain pt's with smallest value per dim
		let smallest_per_dim : skyData pL_t = if !use_many_pts
			then [] :> [0](vector t, pL_t)
			else iota dim
				|> map (\d -> pts |> map (.0) |> vcs.get_col d)
				|> seqmap
					(-1)
					(argmin (lt) (eq) (highest))
				|> gather (pts[0]) pts
		let filterPts = closest_pt ++ smallest_per_dim
		in pts |> filterAgainst filterPts

	def localFilter 'pL_t
		(use_many_pts : bool)
		(angleSchema : mrAngleInfo)
		(pts : skyData pL_t)
	: skyData pL_t =
		if (length pts) == 0 then pts else
		-- Obtain angular subdivision id's
		let subdiv_ids = pts |> map (.0)
			|> map (getSubdivId angleSchema)
		-- Get closest points per angular subdivision
		let closests = hist
			(\i1 i2 ->
				if i1<0 then i2 else if i2<0 then i1 else
				let r1 = pts[i1].0
					|> V.map (\x -> x `times` x)
					|> foldl (plus) zero
				let r2 = pts[i2].0
					|> V.map (\x -> x `times` x)
					|> foldl (plus) zero
				in if (r1 `lt` r2) || ((r1 `eq` r2) && i1<i2) then i1 else i2
			)
			(-1)
			angleSchema.subdiv_total
			subdiv_ids
			(indices pts)
		|> gather (pts[0]) pts
		let smallests : skyData pL_t = if !use_many_pts
			then [] :> [0](vector t, pL_t)
			else (iota dim) |> map (\d ->
				hist
					(\i1 i2 ->
						if i1<0 then i2 else if i2<0 then i1 else
						let v1 = V.get d pts[i1].0
						let v2 = V.get d pts[i2].0
						in if (v1 `lt` v2) || ((v1 `eq` v2) && i1<i2) then i1 else i2
					)
					(-1)
					angleSchema.subdiv_total
					subdiv_ids
					(indices pts)
				|> gather (pts[0]) pts
			) |> flatten
		let filterPts = closests ++ smallests
		in pts |> filterAgainst filterPts

	def intermediateFilters 'pL_t
		(use_many_pts : bool)
		(max_subdiv : i64)
		(min_subdiv : i64)
		(num_schemes : i64)
		(size_thresh: i64)
		(pts : skyData pL_t)
	: skyData pL_t =
		if num_schemes<=0 then pts else
		let subdiv_step = (max_subdiv - min_subdiv) / num_schemes
		let (pts',_) =
			loop (xs, cur_subdiv) = (pts, max_subdiv)
			while (length xs)>size_thresh && cur_subdiv>min_subdiv do
				let (xs',_) = loop (ys,j) = (xs,0)
				while (length ys>size_thresh) && j<(dim-1) do
					let cur_scheme = mk_angularSubdivScheme_singular j cur_subdiv
					let ys' = ys |> localFilter use_many_pts cur_scheme
					in (ys', j+1)
				in (xs', cur_subdiv - subdiv_step)
		in pts'

	def skyline_merge 'pL_t (pts1 : skyData pL_t) (pts2 : skyData pL_t)
	: skyData pL_t =
		let pts1' = pts1 |> filterAgainst pts2
		let pts2' = pts2 |> filterAgainst pts1'
		in pts1' ++ pts2'

	-- Skyline pipeline within the Futhark context.
	-- This serves to illustrate how the pipeline may be implemented elsewhere,
	-- as well as provide a simple entry point if the dataset fits in memory.
	--
	-- The logic is to separate the dataset in sequentially accessed windows,
	-- and merge the windows through "accumulation layers".
	-- These acc layers help reduce the number of unnecessary pairwise comparisons,
	-- as non-skyline points have a chance to be eliminated before "reaching"
	-- the final layer where all current skyline points have been saved,
	-- whereas the number of necessary pairwise comparisons does not increase.
	--
	-- Set number of 3 acc layers.
	-- If this pipeline is instead implemented in C, then can in addition:
	-- 1. have an arbitrary number of acc layers
	-- 2. have actual windowing
	def skyline_internal 'pL_t
		(use_many_pts_prefi : bool)
		(use_many_pts_local : bool)
		(use_many_pts_inter : bool)
		(maxSubdiv_inter : i64)
		(minSubdiv_inter : i64)
		(numScheme_inter : i64)
		(interSizeThresh : i64)
		(localSubdiv : []i64)
		(window_size : i64)
		(dat : skyData pL_t)
	: skyData pL_t =
		let localScheme = mk_angularSubdivScheme localSubdiv
		let n = length dat
		let num_iter = (n + window_size - 1) / window_size
		let (skyDat,_,_) : (skyData pL_t, skyData pL_t, skyData pL_t) =
			loop (acc0,acc1,acc2) = ([],[],[]) for j < num_iter do
				let inf = j*window_size
				let sup = i64.min n ((j+1)*window_size)
				let this_dat = dat[inf:sup]
					|> prefilter use_many_pts_prefi
					|> localFilter use_many_pts_local localScheme
					|> intermediateFilters
						use_many_pts_inter
						maxSubdiv_inter
						minSubdiv_inter
						numScheme_inter
						interSizeThresh
					|> filterSelf
				let acc2' = skyline_merge acc2 this_dat
				let transf21 = (length acc2')>=(length acc1)
				let acc1' = if transf21
					then skyline_merge acc1 acc2'
					else acc1
				let transf10 = (length acc1')>=(length acc0)
				let acc0' = if transf10
					then skyline_merge acc0 acc1'
					else acc0
				in (
					acc0',
					if transf10 then [] else acc1',
					if transf21 then [] else acc2'
				)
		in skyDat
}

module skyline2_f64  = mk_Skyline_real vector_2 f64
module skyline3_f64  = mk_Skyline_real vector_3 f64
module skyline4_f64  = mk_Skyline_real vector_4 f64
module skyline5_f64  = mk_Skyline_real vector_5 f64
module skyline6_f64  = mk_Skyline_real vector_6 f64
module skyline7_f64  = mk_Skyline_real vector_7 f64
module skyline8_f64  = mk_Skyline_real vector_8 f64
module skyline9_f64  = mk_Skyline_real vector_9 f64
module skyline10_f64 = mk_Skyline_real vector_10 f64
module skyline11_f64 = mk_Skyline_real vector_11 f64
module skyline12_f64 = mk_Skyline_real vector_12 f64

type skyData2_f64 = skyline2_f64.skyData i64
type skyData3_f64 = skyline3_f64.skyData i64

