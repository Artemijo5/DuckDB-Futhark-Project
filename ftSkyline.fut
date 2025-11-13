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
			--(skOp.over) ((skOp.plus) ((skOp.minus) M m) ((skOp.minus) n skOp.one)) n
			skOp.over ((skOp.minus) M m) n
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
			(skOp.over) ((skOp.plus) ((skOp.minus) M m) ((skOp.minus) s skOp.one)) s
			--skOp.over (skOp.minus M m) s
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
	let fc = seqmap (skOp.to_f64) coords
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
		|> seqmap (skOp.to_i64)
		|> seqmap (\i -> i64.max i 0)
		|> zip (iota dim)
		|> seqmap (\(d,i) -> i64.min i (skB.grid_partitions_per_dim[d]-1))
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
		|> seqmap (\i -> (f64.pi/2.0) / (f64.i64 skB.angle_partitions_per_dim[i]) )
	let anglePart_ = map4
		(\a ba pf pd ->
			let i_ = i64.f64 ((a/ba)*(f64.i64 pf))
			in
				if i_ < 0 then 0
				else if i_ >= pd then (pd-1)
				else i_
		)
		angles
		base_angles
		skB.angle_part_prefix_sum
		skB.angle_partitions_per_dim
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
	let isPartitionDominated =
		let count_per_grid_part =
			let grid_part_by_idx = sorted_xys_
				|> map (\(x,_) -> 
					get_grid_id_from_coords skOp skB x
				)
			in hist (+) 0 skB.total_grid_no grid_part_by_idx (replicate n 1)
		in (
			loop domArray=(replicate skB.total_grid_no false) for j in (iota skB.total_grid_no) do
				let this_o = skB.start_of_grid_partition[j]
				let isD = (iota skB.total_grid_no)
					|> map (\i ->
						if (i==j || count_per_grid_part[i]==0)
						then false
						else foldl (&&) (true) (seqmap2 (skOp.skyline_lt) this_o skB.start_of_grid_partition[i])
					)
					|> any (id)
				in domArray with [j] = isD
		--
			--skB.start_of_grid_partition
			--	|> map (\this_o ->
			--		let (isD,_) = loop (isDomd, j) = (false, 0)
			--			while !isDomd && j<skB.total_grid_no do
			--				let o_j = skB.start_of_grid_partition[j]
			--				let count_j = count_per_grid_part[j]
			--				let domd = (count_j>0 && foldl (&&) (true) (map2 (skOp.skyline_lt) this_o o_j))
			--				in (domd, j+1)
			--		in isD
			--	)
			)
			|> zip previous_dominance
			|> map (\(alt,neu) -> alt || neu)
	--let sorted_xys = sorted_xys_
	--	|> filter (\(x,_) ->
	--		let g = get_grid_id_from_coords skOp skB x
	--		in !(isPartitionDominated[g])
	--	)
	--let n_filt = length sorted_xys
	--let zuowei =
	--	let part_by_idx = sorted_xys
	--		|> map (\(x,_) -> 
	--			(get_id_measure_from_coords skOp skB x).0
	--		) :> [n_filt]idx_t.t
	--	let counts = hist (+) 0 skB.total_part_no part_by_idx (replicate n_filt 1)
	--	in exscan (+) 0 counts
	let (sorted_xys, zuowei) =
		let (filt_xys,filt_pids) = sorted_xys_
			|> map (\(x,_) -> (get_id_measure_from_coords skOp skB x).0)
			|> zip sorted_xys_
			|> filter (\(_,pid) ->
				let (g,_) = get_grid_angle_from_id skB pid
				in !(isPartitionDominated[g])
			)
			|> unzip
		let n_filt = length filt_pids
		let counts = hist (+) 0 skB.total_part_no (filt_pids :> [n_filt]idx_t.t) (replicate n_filt 1)
		let zw = exscan (+) 0 counts
		in (filt_xys, zw)
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
	let grid_no = skB.total_grid_no
	let sorted_xys_ =
		if !use_measure_for_sorting then
			let pids = xs |> map (\x -> (get_id_measure_from_coords skOp skB x).0)
			in (zip3 pids xs ys)
				|> merge_sort (\(p1,_,_) (p2,_,_) -> p1<=p2)
				|> map (\(p,x,y) -> (x,y))
		else
			let pids = xs |> map (\x -> get_id_measure_from_coords skOp skB x)
			in (zip3 pids xs ys)
				|> merge_sort (\((p1,m1),_,_) ((p2,m2),_,_) -> p1<p2 || (p1==p2 && m1<=m2))
				|> map (\((p,m),x,y) -> (x,y))
	let isPartitionDominated =
		let count_per_grid_part =
			let grid_part_by_idx = sorted_xys_
				|> map (\(x,_) -> 
					get_grid_id_from_coords skOp skB x
				)
			in hist (+) 0 grid_no grid_part_by_idx (replicate n 1)
		in
		-- Loop Inversion
			loop domArray=(replicate grid_no false) for j in (iota grid_no) do
				let this_o = skB.start_of_grid_partition[j]
				let isD = (iota grid_no)
					|> map (\i ->
						if (i==j || count_per_grid_part[i]==0)
						then false
						else foldl (&&) (true) (map2 (skOp.skyline_lt) this_o skB.start_of_grid_partition[i])
					)
					|> any (id)
				in domArray with [j] = isD
		--
			--skB.start_of_grid_partition
			--	|> map (\this_o ->
			--		let (isD,_) = loop (isDomd, j) = (false, 0)
			--			while !isDomd && j<skB.total_grid_no do
			--				let o_j = skB.start_of_grid_partition[j]
			--				let count_j = count_per_grid_part[j]
			--				let domd = (count_j>0 && foldl (&&) (true) (map2 (skOp.skyline_lt) this_o o_j))
			--				in (domd, j+1)
			--		in isD
			--	)
	--let sorted_xys = sorted_xys_
	--	|> filter (\(x,_) ->
	--		let g = get_grid_id_from_coords skOp skB x
	--		in !(isPartitionDominated[g])
	--	)
	--let n_filt = length sorted_xys
	--let zuowei =
	--	let part_by_idx = (iota n_filt)
	--	-- Why can't this be done in parallel!!!!!
	--	-- TODO try re-planning the mapping...
	--		|> seqmap (\i -> 
	--			(get_id_measure_from_coords skOp skB sorted_xys[i].0).0
	--		) :> [n_filt]idx_t.t
	--	let counts = hist (+) 0 skB.total_part_no part_by_idx (replicate n_filt 1)
	--	in exscan (+) 0 counts
	-- -- calculating pids after filter gives "known compiler limitation - cannot handle unsliceable allocation size..."
	let (sorted_xys, zuowei) =
		let (filt_xys,filt_pids) = sorted_xys_
			|> map (\xy -> ( xy , (get_id_measure_from_coords skOp skB xy.0).0 ) )
			|> filter (\(_,pid) ->
				let (g,_) = get_grid_angle_from_id skB pid
				in !(isPartitionDominated[g])
			)
			|> unzip
		let n_filt = length filt_pids
		let counts = hist (+) 0 skB.total_part_no (filt_pids :> [n_filt]idx_t.t) (replicate n_filt 1)
		let zw = exscan (+) 0 counts
		in (filt_xys, zw)
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
	-- Loop Inversion
	let (skyline_xys, skyline_pids) = 
		let part_sizes = (indices skI.part_idx)
			|> map (\i ->
				if i==(length skI.part_idx)-1
				then (length skI.xys)-skI.part_idx[i]
				else skI.part_idx[i+1] - skI.part_idx[i]
			)
		let max_part_size = idx_t.maximum part_sizes
		let filt_idx_ =
			loop idxs = (indices skI.xys) for j in (iota max_part_size) do
				let part_ids = skI.xys
					|> map (\(x,_) -> get_grid_angle_measure_from_coords skOp skB x)
					|> map (\(g,a,_) ->
						get_id_from_grid_angle skB g a
					)
				let cmp_xs = part_ids
					|> map (\pid -> (pid, part_sizes[pid]))
					-- map (\(pid,ps) -> idx_t.max 0 (idx_t.min (skI.part_idx[pid]+j) (skI.part_idx[pid]+ps-1)))
					|> map (\(pid,ps) -> idx_t.min (skI.part_idx[pid]+j) ((length skI.xys)-1)) -- not "true" local skyline
					|> map (\pj -> skI.xys[pj].0)
				in map4
					(\i (this_x,_) cmp_x pid ->
						if (i<0 || i==j) then i else
						let (gid,_) = get_grid_angle_from_id skB pid in
						if skI.isPartitionDominated[gid] then (-1) else
						let elimd =
							(foldl (&&) (true) (seqmap2 (skOp.skyline_leq) this_x cmp_x))
							&&
							(foldl (||) (false) (seqmap2 (skOp.skyline_lt) this_x cmp_x))
						in
							if elimd then (-1) else i
					)
					idxs
					skI.xys
					cmp_xs
					part_ids
		let filt_idx = filt_idx_ |> filter (>=0)
		let sk_xys = filt_idx |> map (\i -> skI.xys[i])
		let sk_pids =
			-- -- calculating pids directly from filt_idx instead of gathering like this
			-- -- gives "known compiler limitation - cannot handle un-sliceable allocation size"...
			let all_pids = skI.xys |> map (\(x,_) -> (get_id_measure_from_coords skOp skB x).0)
			in filt_idx |> map (\i -> all_pids[i])
		in (sk_xys, sk_pids)
	let n_filt = length (skyline_pids)
	let zuowei =
		let counts = hist (+) 0 skB.total_part_no (skyline_pids :> [n_filt]idx_t.t) (replicate n_filt 1)
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
					in i + pref + post - skI1.part_idx[pid]
				)
			in scatter (replicate n_total dummy_elem) scatter_idx skI1.xys
		let buff2 =
			let scatter_idx = indices skI2.xys
				|> map (\i -> 
					let (x,_) = skI2.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[pid]
					let post =
						if pid==(length skI1.part_idx)-1
						then (length skI1.xys) - skI1.part_idx[pid]
						else skI1.part_idx[pid+1] - skI1.part_idx[pid]
					in i + pref + post - skI2.part_idx[pid]
				)
			in scatter buff1 scatter_idx skI2.xys
		let buff3 =
			let scatter_idx = indices skI3.xys
				|> map (\i -> 
					let (x,_) = skI3.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[pid]
					let post =
						if pid==(length skI1.part_idx)-1
						then
							(length skI1.xys)+(length skI2.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])
						else skI1.part_idx[pid+1]+skI2.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])
					in i + pref + post - skI3.part_idx[pid]
				)
			in scatter buff2 scatter_idx skI3.xys
		let buff4 =
			let scatter_idx = indices skI4.xys
				|> map (\i -> 
					let (x,_) = skI4.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[pid]
					let post =
						if pid==(length skI1.part_idx)-1
						then
							(length skI1.xys)+(length skI2.xys)+(length skI3.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])
						else skI1.part_idx[pid+1]+skI2.part_idx[pid+1]+skI3.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])
					in i + pref + post - skI4.part_idx[pid]
				)
			in scatter buff3 scatter_idx skI4.xys
		let buff5 =
			let scatter_idx = indices skI5.xys
				|> map (\i -> 
					let (x,_) = skI5.xys[i]
					let (pid,_) = get_id_measure_from_coords skOp skB x
					let pref = total_indices[pid]
					let post =
						if pid==(length skI1.part_idx)-1
						then
							(length skI1.xys)+(length skI2.xys)+(length skI3.xys)+(length skI4.xys)
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])-(skI4.part_idx[pid])
						else
							skI1.part_idx[pid+1]+skI2.part_idx[pid+1]+skI3.part_idx[pid+1]+skI4.part_idx[pid+1]
							-(skI1.part_idx[pid])-(skI2.part_idx[pid])-(skI3.part_idx[pid])-(skI4.part_idx[pid])
					in i + pref + post - skI5.part_idx[pid]
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
	in merge_Skylines_5 skOp skB skI1 skI2 skI3 skI4 dummy_SL dummy_elem

