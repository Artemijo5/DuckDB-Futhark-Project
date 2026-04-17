import "../../../ftbasics"
import "../../../ft_partition"

-- Transformation Phase for PHJ (GFUR)
-- ==
-- entry: do_transformation
-- input @data/dat.in
-- auto output

entry do_transformation [n1] [n2] [b] [pL_b]
	(radix_bits : [1]i32)
	(xs1 : [n1][b]u8)
	(xs2 : [n2][b]u8)
	(pL1 : [n1][pL_b]u8)
	(pL2 : [n2][pL_b]u8)
=
	let radix_size = radix_bits[0]
	let size_thresh = 2 * n2 / (2**(i64.i32 radix_size))
	let max_depth = i32.min 4 ((i32.i64 b) * u8.num_bits / radix_size)
	let (xs2',is2) = partition_and_deepen 2 radix_size size_thresh max_depth xs1 (iota n1)
	let x2_info= calc_partInfo radix_size 0 size_thresh max_depth xs2'
	let (xs1',is1) = partition_preconfigured 2 radix_size xs2' x2_info xs1 (iota n1)
	let x1_info= calc_partInfo_preconfigured radix_size 0 xs2' x2_info xs1'
	in (
		xs1', xs2', is1, is2, pL1, pL2, radix_bits,
		([x1_info.maxDepth] :> [1]i32), x1_info.bounds, x1_info.depths,
		([x2_info.maxDepth] :> [1]i32), x2_info.bounds, x2_info.depths
	)