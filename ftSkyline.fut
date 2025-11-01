import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

-- partition_id = grid_id*(total_angle_no)+angle_id
-- grid_id = sum{i}(partition in dim i * prefix sum of grid_partitions in previous dims)
-- similar for angle_id

-- TODO
-- calc_intermediate_skyline
-- 
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
type~ skylineInfo [dim] 't 'pL_t = {
	base : skylineBase [dim] t,
	xys : []([dim]t, pL_t),
	part_idx : [base.total_part_no]idx_t.t,
	isPartitionDominated : [base.total_grid_no]bool,
	skyline_lt : t -> t -> bool,
	skyline_leq : t -> t -> bool
	-- TODO move lt and leq to base
}

def dummy_skylineInfo [dim] 't 'pL_t
	(skB : skylineBase [dim] t)
	(_ : ([dim]t, pL_t)) -- dummy elem
	(skyline_lt : t -> t -> bool)
	(skyline_leq : t -> t -> bool)
: dummy_skylineInfo [dim] t pL_t = {
	base = skB,
	xys = [],
	part_idx = replicate skB.total_part_no 0,
	isPartitionDominated = replicate skB.total_grid_no false,
	skyline_lt = skyline_lt,
	skyline_leq = skyline_leq
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

def get_grid_id_from_coords [dim] 't
	(skB: skylineBase [dim] t)
	(coords : [dim]t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
: idx_t.t =
	let point_0 = skB.start_of_grid_partition[0]
	let dists_0 = map2 (minus) coords point_0
	let grid_pos = (map2 (over) dists_0 skB.grid_part_size_per_dim)
		|> map (to_i64)
	in foldl (+) 0 (map2 (*) grid_pos skB.grid_part_prefix_sum)

def get_grid_angle_measure_from_coords [dim] 't
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
: (idx_t.t, idx_t.t, f64) =
	-- get grid id
	let g = get_grid_id_from_coords skB coords (minus) (over) (to_i64)
	-- get angle id
	let dists_g = map2 (minus) coords skB.start_of_grid_partition[g]
	let (measure, angles) = cartesian_to_spherical skB dists_g (to_f64)
	let base_angles = skB.grid_partitions_per_dim
		|> map (\na -> 
			if na == 1
			then (-1.0)
			else f64.floor ((pi / 2.0) / (f64.i64 na)))
	let anglePart_ = map3
		(\a ba pf ->
			if ba<0
			then 0
			else (i64.f64 (a/ba))*pf
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
	-- dominance obtained from previous windows
	(previous_dominance : [skB.total_grid_no]bool)
: skylineInfo [dim] t pL_t =
	let part_leq : [dim]t -> [dim]t -> bool =
		if use_measure_for_sorting
		then
			(\x1 x2 ->
				let (id1, m1) = get_id_measure_from_coords skB x1
					(minus) (over) (to_i64) (to_f64)
				let (id2, m2) = get_id_measure_from_coords skB x2
					(minus) (over) (to_i64) (to_f64)
				in (id1<id2) || (id1==id2 && m1<=m2)
			)
		else
			(\x1 x2 ->
				let (id1, m1) = get_id_measure_from_coords skB x1
					(minus) (over) (to_i64) (to_f64)
				let (id2, m2) = get_id_measure_from_coords skB x2
					(minus) (over) (to_i64) (to_f64)
				in (id1<=id2)
			)
	let sorted_xys_ = 
		merge_sort (\(x1,_) (x2,_) -> part_leq x1 x2) (zip xs ys)
	let isPartitionDominated =
		let count_per_grid_part =
			let grid_part_by_idx = sorted_xys_
				|> map (\(xy) -> 
					get_grid_id_from_coords skB x (minus) (over) (to_i64)
				)
			in hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
		in skB.start_of_grid_partition
			|> map (\this_o ->
				let loop_over = 
					loop (isDomd, j) = (false, 0)
					while !isDomd && j<skB.total_grid_no do
						let o_j = skB.start_of_grid_partition[j]
						let count_j = count_per_grid_part[j]
						let domd = (count_j>0 && all (map2 (skyline_lt) this_o o_j))
						in (domd, j+1)
			)
			|> zip previous_dominance
			|> map (\(alt,neu) -> alt || neu)
	let sorted_xys = sorted_xys_
		|> filter (\(x,_) ->
			let g = get_grid_id_from_coords skB x (minus) (over) (to_i64)
			in !(isPartitionDominated[g])
		)
	let zuowei =
		let part_by_idx = sorted_xys
			|> map (\x -> 
				(get_id_measure_from_coords skB x (minus) (over) (to_i64)).0
			)
		let counts = hist (+) 0 skB.total_part_no part_by_idx (replicate n 1)
		in exscan (+) 0 counts
	in {
		base = skB,
		xys = sorted_xys,
		part_idx = zuowei,
		isPartitionDominated = isPartitionDominated,
		skyline_lt = skyline_lt,
		skyline_leq = skyline_leq
	}

def sort_for_Skyline_without_previous_windowing
	(skB) (xs) (ys) (minus) (over) (to_i64) (to_f64) (use_measure_for_sorting) (skyline_lt) (skyline_leq)
= sort_for_Skyline
	skB xs ys (minus) (over) (to_i64) (to_f64) (use_measure_for_sorting) (skyline_lt) (skyline_leq)
	(replicate skB.total_grid_no false)

def calc_local_Skyline [dim] 't 'pL_t
	(skI : skylineInfo [dim] t pL_t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64 : t -> f64)
: skylineInfo [dim] t pL_t =
	let filt_idx = (indices skI.xys)
		|> map (\i ->
			let this_x = skI.xys[i].0
			let (g, a, _) = get_grid_angle_measure_from_coords skI.base this_x
				(minus) (over) (to_i64) (to_f64)
			in if skI.isPartitionDominated[g] then (-1) else
			let p_id = get_id_from_grid_angle skI.base g a
			let start_idx = skI.part_idx[j]
			let end_idx = if j==skI.base.total_part_no-1 then n else skI.part_idx[j+1]
			let loop_over =
				loop (isElimd, j) = (false, start_idx)
				while !isElimd && j<end_idx do
					let cmp_x = skI.xs[j].0
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
	let skyline_xys = filt_idx |> map (\i -> skI.xys[i])
	let zuowei =
		let skyline_parts = skyline_xys
			|> map (\(x,_) ->
				(get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)).0
			)
		let counts = hist (+) 0 skI.base.total_part_no skyline_parts (replicate (length skyline_parts) 1)
		in exscan (+) 0 counts
	in {
		base = skI.base,
		xys = skyline_xys,
		part_idx = zuowei,
		isPartitionDominated = skI.isPartitionDominated,
		skyline_lt = skI.skyline_lt,
		skyline_leq = skI.skyline_leq
	}

-- this can only be used on skylines with the same base
-- otherwise will have to manually combine their xys, re-sort, etc...
-- ... very verbose...
def merge_Skylines_5 [dim] 't 'pL_t
	(skI1 : skylineInfo [dim] t pL_t)
	(skI2 : skylineInfo [dim] t pL_t)
	(skI3 : skylineInfo [dim] t pL_t)
	(skI4 : skylineInfo [dim] t pL_t)
	(skI5 : skylineInfo [dim] t pL_t)
	(minus : t -> t -> t)
	(over : t -> t -> t)
	(to_i64 : t -> i64)
	(to_f64)
	(dummy_elem : ([dim]t, pL_t))
: skylineInfo [dim] t pL_t =
	let skB = skI1.base
	let partDominated = map5
		(\pd1 pd2 pd3 pd4 pd5 -> pd1 || pd2 || pd3 || pd4 || pd5)
		skI1.isPartitionDominated
		skI2.isPartitionDominated
		skI3.isPartitionDominated
		skI4.isPartitionDominated
		skI5.isPartitionDominated
	let total_indices = map5
		(\i1 i2 i3 i4 i5 ->
			i1+i2+i3+i4+i5
		)
		skI1.part_idx skI2.part_idx skI3.part_idx skI4.part_idx skI5.part_idx
	let new_xys =
		let n_total =
			(length skI1.xys) +
			(length skI2.xys) +
			(length skI3.xys) +
			(length skI4.xys) +
			(length skI5.xys)
		let scatter_idx_base =
			in loop sc_is = (replicate 5 (replicate skI.base.total_part_no 0)) for j in (iota 5) do
				let new_isb =
					if j==0
					then total_indices
					else map2 (+) sc_is[j-1] total_indices
				in sc_is with [j] = new_isb
		let buff1 =
			let scatter_idx = indices skI1.xys
				|> map (\i -> 
					let (x,_) = skI1.xys[i]
					let (pid,_) = get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)
					in i + total_indices[0][pid]
				)
			in scatter (replicate n_total dummy_elem) scatter_idx skI1.xys
		let buff2 =
			let scatter_idx = indices skI2.xys
				|> map (\i -> 
					let (x,_) = skI2.xys[i]
					let (pid,_) = get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)
					in i + total_indices[1][pid]
				)
			in scatter buff1 scatter_idx skI2.xys
		let buff3 =
			let scatter_idx = indices skI3.xys
				|> map (\i -> 
					let (x,_) = skI3.xys[i]
					let (pid,_) = get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)
					in i + total_indices[2][pid]
				)
			in scatter buff2 scatter_idx skI3.xys
		let buff4 =
			let scatter_idx = indices skI4.xys
				|> map (\i -> 
					let (x,_) = skI4.xys[i]
					let (pid,_) = get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)
					in i + total_indices[3][pid]
				)
			in scatter buff3 scatter_idx skI4.xys
		let buff5 =
			let scatter_idx = indices skI5.xys
				|> map (\i -> 
					let (x,_) = skI5.xys[i]
					let (pid,_) = get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)
					in i + total_indices[4][pid]
				)
			in scatter buff4 scatter_idx skI5.xys
		in buff5
	--		|> filter (\(x,_) ->
	--			let g = get_grid_id_from_coords skB x (minus) (over) (to_i64)
	--			in !(partDominated[g])
	--		)
	--let zuowei =
	--	let skyline_parts = new_xys
	--		|> map (\(x,_) ->
	--			(get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)).0
	--		)
	--	let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate (length skyline_parts) 1)
	--	in exscan (+) 0 counts
	in {
		base = skB,
		xys = new_xys,
		part_idx = total_indices,
		isPartitionDominated = partDominated,
		skyline_lt = skI1.skyline_lt,
		skyline_leq = skI1.skyline_leq
	}

