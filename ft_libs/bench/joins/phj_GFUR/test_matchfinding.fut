import "../../../ftbasics"
import "../../../ft_partition"
import "../../../joins/ftPHJ"

-- Join Phase for PHJ (GFUR)
-- ==
-- entry: do_matchfinding
-- input @data/dat.in
-- auto output

entry do_matchfinding [n1] [n2] [b] [pL_b]
	(xs1 : [n1][b]u8)
	(xs2 : [n2][b]u8)
	(is1 : [n1]i64)
	(is2 : [n2]i64)
	(pL1 : [n1][pL_b]u8)
	(pL2 : [n2][pL_b]u8)
	(radix_bits: [1]i32)
	(max_depth : [1]i32)
	(bounds : []i64)
	(depths : []i32)
	(first_idx: []i64)
	(last_idx : []i64)
=
	let radix_size = radix_bits[0]
	let x1_info: partitionInfo = {maxDepth = max_depth[0], bounds = bounds, depths = depths}
	let x1_tbl : radix_hashTable [i64.i32 radix_size] = {
		first_info_idx = first_idx |> sized (2**(i64.i32 radix_size)),
		last_info_idx  = last_idx  |> sized (2**(i64.i32 radix_size))
	}
	let jPairs = innerPHJ radix_size xs1 xs2 x1_info x1_tbl
	in (jPairs.vs, jPairs.ix, jPairs.iy, is1, is2, pL1, pL2)