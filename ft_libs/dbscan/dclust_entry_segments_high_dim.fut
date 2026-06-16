import "../ftbasics"
import "../ftColumns"

import "ft_spindex"
import "ft_undir_graph"
import "ft_dclust"
import "ft_distance"

import "../lib/github.com/athas/vector/vector"


module col_f64 = col_numeric f64
entry init_column_f64 = col_f64.mk_keyCol
entry write_column_f64 = col_f64.update_keyCol
entry crop_column_f64 = col_f64.crop_keyCol

-- HOW TO USE
--
-- Parameters :
--   seed_count : i64    - #slots used to limit nested parallelism
--   subdiv     : i64    - #cells per dimension
--   eps        : f64    - epsilon parameter for DBSCAN
--   minPts     : i64    - minPts parameter for DBSCAN
--   xs         : [n]f64 - x coordinates of 2d points
--   ys         : [n]f64 - y coordinates of 2d points
--
--  1. dat  <- index_dataset_2d_f64 eps subdiv xs ys
--  2. info <- get_part_info_2d_f64 false eps dat
--  3. neigh_counts <- get_neighbour_counts_2d_f64 seed_count eps minPts dat info
--  4. is_core      <- get_is_core minPts neigh_counts
--  5. cores     <- isolate_core_pts_2d_f64 is_core dat info
--  6. core_info <- get_part_core_info_2d_f64 cores dat
--  7. core_cids <- mk_clusters_2d_f64 seed_count eps info cores core_info
--  8. info_bd <- get_part_info_2d_f64 true eps dat
--  9. cluster_id <- assign_cluster_ids_2d_f64 seed_count eps dat is_core info_bd cores core_info core_cids
-- 10. res <- deindex_results_2d dat is_core cluster_id


module vector_2 = cat_vector vector_1 vector_1
module vector_3 = cat_vector vector_2 vector_1

module vector_4  = cat_vector vector_2 vector_2
module vector_5  = cat_vector vector_3 vector_2
module vector_7  = cat_vector vector_4 vector_3
module vector_10 = cat_vector vector_5 vector_5

module eucl4_f64  = euclidean_d vector_4  f64
module eucl5_f64  = euclidean_d vector_5  f64
module eucl7_f64  = euclidean_d vector_7  f64
module eucl10_f64 = euclidean_d vector_10 f64

module dclust4_f64   = ft_dclust vector_4  f64 eucl4_f64
module dclust5_f64   = ft_dclust vector_5  f64 eucl5_f64
module dclust7_f64   = ft_dclust vector_7  f64 eucl7_f64
module dclust10_f64  = ft_dclust vector_10 f64 eucl10_f64

local def cols_to_vectors_4d [n]
	(xs1 : [n]f64)
	(xs2 : [n]f64)
	(xs3 : [n]f64)
	(xs4 : [n]f64)
: [n](vector_4.vector f64) = map4
	(\x1 x2 x3 x4 -> vector_4.replicate x1
		|> vector_4.set 1 x2
		|> vector_4.set 2 x3
		|> vector_4.set 3 x4
	) xs1 xs2 xs3 xs4

local def cols_to_vectors_5d [n]
	(xs1 : [n]f64)
	(xs2 : [n]f64)
	(xs3 : [n]f64)
	(xs4 : [n]f64)
	(xs5 : [n]f64)
: [n](vector_5.vector f64) = map5
	(\x1 x2 x3 x4 x5 -> vector_5.replicate x1
		|> vector_5.set 1 x2
		|> vector_5.set 2 x3
		|> vector_5.set 3 x4
		|> vector_5.set 4 x5
	) xs1 xs2 xs3 xs4 xs5

local def cols_to_vectors_7d [n]
	(xs1 : [n]f64)
	(xs2 : [n]f64)
	(xs3 : [n]f64)
	(xs4 : [n]f64)
	(xs5 : [n]f64)
	(xs6 : [n]f64)
	(xs7 : [n]f64)
