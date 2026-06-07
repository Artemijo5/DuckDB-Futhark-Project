import "../../../ftbasics"
import "../../../ft_partition"

-- Transformation Phase for PHJ (Narrow).
-- ==
-- entry: do_transformation
-- input @data/dat.in
-- auto output

entry do_transformation [n1] [n2] [b]
	(radix_bits : [1]i32)
	(xs1 : [n1][b]u8)
	(xs2 : [n2][b]u8)
	(pL1 : [][]u8)
	(pL2 : [][]u8)
=
	let radix_size = radix_bits[0]
	let size_thresh = 2 * n1 / (2**(i64.i32 radix_size))
	let max_depth = i32.min 4 ((i32.i64 b) * u8.num_bits / radix_size)
	let (xs1',is1) = partition_and_deepen 2 radix_size size_thresh max_depth xs1
		((replicate n1 []) :> [n1][0]i64)
	let x1_info= calc_partInfo radix_size 0 size_thresh max_depth xs1'
	let (xs2',is2) = partition_preconfigured 2 radix_size xs1' x1_info xs2
		((replicate n2 []) :> [n2][0]i64)
	let x2_info= calc_partInfo_preconfigured radix_size 0 xs1' x1_info xs2'
	in (
		xs1', xs2', iota 0, iota 0, pL1, pL2, radix_bits,
		x1_info.maxDepth, x1_info.bounds, x1_info.depths,
		x2_info.maxDepth, x2_info.bounds, x2_info.depths
	)
