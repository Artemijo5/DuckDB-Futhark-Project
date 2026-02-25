import "../../ftDBSCAN"

-- Second step of DBSCAN (f32 data).
-- Organise core points into clusters.
--
-- ==
-- entry: dbscan2_f32
-- input @data/sparse2d_f32.in
-- auto output
-- input @data/dense2d_f32.in
-- auto output
-- input @data/sparse3d_f32.in
-- auto output
-- input @data/dense3d_f32.in
-- auto output

entry dbscan2_f32 [n] [nc] [d] (dat : [n][d]f32) (corePts : [nc][d]f32)
: ([n][d]f32, [nc][d]f32, [nc]i64) =
	let cids = dbscan_float.new_find_cluster_ids corePts 2.0 n (i64.highest)
	in (dat, corePts, cids)

-- Second step of DBSCAN (f64 data).
-- Organise core points into clusters.
--
-- ==
-- entry: dbscan2_f64
-- input @data/sparse2d_f64.in
-- auto output
-- input @data/dense2d_f64.in
-- auto output
-- input @data/sparse3d_f64.in
-- auto output
-- input @data/dense3d_f64.in
-- auto output

entry dbscan2_f64 [n] [nc] [d] (dat : [n][d]f64) (corePts : [nc][d]f64)
: ([n][d]f64, [nc][d]f64, [nc]i64) =
	let cids = dbscan_double.new_find_cluster_ids corePts 2.0 n (i64.highest)
	in (dat, corePts, cids)