def merge_Skylines_3 (skOp) (skB) (skI1) (skI2) (skI3) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skB dummy_elem
	in merge_Skylines_4 skOp skB skI1 skI2 skI3 dummy_SL dummy_elem

def merge_Skylines_2 (skOp) (skB) (skI1) (skI2) (dummy_elem) =
	let dummy_SL = dummy_skylineInfo skB dummy_elem
	in merge_Skylines_3 skOp skB skI1 skI2 dummy_SL dummy_elem

def intermediate_SkylineInfo [dim] 't 'pL_t
	(skOp : skylineOp t)
	(skB : skylineBase [dim] t)
	(skI : skylineInfo [dim] t pL_t)
	(omit_dims : idx_t.t)
: (skylineBase [dim] t, skylineInfo [dim] t pL_t) =
	let grid_no_omitted =
		if omit_dims == dim
		then skB.total_grid_no
		else skB.grid_part_prefix_sum[omit_dims]
	let new_grid_partitions_per_dim = skB.grid_partitions_per_dim
		|> zip (iota dim)
		|> map (\(i,pd) -> if i<omit_dims then 1 else pd)
	let new_grid_part_prefix_sum = exscan (*) 1 new_grid_partitions_per_dim
	let new_total_grid_no = i64.product new_grid_partitions_per_dim
	let new_starts = skB.start_of_grid_partition
		|> zip (indices skB.start_of_grid_partition)
		|> filter (\(i,_) -> (i%grid_no_omitted)==0)
		|> map (\(_,sp) -> sp)
	let new_sizes = iota dim
		|> map (\d ->
			if d>=omit_dims
			then skB.grid_part_size_per_dim[d]
			else skOp.times skB.grid_part_size_per_dim[d] (skOp.from_i64 skB.grid_partitions_per_dim[d])
		)
	let new_skB : skylineBase [dim] t = {
		grid_partitions_per_dim = new_grid_partitions_per_dim,
		angle_partitions_per_dim = replicate (dim-1) 1,
		grid_part_prefix_sum = exscan (*) 1 new_grid_partitions_per_dim,
		angle_part_prefix_sum = replicate (dim-1) 1,
		total_grid_no = new_total_grid_no,
		total_angle_no = 1,
		total_part_no = new_total_grid_no,
		start_of_grid_partition = new_starts,
		grid_part_size_per_dim = new_sizes
	}
	let new_part_idx = skI.part_idx
		|> zip (indices skI.part_idx)
		|> filter (\(i,_) -> (i%(grid_no_omitted*skB.total_angle_no)) == 0)
		|> map (\(_,pidx) -> pidx)
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
	(include_zero_step : bool)
	(omit_step : idx_t.t)
	(max_steps : idx_t.t)
	(size_thresh : idx_t.t)
