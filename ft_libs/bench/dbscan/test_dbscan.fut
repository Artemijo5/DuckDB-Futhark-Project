import "../../ftDBSCAN"

-- DBSCAN with f32 data.
-- Test 1: without compact list (use quadratic space)
-- Test 2: with compact list (G-DBSCAN)
-- Also separate runs for dbscan*.
--
-- ==
-- entry: dbscan_quadratic_f32 dbscan_list_f32 dbscanStar_quadratic_f32 dbscanStar_list_f32
-- input @data/pts_f32
-- auto output

entry dbscan_quadratic_f32 [n] [d] (pts : [n][d]f32) =
	ftDBSCAN_float pts 2.0 (d+2) (i64.highest) (i64.highest) false

entry dbscan_list_f32 [n] [d] (pts : [n][d]f32) =
	ftDBSCAN_float pts 2.0 (d+2) (i64.highest) (i64.highest) true

entry dbscanStar_quadratic_f32 [n] [d] (pts : [n][d]f32) =
	let star = ftDBSCAN_star_float pts 2.0 (d+2) (i64.highest) (i64.highest) false
	in (star.core_pts, star.core_ids)

entry dbscanStar_list_f32 [n] [d] (pts : [n][d]f32) =
	let star = ftDBSCAN_star_float pts 2.0 (d+2) (i64.highest) (i64.highest) true
	in (star.core_pts, star.core_ids)

-- DBSCAN with f64 data.
-- Test 1: without compact list (use quadratic space)
-- Test 2: with compact list (G-DBSCAN)
-- Also separate runs for dbscan*.
--
-- ==
-- entry: dbscan_quadratic_f64 dbscan_list_f64 dbscanStar_quadratic_f64 dbscanStar_list_f64
-- input @data/pts_f64
-- auto output

entry dbscan_quadratic_f64 [n] [d] (pts : [n][d]f64) =
	ftDBSCAN_double pts 2.0 (d+2) (i64.highest) (i64.highest) false

entry dbscan_list_f64 [n] [d] (pts : [n][d]f64) =
	ftDBSCAN_double pts 2.0 (d+2) (i64.highest) (i64.highest) true

entry dbscanStar_quadratic_f64 [n] [d] (pts : [n][d]f64) =
	let star = ftDBSCAN_star_double pts 2.0 (d+2) (i64.highest) (i64.highest) false
	in (star.core_pts, star.core_ids)

entry dbscanStar_list_f64 [n] [d] (pts : [n][d]f64) =
	let star = ftDBSCAN_star_double pts 2.0 (d+2) (i64.highest) (i64.highest) true
	in (star.core_pts, star.core_ids)