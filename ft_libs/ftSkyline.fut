import "ftbasics"

type~ skylineBase_t [dim] 't = {
	grid_partitions_per_dim : [dim]i64,
	angle_partitions_per_dim : [dim-1]i64,
	grid_part_prefix_sum : [dim]i64,
	angle_part_prefix_sum : [dim-1]i64,
	total_grid_no : i64,
	total_angle_no : i64,
	total_part_no : i64,
	start_of_grid_partition : [][dim]t,
	grid_part_size_per_dim : [dim]t,
	m_size : i64
}
type~ skylineInfo_t [dim] 't 'pL_t = {
	xys : []([dim]t, pL_t),
	isPartitionDominated : []bool,
}
type~ skylineData_t [dim] 't 'pL_t = {
	len : i64,
	dat : [][dim]t,
	pL : []pL_t
}

module skyline_real (F:real) = {
	-- Types and Operators
		type t = F.t

		type~ skylineBase [dim] = skylineBase_t [dim] t
		type~ skylineInfo [dim] 'pL_t = skylineInfo_t [dim] t pL_t
		type~ skylineData [dim] 'pL_t = skylineData_t [dim] t pL_t

		local def gt = (F.>)
		local def geq = (F.>=)
		local def lt = (F.<)
		local def leq = (F.<=)
		local def eq = (F.==)

		local def plus = (F.+)
		local def minus = (F.-)
		local def times = (F.*)
		local def over = (F./)

		local def from_i64 = (F.i64)
		local def to_i64 = (F.to_i64)

		local def atan2 = (F.atan2)
		local def sqrt = (F.sqrt)

		local def zero = F.i32 0
		local def one = F.i32 1
		local def piSeconds = over (F.pi) (F.i32 2)
		local def highest = F.highest

