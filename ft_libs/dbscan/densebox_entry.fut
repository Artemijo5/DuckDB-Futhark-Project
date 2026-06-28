import "../vector_cols"
import "../ftColumns"

import "ft_distance"

import "ft_densebox"

module euclidean2_f64 = euclidean_d vector_2 f64
module euclidean3_f64 = euclidean_d vector_3 f64
module euclidean4_f64 = euclidean_d vector_4 f64
module euclidean5_f64 = euclidean_d vector_5 f64
module euclidean7_f64 = euclidean_d vector_7 f64

module densebox2_f64 = ft_densebox vector_2 f64 euclidean2_f64
module densebox3_f64 = ft_densebox vector_3 f64 euclidean3_f64
module densebox4_f64 = ft_densebox vector_4 f64 euclidean4_f64
module densebox5_f64 = ft_densebox vector_5 f64 euclidean5_f64
module densebox7_f64 = ft_densebox vector_7 f64 euclidean7_f64

type dbscan_result [n] = {is_core : [n]bool, cluster_id : [n]i64}

module col_f64 = col_numeric f64

-- Entry points for loading data.

entry init_column_f64 = col_f64.mk_keyCol
entry write_column_f64 = col_f64.update_keyCol
entry crop_column_f64 = col_f64.crop_keyCol

-- 2-D Entry Points

	local def cols_to_vectors_2d [n] (xs1 : [n]f64) (xs2 : [n]f64)
	: [n](vector_2.vector f64) = map2
		(\x1 x2 -> vector_2.replicate x1 |> vector_2.set 1 x2)
		xs1 xs2

	local def vectors_to_cols_2d [n] (pts : [n](vector_2.vector f64))
	: ([n]f64, [n]f64) =
		let xs1 = pts |> map (vector_2.get 0)
		let xs2 = pts |> map (vector_2.get 1)
		in (xs1,xs2)

	type~ indexed_data_2d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		pids : [n]i64,
		og_is : [n]i64,
		parts_No : i64,
		part_is  : []i64,
		part_sz  : []i64,
		part_pairs : [](i64,i64),
		part_pairs_is : []i64,
		part_pairs_sz : []i64
	}

	entry densebox_index_dataset_2d_f64 [n]
		(window_size : i64)
		(eps : f64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
	: indexed_data_2d_f64 [n] =
		let pts = cols_to_vectors_2d xs1 xs2
		let (
			pts',
			cell_vecs, part_is, og_is
		)
			= densebox2_f64.partition_dataset eps pts
		let (part_sz,_,pids) = densebox2_f64.get_part_info 1 part_is pts
		let (part_pairs, part_pairs_is, part_pairs_sz)
			= densebox2_f64.get_box_neighbourhoods
			window_size cell_vecs
		let (xs1',xs2') = vectors_to_cols_2d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			pids = pids,
			og_is = og_is,
			parts_No = length part_is,
			part_is = part_is,
			part_sz = part_sz,
			part_pairs = part_pairs,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz
		}

	entry densebox_do_dbscan_2d_f64 [n]
		(eps : f64)
		(minPts : i64)
		(dat : indexed_data_2d_f64 [n])
	: dbscan_result [n] =
		let pts = cols_to_vectors_2d dat.xs1 dat.xs2
		let is_core = densebox2_f64.find_core_pts
			eps minPts
			pts dat.pids
			(dat.part_is |> sized dat.parts_No)
			(dat.part_sz |> sized dat.parts_No)
			dat.part_pairs
			(dat.part_pairs_is |> sized dat.parts_No)
			(dat.part_pairs_sz |> sized dat.parts_No)
		let part_cids = densebox2_f64.mk_clusters eps
			pts dat.pids is_core
			dat.part_pairs
			dat.part_is
		let clust_ids = densebox2_f64.assign_cluster_ids eps
			pts is_core dat.pids
			dat.part_pairs part_cids
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let clust_ids' = scatter (replicate n (-1)) dat.og_is clust_ids
		in {is_core = is_core', cluster_id = clust_ids'}

