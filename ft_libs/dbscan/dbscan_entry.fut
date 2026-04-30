import "../vector_cols"

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

type v2_f64 = vector_2.vector f64
type v3_f64 = vector_3.vector f64

type dbscan_result [n] = {is_core : [n]bool, cluster_id : [n]i64}

-- Entry points for loading data.

entry init_cols_2d_f64 n : [n]v2_f64 = vcs2.init_cols 0f64 n
entry init_cols_3d_f64 n : [n]v3_f64 = vcs3.init_cols 0f64 n

entry write_cols_2d_f64 [n]
	i
	(dat1 : [n]f64)
	(dat2 : [n]f64)
	(vecs : *[]v2_f64)
: []v2_f64 = vecs with [i:i+n] = map2 (\x1 x2 ->
	vector_2.replicate x1 |> vector_2.set 1 x2
) dat1 dat2

entry write_cols_3d_f64 [n]
	i
	(dat1 : [n]f64)
	(dat2 : [n]f64)
	(dat3 : [n]f64)
	(vecs : *[]v3_f64)
: []v3_f64 = vecs with [i:i+n] = map3 (\x1 x2 x3 ->
	vector_3.replicate x1 |> vector_3.set 1 x2 |> vector_3.set 2 x3
) dat1 dat2 dat3

entry crop_cols_2d_f64 offs limt (vecs : *[]v2_f64)
	: []v2_f64 = vcs2.crop_cols offs limt vecs
entry crop_cols_3d_f64 offs limt (vecs : *[]v3_f64)
	: []v3_f64 = vcs3.crop_cols offs limt vecs

-- indexing + DBSCAN entry points

entry do_grid_dbscan_2d_f64 [n]
	(extPar : i64)
	(subdiv : i64)
	(eps : f64)
	(minPts : i64)
	(pts : [n]v2_f64)
: dbscan_result [n] =
	let (pts',p_minmax,p_is,is) = grid2_f64.index_dataset (replicate 2 subdiv) pts
	let np = length p_minmax
	let buff = dbscan2_grid_f64.internal_dbscan
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
	(pts : [n]v3_f64)
: dbscan_result [n] =
	let (pts',p_minmax,p_is,is) = grid3_f64.index_dataset (replicate 3 subdiv) pts
	let np = length p_minmax
	let buff = dbscan3_grid_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster} 

entry do_kd_dbscan_2d_f64 [n]
	(extPar : i64)
	(depth  : i64)
	(eps : f64)
	(minPts : i64)
	(pts : [n]v2_f64)
: dbscan_result [n] =
	let (pts',p_minmax,p_is,is) = kd2_f64.index_dataset (replicate 1 depth) pts
	let np = length p_minmax
	let buff = dbscan2_kd_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster} 

entry do_kd_dbscan_3d_f64 [n]
	(extPar : i64)
	(depth  : i64)
	(eps : f64)
	(minPts : i64)
	(pts : [n]v3_f64)
: dbscan_result [n] =
	let (pts',p_minmax,p_is,is) = kd3_f64.index_dataset (replicate 1 depth) pts
	let np = length p_minmax
	let buff = dbscan3_kd_f64.internal_dbscan
		extPar eps minPts
		(p_minmax |> sized np) (p_is |> sized np)
		pts'
	let is_core = scatter (replicate n false) is buff.is_core
	let cluster = scatter (replicate n (-1) ) is buff.chain_id
	in {is_core = is_core, cluster_id = cluster} 