: skylineInfo [dim] t pL_t =
	let (_, intermediate_skI,_) =
		loop (sd_skB,sd_skI,i) = (skB,skI, (if include_zero_step then 0 else 1))
		while (i<=max_steps && ((i-1)*omit_step<dim) && (length sd_skI.xys)>size_thresh) do
			let (sd_skB_, sd_skI_) = intermediate_SkylineInfo skOp sd_skB sd_skI (idx_t.min (i*omit_step) dim)
			in (sd_skB_, calc_local_Skyline skOp sd_skB_ sd_skI_, i+1)
	let zuowei =
		let n_ = length (intermediate_skI.xys)
		let skyline_parts = intermediate_skI.xys
		-- TODO currently can't think of a better solution here...
			|> seqmap (\(x,_) ->
				(get_id_measure_from_coords skOp skB x).0
			) :> [n_]idx_t.t
		let counts = hist (+) 0 skB.total_part_no skyline_parts (replicate n_ 1)
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
	-- Loop Inversion
	let (new_xys, new_pids) =
		let new_is_ = loop ni = (iota n) for j in (iota n) do
			let cmp_x = skI.xys[j].0
			in ni |> map (\i ->
				if (i<0 || i==j) then i else
				let this_x = skI.xys[i].0
				let elimd =
						(foldl (&&) (true) (seqmap2 (skOp.skyline_leq) this_x cmp_x))
						&&
						(foldl (||) (false) (seqmap2 (skOp.skyline_lt) this_x cmp_x))
				in
					if elimd then (-1) else i
			)
		let new_is = new_is_ |> filter (>=0)
		let nxys = new_is |> map (\i -> skI.xys[i])
		let npids =
			-- -- calculating pids directly from filt_idx instead of gathering like this
			-- -- gives "known compiler limitation - cannot handle un-sliceable allocation size"...
			let all_pids = skI.xys |> map (\(x,_) -> (get_id_measure_from_coords skOp skB x).0)
			in new_is |> map (\i -> all_pids[i])
		in (nxys,npids)
	--
		--let new_xys = (indices skI.xys)
		--	|> map (\i ->
		--		let this_x = skI.xys[i].0
		--		let loop_over =
		--			loop (isElimd, j) = (false, 0)
		--			while !isElimd && j<n do
		--				let cmp_x = skI.xys[j].0
		--				let elimd =
		--					(foldl (&&) (true) (map2 (skOp.skyline_leq) this_x cmp_x))
		--					&&
		--					(foldl (||) (false) (map2 (skOp.skyline_lt) this_x cmp_x))
		--				in (elimd, j+1)
		--		in
		--			if loop_over.0
		--			then (-1)
		--			else i
		--	)
		--	|> filter (>=0)
		--	|> map (\i -> skI.xys[i])
	let zuowei =
		let n_filt = length new_pids
		let counts = hist (+) 0 skB.total_part_no (new_pids :> [n_filt]idx_t.t) (replicate n_filt 1)
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