-- 3-D Entry Points

	local def cols_to_vectors_3d [n]
		(xs1 : [n]f64) (xs2 : [n]f64) (xs3 : [n]f64)
	: [n](vector_3.vector f64) = map3
		(\x1 x2 x3 ->
			vector_3.replicate x1
			|> vector_3.set 1 x2
			|> vector_3.set 2 x3
		) xs1 xs2 xs3

	local def vectors_to_cols_3d [n] (pts : [n](vector_3.vector f64))
	: ([n]f64, [n]f64, [n]f64) =
		let xs1 = pts |> map (vector_3.get 0)
		let xs2 = pts |> map (vector_3.get 1)
		let xs3 = pts |> map (vector_3.get 2)
		in (xs1,xs2,xs3)

	type~ indexed_data_3d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		xs3 : [n]f64,
		pids : [n]i64,
		og_is : [n]i64,
		parts_No : i64,
		part_is  : []i64,
		part_sz  : []i64,
		part_pairs : [](i64,i64),
		part_pairs_is : []i64,
		part_pairs_sz : []i64
	}

	entry densebox_index_dataset_3d_f64 [n]
		(window_size : i64)
		(eps : f64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
	: indexed_data_3d_f64 [n] =
		let pts = cols_to_vectors_3d xs1 xs2 xs3
		let (
			pts',
			cell_vecs, part_is, og_is
		)
			= densebox3_f64.partition_dataset eps pts
		let (part_sz,_,pids) = densebox3_f64.get_part_info 1 part_is pts
		let (part_pairs, part_pairs_is, part_pairs_sz)
			= densebox3_f64.get_box_neighbourhoods
			window_size cell_vecs
		let (xs1',xs2',xs3') = vectors_to_cols_3d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			xs3 = xs3',
			pids = pids,
			og_is = og_is,
			parts_No = length part_is,
			part_is = part_is,
			part_sz = part_sz,
			part_pairs = part_pairs,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz
		}

	entry densebox_do_dbscan_3d_f64 [n]
		(eps : f64)
		(minPts : i64)
		(dat : indexed_data_3d_f64 [n])
	: dbscan_result [n] =
		let pts = cols_to_vectors_3d dat.xs1 dat.xs2 dat.xs3
		let is_core = densebox3_f64.find_core_pts
			eps minPts
			pts dat.pids
			(dat.part_is |> sized dat.parts_No)
			(dat.part_sz |> sized dat.parts_No)
			dat.part_pairs
			(dat.part_pairs_is |> sized dat.parts_No)
			(dat.part_pairs_sz |> sized dat.parts_No)
		let part_cids = densebox3_f64.mk_clusters eps
			pts dat.pids is_core
			dat.part_pairs
			dat.part_is
		let clust_ids = densebox3_f64.assign_cluster_ids eps
			pts is_core dat.pids
			dat.part_pairs part_cids
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let clust_ids' = scatter (replicate n (-1)) dat.og_is clust_ids
		in {is_core = is_core', cluster_id = clust_ids'}

-- 4-D Entry Points

	local def cols_to_vectors_4d [n]
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
	: [n](vector_4.vector f64) = map4
		(\x1 x2 x3 x4 ->
			vector_4.replicate x1
			|> vector_4.set 1 x2
			|> vector_4.set 2 x3
			|> vector_4.set 3 x4
		) xs1 xs2 xs3 xs4

	local def vectors_to_cols_4d [n] (pts : [n](vector_4.vector f64))
	: ([n]f64, [n]f64, [n]f64, [n]f64) =
		let xs1 = pts |> map (vector_4.get 0)
		let xs2 = pts |> map (vector_4.get 1)
		let xs3 = pts |> map (vector_4.get 2)
		let xs4 = pts |> map (vector_4.get 3)
		in (xs1,xs2,xs3,xs4)

	type~ indexed_data_4d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		xs3 : [n]f64,
		xs4 : [n]f64,
		pids : [n]i64,
		og_is : [n]i64,
		parts_No : i64,
		part_is  : []i64,
		part_sz  : []i64,
		part_pairs : [](i64,i64),
		part_pairs_is : []i64,
		part_pairs_sz : []i64
	}

	entry densebox_index_dataset_4d_f64 [n]
		(window_size : i64)
		(eps : f64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
	: indexed_data_4d_f64 [n] =
		let pts = cols_to_vectors_4d xs1 xs2 xs3 xs4
		let (
			pts',
			cell_vecs, part_is, og_is
		)
			= densebox4_f64.partition_dataset eps pts
		let (part_sz,_,pids) = densebox4_f64.get_part_info 1 part_is pts
		let (part_pairs, part_pairs_is, part_pairs_sz)
			= densebox4_f64.get_box_neighbourhoods
			window_size cell_vecs
		let (xs1',xs2',xs3',xs4') = vectors_to_cols_4d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			xs3 = xs3',
			xs4 = xs4',
			pids = pids,
			og_is = og_is,
			parts_No = length part_is,
			part_is = part_is,
			part_sz = part_sz,
			part_pairs = part_pairs,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz
		}

	entry densebox_do_dbscan_4d_f64 [n]
		(eps : f64)
		(minPts : i64)
		(dat : indexed_data_4d_f64 [n])
	: dbscan_result [n] =
		let pts = cols_to_vectors_4d
			dat.xs1 dat.xs2 dat.xs3 dat.xs4
		let is_core = densebox4_f64.find_core_pts
			eps minPts
			pts dat.pids
			(dat.part_is |> sized dat.parts_No)
			(dat.part_sz |> sized dat.parts_No)
			dat.part_pairs
			(dat.part_pairs_is |> sized dat.parts_No)
			(dat.part_pairs_sz |> sized dat.parts_No)
		let part_cids = densebox4_f64.mk_clusters eps
			pts dat.pids is_core
			dat.part_pairs
			dat.part_is
		let clust_ids = densebox4_f64.assign_cluster_ids eps
			pts is_core dat.pids
			dat.part_pairs part_cids
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let clust_ids' = scatter (replicate n (-1)) dat.og_is clust_ids
		in {is_core = is_core', cluster_id = clust_ids'}

