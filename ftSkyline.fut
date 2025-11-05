import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

type^ skylineOp 't = {
	plus : t -> t -> t,
	minus : t -> t -> t,
	times : t -> t -> t,
	over : t -> t -> t,
	skyline_lt : t -> t -> bool,
	skyline_leq : t -> t -> bool,
	from_i64 : i64 -> t,
	to_i64 : t -> i64,
	to_f64 : t -> f64,
	one : t
}
type~ skylineBase [dim] 't = {
-- will need to be constructed IN entry point...
	grid_partitions_per_dim : [dim]idx_t.t,
	angle_partitions_per_dim : [dim-1]idx_t.t,
	grid_part_prefix_sum : [dim]idx_t.t,
	angle_part_prefix_sum : [dim-1]idx_t.t,
	total_grid_no : idx_t.t,
	total_angle_no : idx_t.t,
	total_part_no : idx_t.t,
	start_of_grid_partition : [][dim]t,
	grid_part_size_per_dim : [dim]t
}
type~ skylineInfo [dim] 't 'pL_t = {
	xys : []([dim]t, pL_t),
	part_idx : []idx_t.t,
	isPartitionDominated : []bool,
}

def dummy_skylineInfo [dim] 't 'pL_t
	(skB : skylineBase [dim] t)
	(_ : ([dim]t, pL_t)) -- dummy elem
: skylineInfo [dim] t pL_t = {
	xys = [],
	part_idx = replicate skB.total_part_no 0,
	isPartitionDominated = replicate skB.total_grid_no false,
}

local def do_mk_skylineBase [dim] 't
	(skOp : skylineOp t)
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(partitionSizePerDim : [dim]t)
	(grid_partitions_per_dim : [dim]idx_t.t)
	(angle_partitions_per_dim : [dim-1]idx_t.t)