: [n](vector_7.vector f64) = map5
	(\x1 x2 x3 x4 x5 -> vector_7.replicate x1
		|> vector_7.set 1 x2
		|> vector_7.set 2 x3
		|> vector_7.set 3 x4
		|> vector_7.set 4 x5
	) xs1 xs2 xs3 xs4 xs5
	|> map3 (\x6 x7 x1_5 -> x1_5
		|> vector_7.set 5 x6
		|> vector_7.set 6 x7
	) xs6 xs7

local def cols_to_vectors_10d [n]
	(xs1  : [n]f64)
	(xs2  : [n]f64)
	(xs3  : [n]f64)
	(xs4  : [n]f64)
	(xs5  : [n]f64)
	(xs6  : [n]f64)
	(xs7  : [n]f64)
	(xs8  : [n]f64)
	(xs9  : [n]f64)
	(xs10 : [n]f64)
: [n](vector_10.vector f64) = map5
	(\x1 x2 x3 x4 x5 -> vector_10.replicate x1
		|> vector_10.set 1 x2
		|> vector_10.set 2 x3
		|> vector_10.set 3 x4
		|> vector_10.set 4 x5
	) xs1 xs2 xs3 xs4 xs5
	|> map5 (\x6 x7 x8 x9 x1_5 -> x1_5
		|> vector_10.set 5 x6
		|> vector_10.set 6 x7
		|> vector_10.set 7 x8
		|> vector_10.set 8 x9
	) xs6 xs7 xs8 xs9
	|> map2 (\x10 x1_9 -> x1_9 |> vector_10.set 9 x10) xs10


local def vectors_to_cols_4d [n] (pts : [n](vector_4.vector f64))
: ([n]f64, [n]f64, [n]f64, [n]f64) =
	let xs1 = pts |> map (vector_4.get 0)
	let xs2 = pts |> map (vector_4.get 1)
	let xs3 = pts |> map (vector_4.get 2)
	let xs4 = pts |> map (vector_4.get 3)
	in (xs1,xs2,xs3,xs4)

local def vectors_to_cols_5d [n] (pts : [n](vector_5.vector f64))
: ([n]f64, [n]f64, [n]f64, [n]f64, [n]f64) =
	let xs1 = pts |> map (vector_5.get 0)
	let xs2 = pts |> map (vector_5.get 1)
	let xs3 = pts |> map (vector_5.get 2)
	let xs4 = pts |> map (vector_5.get 3)
	let xs5 = pts |> map (vector_5.get 4)
	in (xs1,xs2,xs3,xs4,xs5)

