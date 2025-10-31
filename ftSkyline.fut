import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

-- partition_id = grid_id*(total_angle_no)+angle_id
-- grid_id = sum{i}(partition in dim i * prefix sum of grid_partitions in previous dims)
-- similar for angle_id

-- TODO
-- take care of negatives depending on skyline orientation...
-- test everything...
-- and then entry points ig...

type skylineBase [dim] 't = {
	grid_partitions_per_dim : [dim]idx_t.t,
	angle_partitions_per_dim : [dim-1]idx_t.t,
	grid_part_prefix_sum : [dim]idx_t.t,
	angle_part_prefix_sum : [dim-1]idx_t.t,
	total_grid_no = idx_t.t,
	total_angle_no = idx_t.t,
	total_part_no = idx_t.t,
	start_of_grid_partition = [total_grid_no][dim]t,
	grid_part_size_per_dim = [dim]t
}
type skylineInfo [n] [dim] 't 'pL_t = {
	base : skylineBase [dim] t,
	xs : [n][dim]t,
	ys : [n]pL_t,
	part_idx : [base.total_part_no]idx_t.t,
	isPartitionDominated : [base.total_grid_no]bool,
	skyline_lt : t -> t -> bool,
	skyline_leq : t -> t -> bool
}

local def do_mk_skylineBase [dim] 't
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(partitionSizePerDim : [dim]t)
	(grid_partitions_per_dim : [dim]idx_t.t)
	(angle_partitions_per_dim : [dim]idx_t.t)
	(plus : t -> t -> t)
	(minus : t -> t -> t)
	(times : t -> t -> t)
	(over : t -> t -> t)
	(from_i64 : i64 -> t)
	(to_i64 : t -> i64)
: skylineBase [dim] =
	let total_grid_no = i64.product grid_partitions_per_dim
	let total_angle_no = i64.product angle_partitions_per_dim
	let total_part_no = total_grid_no*total_angle_no
	let grid_prefix = exscan (*) 1 grid_partitions_per_dim
	let ang_prefix = exscan (*) 1 angle_partitions_per_dim
	let start_points = (iota total_grid_no)
		|> map (\grid_id ->
			(iota total_grid_no)
				|> map (\i ->
					let perDim = grid_partitions_per_dim[i]
					let preDim = grid_prefix[i]
					let dim_id = from_i64 ((id_per_dim / preDim)%perDim)
					in min_per_dim[i] `plus` (dim_id`times`partitionSizePerDim[i])
				)
		)
	in {
		grid_partitions_per_dim = grid_partitions_per_dim,
		angle_partitions_per_dim = angle_partitions_per_dim,
		grid_part_prefix_sum = grid_prefix,
		angle_part_prefix_sum = ang_prefix,
		total_grid_no = total_grid_no,
		total_angle_no = total_angle_no,
		total_part_no = total_part_no,
		start_of_grid_partition = start_points,
		grid_part_size_per_dim = partitionSizePerDim
	}

def mk_skylineBase_from_grid [dim] 't
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(grid_partitions_per_dim : [dim]idx_t.t)
	(angle_partitions_per_dim : [dim]idx_t.t)
	(plus : t -> t -> t)
	(minus : t -> t -> t)
	(times : t -> t -> t)
	(over : t -> t -> t)
	(from_i64 : i64 -> t)
	(to_i64 : t -> i64)
: skylineBase [dim] =
	let grid_per_dim = map (from_i64) grid_partitions_per_dim
	let partitionSizePerDim = map3
		(\m M n ->
			(M`minus`m)`over`n
		)
		min_per_dim
		max_per_dim
		grid_per_dim
	in do_mk_skylineBase
		min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim
		(plus) (minus) (times) (over) (from_i64) (to_i64)

def mk_skylineBase_from_boundaries [dim] 't
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(partitionSizePerDim : [dim]t)
	(angle_partitions_per_dim : [dim]idx_t.t)
	(plus : t -> t -> t)
	(minus : t -> t -> t)
	(times : t -> t -> t)
	(over : t -> t -> t)
	(from_i64 : i64 -> t)
	(to_i64 : t -> i64)
: skylineBase [dim] =
	let grid_per_dim = map3
		(\m M s ->
			(M`minus`m)`over`s
		)
		min_per_dim
		max_per_dim
		partitionSizePerDim
	let grid_partitions_per_dim = map (to_i64) grid_per_dim
	in do_mk_skylineBase
		min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim
		(plus) (minus) (times) (over) (from_i64) (to_i64)


def get_id_from_grid_angle [dim] 't
	(skB : skylineBase [dim] t)
	(grid_id : idx_t.t)
	(angle_id : idx_t.t)
: idx_t.t =
	grid_id*(skB.total_angle_no) + angle_id

def get_grid_angle_from_id [dim] 't
	(skB : skylineBase [dim] t)
	(part_id : idx_t.t)
: (idx_t.t, idx_t.t) =
	let grid_id = part_id / skB.total_angle_no
	let angle_id = part_id % skB.total_angle_no
	in (grid_id, angle_id)

def cartesian_to_spherical [dim] 't
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
	(to_f64 : t -> f64)
: (f64, [dim-1]f64) =
	let fc = map (to_f64) coords
	let r = f64.sqrt (foldl (+) 0 (map (\x -> x*x) fc))
	let phis : [dim-1]f64 =
		loop p = (replicate (dim-1) 0)
		for j in (iota (dim-1)) do
			let this_x = fc[j]
			let this_y = f64.sqrt (foldl (+) 0 (map (\x -> x*x) fc[(j+1):dim]))
			let new_phi = f64.atan2(this_y,this_x)
		in p with [j] = new_phi
	in (r, phis)