: skylineBase [dim] t =
	let total_grid_no = i64.product grid_partitions_per_dim
	let total_angle_no = i64.product angle_partitions_per_dim
	let total_part_no = total_grid_no*total_angle_no
	let grid_prefix = exscan (*) 1 grid_partitions_per_dim
	let ang_prefix = exscan (*) 1 angle_partitions_per_dim
	let start_points = (iota total_grid_no)
		|> map (\grid_id ->
			(iota dim)
				|> map (\i ->
					let perDim = grid_partitions_per_dim[i]
					let preDim = grid_prefix[i]
					let dim_id = skOp.from_i64 ((grid_id / preDim)%perDim)
					in (skOp.plus) min_per_dim[i] ((skOp.times) dim_id partitionSizePerDim[i])
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
	(skOp : skylineOp t)
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(grid_partitions_per_dim : [dim]idx_t.t)
	(angle_partitions_per_dim : [dim-1]idx_t.t)
: skylineBase [dim] t =
	let grid_per_dim = map (skOp.from_i64) grid_partitions_per_dim
	let partitionSizePerDim = map3
	(\m M n ->
			(skOp.over) ((skOp.plus) ((skOp.minus) M m) ((skOp.minus) n skOp.one)) n
		)
		min_per_dim
		max_per_dim
		grid_per_dim
	in do_mk_skylineBase
		skOp min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim

def mk_skylineBase_from_boundaries [dim] 't
	(skOp : skylineOp t)
	(min_per_dim : [dim]t)
	(max_per_dim : [dim]t)
	(partitionSizePerDim : [dim]t)
	(angle_partitions_per_dim : [dim-1]idx_t.t)
: skylineBase [dim] t =
	let grid_per_dim = map3
		(\m M s ->
			--(skOp.over) ((skOp.plus) ((skOp.minus) M m) ((skOp.minus) s skOp.one)) s
			skOp.over (skOp.minus M m) s
		)
		min_per_dim
		max_per_dim
		partitionSizePerDim
	let grid_partitions_per_dim = map (skOp.to_i64) grid_per_dim
	in do_mk_skylineBase
		skOp min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim

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
	(skOp : skylineOp t)
	(coords : [dim]t)
: (f64, [dim-1]f64) =
	let fc = map (skOp.to_f64) coords
	let r = f64.sqrt (foldl (+) 0 (map (\x -> x*x) fc))
	let phis : [dim-1]f64 =
		loop p = (replicate (dim-1) 0)
		for j in (iota (dim-1)) do
			let this_x = fc[j]
			let this_y = f64.sqrt (foldl (+) 0 (map (\x -> x*x) fc[(j+1):dim]))
			let new_phi = f64.atan2 this_y this_x
		in p with [j] = new_phi
	in (r, phis)

def get_grid_id_from_coords [dim] 't
	(skOp : skylineOp t)
	(skB: skylineBase [dim] t)
	(coords : [dim]t)
: idx_t.t =
	let point_0 = skB.start_of_grid_partition[0]
	let dists_0 = map2 (skOp.minus) coords point_0
	let grid_pos = (map2 (skOp.over) dists_0 skB.grid_part_size_per_dim)
		|> map (skOp.to_i64)
		|> map (\i -> i64.max i 0)
		|> map (\i -> i64.min i (skB.total_grid_no-1))
	in foldl (+) 0 (map2 (*) grid_pos skB.grid_part_prefix_sum)

def get_grid_angle_measure_from_coords [dim] 't
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
: (idx_t.t, idx_t.t, f64) =
	-- get grid id
	let g = get_grid_id_from_coords skOp skB coords
	-- get angle id
	let dists_g = map2 (skOp.minus) coords skB.start_of_grid_partition[g]
	let (measure, angles) = cartesian_to_spherical skOp dists_g
	let base_angles = iota (dim-1)
		|> map (\i -> (f64.pi/2.0) / (f64.i64 skB.angle_partitions_per_dim[i]) )
	let anglePart_ = map3
		(\a ba pf ->
			let i_ = (i64.f64 (a/ba))*pf
			in
				if i_ < 0 then 0
				else if i_ >= pf then (pf-1)
				else i_
		)
		angles
		base_angles
		skB.angle_part_prefix_sum
	let a = foldl (+) 0 anglePart_
	-- returns
	in (g, a, measure)

def get_id_measure_from_coords [dim] 't
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(coords : [dim]t)
: (idx_t.t, f64) =
	let (g,a,m) = get_grid_angle_measure_from_coords skOp skB coords
	in (get_id_from_grid_angle skB g a, m)

def sort_for_Skyline [n] [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(xs : [n][dim]t)
	(ys : [n]pL_t)
	(use_measure_for_sorting : bool)
	(previous_dominance : []bool)
: skylineInfo [dim] t pL_t =
	let part_leq : [dim]t -> [dim]t -> bool =
		(\x1 x2 ->
			let (id1, m1) = get_id_measure_from_coords skOp skB x1
			let (id2, m2) = get_id_measure_from_coords skOp skB x2
			in
				if use_measure_for_sorting
				then (id1<id2) || (id1==id2 && m1<=m2)
				else (id1<=id2)
		)
	let sorted_xys_ = merge_sort (\(x1,_) (x2,_) -> part_leq x1 x2) (zip xs ys)
	-- TODO scatter by id offset if !use_measure_for_sorting ?
	let isPartitionDominated =
		let count_per_grid_part =
			let grid_part_by_idx = sorted_xys_
				|> map (\(x,_) -> 
					get_grid_id_from_coords skOp skB x
				)
			in hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
		in skB.start_of_grid_partition
			|> map (\this_o ->
				let (isD,_) = loop (isDomd, j) = (false, 0)
					while !isDomd && j<skB.total_grid_no do
						let o_j = skB.start_of_grid_partition[j]
						let count_j = count_per_grid_part[j]
						let domd = (count_j>0 && foldl (&&) (true) (map2 (skOp.skyline_lt) this_o o_j))
						in (domd, j+1)
				in isD
			)
			|> zip previous_dominance
			|> map (\(alt,neu) -> alt || neu)
	let sorted_xys = sorted_xys_
		|> filter (\(x,_) ->
			let g = get_grid_id_from_coords skOp skB x
			in !(isPartitionDominated[g])
		)
	let n_filt = length sorted_xys
	let zuowei =
		let part_by_idx = sorted_xys
			|> map (\(x,_) -> 
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_filt]idx_t.t
		let counts = hist (+) 0 skB.total_part_no part_by_idx (replicate n_filt 1)
		in exscan (+) 0 counts
	in {
		xys = sorted_xys,
		part_idx = zuowei,
		isPartitionDominated = isPartitionDominated,
	}

def sort_for_Skyline_without_previous_windowing [n] [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(xs : [n][dim]t)
	(ys : [n]pL_t)
	(use_measure_for_sorting : bool)
: skylineInfo [dim] t pL_t =
	let part_leq : [dim]t -> [dim]t -> bool =
		(\x1 x2 ->
			let (id1, m1) = get_id_measure_from_coords skOp skB x1
			let (id2, m2) = get_id_measure_from_coords skOp skB x2
			in
				if use_measure_for_sorting
				then (id1<id2) || (id1==id2 && m1<=m2)
				else (id1<=id2)
		)
	let sorted_xys_ = merge_sort (\(x1,_) (x2,_) -> part_leq x1 x2) (zip xs ys)
	-- TODO scatter by id offset if !use_measure_for_sorting ?
	let isPartitionDominated =
		let count_per_grid_part =
			let grid_part_by_idx = sorted_xys_
				|> map (\(x,_) -> 
					get_grid_id_from_coords skOp skB x
				)
			in hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
		in skB.start_of_grid_partition
			|> map (\this_o ->
				let (isD,_) = loop (isDomd, j) = (false, 0)
					while !isDomd && j<skB.total_grid_no do
						let o_j = skB.start_of_grid_partition[j]
						let count_j = count_per_grid_part[j]
						let domd = (count_j>0 && foldl (&&) (true) (map2 (skOp.skyline_lt) this_o o_j))
						in (domd, j+1)
				in isD
			)
	let sorted_xys = sorted_xys_
		|> filter (\(x,_) ->
			let g = get_grid_id_from_coords skOp skB x
			in !(isPartitionDominated[g])
		)
	let n_filt = length sorted_xys
	let zuowei =
		let part_by_idx = sorted_xys
			|> map (\(x,_) -> 
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_filt]idx_t.t
		let counts = hist (+) 0 skB.total_part_no part_by_idx (replicate n_filt 1)
		in exscan (+) 0 counts
	in {
		xys = sorted_xys,
		part_idx = zuowei,
		isPartitionDominated = isPartitionDominated,
	}




def calc_local_Skyline [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI : skylineInfo [dim] t pL_t)
: skylineInfo [dim] t pL_t =
	let filt_idx = (indices skI.xys)
		|> map (\i ->
			let this_x = skI.xys[i].0
			let (g, a, _) = get_grid_angle_measure_from_coords skOp skB this_x
			in if skI.isPartitionDominated[g] then (-1) else
			let p_id = get_id_from_grid_angle skB g a
			let start_idx = skI.part_idx[i]
			let end_idx = if i==skB.total_part_no-1 then (length skI.xys) else skI.part_idx[i+1]
			let loop_over =
				loop (isElimd, j) = (false, start_idx)
				while !isElimd && j<end_idx do
					let cmp_x = skI.xys[j].0
					let elimd =
						(foldl (&&) (true) (map2 (skOp.skyline_leq) this_x cmp_x))
						&&
						(foldl (||) (false) (map2 (skOp.skyline_lt) this_x cmp_x))
					in (elimd, j+1)
			in
				if loop_over.0
				then (-1)
				else i
		)
		|> filter (>=0)
	let skyline_xys = filt_idx |> map (\i -> skI.xys[i])
	let n_filt = length (skyline_xys)
	let zuowei =
		let skyline_parts = skyline_xys
			|> map (\(x,_) ->
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_filt]idx_t.t
		let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate n_filt 1)
		in exscan (+) 0 counts
	in {
		xys = skyline_xys,
		part_idx = zuowei,
		isPartitionDominated = skI.isPartitionDominated
	}

-- this can only be used on skylines with the same base
-- otherwise will have to manually combine their xys, re-sort, etc...
def merge_Skylines_5 [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI1 : skylineInfo [dim] t pL_t)
	(skI2 : skylineInfo [dim] t pL_t)
	(skI3 : skylineInfo [dim] t pL_t)
	(skI4 : skylineInfo [dim] t pL_t)
	(skI5 : skylineInfo [dim] t pL_t)
	(dummy_elem : ([dim]t, pL_t))
: skylineInfo [dim] t pL_t =
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
			loop sc_is = (replicate 5 (replicate skB.total_part_no 0)) for j in (iota 5) do
				let new_isb =
					if j==0
					then total_indices
					else map2 (+) sc_is[j-1] total_indices
				in sc_is with [j] = new_isb
		let buff1 =
			let scatter_idx = indices skI1.xys
				|> map (\i -> 
					let (x,_) = skI1.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[pid]
					let post = 0
					in i + pref + post
				)
			in scatter (replicate n_total dummy_elem) scatter_idx skI1.xys
		let buff2 =
			let scatter_idx = indices skI2.xys
				|> map (\i -> 
					let (x,_) = skI2.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[i]
					let post =
						if pid==(length skI1.part_idx)
						then (length skI1.xys) - skI1.part_idx[pid]
						else skI1.part_idx[pid+1] - skI1.part_idx[pid]
					in i + pref + post
				)
			in scatter buff1 scatter_idx skI2.xys
		let buff3 =
			let scatter_idx = indices skI3.xys
				|> map (\i -> 
					let (x,_) = skI3.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[i]
					let post =
						if pid==(length skI1.part_idx)
						then
							(length skI1.xys)+(length skI2.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])
						else skI1.part_idx[pid+1]+skI2.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])
					in i + pref + post
				)
			in scatter buff2 scatter_idx skI3.xys
		let buff4 =
			let scatter_idx = indices skI4.xys
				|> map (\i -> 
					let (x,_) = skI4.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[i]
					let post =
						if pid==(length skI1.part_idx)
						then
							(length skI1.xys)+(length skI2.xys)+(length skI3.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])
						else skI1.part_idx[pid+1]+skI2.part_idx[pid+1]+skI3.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])
					in i + pref + post
				)
			in scatter buff3 scatter_idx skI4.xys
		let buff5 =
			let scatter_idx = indices skI5.xys
				|> map (\i -> 
					let (x,_) = skI5.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[i]
					let post =
						if pid==(length skI1.part_idx)
						then
							(length skI1.xys)+(length skI2.xys)+(length skI3.xys)+(length skI4.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])-(skI4.part_idx[pid])
						else
							skI1.part_idx[pid+1]+skI2.part_idx[pid+1]+skI3.part_idx[pid+1]+skI4.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])-(skI4.part_idx[pid])
					in i + pref + post
				)
			in scatter buff4 scatter_idx skI5.xys
		in buff5
	--		|> filter (\(x,_) ->
	--			let g = get_grid_id_from_coords skOp skB x
	--			in !(partDominated[g])
	--		)
	--let zuowei =
	--	let n_filt = length buff5
	--	let skyline_parts = new_xys
	--		|> map (\(x,_) ->
	--			(get_id_measure_from_coords skOp skB x).0
	--		) :> [n_filt]idx_t.t
	--	let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate n_filt 1)
	--	in exscan (+) 0 counts
	in {
		xys = new_xys,
		part_idx = total_indices,
		isPartitionDominated = partDominated
	}

