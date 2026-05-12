import "../vector_cols"
import "../ftColumns"

import "ft_distance"

--import "ft_dclust"
import "dclust_alt"

module euclidean2_f64 = euclidean_d vector_2 f64
module euclidean3_f64 = euclidean_d vector_3 f64

module dclust2_f64 = ft_dclust vector_2 f64 euclidean2_f64
module dclust3_f64 = ft_dclust vector_3 f64 euclidean3_f64

type dbscan_result [n] = {is_core : [n]bool, cluster_id : [n]i64}

module col_f64 = col_numeric f64

-- Entry points for loading data.

entry init_column_f64 = col_f64.mk_keyCol
entry write_column_f64 = col_f64.update_keyCol
entry crop_column_f64 = col_f64.crop_keyCol

-- d-clust entry points

entry do_dclust_2d_f64 [n]
	(extPar : i64)
	(seed_No : i64)
	(subdiv : i64)
	(eps : f64)
	(minPts : i64)
	(dat1 : [n]f64)
	(dat2 : [n]f64)
: dbscan_result [n] =
	let pts = map2 (\x1 x2 ->
		vector_2.replicate x1 |> vector_2.set 1 x2
	) dat1 dat2
	let (is_core,cluster) = dclust2_f64.do_dclust extPar seed_No
		(replicate vector_2.length subdiv)
		eps minPts pts
	in {is_core = is_core, cluster_id = cluster}

entry do_dclust_3d_f64 [n]
	(extPar : i64)
	(seed_No : i64)
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
	let (is_core,cluster) = dclust3_f64.do_dclust extPar seed_No
		(replicate vector_3.length subdiv)
		eps minPts pts
	in {is_core = is_core, cluster_id = cluster}