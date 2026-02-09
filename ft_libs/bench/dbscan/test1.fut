import "../../ftDBSCAN"

-- First step of DBSCAN (f32 data).
-- Identify & isolate core points.
--
-- ==
-- entry: dbscan1_f32
-- input @data/sparse2d_f32.in
-- auto output
-- input @data/dense2d_f32.in
-- auto output
-- input @data/sparse3d_f32.in
-- auto output
-- input @data/dense3d_f32.in
-- auto output

entry dbscan1_f32 [n] [d] (dat : [n][d]f32)
: ([n][d]f32, [][d]f32) =
	let neighCounts = dbscan_float.get_num_neighbours dat 2.0 n
	let isCore = dbscan_float.find_core_points neighCounts (d+2)
	let corePts = dbscan_float.isolate_core_points dat isCore
	in (dat, corePts)

-- First step of DBSCAN (f64 data).
-- Identify & isolate core points.
--
-- ==
-- entry: dbscan1_f64
-- input @data/sparse2d_f64.in
-- auto output
-- input @data/dense2d_f64.in
-- auto output
-- input @data/sparse3d_f64.in
-- auto output
-- input @data/dense3d_f64.in
-- auto output

entry dbscan1_f64 [n] [d] (dat : [n][d]f64)
: ([n][d]f64, [][d]f64) =
	let neighCounts = dbscan_double.get_num_neighbours dat 2.0 (i64.highest)
	let isCore = dbscan_double.find_core_points neighCounts (d+2)
	let corePts = dbscan_double.isolate_core_points dat isCore
	in (dat, corePts)