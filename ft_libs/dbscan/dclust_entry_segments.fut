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


-- | Module for 2-dimensional vectors.
module vector_2 = cat_vector vector_1 vector_1
-- | Module for 3-dimensional vectors.
module vector_3 = cat_vector vector_2 vector_1

-- | Euclidean distance module for 2d points of f64 values.
module eucl2_f64 = euclidean_d vector_2 f64
-- | Euclidean distance module for 3d points of f64 values.
module eucl3_f64 = euclidean_d vector_3 f64

-- | DClust for 2d f64 points using Euclidean distance.
module dclust2_f64 = ft_dclust vector_2 f64 eucl2_f64
-- | DClust for 3d f64 points using Euclidean distance.
module dclust3_f64 = ft_dclust vector_3 f64 eucl3_f64

local def cols_to_vectors_2d [n] (xs : [n]f64) (ys : [n]f64)
: [n](vector_2.vector f64) = map2
	(\x y -> vector_2.replicate x |> vector_2.set 1 y)
	xs ys

local def cols_to_vectors_3d [n] (xs : [n]f64) (ys : [n]f64) (zs : [n]f64)
: [n](vector_3.vector f64) = map3
	(\x y z -> vector_3.replicate x |> vector_3.set 1 y |> vector_3.set 2 z)
	xs ys zs

local def vectors_to_cols_2d [n] (pts : [n](vector_2.vector f64))
: ([n]f64, [n]f64) =
	let xs = pts |> map (vector_2.get 0)
	let ys = pts |> map (vector_2.get 1)
	in (xs,ys)

local def vectors_to_cols_3d [n] (pts : [n](vector_3.vector f64))
: ([n]f64, [n]f64, [n]f64) =
	let xs = pts |> map (vector_3.get 0)
	let ys = pts |> map (vector_3.get 1)
	let zs = pts |> map (vector_3.get 2)
	in (xs,ys,zs)

type~ indexed_data_2d_f64 [n] = {
	xs : [n]f64,
	ys : [n]f64,
	subdiv : [vector_2.length]i64,
	part_is : []i64,
	cell_ids : []i64,
	og_is : [n]i64
}
type~ indexed_data_3d_f64 [n] = {
	xs : [n]f64,
	ys : [n]f64,
	zs : [n]f64,
	subdiv : [vector_3.length]i64,
	part_is : []i64,
	cell_ids : []i64,
	og_is : [n]i64
}

entry index_dataset_2d_f64 [n]
	(eps : f64)
	(subdiv : i64)
	(xs : [n]f64)
	(ys : [n]f64)
: indexed_data_2d_f64 [n] =
	let pts = cols_to_vectors_2d xs ys
	let (subdiv', pts',part_is,cell_ids,og_is)
	= dclust2_f64.partition_dataset eps (replicate vector_2.length subdiv) pts
	let (xs',ys') = vectors_to_cols_2d pts'
	in {
		xs = xs',
		ys = ys',
		subdiv = subdiv',
		part_is = part_is,
		cell_ids = cell_ids,
		og_is = og_is
	}
entry index_dataset_3d_f64 [n]
	(eps : f64)
	(subdiv : i64)
	(xs : [n]f64)
	(ys : [n]f64)
	(zs : [n]f64)