-- ENTRY POINTS ----------------------------------------------------------------------------------------------------

	def skylineOp_double : skylineOp f64 = {
		plus = (+),
		minus = (-),
		times = (*),
		over = (/),
		skyline_lt = (>),
		skyline_leq = (>=),
		from_i64 = (f64.i64),
		to_i64 = (i64.f64),
		to_f64 = (id),
		one = f64.i64 1
	}

	type~ skylineBase_double [dim] = skylineBase [dim] f64
	type~ skylineInfo_GFUR_double [dim] = skylineInfo [dim] f64 i64

	entry define_skyline_space_double [dim]
		(min_per_dim : [dim]f64)
		(max_per_dim : [dim]f64)
		(grid_partitions_per_dim : [dim]idx_t.t)
		(angle_partitions_per_dim : [dim-1]idx_t.t)
	: skylineBase_double [dim] =
		mk_skylineBase_from_grid
			skylineOp_double
			min_per_dim
			max_per_dim
			grid_partitions_per_dim
			angle_partitions_per_dim

	entry sort_for_Skyline_GFUR_double [n] [dim]
		(skB : skylineBase_double [dim])
		(xs : [n][dim]f64)
		(offset : idx_t.t)
		(use_measure_for_sorting : bool)
	: skylineInfo_GFUR_double [dim] =
		sort_for_Skyline_without_previous_windowing
			skylineOp_double
			skB
			xs
			(map (\i -> i+offset) (indices xs))
			use_measure_for_sorting

	entry calc_local_Skyline_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		calc_local_Skyline skylineOp_double skB skI

	entry calc_global_Skyline_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		calc_global_Skyline skylineOp_double skB skI

	-- takes a very long time to compile...
	entry merge_Skylines_5_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI1 : skylineInfo_GFUR_double [dim])
		(skI2 : skylineInfo_GFUR_double [dim])
		(skI3 : skylineInfo_GFUR_double [dim])
		(skI4 : skylineInfo_GFUR_double [dim])
		(skI5 : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		let dummy = ((replicate dim f64.highest),-1)
		in merge_Skylines_5 skylineOp_double skB skI1 skI2 skI3 skI4 skI5 dummy
	entry merge_Skylines_4_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI1 : skylineInfo_GFUR_double [dim])
		(skI2 : skylineInfo_GFUR_double [dim])
		(skI3 : skylineInfo_GFUR_double [dim])
		(skI4 : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		let dummy = ((replicate dim f64.highest),-1)
		in merge_Skylines_4 skylineOp_double skB skI1 skI2 skI3 skI4 dummy
	entry merge_Skylines_3_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI1 : skylineInfo_GFUR_double [dim])
		(skI2 : skylineInfo_GFUR_double [dim])
		(skI3 : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		let dummy = ((replicate dim f64.highest),-1)
		in merge_Skylines_3 skylineOp_double skB skI1 skI2 skI3 dummy
	entry merge_Skylines_2_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI1 : skylineInfo_GFUR_double [dim])
		(skI2 : skylineInfo_GFUR_double [dim])
	: skylineInfo_GFUR_double [dim] =
		let dummy = ((replicate dim f64.highest),-1)
		in merge_Skylines_2 skylineOp_double skB skI1 skI2 dummy

	entry calc_intermSkyline_GFUR_double [dim]
		(skB : skylineBase_double [dim])
		(skI : skylineInfo_GFUR_double [dim])
		(include_zero_step : bool)
		(omit_step : idx_t.t)
		(max_steps : idx_t.t)
		(size_thresh : idx_t.t)
	: skylineInfo_GFUR_double [dim] =
		calc_intermediate_skyline skylineOp_double skB skI include_zero_step omit_step max_steps size_thresh


