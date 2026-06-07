import "../vector_cols"
import "../ftColumns"

import "ft_distance"

import "dbscan"

-- Modules for 2d & 3d dbscan entry points.

module euclidean2_f64 = euclidean_d vector_2 f64
module euclidean3_f64 = euclidean_d vector_3 f64

module dbscan2_f64 = ft_dbscan vector_2 f64 euclidean2_f64
module dbscan3_f64 = ft_dbscan vector_3 f64 euclidean3_f64

type dbscan_result [n] = {is_core : [n]bool, cluster_id : [n]i64}

module col_f64 = col_numeric f64

-- Entry points for loading data.

entry init_column_f64 = col_f64.mk_keyCol
entry write_column_f64 = col_f64.update_keyCol
entry crop_column_f64 = col_f64.crop_keyCol

def tup_to_res [n] (tup : ([n]bool, [n]i64))
: dbscan_result [n] = {is_core = tup.0, cluster_id = tup.1}

-- DBSCAN entry points
-- redudant definitions because used to perform different indexing techniques

entry do_grid_dbscan_2d_f64 [n]
	(extPar : i64)
	(subdiv : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
: dbscan_result [n] =
	let pts = map2 (\x1 x2 -> vector_2.replicate x1 |> vector_2.set 1 x2) dat1 dat2
	in dbscan2_f64.internal_dbscan extPar eps minPts pts
		|> tup_to_res

entry do_kd_dbscan_2d_f64 [n]
	(extPar : i64)
	(depth : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
: dbscan_result [n] =
	let pts = map2 (\x1 x2 -> vector_2.replicate x1 |> vector_2.set 1 x2) dat1 dat2
	in dbscan2_f64.internal_dbscan extPar eps minPts pts
		|> tup_to_res

entry do_grid_dbscan_3d_f64 [n]
	(extPar : i64)
	(subdiv : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
	(dat3 : [n]f64)
: dbscan_result [n] =
	let pts = map3 (\x1 x2 x3 ->
		vector_3.replicate x1 |> vector_3.set 1 x2 |> vector_3.set 2 x3
	) dat1 dat2 dat3
	in dbscan3_f64.internal_dbscan extPar eps minPts pts
		|> tup_to_res

entry do_kd_dbscan_3d_f64 [n]
	(extPar : i64)
	(depth : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
	(dat3 : [n]f64)
: dbscan_result [n] =
	let pts = map3 (\x1 x2 x3 ->
		vector_3.replicate x1 |> vector_3.set 1 x2 |> vector_3.set 2 x3
	) dat1 dat2 dat3
	in dbscan3_f64.internal_dbscan extPar eps minPts pts
		|> tup_to_res