local def vectors_to_cols_7d [n] (pts : [n](vector_7.vector f64))
: ([n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64) =
	let xs1 = pts |> map (vector_7.get 0)
	let xs2 = pts |> map (vector_7.get 1)
	let xs3 = pts |> map (vector_7.get 2)
	let xs4 = pts |> map (vector_7.get 3)
	let xs5 = pts |> map (vector_7.get 4)
	let xs6 = pts |> map (vector_7.get 5)
	let xs7 = pts |> map (vector_7.get 6)
	in (xs1,xs2,xs3,xs4,xs5,xs6,xs7)

local def vectors_to_cols_10d [n] (pts : [n](vector_10.vector f64))
: ([n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64, [n]f64) =
	let xs1  = pts |> map (vector_10.get 0)
	let xs2  = pts |> map (vector_10.get 1)
	let xs3  = pts |> map (vector_10.get 2)
	let xs4  = pts |> map (vector_10.get 3)
	let xs5  = pts |> map (vector_10.get 4)
	let xs6  = pts |> map (vector_10.get 5)
	let xs7  = pts |> map (vector_10.get 6)
	let xs8  = pts |> map (vector_10.get 7)
	let xs9  = pts |> map (vector_10.get 8)
	let xs10 = pts |> map (vector_10.get 9)
	in (xs1,xs2,xs3,xs4,xs5,xs6,xs7,xs8,xs9,xs10)

-- Entry Points for 4D

	type~ indexed_data_4d_f64 [n] = {
		xs1 : [n]f64,
		xs2 : [n]f64,
		xs3 : [n]f64,
		xs4 : [n]f64,
		subdiv : [vector_4.length]i64,
		part_is : []i64,
		cell_ids : []i64,
		og_is : [n]i64
	}

	entry index_dataset_4d_f64 [n]
		(eps : f64)
		(subdiv : i64)
		(xs1 : [n]f64)
		(xs2 : [n]f64)
		(xs3 : [n]f64)
		(xs4 : [n]f64)
	: indexed_data_4d_f64 [n] =
		let pts = cols_to_vectors_4d xs1 xs2 xs3 xs4
		let (subdiv', pts',part_is,cell_ids,og_is)
		= dclust4_f64.partition_dataset eps (replicate vector_4.length subdiv) pts
		let (xs1',xs2',xs3',xs4') = vectors_to_cols_4d pts'
		in {
			xs1 = xs1',
			xs2 = xs2',
			xs3 = xs3',
			xs4 = xs4',
			subdiv = subdiv',
			part_is = part_is,
			cell_ids = cell_ids,
			og_is = og_is
		}

	type~ partition_info_f64 [n] = {
		num_parts : i64,
		partition_pairs_0 : []i64,
		partition_pairs_1 : []i64,
		part_sz : []i64,
		part_pairs_is : []i64,
		part_pairs_sz : []i64,
		pids : [n]i64
	}

	entry get_part_info_4d_f64 [n]
		(bidir : bool)
		(eps : f64)
		(dat : indexed_data_4d_f64 [n])
	: partition_info_f64 [n] =
		let pts = cols_to_vectors_4d dat.xs1 dat.xs2 dat.xs3 dat.xs4
		let (pids,part_sz,part_pairs,part_pairs_sz,part_pairs_is)
		= dclust4_f64.partition_information bidir 1 eps dat.subdiv dat.part_is dat.cell_ids pts
		in {
			num_parts = length dat.part_is,
			partition_pairs_0 = part_pairs |> map (.0),
			partition_pairs_1 = part_pairs |> map (.1),
			part_sz = part_sz,
			part_pairs_is = part_pairs_is,
			part_pairs_sz = part_pairs_sz,
			pids = pids
		}

	entry get_neighbour_counts_4d_f64 [n]
		(seed_count : i64)
		(eps : f64)
		(minPts : i64)
		(dat  : indexed_data_4d_f64 [n])
		(info : partition_info_f64 [n])
	: [n]i64 =
		let pts = cols_to_vectors_4d dat.xs1 dat.xs2 dat.xs3 dat.xs4
		let part_pairs = indices info.partition_pairs_0
			|> map (\i ->
				(info.partition_pairs_0[i], info.partition_pairs_1[i])
			)
		in dclust4_f64.get_neighbour_counts
			seed_count
			eps
			minPts
			pts
			info.pids
			part_pairs
			(dat.part_is  |> sized info.num_parts)
			(info.part_sz |> sized info.num_parts)
			(info.part_pairs_is |> sized info.num_parts)
			(info.part_pairs_sz |> sized info.num_parts)

	entry get_is_core [n]
		(minPts : i64)
		(neigh_counts : [n]i64)
	: [n]bool = neigh_counts |> map (\nc -> nc>=minPts)

	type~ isolated_core_pts_4d_f64 = {
		num_cores : i64,
		core_xs1 : []f64,
		core_xs2 : []f64,
		core_xs3 : []f64,
		core_xs4 : []f64,
		core_pids : []i64,
		core_is : []i64,
		non_core_is : []i64
	}

	entry isolate_core_pts_4d_f64 [n]
		(is_core : [n]bool)
		(dat  : indexed_data_4d_f64 [n])
		(info : partition_info_f64 [n])
	: isolated_core_pts_4d_f64 =
		let (cores,non_cores) = iota n
			|> partition (\i -> is_core[i])
		in {
			num_cores = length cores,
			core_xs1 = cores |> map (\i -> dat.xs1[i]),
			core_xs2 = cores |> map (\i -> dat.xs2[i]),
			core_xs3 = cores |> map (\i -> dat.xs3[i]),
			core_xs4 = cores |> map (\i -> dat.xs4[i]),
			core_pids = cores |> map (\i -> info.pids[i]),
			core_is = cores,
			non_core_is = non_cores
		}

	type~ part_core_info = {
		part_core_sz : []i64,
		part_core_is : []i64
	}

	entry get_part_core_info_4d_f64 [n]
		(cores : isolated_core_pts_4d_f64)
		(dat   : indexed_data_4d_f64 [n])
	: part_core_info =
		let (part_core_sz, part_core_is) = dclust4_f64.part_get_core_info
			cores.core_pids
			dat.part_is
		in {part_core_sz = part_core_sz, part_core_is = part_core_is}

	entry mk_clusters_4d_f64 [n]
		(seed_count : i64)
		(eps : f64)
		(info  : partition_info_f64 [n])
		(cores : isolated_core_pts_4d_f64)
		(core_info : part_core_info)
	: []i64 =
		let core_pts = cols_to_vectors_4d
			(cores.core_xs1 |> sized cores.num_cores)
			(cores.core_xs2 |> sized cores.num_cores)
			(cores.core_xs3 |> sized cores.num_cores)
			(cores.core_xs4 |> sized cores.num_cores)
		let part_pairs = indices info.partition_pairs_0
			|> map (\i ->
				(info.partition_pairs_0[i], info.partition_pairs_1[i])
			)
		in dclust4_f64.find_clusters
			seed_count
			eps
			core_pts
			(cores.core_pids |> sized cores.num_cores)
			part_pairs
			(core_info.part_core_is |> sized info.num_parts)
			(core_info.part_core_sz |> sized info.num_parts)
			(info.part_pairs_is     |> sized info.num_parts)
			(info.part_pairs_sz     |> sized info.num_parts)

	entry assign_cluster_ids_4d_f64 [n]
		(seed_count : i64)
		(eps : f64)
		(dat : indexed_data_4d_f64 [n])
		(is_core : [n]bool)
		(info_bd   : partition_info_f64 [n])
		(cores     : isolated_core_pts_4d_f64)
		(core_info : part_core_info)
		(core_cids : []i64)
	: [n]i64 =
		let pts = cols_to_vectors_4d
			dat.xs1 dat.xs2 dat.xs3 dat.xs4
		let core_pts = cols_to_vectors_4d
			(cores.core_xs1 |> sized cores.num_cores)
			(cores.core_xs2 |> sized cores.num_cores)
			(cores.core_xs3 |> sized cores.num_cores)
			(cores.core_xs4 |> sized cores.num_cores)
		let part_pairs_bd = indices info_bd.partition_pairs_0
			|> map (\i ->
				(info_bd.partition_pairs_0[i], info_bd.partition_pairs_1[i])
			)
		in dclust4_f64.assign_cluster_ids
			seed_count
			eps
			pts
			info_bd.pids
			is_core
			core_pts
			(core_cids |> sized cores.num_cores)
			part_pairs_bd
			(core_info.part_core_is |> sized info_bd.num_parts)
			(core_info.part_core_sz |> sized info_bd.num_parts)
			(info_bd.part_pairs_is  |> sized info_bd.num_parts)
			(info_bd.part_pairs_sz  |> sized info_bd.num_parts)

	type dbscan_result [n] = {
		is_core : [n]bool,
		cluster_id : [n]i64
	}

	entry deindex_results_4d [n]
		(dat : indexed_data_4d_f64 [n])
		(is_core : [n]bool)
		(cluster_id : [n]i64)
	: dbscan_result [n] =
		let is_core' = scatter (replicate n false) dat.og_is is_core
		let cluster_id' = scatter (replicate n 0) dat.og_is cluster_id
		in {is_core = is_core', cluster_id = cluster_id'}

-- Entry Points for 5D


-- Entry Points for 7D


-- Entry Points for 10D































