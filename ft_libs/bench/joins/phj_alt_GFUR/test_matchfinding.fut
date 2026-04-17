import "../../../ftbasics"
import "../../../ft_partition"
import "../../../joins/ftPHJ_alt"

-- Join Phase for PHJ (GFUR)
-- ==
-- entry: do_matchfinding
-- input @data/dat.in
-- auto output

entry do_matchfinding [n1] [n2] [b] [pL_b]
	(xs1 : [n1][b]u8)
	(xs2 : [n2][b]u8)
	(is1 : []i64)
	(is2 : []i64)
	(pL1 : [][pL_b]u8)
	(pL2 : [][pL_b]u8)
	(radix_bits: []i32)
	(max_depth1 : []i32)
	(bounds1 : []i64)
	(depths1 : []i32)
	(max_depth2 : []i32)
	(bounds2 : []i64)
	(depths2 : []i32)
=
	let radix_size = radix_bits[0]
	let x1_info: partitionInfo = {maxDepth = max_depth1[0], bounds = bounds1, depths = depths1}
	let x2_info: partitionInfo = {maxDepth = max_depth2[0], bounds = bounds2, depths = depths2}
	let jPairs = innerPHJ radix_size xs1 xs2 x1_info x2_info
	in (jPairs.vs, jPairs.ix, jPairs.iy, is1, is2, pL1, pL2)