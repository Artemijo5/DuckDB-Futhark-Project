import "../../ftDBSCAN"

-- Third step of DBSCAN (f32 data).
-- Assign id's to border points, identify noise.
--
-- ==
-- entry: dbscan3_f32
-- input @data/sparse2d_f32.in
-- input @data/dense2d_f32.in
-- input @data/sparse3d_f32.in
-- input @data/dense3d_f32.in

entry dbscan3_f32 [n] [nc] [d] (dat : [n][d]f32) (corePts : [nc][d]f32) (core_cids : [nc]i64)
: ([n]i64) =
	dbscan_float.match_to_cluster_head dat corePts core_cids 2.0 (i64.highest)

-- Third step of DBSCAN (f64 data).
-- Assign id's to border points, identify noise.
--
-- ==
-- entry: dbscan3_f64
-- input @data/sparse2d_f64.in
-- input @data/dense2d_f64.in
-- input @data/sparse3d_f64.in
-- input @data/dense3d_f64.in

entry dbscan3_f64 [n] [nc] [d] (dat : [n][d]f64) (corePts : [nc][d]f64) (core_cids : [nc]i64)
: ([n]i64) =
	dbscan_double.match_to_cluster_head dat corePts core_cids 2.0 (i64.highest)