def get_grid_angle_measure_from_coords [dim] 't
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
: (idx_t.t, idx_t.t, f64) =
	-- get grid id
	let point_0 = skB.start_of_grid_partition[0]
	let dists_0 = map2 (minus) coords point_0
	let grid_pos = (map2 (over) dists_0 skB.grid_part_size_per_dim)
		|> map (to_i64)
	let g = foldl (+) 0 (map2 (*) grid_pos skB.grid_part_prefix_sum)
	-- get angle id
	let dists_g = map2 (minus) coords skB.start_of_grid_partition[g]
	let (measure, angles) = cartesian_to_spherical skB dists_g (to_f64)
	let base_angles = skB.grid_partitions_per_dim
		|> map (\na -> (pi / 2.0) / (f64.i64 na))
	let anglePart_ = map3
		(\a ba pf ->
			(i64.f64 (a/ba))*pf
		)
		angles
		base_angles
		skB.angle_part_prefix_sum
	let a = foldl (+) 0 anglePart_
	-- returns
	in (g, a, measure)

def get_id_measure_from_coords [dim] 't
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
: (idx_t.t, f64) =
	let (g,a,m) = get_grid_angle_from_coords skB coords (minus) (over) (to_i64) (to_f64)
	in (get_id_from_grid_angle skB g a, m)

def sort_for_Skyline [n] [dim] 't 'pL_t
	(skB : skylineBase [dim] t)
	(xs : [n][dim]t)
	(ys : [n]pL_t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
	(use_measure_for_sorting : bool)
	-- skyline_lt is further from skyline - can be lt or gt depending on application
	(skyline_lt : t -> t -> bool)
	(skyline_leq : t -> t -> bool)
: skylineInfo [n] [dim] t pL_t =
	let part_leq : [dim]t -> [dim]t -> bool =
		(\x1 x2 ->
			let (id1, m1) = get_id_measure_from_coords skB x1
				(minus) (over) (to_i64) (to_f64)
			let (id2, m2) = get_id_measure_from_coords skB x2
				(minus) (over) (to_i64) (to_f64)
				if !use_measure_for_sorting
				then (id1<=id2)
				else (id1<id2) || (id1==id2 && m1<=m2)
		)
	let (sorted_xs, sorted_ys) = unzip
		(merge_sort (\(x1,_) (x2,_) -> part_leq x1 x2) (zip xs ys))
		-- TODO maybe loop-exscan-scatter would be better here?
	let count_per_grid_part =
		let grid_part_by_idx = sorted_xs
			|> map (\x -> 
				(get_grid_angle_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)).0
			)
		in hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
	let isPartitionDominated = skB.start_of_grid_partition
		|> map (\this_o ->
			let loop_over = 
				loop (isDomd, j) = (false, 0)
				while !isDomd && j<skB.total_grid_no do
					let o_j = skB.start_of_grid_partition[j]
					let count_j = count_per_grid_part[j]
					let domd = (count_j>0 && all (map2 (skyline_lt) this_o o_j))
					in (domd, j+1)
		)
	let first_idx_per_part =
		let part_by_idx = sorted_xs
			|> map (\x -> 
				(get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)).0
			)
		let count_by_part = hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
		in exscan (+) 0 count_by_part
	in {
		base = skB,
		xs = sorted_xs,
		ys = sorted_ys,
		part_idx = first_idx_per_part,
		isPartitionDominated = isPartitionDominated,
		skyline_lt = skyline_lt,
		skyline_leq = skyline_leq
	}



def calc_local_Skyline [n] [dim] 't 'pL_t
	(skI : skylineInfo [n] [dim] t pL_t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
: []([dim]t, pL_t) =
	let filt_idx = (iota n)
		|> map (\i ->
			let this_x = skI.xs[i]
			let (g, a, _) = get_grid_angle_measure_from_coords skI.base this_x
				(minus) (over) (to_i64) (to_f64)
			in if skI.isPartitionDominated[g] then (-1) else
			let p_id = get_id_from_grid_angle skI.base g a
			let start_idx = skI.part_idx[j]
			let end_idx = if j==skI.base.total_part_no-1 then n else skI.part_idx[j+1]
			let loop_over =
				loop (isElimd, j) = (false, start_idx)
				while !isElimd && j<end_idx do
					let cmp_x = skI.xs[j]
					let elimd =
						(all (map2 (skI.skyline_leq) this_x cmp_x))
						&&
						(any (map2 (skI.skyline_lt) this_x cmp_x))
					in (elimd, j+1)
			in
				if loop_over.0
				then (-1)
				else i
		)
		|> filter (>=0)
	in filt_idx |> map (\i -> (skI.xs[i], skI.ys[i]))

def calc_global_Skyline [n] [dim] 't 'pL_t
	(xys : [n]([dim]t, pL_t))
	(skyline_lt : t -> t -> bool)
	(skyline_leq : t -> t -> bool)
: ([][dim]t, []pL_t) =
	(iota n) |>
		map (\i ->
			let this_x = xys[i].0
			let loop_over =
				loop (isElimd, j) = (false, 0)
				while !isElimd && j<n do
					let cmp_x = xys[j].0
					let elimd =
						(all (map2 (skyline_leq) this_x cmp_x))
						&&
						(any (map2 (skyline_lt) this_x cmp_x))
					in (elimd, j+1)
			in
				if loop_over.0
				then (-1)
				else i
		)
		|> filter (>=0)
		|> map (\i -> xys[i])
		|> unzip