-- TESTING ---------------------------------------------------------------------------------------------------------

	def test_skB_2d =
		mk_skylineBase_from_grid
		(skylineOp_double)
		([0,0])
		([10,10])
		([4,4])
		([2] :> [2-1]i64)

	def test_skB_3d =
		mk_skylineBase_from_grid
		(skylineOp_double)
		([0,0,0])
		([12,10,10])
		([3,2,4])
		([2,3] :> [3-1]i64)

	def test_skB_5d =
		mk_skylineBase_from_grid
		(skylineOp_double)
		([0,0,0,0,0])
		([12,10,10,12,10])
		([3,2,2,3,2])
		([2,3,5,2] :> [5-1]i64)

	def test_coords_2d =
		let skOp = skylineOp_double
		let skB = test_skB_2d
		let points : [][]f64 = [[0,0],[1,0],[0,2],[10,10],[12,11],[-1,5],[2,1],[4,2.7],[5.0,7.5],[7.6,2.2]]
		let (g,a,m) = points |> map (\c -> get_grid_angle_measure_from_coords skOp skB c) |> unzip3
		let pid = map2 (get_id_from_grid_angle skB) g a
		in zip5 points g a m pid

	def test_sort_2d (use_measure) =
		let skOp = skylineOp_double
		let skB = copy test_skB_2d
		let points = [[2,1],[0.2,1],[2.5,4],[0.17,7.6],[0.1,7.51],[0.05,0],[2.7,4],[8,8],[9,8],[2.2,7.65]]
		in sort_for_Skyline_without_previous_windowing skOp skB points (indices points) use_measure

	def test_local_skyline_2d (use_measure) =
		let skOp = skylineOp_double
		let skB = copy test_skB_2d
		let skI = test_sort_2d use_measure
		in calc_local_Skyline skOp skB skI

	def test_global_skyline_2d (use_measure) =
		let skOp = skylineOp_double
		let skB = copy test_skB_2d
		let skI = test_local_skyline_2d use_measure
		in calc_global_Skyline skOp skB skI

	def test_skylineBase_3d =
		mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)

	def test_points_3d : [][]f64 = [
			[6,1,1],[6.4,0.5,0.2],[5.6,1.4,0.3],[6.5,2.3,0.4],
			[1,6,4],[0.6,6.6,0.4],[1.2,5.8,2.2],[2.2,6.2,3],
			[6,12,1],[5,13,1],[6.2,11,1],[5.1,12,1],
			[2,2,6.2],[1,3,6],[1,2,6.1],[2,1,6.1],
			[7,9.5,8],[9,5.5,6],[5.1,5.7,5.2],[8,5,6],
			[12,9,9.5],[15,6,8],[12,6,8],[10,8,6],
			[12,14,9.4],[15,11,7.9],[12,11,7.9],[10,13,6.1],
			[10,4,10],[11,3,11],[10,2,11],[11,1,10],
			[5,5,11],[6,5,10],[6,5.2,12],[7,5.3,12],
			[6,12,12],[7,11,13],[5,11,12],[6,12,10]
		]

	def test_sort_3d (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let points = [
			[6,1,1],[6.4,0.5,0.2],[5.6,1.4,0.3],[6.5,2.3,0.4],
			[1,6,4],[0.6,6.6,0.4],[1.2,5.8,2.2],[2.2,6.2,3],
			[6,12,1],[5,13,1],[6.2,11,1],[5.1,12,1],
			[2,2,6.2],[1,3,6],[1,2,6.1],[2,1,6.1],
			[7,9.5,8],[9,5.5,6],[5.1,5.7,5.2],[8,5,6],
			[12,9,9.5],[15,6,8],[12,6,8],[10,8,6],
			[12,14,9.4],[15,11,7.9],[12,11,7.9],[10,13,6.1],
			[10,4,10],[11,3,11],[10,2,11],[11,1,10],
			[5,5,11],[6,5,10],[6,5.2,12],[7,5.3,12],
			[6,12,12],[7,11,13],[5,11,12],[6,12,10]
		]
		in sort_for_Skyline_without_previous_windowing skOp skB points (indices points) use_measure

	def test_local_skyline_3d (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let skI = test_sort_3d use_measure
		in calc_local_Skyline skOp skB skI

	def test_global_skyline_3d (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let skI = test_local_skyline_3d use_measure
		in calc_global_Skyline skOp skB skI

	def test_merge_skylines_5 (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let points1 = [
			[6,1,1],
			[1,6,4],
			[6,12,1],
			[2,2,6.2],
			[7,9.5,8],
			[12,9,9.5],
			[12,14,9.4],
			[5,5,11]
		]
		let points2 = [
			[6.4,0.5,0.2],
			[0.6,6.6,0.4],
			[5,13,1],
			[1,3,6],
			[9,5.5,6],
			[15,6,8],
			[15,11,7.9],
			[6,5,10]
		]
		let points3 = [
			[5.6,1.4,0.3],
			[1.2,5.8,2.2],
			[6.2,11,1],
			[1,2,6.1],
			[5.1,5.7,5.2],
			[12,6,8],
			[12,11,7.9],
			[6,5.2,12]
		]
		let points4 = [
			[6.5,2.3,0.4],
			[2.2,6.2,3],
			[5.1,12,1],
			[2,1,6.1],
			[8,5,6],
			[10,8,6],
			[10,13,6.1],
			[7,5.3,12]
		]
		let points5 = [
			[10,4,10],[11,3,11],[10,2,11],[11,1,10],
			[6,12,12],[7,11,13],[5,11,12],[6,12,10]
		]
		let skI1 = sort_for_Skyline_GFUR_double skB points1 0 use_measure
		let skI2 = sort_for_Skyline_GFUR_double skB points2 8 use_measure
		let skI3 = sort_for_Skyline_GFUR_double skB points3 16 use_measure
		let skI4 = sort_for_Skyline_GFUR_double skB points4 24 use_measure
		let skI5 = sort_for_Skyline_GFUR_double skB points5 32 use_measure
		in merge_Skylines_5 skOp skB skI1 skI2 skI3 skI4 skI5 ([9000,9000,9000],-1)

	def test_merge_skylines_3 (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let points1 = [
			[6,1,1],[6.4,0.5,0.2],
			[1,6,4],[0.6,6.6,0.4],
			[6,12,1],[5,13,1],
			[2,2,6.2],[1,3,6],
			[7,9.5,8],[9,5.5,6],
			[12,9,9.5],[15,6,8],
			[12,14,9.4],[15,11,7.9],
			[5,5,11],[6,5,10]
		]
		let points2 = [
			[5.6,1.4,0.3],[6.5,2.3,0.4],
			[1.2,5.8,2.2],[2.2,6.2,3],
			[6.2,11,1],[5.1,12,1],
			[1,2,6.1],[2,1,6.1],
			[5.1,5.7,5.2],[8,5,6],
			[12,6,8],[10,8,6],
			[12,11,7.9],[10,13,6.1],
			[6,5.2,12],[7,5.3,12]
		]
		let points3 = [
			[10,4,10],[11,3,11],[10,2,11],[11,1,10],
			[6,12,12],[7,11,13],[5,11,12],[6,12,10]
		]
		let skI1 = sort_for_Skyline_GFUR_double skB points1 0 use_measure
		let skI2 = sort_for_Skyline_GFUR_double skB points2 8 use_measure
		let skI3 = sort_for_Skyline_GFUR_double skB points3 16 use_measure
		in merge_Skylines_3 skOp skB skI1 skI2 skI3 ([9000,9000,9000],-1)

	def test_merge_skylines_2 (use_measure) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let points1 = [
			[6,1,1],[6.4,0.5,0.2],
			[1,6,4],[0.6,6.6,0.4],
			[6,12,1],[5,13,1],
			[2,2,6.2],[1,3,6],
			[7,9.5,8],[9,5.5,6],
			[12,9,9.5],[15,6,8],
			[12,14,9.4],[15,11,7.9],
			[10,4,10],[11,3,11],
			[5,5,11],[6,5,10],
			[6,12,12],[7,11,13]
		]
		let points2 = [
			[5.6,1.4,0.3],[6.5,2.3,0.4],
			[1.2,5.8,2.2],[2.2,6.2,3],
			[6.2,11,1],[5.1,12,1],
			[1,2,6.1],[2,1,6.1],
			[5.1,5.7,5.2],[8,5,6],
			[12,6,8],[10,8,6],
			[12,11,7.9],[10,13,6.1],
			[10,2,11],[11,1,10],
			[6,5.2,12],[7,5.3,12],
			[5,11,12],[6,12,10]
		]
		let skI1 = sort_for_Skyline_GFUR_double skB points1 0 use_measure
		let skI2 = sort_for_Skyline_GFUR_double skB points2 8 use_measure
		in merge_Skylines_2 skOp skB skI1 skI2 ([9000,9000,9000],-1)

	def test_intermediate_skyline (subdiv:idx_t.t) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let skI = test_local_skyline_3d false
		in intermediate_SkylineInfo skOp skB skI subdiv

	def test_calc_interm_skyline (include_zero_step : bool) (omit_step) (max_steps) (size_thresh) =
		let skOp = skylineOp_double
		let skB = mk_skylineBase_from_grid
			(skylineOp_double)
			([0,0,0])
			([15,15,15])
			([3,3,3])
			([2,2] :> [3-1]i64)
		let skI = test_local_skyline_3d false
		in calc_intermediate_skyline skOp skB skI include_zero_step omit_step max_steps size_thresh