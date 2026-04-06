import "../../skyline/skyline_entry"

-- Benchmark skyline for 2d data.
-- ==
-- entry: do_skyline
-- random input { [2][100]f64 }
-- random input { [2][1000]f64 }
-- random input { [2][10000]f64 }

entry do_skyline
	(dat : [2][]f64)
: [2][]f64 =
	let asBuff : skyBuffer_f64 [2] = {len = length dat[0], dat = dat, is = indices dat[0]}
	let buff = skyline2_f64_min.do_skyline
		true true false true ([3] :> [2-1]i64)
		1 1 1 100
		[1,1] 100 asBuff
	in buff.dat