def merge_Skylines_4 (skI1) (skI2) (skI3) (skI4) (minus) (over) (to_i64) (to_f64) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skI1.base dummy_elem skI1.skyline_lt skI1.skyline_leq
	in merge_Skylines_5 skI1 skI2 skI3 skI4 dummy_SL (minus) (over) (to_i64) (to_f64) (dummy_elem)

def merge_Skylines_3 (skI1) (skI2) (skI3) (minus) (over) (to_i64) (to_f64) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skI1.base dummy_elem skI1.skyline_lt skI1.skyline_leq
	in merge_Skylines_4 skI1 skI2 skI3 dummy_SL (minus) (over) (to_i64) (to_f64) (dummy_elem)

def merge_Skylines_2 (skI1) (skI2) (minus) (over) (to_i64) (to_f64) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skI1.base dummy_elem skI1.skyline_lt skI1.skyline_leq
	in merge_Skylines_3 skI1 skI2 dummy_SL (minus) (over) (to_i64) (to_f64) (dummy_elem)

-- TODO
-- calc intermediate Skyline

def calc_global_Skyline[dim] 't 'pL_t
	(skI : skylineInfo [dim] t pL_t)
	(skyline_lt : t -> t -> bool)
	(skyline_leq : t -> t -> bool)
: skylineInfo [dim] t pL_t =
	let new_xys = (indices skI.xys)
		|> map (\i ->
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
	let zuowei =
		let skyline_parts = new_xys
			|> map (\(x,_) ->
				(get_id_measure_from_coords skB x (minus) (over) (to_i64) (to_f64)).0
			)
		let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate (length skyline_parts) 1)
		in exscan (+) 0 counts
	in {
		base = skI.base,
		xys = new_xys,
		part_idx = zuowei,
		isPartitionDominated = skI.isPartitionDominated,
		skyline_lt = skI.skyline_lt,
		skyline_leq = skI.skyline_leq
	}

