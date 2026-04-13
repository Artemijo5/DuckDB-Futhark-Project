import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "skyline_base"
import "skyline_subdiv"

-- | Implementation of the overall skyline logic.
-- Uses a combination of elimination techniques:
-- 1. angular subdivisions to cheaply filter points.
-- 2. count-based windowing for divide-and-conquer.
-- 3. grid-based subdivisions to avoid unnecessary comparisons.
-- Externally, can use accumulation to avoid even more unecessary comparisons.
module skyline_pipeline
	(V : vector)
	(F : real)
	(S : skyline_base with t = F.t with tup = V.vector F.t)
= {
	type vector 'a = V.vector a
	type t = F.t
	type tup = vector t

	module skyline = mk_skyline V F S
	module grid  = skyline_grid V F S
	module angle = skyline_angle V F S

	-- | Function that internally implements the combined skyline logic.
	-- This logic can also be followed for external pipelining.
	-- External pipelining would also allow for:
	-- 2. arbitrary windowing
	def do_skyline
		(skip_local : bool)
		(skip_multi : bool)
		(use_many_local : bool)
		(use_many_multi : bool)
		(angle_subdiv : [V.length - 1]i64)
		(multi_subdiv_min : i64)
		(multi_subdiv_max : i64)
		(multi_subdiv_step : i64)
		(multi_size_thresh : i64)
		(grid_subdiv : [V.length]i64)
		(window_size : i64)
		(pts : skyData tup)
	: skyData tup =
		let minmax = skyline.get_minmax pts
		let g_subdiv = V.from_array grid_subdiv
		let pts_loc = if skip_local then pts
			else pts |> angle.filter_self use_many_local minmax angle_subdiv
		let pts_loc' = if skip_multi then pts_loc
			else pts_loc |> angle.multi_filter_self use_many_multi minmax
				multi_subdiv_min multi_subdiv_max multi_subdiv_step
				multi_size_thresh
		let n = length pts_loc'
		let num_iter = (n + window_size - 1) / window_size
		in loop skyDat : skyData tup = [] for j<num_iter do
			let inf = j*window_size
			let sup = i64.min (inf + window_size) n
			let cur_dat = pts_loc'[inf:sup]
				|> grid.filter_self g_subdiv minmax
			in grid.merge g_subdiv minmax skyDat cur_dat
-- Following leads to memory allocation errors...
	--		let (skyDat,_,_,_) = loop (acc0,acc1,acc2,acc3)
	--		: (skyData tup, skyData tup, skyData tup, skyData tup) = ([],[],[],[])
	--		for j<num_iter do
	--			let inf = j*window_size
	--			let sup = i64.min (inf + window_size) n
	--			let dat = pts_loc'[inf:sup]
	--				|> grid.filter_self g_subdiv minmax
	--			-- merge with 1st accumulator
	--			let acc3' = grid.merge g_subdiv minmax acc3 dat
	--			-- merge with 2nd accumulator if the 1st has exceeded it
	--			let transf32 = (length acc3')>=(length acc2) || (j==num_iter-1)
	--			let acc2' = if !transf32 then acc2
	--				else grid.merge g_subdiv minmax acc2 acc3'
	--			-- merge with 3rd accumulator if the 2nd has exceeded it
	--			let transf21 = (length acc2')>=(length acc1) || (j==num_iter-1)
	--			let acc1' = if !transf21 then acc1
	--				else grid.merge g_subdiv minmax acc1 acc2'
	--			-- merge with final accumulator if the 3rd has exceeded it
	--			let transf10 = (length acc1')>=(length acc0) || (j==num_iter-1)
	--			let acc0' = if !transf10 then acc0
	--				else grid.merge g_subdiv minmax acc0 acc1'
	--			in (
	--				acc0',
	--				if !transf10 then acc1' else [],
	--				if !transf21 then acc2' else [],
	--				if !transf32 then acc3' else []
	--			)
	--		in skyDat
}

type~ skyBuffer [d] 't = {
	len : i64,
	dat : [d][]t,
	is  : []i64
}

type~ skyBuffer_f32 [d] = skyBuffer [d] f32
type~ skyBuffer_f64 [d] = skyBuffer [d] f64

-- | Module providing array-based entry points for skyline_pipeline.
module skyline_entry
	(V : vector)
	(F : real)
	(S : skyline_base with t = F.t with tup = V.vector F.t)
= {
	type vector 'a = V.vector a
	type t = F.t
	type tup = vector t

	module skyline = skyline_pipeline V F S

	local def buffer_to_vectors [d] (buff : skyBuffer [d] t)
	: skyData tup =
		let as_vecs = buff.dat |> transpose
			|> map (sized V.length) |> map (V.from_array)
		in zip
			(as_vecs |> sized buff.len)
			(buff.is |> sized buff.len)

	local def buffer_from_vectors (pts : skyData tup)
	: skyBuffer [V.length] t =
		let len = length pts
		let (vecs,is) = pts |> unzip
		let dat = vecs |> map (V.to_array) |> transpose
		in {len=len,dat=dat,is=is}

	def do_skyline [d]
		(skip_local : bool)
		(skip_multi : bool)
		(use_many_local : bool)
		(use_many_multi : bool)
		(angle_subdiv : [d-1]i64)
		(multi_subdiv_min : i64)
		(multi_subdiv_max : i64)
		(multi_subdiv_step : i64)
		(multi_size_thresh : i64)
		(grid_subdiv : [d]i64)
		(window_size : i64)
		(pts : skyBuffer [d] t)
	: skyBuffer [d] t = pts
		|> buffer_to_vectors
		|> skyline.do_skyline skip_local skip_multi use_many_local use_many_multi
			(angle_subdiv |> sized (V.length-1))
			multi_subdiv_min multi_subdiv_max multi_subdiv_step multi_size_thresh
			(grid_subdiv |> sized V.length) window_size
		|> buffer_from_vectors :> skyBuffer [d] t
}

import "../vector_cols"

-- Entry points

module array_cols_f32 = array_cols_numeric f32
entry init_cols_f32 = array_cols_f32.init_cols
entry crop_cols_f32 = array_cols_f32.crop_cols
entry write_col_f32 = array_cols_f32.write_col
entry read_col_f32 = array_cols_f32.read_col

module array_cols_f64 = array_cols_numeric f64
entry init_cols_f64 = array_cols_f64.init_cols
entry crop_cols_f64 = array_cols_f64.crop_cols
entry write_col_f64 = array_cols_f64.write_col
entry read_col_f64 = array_cols_f64.read_col

module skyline2_f64_min = skyline_entry vector_2 f64 (mk_skyline_minwise vector_2 f64)
module skyline3_f64_min = skyline_entry vector_3 f64 (mk_skyline_minwise vector_3 f64)
module skyline4_f64_min = skyline_entry vector_4 f64 (mk_skyline_minwise vector_4 f64)
module skyline5_f64_min = skyline_entry vector_5 f64 (mk_skyline_minwise vector_5 f64)
module skyline6_f64_min = skyline_entry vector_6 f64 (mk_skyline_minwise vector_6 f64)
module skyline7_f64_min = skyline_entry vector_7 f64 (mk_skyline_minwise vector_7 f64)
module skyline8_f64_min = skyline_entry vector_8 f64 (mk_skyline_minwise vector_8 f64)
module skyline9_f64_min = skyline_entry vector_9 f64 (mk_skyline_minwise vector_9 f64)
module skyline10_f64_min = skyline_entry vector_10 f64 (mk_skyline_minwise vector_10 f64)
module skyline11_f64_min = skyline_entry vector_11 f64 (mk_skyline_minwise vector_11 f64)
module skyline12_f64_min = skyline_entry vector_12 f64 (mk_skyline_minwise vector_12 f64)

module skyline2_f64_max = skyline_entry vector_2 f64 (mk_skyline_maxwise vector_2 f64)
module skyline3_f64_max = skyline_entry vector_3 f64 (mk_skyline_maxwise vector_3 f64)
module skyline4_f64_max = skyline_entry vector_4 f64 (mk_skyline_maxwise vector_4 f64)
module skyline5_f64_max = skyline_entry vector_5 f64 (mk_skyline_maxwise vector_5 f64)
module skyline6_f64_max = skyline_entry vector_6 f64 (mk_skyline_maxwise vector_6 f64)
module skyline7_f64_max = skyline_entry vector_7 f64 (mk_skyline_maxwise vector_7 f64)
module skyline8_f64_max = skyline_entry vector_8 f64 (mk_skyline_maxwise vector_8 f64)
module skyline9_f64_max = skyline_entry vector_9 f64 (mk_skyline_maxwise vector_9 f64)
module skyline10_f64_max = skyline_entry vector_10 f64 (mk_skyline_maxwise vector_10 f64)
module skyline11_f64_max = skyline_entry vector_11 f64 (mk_skyline_maxwise vector_11 f64)
module skyline12_f64_max = skyline_entry vector_12 f64 (mk_skyline_maxwise vector_12 f64)

-- | Entry point to merge 2 skyBuffers.
-- This does no eliminations: will have to apply filtering function afterwards.
entry merge_skyBuffers_f64 [d] (skB1 : skyBuffer_f64 [d]) (skB2 : skyBuffer_f64 [d])
: skyBuffer_f64 [d] = {
	len = skB1.len +  skB2.len,
	dat = skB1.dat ++ skB2.dat,
	is  = skB1.is  ++ skB2.is
}

entry do_skyline2_f64_min : bool -> bool -> bool -> bool -> [2-1]i64
	-> i64 -> i64 -> i64 -> i64 -> [2]i64 -> i64 -> skyBuffer_f64 [2]
	-> skyBuffer_f64 [2] = skyline2_f64_min.do_skyline
entry do_skyline3_f64_min : bool -> bool -> bool -> bool -> [3-1]i64
	-> i64 -> i64 -> i64 -> i64 -> [3]i64 -> i64 -> skyBuffer_f64 [3]
	-> skyBuffer_f64 [3] = skyline3_f64_min.do_skyline

entry do_skyline2_f64_max : bool -> bool -> bool -> bool -> [2-1]i64
	-> i64 -> i64 -> i64 -> i64 -> [2]i64 -> i64 -> skyBuffer_f64 [2]
	-> skyBuffer_f64 [2] = skyline2_f64_max.do_skyline
entry do_skyline3_f64_max : bool -> bool -> bool -> bool -> [3-1]i64
	-> i64 -> i64 -> i64 -> i64 -> [3]i64 -> i64 -> skyBuffer_f64 [3]
	-> skyBuffer_f64 [3] = skyline3_f64_max.do_skyline

