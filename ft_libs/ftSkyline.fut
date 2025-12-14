import "ftbasics"

-- TODO change local logic so that it only filters using skyline sample pts
-- this way it doesn't need to sort, and merging doesn't need to respect any order
-- also ig save mem_size in skylineBase (?)

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

module skyline_real (F:real) = {
	-- Types and Operators
		type t = F.t

		type skylineBase [dim] = skylineBase_t [dim] t
		type skylineInfo [dim] = skylineInfo_t [dim] t

		local def gt = (F.>)
		local def geq = (F.>=)
		local def lt = (F.<)
		local def leq = (F.>=)
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

		def dummy_skylineInfo [dim] 'pL_t
		(skB : skylineBase [dim])
		(_ : ([dim]t, pL_t)) -- dummy elem
		: skylineInfo [dim] t pL_t = {
			xys = [],
			isPartitionDominated = replicate skB.total_grid_no false,
		}

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
				skOp min_per_dim max_per_dim partitionSizePerDim grid_partitions_per_dim angle_partitions_per_dim

	local def get_id_from_grid_angle [dim]
	(skB : skylineBase [dim])
	(grid_id : i64)
	(angle_id : i64)
	: idx_t.t =
		grid_id*(skB.total_angle_no) + angle_id

	def get_grid_angle_from_id [dim]
		(skB : skylineBase [dim])
		(part_id : i64)
	: (idx_t.t, idx_t.t) =
		let grid_id = part_id / skB.total_angle_no
		let angle_id = part_id % skB.total_angle_no
		in (grid_id, angle_id)

	def cartesian_to_spherical [dim]
		(coords : [dim]t)
	: (t, [dim-1]t) =
		let r = coords |> map (\x -> times x x) |> sm_red (plus) (zero) |> sqrt
		let phis : [dim-1]t =
			loop p = (replicate (dim-1) (zero))
			for j in (iota (dim-1)) do
				let this_x = coords[j]
				let this_y = coords[(j+1):dim] |> map (\x -> times x x) |> sm_red (plus) (zero) |> sqrt
				let new_phi = atan2 this_y this_x
			in p with [j] = new_phi
		in (r, phis)

}