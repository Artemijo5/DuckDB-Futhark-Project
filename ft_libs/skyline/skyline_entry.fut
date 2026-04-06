import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "skyline_base"
import "skyline_subdiv"

-- | Implementation of the overall skyline logic.
-- Uses a combination of elimination techniques:
-- 1. angular subdivisions to cheaply filter points.
-- 2. count-based windowing for divide-and-conquer.
-- 3. grid-based subdivisions to avoid unnecessary comparisons.
-- 4. accumulation to eliminate points before they'd be compared with the entire dataset.
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
		let (skyDat,_,_,_) = loop (acc0,acc1,acc2,acc3)
		: (skyData tup, skyData tup, skyData tup, skyData tup) = ([],[],[],[])
		for j<num_iter do
			let inf = j*window_size
			let sup = i64.min (inf + window_size) n
			let dat = pts_loc'[inf:sup]
				|> grid.filter_self g_subdiv minmax
			-- merge with 1st accumulator
			let acc3' = grid.merge g_subdiv minmax acc3 dat
			-- merge with 2nd accumulator if the 1st has exceeded it
			let transf32 = (length acc3')>=(length acc2) || (j==num_iter-1)
			let acc2' = if !transf32 then acc2
				else grid.merge g_subdiv minmax acc2 acc3'
			-- merge with 3rd accumulator if the 2nd has exceeded it
			let transf21 = (length acc2')>=(length acc1) || (j==num_iter-1)
			let acc1' = if !transf21 then acc1
				else grid.merge g_subdiv minmax acc1 acc2'
			-- merge with final accumulator if the 3rd has exceeded it
			let transf10 = (length acc1')>=(length acc0) || (j==num_iter-1)
			let acc0' = if !transf10 then acc0
				else grid.merge g_subdiv minmax acc0 acc1'
			in (
				acc0',
				if !transf10 then acc1' else [],
				if !transf21 then acc2' else [],
				if !transf32 then acc3' else []
			)
		in skyDat
}

module vector_2 = cat_vector vector_1 vector_1

module skyline2_f64 = skyline_pipeline vector_2 f64 (mk_skyline_minwise vector_2 f64)

entry do_skyline2_f64 = skyline2_f64.do_skyline