def merge_Skylines_4 (skOp) (skB) (skI1) (skI2) (skI3) (skI4) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skB dummy_elem
	in merge_Skylines_5 skOp skB skI1 skI2 skI3 skI4 dummy_SL

def merge_Skylines_3 (skOp) (skB) (skI1) (skI2) (skI3) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skB dummy_elem
	in merge_Skylines_4 skOp skB skI1 skI2 skI3 dummy_SL

def merge_Skylines_2 (skOp) (skB) (skI1) (skI2) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skB dummy_elem
	in merge_Skylines_3 skOp skB skI1 skI2 dummy_SL

def intermediate_SkylineInfo [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI : skylineInfo [dim] t pL_t)
	(subdiv : idx_t.t)
: (skylineBase [dim] t, skylineInfo [dim] t pL_t) =
	-- 1. calculate dimensions to omit
	let (first_dim, subdiv_) =
		loop (d,sd) = (0,subdiv) while (d<dim && sd>1) do
			let d_parts = skB.grid_partitions_per_dim[d] in
			if d_parts>sd then (d,1)
			else (d+1, (sd+d_parts-1)/d_parts)
	let new_grid_parts_per_dim = (iota dim)
		|> map (\i ->
			if i<first_dim then 1
			else if i==first_dim then skB.grid_partitions_per_dim[i]/subdiv_
			else skB.grid_partitions_per_dim[i]
		)
	let new_grid_part_size_per_dim = (iota dim)
		|> map (\i ->
			if i < first_dim
				then ((skOp.times) skB.grid_part_size_per_dim[i] (skOp.from_i64 skB.grid_partitions_per_dim[i]))
			else if i==first_dim
				then ((skOp.times) skB.grid_part_size_per_dim[i] (skOp.from_i64 subdiv_))
			else
				skB.grid_part_size_per_dim[i]
			)
	let new_grid_part_starts = iota skB.total_grid_no
		|> filter (\i -> 
			(i % (skB.grid_part_prefix_sum[first_dim]*subdiv_) != 0)
		)
		|> map (\i -> skB.start_of_grid_partition[i])
	let new_grid_total = length new_grid_part_starts
	let new_skB = {
		grid_partitions_per_dim = new_grid_parts_per_dim,
		angle_partitions_per_dim = replicate (dim-1) 1,
		grid_part_prefix_sum = exscan (*) 1 new_grid_parts_per_dim,
		angle_part_prefix_sum = replicate (dim-1) 1,
		total_grid_no = new_grid_total,
		total_angle_no = 1,
		total_part_no = new_grid_total,
		start_of_grid_partition = new_grid_part_starts,
		grid_part_size_per_dim = new_grid_part_size_per_dim,
	}
	let new_part_idx = iota skB.total_grid_no
		|> filter (\i -> 
			(i % (skB.grid_part_prefix_sum[first_dim]*subdiv_) != 0)
		)
		|> map (\i -> skI.part_idx[i])
	let new_skI = {
		xys = skI.xys,
		part_idx = new_part_idx,
		isPartitionDominated = replicate skB.total_grid_no false
	}
	in (new_skB, new_skI)