: indexed_data_3d_f64 [n] =
	let pts = cols_to_vectors_3d xs ys zs
	let (subdiv', pts',part_is,cell_ids,og_is)
	= dclust3_f64.partition_dataset eps (replicate vector_3.length subdiv) pts
	let (xs',ys',zs') = vectors_to_cols_3d pts'
	in {
		xs = xs',
		ys = ys',
		zs = zs',
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

entry get_part_info_2d_f64 [n]
	(bidir : bool)
	(eps : f64)
	(dat : indexed_data_2d_f64 [n])
: partition_info_f64 [n] =
	let pts = cols_to_vectors_2d dat.xs dat.ys
	let (pids,part_sz,part_pairs,part_pairs_sz,part_pairs_is)
	= dclust2_f64.partition_information bidir 1 eps dat.subdiv dat.part_is dat.cell_ids pts
	in {
		num_parts = length dat.part_is,
		partition_pairs_0 = part_pairs |> map (.0),
		partition_pairs_1 = part_pairs |> map (.1),
		part_sz = part_sz,
		part_pairs_is = part_pairs_is,
		part_pairs_sz = part_pairs_sz,
		pids = pids
	}
entry get_part_info_3d_f64 [n]
	(bidir : bool)
	(eps : f64)
	(dat : indexed_data_3d_f64 [n])
: partition_info_f64 [n] =
	let pts = cols_to_vectors_3d dat.xs dat.ys dat.zs
	let (pids,part_sz,part_pairs,part_pairs_sz,part_pairs_is)
	= dclust3_f64.partition_information bidir 1 eps dat.subdiv dat.part_is dat.cell_ids pts
	in {
		num_parts = length dat.part_is,
		partition_pairs_0 = part_pairs |> map (.0),
		partition_pairs_1 = part_pairs |> map (.1),
		part_sz = part_sz,
		part_pairs_is = part_pairs_is,
		part_pairs_sz = part_pairs_sz,
		pids = pids
	}

entry get_neighbour_counts_2d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(minPts : i64)
	(dat  : indexed_data_2d_f64 [n])
	(info : partition_info_f64 [n])
: [n]i64 =
	let pts = cols_to_vectors_2d dat.xs dat.ys
	let part_pairs = indices info.partition_pairs_0
		|> map (\i ->
			(info.partition_pairs_0[i], info.partition_pairs_1[i])
		)
	in dclust2_f64.get_neighbour_counts
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

entry get_neighbour_counts_3d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(minPts : i64)
	(dat  : indexed_data_3d_f64 [n])
	(info : partition_info_f64 [n])
: [n]i64 =
	let pts = cols_to_vectors_3d dat.xs dat.ys dat.zs
	let part_pairs = indices info.partition_pairs_0
		|> map (\i ->
			(info.partition_pairs_0[i], info.partition_pairs_1[i])
		)
	in dclust3_f64.get_neighbour_counts
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

type~ isolated_core_pts_2d_f64 = {
	num_cores : i64,
	core_xs : []f64,
	core_ys : []f64,
	core_pids : []i64,
	core_is : []i64,
	non_core_is : []i64
}

type~ isolated_core_pts_3d_f64 = {
	num_cores : i64,
	core_xs : []f64,
	core_ys : []f64,
	core_zs : []f64,
	core_pids : []i64,
	core_is : []i64,
	non_core_is : []i64
}

entry isolate_core_pts_2d_f64 [n]
	(is_core : [n]bool)
	(dat  : indexed_data_2d_f64 [n])
	(info : partition_info_f64 [n])
: isolated_core_pts_2d_f64 =
	let (cores,non_cores) = iota n
		|> partition (\i -> is_core[i])
	let core_xs = cores |> map (\i -> dat.xs[i])
	let core_ys = cores |> map (\i -> dat.ys[i])
	let core_pids = cores |> map (\i -> info.pids[i])
	in {
		num_cores = length cores,
		core_xs = core_xs,
		core_ys = core_ys,
		core_pids = core_pids,
		core_is = cores,
		non_core_is = non_cores
	}

entry isolate_core_pts_3d_f64 [n]
	(is_core : [n]bool)
	(dat  : indexed_data_3d_f64 [n])
	(info : partition_info_f64 [n])
: isolated_core_pts_3d_f64 =
	let (cores,non_cores) = iota n
		|> partition (\i -> is_core[i])
	let core_xs = cores |> map (\i -> dat.xs[i])
	let core_ys = cores |> map (\i -> dat.ys[i])
	let core_zs = cores |> map (\i -> dat.zs[i])
	let core_pids = cores |> map (\i -> info.pids[i])
	in {
		num_cores = length cores,
		core_xs = core_xs,
		core_ys = core_ys,
		core_zs = core_zs,
		core_pids = core_pids,
		core_is = cores,
		non_core_is = non_cores
	}

type~ part_core_info = {
	part_core_sz : []i64,
	part_core_is : []i64
}

entry get_part_core_info_2d_f64 [n]
	(cores : isolated_core_pts_2d_f64)
	(dat   : indexed_data_2d_f64 [n])
: part_core_info =
	let (part_core_sz, part_core_is) = dclust2_f64.part_get_core_info
		cores.core_pids
		dat.part_is
	in {part_core_sz = part_core_sz, part_core_is = part_core_is}

entry get_part_core_info_3d_f64 [n]
	(cores : isolated_core_pts_3d_f64)
	(dat   : indexed_data_3d_f64 [n])
: part_core_info =
	let (part_core_sz, part_core_is) = dclust2_f64.part_get_core_info
		cores.core_pids
		dat.part_is
	in {part_core_sz = part_core_sz, part_core_is = part_core_is}


entry mk_clusters_2d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(info  : partition_info_f64 [n])
	(cores : isolated_core_pts_2d_f64)
	(core_info : part_core_info)
: []i64 =
	let core_pts = cols_to_vectors_2d
		(cores.core_xs |> sized cores.num_cores)
		(cores.core_ys |> sized cores.num_cores)
	let part_pairs = indices info.partition_pairs_0
		|> map (\i ->
			(info.partition_pairs_0[i], info.partition_pairs_1[i])
		)
	in dclust2_f64.find_clusters
		seed_count
		eps
		core_pts
		(cores.core_pids |> sized cores.num_cores)
		part_pairs
		(core_info.part_core_is |> sized info.num_parts)
		(core_info.part_core_sz |> sized info.num_parts)
		(info.part_pairs_is     |> sized info.num_parts)
		(info.part_pairs_sz     |> sized info.num_parts)

entry mk_clusters_3d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(info  : partition_info_f64 [n])
	(cores : isolated_core_pts_3d_f64)
	(core_info : part_core_info)
: []i64 =
	let core_pts = cols_to_vectors_3d
		(cores.core_xs |> sized cores.num_cores)
		(cores.core_ys |> sized cores.num_cores)
		(cores.core_zs |> sized cores.num_cores)
	let part_pairs = indices info.partition_pairs_0
		|> map (\i ->
			(info.partition_pairs_0[i], info.partition_pairs_1[i])
		)
	in dclust3_f64.find_clusters
		seed_count
		eps
		core_pts
		(cores.core_pids |> sized cores.num_cores)
		part_pairs
		(core_info.part_core_is |> sized info.num_parts)
		(core_info.part_core_sz |> sized info.num_parts)
		(info.part_pairs_is     |> sized info.num_parts)
		(info.part_pairs_sz     |> sized info.num_parts)

entry assign_cluster_ids_2d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(dat       : indexed_data_2d_f64 [n])
	(is_core : [n]bool)
	(info_bd   : partition_info_f64 [n])
	(cores     : isolated_core_pts_2d_f64)
	(core_info : part_core_info)
	(core_cids : []i64)
: [n]i64 =
	let pts = cols_to_vectors_2d
		dat.xs dat.ys
	let core_pts = cols_to_vectors_2d
		(cores.core_xs |> sized cores.num_cores)
		(cores.core_ys |> sized cores.num_cores)
	let part_pairs_bd = indices info_bd.partition_pairs_0
		|> map (\i ->
			(info_bd.partition_pairs_0[i], info_bd.partition_pairs_1[i])
		)
	in dclust2_f64.assign_cluster_ids
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

entry assign_cluster_ids_3d_f64 [n]
	(seed_count : i64)
	(eps : f64)
	(dat       : indexed_data_3d_f64 [n])
	(is_core : [n]bool)
	(info_bd   : partition_info_f64 [n])
	(cores     : isolated_core_pts_3d_f64)
	(core_info : part_core_info)
	(core_cids : []i64)
: [n]i64 =
	let pts = cols_to_vectors_3d
		dat.xs dat.ys dat.zs
	let core_pts = cols_to_vectors_3d
		(cores.core_xs |> sized cores.num_cores)
		(cores.core_ys |> sized cores.num_cores)
		(cores.core_zs |> sized cores.num_cores)
	let part_pairs_bd = indices info_bd.partition_pairs_0
		|> map (\i ->
			(info_bd.partition_pairs_0[i], info_bd.partition_pairs_1[i])
		)
	in dclust3_f64.assign_cluster_ids
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

entry deindex_results_2d [n]
	(dat : indexed_data_2d_f64 [n])
	(is_core : [n]bool)
	(cluster_id : [n]i64)
: dbscan_result [n] =
	let is_core' = scatter (replicate n false) dat.og_is is_core
	let cluster_id' = scatter (replicate n 0) dat.og_is cluster_id
	in {is_core = is_core', cluster_id = cluster_id'}

entry deindex_results_3d [n]
	(dat : indexed_data_3d_f64 [n])
	(is_core : [n]bool)
	(cluster_id : [n]i64)
: dbscan_result [n] =
	let is_core' = scatter (replicate n false) dat.og_is is_core
	let cluster_id' = scatter (replicate n 0) dat.og_is cluster_id
	in {is_core = is_core', cluster_id = cluster_id'}