	-- Create SkylineBase
		local def do_mk_skylineBase [dim]
			(min_per_dim : [dim]t)
			(max_per_dim : [dim]t)
			(partitionSizePerDim : [dim]t)
			(grid_partitions_per_dim : [dim]i64)
			(angle_partitions_per_dim : [dim-1]i64)
			(m_size : i64)
		: skylineBase [dim] =
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
							let dim_id = from_i64 ((grid_id / preDim)%perDim)
							in plus min_per_dim[i] (times dim_id partitionSizePerDim[i])
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
				grid_part_size_per_dim = partitionSizePerDim,
				m_size = m_size
			}

		def mk_skylineBase_from_grid [dim]
			(min_per_dim : [dim]t)
			(max_per_dim : [dim]t)
			(grid_partitions_per_dim : [dim]i64)
			(angle_partitions_per_dim : [dim-1]i64)
			(m_size : i64)
		: skylineBase [dim] =
			let grid_per_dim = map (from_i64) grid_partitions_per_dim
			let partitionSizePerDim = map3
			(\m M n ->
					over (minus M m) n
				)
				min_per_dim
				max_per_dim
				grid_per_dim
			in do_mk_skylineBase
				min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim m_size

	-- Partition ID & measure calculation
		local def get_id_from_grid_angle [dim]
		(skB : skylineBase [dim])
		(grid_id : i64)
		(angle_id : i64)
		: i64 =
			grid_id*(skB.total_angle_no) + angle_id

		local def get_grid_angle_from_id [dim]
			(skB : skylineBase [dim])
			(part_id : i64)
		: (i64, i64) =
			let grid_id = part_id / skB.total_angle_no
			let angle_id = part_id % skB.total_angle_no
			in (grid_id, angle_id)

		local def cartesian_to_spherical [dim]
			(coords : [dim]t)
		: (t, [dim-1]t) =
			let r = coords |> map (\x -> times x x) |> foldl (plus) (zero) |> sqrt
			let phis : [dim-1]t =
				loop p = (replicate (dim-1) (zero))
				for j in (iota (dim-1)) do
					let this_x = coords[j]
					let this_y = coords[(j+1):dim] |> map (\x -> times x x) |> foldl (plus) (zero) |> sqrt
					let new_phi = atan2 this_y this_x
				in p with [j] = new_phi
			in (r, phis)

		local def get_grid_id_from_coords [dim]
			(skB: skylineBase [dim])
			(coords : [dim]t)
		: i64 =
			let point_0 = skB.start_of_grid_partition[0]
			let dists_0 = map2 (minus) coords point_0
			let grid_pos = (map2 (over) dists_0 skB.grid_part_size_per_dim)
				|> map (to_i64)
				|> map (\i -> i64.max i 0)
				|> zip (iota dim)
				|> map (\(d,i) -> i64.min i (skB.grid_partitions_per_dim[d]-1))
			in foldl (+) 0 (map2 (*) grid_pos skB.grid_part_prefix_sum)

		local def get_grid_angle_measure_from_coords [dim]
			(skB : skylineBase [dim])
			(coords : [dim]t)
		: (i64, i64, t) =
			-- get grid id
			let g = get_grid_id_from_coords skB coords
			-- get angle id
			let dists_g = map2 (minus) coords skB.start_of_grid_partition[g]
			let (measure, angles) = cartesian_to_spherical dists_g
			let base_angles = iota (dim-1)
				|> map (\i -> over (piSeconds) (from_i64 skB.angle_partitions_per_dim[i]) )
			let anglePart_ = map4
				(\a ba pf pd ->
					let i_ = to_i64 (times (over a ba) (from_i64 pf))
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

		local def get_id_measure_from_coords [dim]
			(skB : skylineBase [dim])
			(coords : [dim]t)
		: (i64, t) =
			let (g,a,m) = get_grid_angle_measure_from_coords skB coords
			in (get_id_from_grid_angle skB g a, m)

	-- Pre-filtering ('slice and dice')
		local def pointwise_filter_for_Skyline [n] [dim] 'pL_t
			(skB : skylineBase [dim])
			(xs : [n][dim]t)
			(ys : [n]pL_t)
			(use_many_points : bool)
		: skylineInfo [dim] pL_t =
			let closestPoint =
				let dists = xs
					|> map (\x -> map2 (minus) x skB.start_of_grid_partition[0])
					|> map (\ds -> map (\d -> times d d) ds)
					|> map (sm_red (plus) (zero))
				let closestIdx = argmin (lt) (eq) (highest) dists
				in xs[closestIdx]
			-- Use the closest Euclidean point + the points with each smallest dim value
			let num_fpts = if use_many_points then (dim+1) else 1
			let filtering_points = (iota num_fpts)
				|> map (\d ->
					if d==0
					then closestPoint
					else
						let i = argmin
							(lt)
							(eq)
							(highest)
							(xs |> map (\x -> x[d-1]))
						in xs[i]
				)
			let filt_idxs = xs
				|> map (\x ->
					filtering_points
						|> map (\cmp_x ->
							sm_red (&&) (true) (map2 (geq) x cmp_x)
							&&
							sm_red (||) (false) (map2 (gt) x cmp_x)
						)
						|> sm_red (||) (false)
				)
				|> zip (iota n)
				|> filter (\(_,elimd) -> !elimd)
				|> map (\(i,_) -> i)
			let filt_xys = filt_idxs |> map (\i -> (xs[i],ys[i]))
			in {
				xys = filt_xys,
				isPartitionDominated = replicate skB.total_grid_no false
			}

		def skyline_slice_and_dice [n] [dim] 'pL_t
			(skB : skylineBase [dim])
			(xs : [n][dim]t)
			(ys : [n]pL_t)
			(use_many_points : bool)
		: skylineInfo [dim] pL_t =
			let grid_no = skB.total_grid_no
			let filt_skI = pointwise_filter_for_Skyline skB xs ys use_many_points
			in if (any (== 1) skB.grid_partitions_per_dim) then filt_skI else -- no partition elimination
			let filt_gids = filt_skI.xys |> map (\(x,_) -> get_grid_id_from_coords skB x)
			let n_filt = length filt_gids
			let count_per_grid_part = hist (+) 0 grid_no (filt_gids :> [n_filt]i64) (replicate n_filt 1)
			let isPartitionDomd = iota grid_no
				|> map (\this_gp ->
					let this_o = skB.start_of_grid_partition[this_gp]
					in iota grid_no
						|> any (\other_gp ->
							let other_o = skB.start_of_grid_partition[other_gp]
							in
								(count_per_grid_part[other_gp]>0)
								&&
								(sm_red (&&) (true) (map2 (gt) this_o other_o))
						)
				)
			let elimd_xys = filt_skI.xys
				|> zip (filt_gids)
				|> map (\(gid,xy) -> (isPartitionDomd[gid],xy))
				|> filter (\(isD,_) -> !isD)
				|> map (.1)
			in {
				xys = elimd_xys,
				isPartitionDominated = isPartitionDomd
			}

	-- Local filtering (using a skyline sample per partition)
		def skyline_local_filter [dim] 'pL_t
			(skB : skylineBase [dim])
			(skI : skylineInfo [dim] pL_t)
			(use_many_points : bool)
		: skylineInfo [dim] pL_t =
			let n = length skI.xys
			let part_no = skB.total_part_no
			let (pids,measures) = (skI.xys :> [n]([dim]t, pL_t))
				|> map (\(x,_) -> get_id_measure_from_coords skB x)
				|> unzip
			-- find minimum point per partition
			let partitionWise_closest = hist
				(\(i1,m1) (i2,m2) ->
					if (m1 `lt` m2) || ((m1 `eq` m2) && (i1<i2))
					then (i1,m1)
					else (i2,m2)
				)
				(n,highest)
				part_no
				pids
				(zip (iota n) (measures))
			|> map (\(i,_) -> if i<n then skI.xys[i].0 else (replicate dim highest))
			-- Use the closest Euclidean point + the points with each smallest dim value (per partition)
			let num_fpts = if use_many_points then (dim+1) else 1
			let filtering_points = (iota num_fpts)
				|> map (\d ->
					if d==0
					then partitionWise_closest
					else hist
						(\(i1,x1) (i2,x2) ->
							if (x1 `lt` x2) || ((x1 `eq` x2) && (i1<i2))
							then (i1,x1)
							else (i2,x2)
						)
						(n,highest)
						part_no
						pids
						(zip (iota n) (skI.xys |> map (\(x,_) -> x[d-1]) :> [n]t))
					-- TODO could use segmented gather here
					|> map (\(i,_) -> if i<n then skI.xys[i].0 else (replicate dim highest))
				) 
			let filt_xys = (skI.xys :> [n]([dim]t, pL_t))
				|> zip (pids :> [n]i64)
				|> map (\(pid,xy) ->
					let isElimd = filtering_points
						|> map (\f_per_dim -> f_per_dim[pid])
						|> map (\cmp_x ->
							sm_red (&&) (true) (map2 (geq) xy.0 cmp_x)
							&&
							sm_red (||) (false) (map2 (gt) xy.0 cmp_x)
						)
						|> sm_red (||) (false)
					let (gid,_) = get_grid_angle_from_id skB pid
					in (xy, skI.isPartitionDominated[gid] || isElimd)
				)
				|> filter (\(_, isElimd) -> !isElimd)
				|> map (\(xy,_) -> xy)
			in {xys = filt_xys, isPartitionDominated = skI.isPartitionDominated}

	-- Global Skyline
		def calc_global_Skyline [dim] 'pL_t
			(skB : skylineBase [dim])
			(skI : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let n = length skI.xys
			let extPar = idx_t.min n (idx_t.max 1 (skB.m_size/n))
			let num_iter = (extPar + n - 1)/extPar
			let new_xys =
				let new_is_ = loop ni = (iota n) for j in (iota num_iter) do
					let inf = j*extPar
					let sup = idx_t.min (inf+extPar) n
					let idx_range = (inf..<sup) :> [sup-inf]i64
					let this_xs = skI.xys[inf:sup] |> map (.0)
					let upd_idx = this_xs
						|> map (\this_x ->
							skI.xys
								|> map (\(cmp_x,_) ->
									(sm_red (&&) (true) (map2 (geq) this_x cmp_x))
									&&
									(sm_red (||) (false) (map2 (gt) this_x cmp_x))
								)
						)
						|> map (any (id))
						|> zip idx_range
						|> map (\(i,elimd) -> if elimd then (-1) else i)
					in ni with [inf:sup] = upd_idx
				let new_is = new_is_ |> filter (>=0)
				let nxys = new_is |> map (\i -> skI.xys[i])
				in nxys
			in {
				xys = new_xys,
				isPartitionDominated = skI.isPartitionDominated
			}

	-- Intermediate Filtering
		def calc_intermediate_skyline [dim] 'pL_t
			(skB : skylineBase [dim])
			(skI : skylineInfo [dim] pL_t)
			(max_subdiv : i64)
			(min_subdiv : i64)
			(subdiv_step : i64)
			(size_thresh : i64)
		: skylineInfo [dim] pL_t =
			-- start with thin angular slices, and gradually broaden them
			let skI_ = {xys = skI.xys, isPartitionDominated = [false]}
			let (intermediate_skI,_) =
				loop (sd_skI, subdiv) = (skI_, max_subdiv)
				while (subdiv>min_subdiv)&&(subdiv>0)&&(length sd_skI.xys > size_thresh) do
					-- if 1 subdivision, just do global skyline
					if subdiv==1
					then (calc_global_Skyline skB sd_skI, -1)
					-- split each dimension in isolation
					else let (perDim_skI,_)=
						loop (dsd_skI,curD) = (sd_skI, 0)
						while (curD<dim-1)&&(length dsd_skI.xys > size_thresh) do
							let sd_skB = mk_skylineBase_from_grid
								(skB.start_of_grid_partition[0])
								(skB.start_of_grid_partition[0])
								(replicate dim 1)
								((replicate (dim-1) 1) with [curD] = subdiv)
								skB.m_size
							let new_skI = skyline_local_filter sd_skB dsd_skI false
							in (new_skI, dim+1)
						in (perDim_skI, subdiv - subdiv_step)
			in {xys = intermediate_skI.xys, isPartitionDominated = skI.isPartitionDominated}

	-- Window Merging
		def merge_Skylines_5 [dim] 'pL_t
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
			(skI4 : skylineInfo [dim] pL_t)
			(skI5 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			{
				xys = skI1.xys ++ skI2.xys ++ skI3.xys ++ skI4.xys ++ skI5.xys,
				isPartitionDominated = map5
					(\d1 d2 d3 d4 d5 -> d1 || d2 || d3 || d4 || d5)
					skI1.isPartitionDominated
					skI2.isPartitionDominated
					skI3.isPartitionDominated
					skI4.isPartitionDominated
					skI5.isPartitionDominated
			}
		def merge_Skylines_4 [dim] 'pL_t
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
			(skI4 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			{
				xys = skI1.xys ++ skI2.xys ++ skI3.xys ++ skI4.xys,
				isPartitionDominated = map4
					(\d1 d2 d3 d4 -> d1 || d2 || d3 || d4)
					skI1.isPartitionDominated
					skI2.isPartitionDominated
					skI3.isPartitionDominated
					skI4.isPartitionDominated
			}
		def merge_Skylines_3 [dim] 'pL_t
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			{
				xys = skI1.xys ++ skI2.xys ++ skI3.xys,
				isPartitionDominated = map3
					(\d1 d2 d3 -> d1 || d2 || d3)
					skI1.isPartitionDominated
					skI2.isPartitionDominated
					skI3.isPartitionDominated
			}
		def merge_Skylines_2 [dim] 'pL_t
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			{
				xys = skI1.xys ++ skI2.xys,
				isPartitionDominated = map2
					(\d1 d2 -> d1 || d2)
					skI1.isPartitionDominated
					skI2.isPartitionDominated
			}

	-- Window Merging & Simultaneous Filtering
		def filter_against [dim] 'pL_t
			(skB : skylineBase [dim])
			(skI2 : skylineInfo [dim] pL_t)
			(skI1 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let n1 = length skI1.xys
			let n2 = length skI2.xys
			let extPar = idx_t.min n1 (idx_t.max 1 (skB.m_size/n2))
			let num_iter = (extPar + n1 - 1)/extPar
			let new_xys =
				let new_is_ = loop ni = (iota n1) for j in (iota num_iter) do
					let inf = j*extPar
					let sup = idx_t.min (inf+extPar) n1
					let idx_range = (inf..<sup) :> [sup-inf]i64
					let this_xs = skI1.xys[inf:sup] |> map (.0)
					let upd_idx = this_xs
						|> map (\this_x ->
							skI2.xys
								|> map (\(cmp_x,_) ->
									(sm_red (&&) (true) (map2 (geq) this_x cmp_x))
									&&
									(sm_red (||) (false) (map2 (gt) this_x cmp_x))
								)
						)
						|> map (any (id))
						|> zip idx_range
						|> map (\(i,elimd) -> if elimd then (-1) else i)
					in ni with [inf:sup] = upd_idx
				let new_is = new_is_ |> filter (>=0)
				let nxys = new_is |> map (\i -> skI1.xys[i])
				in nxys
			in {
				xys = new_xys,
				isPartitionDominated = skI1.isPartitionDominated
			}

		def mergeFilter_Skylines_5 [dim] 'pL_t
			(self_filter_first : bool)
			(skB : skylineBase [dim])
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
			(skI4 : skylineInfo [dim] pL_t)
			(skI5 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let skI1_ =
				if self_filter_first
				then calc_global_Skyline skB skI1
				else skI1
			let skI2_ = calc_global_Skyline skB skI2
				|> filter_against skB skI1_
			let skI3_ = calc_global_Skyline skB skI3
				|> filter_against skB skI1_
			let skI4_ = calc_global_Skyline skB skI4
				|> filter_against skB skI1_
			let skI5_ = calc_global_Skyline skB skI5
				|> filter_against skB skI1_
			let skI1_filtered = skI1_
				|> filter_against skB skI2_
				|> filter_against skB skI3_
				|> filter_against skB skI4_
				|> filter_against skB skI5_
			let skI2_filtered = skI2_
				|> filter_against skB skI3_
				|> filter_against skB skI4_
				|> filter_against skB skI5_
			let skI3_filtered = skI3_
				|> filter_against skB skI2_filtered
				|> filter_against skB skI4_
				|> filter_against skB skI5_
			let skI4_filtered = skI4_
				|> filter_against skB skI2_filtered
				|> filter_against skB skI3_filtered
				|> filter_against skB skI5_
			let skI5_filtered = skI5_
				|> filter_against skB skI2_filtered
				|> filter_against skB skI3_filtered
				|> filter_against skB skI4_filtered
			in merge_Skylines_5 skI1_filtered skI2_filtered skI3_filtered skI4_filtered skI5_filtered
		def mergeFilter_Skylines_4 [dim] 'pL_t
			(self_filter_first : bool)
			(skB : skylineBase [dim])
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
			(skI4 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let skI1_ =
				if self_filter_first
				then calc_global_Skyline skB skI1
				else skI1
			let skI2_ = calc_global_Skyline skB skI2
				|> filter_against skB skI1_
			let skI3_ = calc_global_Skyline skB skI3
				|> filter_against skB skI1_
			let skI4_ = calc_global_Skyline skB skI4
				|> filter_against skB skI1_
			let skI1_filtered = skI1_
				|> filter_against skB skI2_
				|> filter_against skB skI3_
				|> filter_against skB skI4_
			let skI2_filtered = skI2_
				|> filter_against skB skI3_
				|> filter_against skB skI4_
			let skI3_filtered = skI3_
				|> filter_against skB skI2_filtered
				|> filter_against skB skI4_
			let skI4_filtered = skI4_
				|> filter_against skB skI2_filtered
				|> filter_against skB skI3_filtered
			in merge_Skylines_4 skI1_filtered skI2_filtered skI3_filtered skI4_filtered
		def mergeFilter_Skylines_3 [dim] 'pL_t
			(self_filter_first : bool)
			(skB : skylineBase [dim])
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
			(skI3 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let skI1_ =
				if self_filter_first
				then calc_global_Skyline skB skI1
				else skI1
			let skI2_ = calc_global_Skyline skB skI2
				|> filter_against skB skI1_
			let skI3_ = calc_global_Skyline skB skI3
				|> filter_against skB skI1_
			let skI1_filtered = skI1_
				|> filter_against skB skI2_
				|> filter_against skB skI3_
			let skI2_filtered = skI2_
				|> filter_against skB skI3_
			let skI3_filtered = skI3_
				|> filter_against skB skI2_filtered
			in merge_Skylines_3 skI1_filtered skI2_filtered skI3_filtered
		def mergeFilter_Skylines_2 [dim] 'pL_t
			(self_filter_first : bool)
			(skB : skylineBase [dim])
			(skI1 : skylineInfo [dim] pL_t)
			(skI2 : skylineInfo [dim] pL_t)
		: skylineInfo [dim] pL_t =
			let skI1_ =
				if self_filter_first
				then calc_global_Skyline skB skI1
				else skI1
			let skI2_filtered = calc_global_Skyline skB skI2
				|> filter_against skB skI1_
			let skI1_filtered = skI1_
				|> filter_against skB skI2_filtered
			in merge_Skylines_2 skI1_filtered skI2_filtered

	-- Unwrap Data
		def crack_skyline [dim] 'pL_t
			(skI : skylineInfo [dim] pL_t)
		: skylineData [dim] pL_t =
			let (xs,ys) = skI.xys |> unzip
			in {len = length xs, dat = xs, pL = ys}
}

-- Entry Points
	-- Float
		module skyline_float = skyline_real (f32)

		type~ skylineBase_float [dim] = skyline_float.skylineBase [dim]
		type~ skylineInfo_float [dim] = skyline_float.skylineInfo [dim] i64
		type~ skylineData_float [dim] = skyline_float.skylineData [dim] i64

		entry make_skylineBase_float [dim]
			(min_per_dim : [dim]f32)
			(max_per_dim : [dim]f32)
			(grid_partitions_per_dim : [dim]i64)
			(angle_partitions_per_dim : [dim-1]i64)
			(m_size : i64)
		: skylineBase_float [dim] =
			skyline_float.mk_skylineBase_from_grid
				min_per_dim max_per_dim grid_partitions_per_dim angle_partitions_per_dim m_size

		entry skyline_slice_and_dice_float [n] [dim]
			(skB : skylineBase_float [dim])
			(xs : [n][dim]f32)
			(offs : i64)
			(use_many_pts : bool)
		: skylineInfo_float [dim] =
			skyline_float.skyline_slice_and_dice skB xs (iota n |> map (\i -> i+offs)) use_many_pts

		entry skyline_local_filter_float [dim]
			(skB : skylineBase_float [dim])
			(skI : skylineInfo_float [dim])
			(use_many_pts : bool)
		: skylineInfo_float [dim] =
			skyline_float.skyline_local_filter skB skI use_many_pts

		entry skyline_intermediate_filter_float [dim]
			(skB : skylineBase_float [dim])
			(skI : skylineInfo_float [dim])
			(max_subdiv : i64)
			(min_subdiv : i64)
			(subdiv_step : i64)
			(size_thresh : i64)
		: skylineInfo_float [dim] =
			skyline_float.calc_intermediate_skyline skB skI max_subdiv min_subdiv subdiv_step size_thresh

		entry skyline_merge_5_float [dim]
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
			(skI4 : skylineInfo_float [dim])
			(skI5 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.merge_Skylines_5 skI1 skI2 skI3 skI4 skI5
		entry skyline_merge_4_float [dim]
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
			(skI4 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.merge_Skylines_4 skI1 skI2 skI3 skI4
		entry skyline_merge_3_float [dim]
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.merge_Skylines_3 skI1 skI2 skI3
		entry skyline_merge_2_float [dim]
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.merge_Skylines_2 skI1 skI2

		entry skyline_mergeFilter_5_float [dim]
			(self_filter_first : bool)
			(skB : skylineBase_float [dim])
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
			(skI4 : skylineInfo_float [dim])
			(skI5 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.mergeFilter_Skylines_5 self_filter_first skB skI1 skI2 skI3 skI4 skI5
		entry skyline_mergeFilter_4_float [dim]
			(self_filter_first : bool)
			(skB : skylineBase_float [dim])
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
			(skI4 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.mergeFilter_Skylines_4 self_filter_first skB skI1 skI2 skI3 skI4
		entry skyline_mergeFilter_3_float [dim]
			(self_filter_first : bool)
			(skB : skylineBase_float [dim])
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
			(skI3 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.mergeFilter_Skylines_3 self_filter_first skB skI1 skI2 skI3
		entry skyline_mergeFilter_2_float [dim]
			(self_filter_first : bool)
			(skB : skylineBase_float [dim])
			(skI1 : skylineInfo_float [dim])
			(skI2 : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.mergeFilter_Skylines_2 self_filter_first skB skI1 skI2

		entry calc_Global_Skyline_float [dim]
			(skB : skylineBase_float [dim])
			(skI : skylineInfo_float [dim])
		: skylineInfo_float [dim] =
			skyline_float.calc_global_Skyline skB skI

		entry crack_skylineInfo_float [dim] (skI : skylineInfo_float [dim]) : skylineData_float [dim] =
			skyline_float.crack_skyline skI

	-- TODO double, half entry points