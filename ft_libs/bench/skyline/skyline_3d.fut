import "../../skyline/skyline_entry"

-- Benchmark skyline for 3d data.
-- ==
-- entry: do_skyline
-- input @data/dat.in
-- auto output

entry do_skyline
	(angle_subdiv : [1]i64)
	(multi_subdiv_min : [1]i64)
	(multi_subdiv_max : [1]i64)
	(multi_subdiv_step : [1]i64)
	(multi_size_thresh : [1]i64)
	(grid_subdiv : [1]i64)
	(window_size : [1]i64)
	(dat : [3][]f64)
: [3][]f64 =
	let asBuff : skyBuffer_f64 [3] = {len = length dat[0], dat = dat, is = indices dat[0]}
	let buff = skyline2_f64_min.do_skyline
		false true false true (replicate (3-1) angle_subdiv[0])
		multi_subdiv_min[0] multi_subdiv_max[0] multi_subdiv_step[0] multi_size_thresh[0]
		(replicate 3 grid_subdiv[0]) window_size[0] asBuff
	in buff.dat