def calc_intermediate_skyline [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI : skylineInfo [dim] t pL_t)
	(subdiv : idx_t.t)
	(size_thresh : idx_t.t)
: skylineInfo [dim] t pL_t =
	let last_sd = (skB.total_grid_no+subdiv-1)/subdiv
	let (intermediate_skB, intermediate_skI,_) =
		loop (sd_skB,sd_skI,sd) = (skB,skI,subdiv)
		while (sd<last_sd && (length sd_skI.xys)>size_thresh) do
			let (sd_skB_, sd_skI_) = intermediate_SkylineInfo skOp sd_skB sd_skI sd
			in (sd_skB_, calc_local_Skyline skOp sd_skB_ sd_skI_, sd*subdiv)
	let zuowei =
		let n_ = length (intermediate_skI.xys)
		let skyline_parts = intermediate_skI.xys
			|> map (\(x,_) ->
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_]idx_t.t
		let counts = hist (+) 0 intermediate_skB.total_part_no skyline_parts (replicate n_ 1)
		in exscan (+) 0 counts
	in {
		xys = intermediate_skI.xys,
		part_idx = zuowei,
		isPartitionDominated = skI.isPartitionDominated
	}

def calc_global_Skyline [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI : skylineInfo [dim] t pL_t)
: skylineInfo [dim] t pL_t =
	let n = length skI.xys
	let new_xys = (indices skI.xys)
		|> map (\i ->
			let this_x = skI.xys[i].0
			let loop_over =
				loop (isElimd, j) = (false, 0)
				while !isElimd && j<n do
					let cmp_x = skI.xys[j].0
					let elimd =
						(foldl (&&) (true) (map2 (skOp.skyline_leq) this_x cmp_x))
						&&
						(foldl (||) (false) (map2 (skOp.skyline_lt) this_x cmp_x))
					in (elimd, j+1)
			in
				if loop_over.0
				then (-1)
				else i
		)
		|> filter (>=0)
		|> map (\i -> skI.xys[i])
	let zuowei =
		let n_filt = length new_xys
		let skyline_parts = new_xys
			|> map (\(x,_) ->
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_filt]idx_t.t
		let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate n_filt 1)
		in exscan (+) 0 counts
	in {
		xys = new_xys,
		part_idx = zuowei,
		isPartitionDominated = skI.isPartitionDominated
	}

-- "crack" it like an egg to get the final result
def crack_Skyline [dim] 't 'pL_t
	(skI : skylineInfo [dim] t pL_t)
: ([][dim]t, []pL_t) =
	unzip skI.xys