-- 5-D Entry Points

	local def cols_to_vectors_5d [n]
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
		(xs5 : [n]f64)
	: [n](vector_5.vector f64) = map5
		(\x1 x2 x3 x4 x5 ->
			vector_5.replicate x1
			|> vector_5.set 1 x2
			|> vector_5.set 2 x3
			|> vector_5.set 3 x4
			|> vector_5.set 4 x5
		) xs1 xs2 xs3 xs4 xs5

	local def vectors_to_cols_5d [n] (pts : [n](vector_5.vector f64))
	: ([n]f64, [n]f64, [n]f64, [n]f64, [n]f64) =
		let xs1 = pts |> map (vector_5.get 0)
		let xs2 = pts |> map (vector_5.get 1)
		let xs3 = pts |> map (vector_5.get 2)
		let xs4 = pts |> map (vector_5.get 3)
		let xs5 = pts |> map (vector_5.get 4)
		in (xs1,xs2,xs3,xs4,xs5)

	type~ indexed_data_5d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		xs3 : [n]f64,
		xs4 : [n]f64,
		xs5 : [n]f64,
		pids : [n]i64,
		og_is : [n]i64,
		parts_No : i64,
		part_is  : []i64,
		part_sz  : []i64,
		part_pairs : [](i64,i64),
		part_pairs_is : []i64,
		part_pairs_sz : []i64
	}

	entry densebox_index_dataset_5d_f64 [n]
		(window_size : i64)
		(eps : f64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
		(xs5 : [n]f64)
	: indexed_data_5d_f64 [n] =
		let pts = cols_to_vectors_5d 
			xs1 xs2 xs3 xs4 xs5
		let (
			pts',
			cell_vecs, part_is, og_is
		)
			= densebox5_f64.partition_dataset eps pts
		let (part_sz,_,pids) = densebox5_f64.get_part_info 1 part_is pts
		let (part_pairs, part_pairs_is, part_pairs_sz)
			= densebox5_f64.get_box_neighbourhoods
			window_size cell_vecs
		let (xs1',xs2',xs3',xs4',xs5') = vectors_to_cols_5d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			xs3 = xs3',
			xs4 = xs4',
			xs5 = xs5',
			pids = pids,
			og_is = og_is,
			parts_No = length part_is,
			part_is = part_is,
			part_sz = part_sz,
			part_pairs = part_pairs,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz
		}

	entry densebox_do_dbscan_5d_f64 [n]
		(eps : f64)
		(minPts : i64)
		(dat : indexed_data_5d_f64 [n])
	: dbscan_result [n] =
		let pts = cols_to_vectors_5d
			dat.xs1 dat.xs2 dat.xs3 dat.xs4 dat.xs5
		let is_core = densebox5_f64.find_core_pts
			eps minPts
			pts dat.pids
			(dat.part_is |> sized dat.parts_No)
			(dat.part_sz |> sized dat.parts_No)
			dat.part_pairs
			(dat.part_pairs_is |> sized dat.parts_No)
			(dat.part_pairs_sz |> sized dat.parts_No)
		let part_cids = densebox5_f64.mk_clusters eps
			pts dat.pids is_core
			dat.part_pairs
			dat.part_is
		let clust_ids = densebox5_f64.assign_cluster_ids eps
			pts is_core dat.pids
			dat.part_pairs part_cids
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let clust_ids' = scatter (replicate n (-1)) dat.og_is clust_ids
		in {is_core = is_core', cluster_id = clust_ids'}

-- 7-D Entry Points

	local def cols_to_vectors_7d [n]
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
		(xs5 : [n]f64)
		(xs6 : [n]f64)
		(xs7 : [n]f64)
	: [n](vector_7.vector f64) = map5
		(\x1 x2 x3 x4 x5 ->
			vector_7.replicate x1
			|> vector_7.set 1 x2
			|> vector_7.set 2 x3
			|> vector_7.set 3 x4
			|> vector_7.set 4 x5
		) xs1 xs2 xs3 xs4 xs5
		|> map3 (\x6 x7 vec -> vec
			|> vector_7.set 5 x6
			|> vector_7.set 6 x7
		) xs6 xs7

	local def vectors_to_cols_7d [n] (pts : [n](vector_7.vector f64))
	: ([n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64) =
		let xs1 = pts |> map (vector_7.get 0)
		let xs2 = pts |> map (vector_7.get 1)
		let xs3 = pts |> map (vector_7.get 2)
		let xs4 = pts |> map (vector_7.get 3)
		let xs5 = pts |> map (vector_7.get 4)
		let xs6 = pts |> map (vector_7.get 3)
		let xs7 = pts |> map (vector_7.get 4)
		in (xs1,xs2,xs3,xs4,xs5,xs6,xs7)

	type~ indexed_data_7d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		xs3 : [n]f64,
		xs4 : [n]f64,
		xs5 : [n]f64,
		xs6 : [n]f64,
		xs7 : [n]f64,
		pids : [n]i64,
		og_is : [n]i64,
		parts_No : i64,
		part_is  : []i64,
		part_sz  : []i64,
		part_pairs : [](i64,i64),
		part_pairs_is : []i64,
		part_pairs_sz : []i64
	}

	entry densebox_index_dataset_7d_f64 [n]
		(window_size : i64)
		(eps : f64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
		(xs5 : [n]f64)
		(xs6 : [n]f64)
		(xs7 : [n]f64)
	: indexed_data_7d_f64 [n] =
		let pts = cols_to_vectors_7d 
			xs1 xs2 xs3 xs4 xs5 xs6 xs7
		let (
			pts',
			cell_vecs, part_is, og_is
		)
			= densebox7_f64.partition_dataset eps pts
		let (part_sz,_,pids) = densebox7_f64.get_part_info 1 part_is pts
		let (part_pairs, part_pairs_is, part_pairs_sz)
			= densebox7_f64.get_box_neighbourhoods
			window_size cell_vecs
		let (xs1',xs2',xs3',xs4',xs5',xs6',xs7')
			= vectors_to_cols_7d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			xs3 = xs3',
			xs4 = xs4',
			xs5 = xs5',
			xs6 = xs6',
			xs7 = xs7',
			pids = pids,
			og_is = og_is,
			parts_No = length part_is,
			part_is = part_is,
			part_sz = part_sz,
			part_pairs = part_pairs,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz
		}

	entry densebox_do_dbscan_7d_f64 [n]
		(eps : f64)
		(minPts : i64)
		(dat : indexed_data_7d_f64 [n])
	: dbscan_result [n] =
		let pts = cols_to_vectors_7d
			dat.xs1 dat.xs2 dat.xs3 dat.xs4
			dat.xs5 dat.xs6 dat.xs7
		let is_core = densebox7_f64.find_core_pts
			eps minPts
			pts dat.pids
			(dat.part_is |> sized dat.parts_No)
			(dat.part_sz |> sized dat.parts_No)
			dat.part_pairs
			(dat.part_pairs_is |> sized dat.parts_No)
			(dat.part_pairs_sz |> sized dat.parts_No)
		let part_cids = densebox7_f64.mk_clusters eps
			pts dat.pids is_core
			dat.part_pairs
			dat.part_is
		let clust_ids = densebox7_f64.assign_cluster_ids eps
			pts is_core dat.pids
			dat.part_pairs part_cids
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let clust_ids' = scatter (replicate n (-1)) dat.og_is clust_ids
		in {is_core = is_core', cluster_id = clust_ids'}