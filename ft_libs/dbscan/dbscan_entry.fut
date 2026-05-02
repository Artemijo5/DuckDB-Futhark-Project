import "../vector_cols"
import "../ftColumns"

import "ft_spindex"
import "ft_distance"

import "dbscan"

-- Modules for 2d & 3d dbscan entry points.

module euclidean2_f64 = euclidean_d vector_2 f64
module euclidean3_f64 = euclidean_d vector_3 f64

module grid2_f64 = grid_index vector_2 f64
module grid3_f64 = grid_index vector_3 f64

module kd2_f64 = kd_index vector_2 f64
module kd3_f64 = kd_index vector_3 f64

module dbscan2_grid_f64 = ft_dbscan vector_2 f64 grid2_f64 euclidean2_f64
module dbscan3_grid_f64 = ft_dbscan vector_3 f64 grid3_f64 euclidean3_f64

module dbscan2_kd_f64 = ft_dbscan vector_2 f64 kd2_f64 euclidean2_f64
module dbscan3_kd_f64 = ft_dbscan vector_3 f64 kd3_f64 euclidean3_f64

type dbscan_result [n] = {is_core : [n]bool, cluster_id : [n]i64}

module col_f64 = col_numeric f64

-- Entry points for loading data.

entry init_column_f64 = col_f64.mk_keyCol
entry write_column_f64 = col_f64.update_keyCol
entry crop_column_f64 = col_f64.crop_keyCol

-- indexing + DBSCAN entry points

entry do_grid_dbscan_2d_f64 [n]
	(extPar : i64)
	(subdiv : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
: dbscan_result [n] =
	let pts = map2 (\x1 x2 -> vector_2.replicate x1 |> vector_2.set 1 x2) dat1 dat2
	let (pts',p_minmax,p_is,is) = grid2_f64.index_dataset (replicate 2 subdiv) pts
	let np = length p_minmax
	let buff = dbscan2_grid_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster}

entry do_kd_dbscan_2d_f64 [n]
	(extPar : i64)
	(depth : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
: dbscan_result [n] =
	let pts = map2 (\x1 x2 -> vector_2.replicate x1 |> vector_2.set 1 x2) dat1 dat2
	let (pts',p_minmax,p_is,is) = kd2_f64.index_dataset (replicate 1 depth) pts
	let np = length p_minmax
	let buff = dbscan2_kd_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster}

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
	let (pts',p_minmax,p_is,is) = grid3_f64.index_dataset (replicate 3 subdiv) pts
	let np = length p_minmax
	let buff = dbscan3_grid_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster}

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
	let (pts',p_minmax,p_is,is) = kd3_f64.index_dataset (replicate 1 depth) pts
	let np = length p_minmax
	let buff = dbscan3_kd_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster}