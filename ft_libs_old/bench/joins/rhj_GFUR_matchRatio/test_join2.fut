import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- Radix-hash join.
-- Narrow joins, 100% match rate.
--
-- ==
-- entry: rhj
-- input @data/i32_100.in
-- auto output
-- input @data/i32_50.in
-- auto output
-- input @data/i32_25.in
-- auto output
-- input @data/i32_125.in
-- auto output

-- -- input @data/i32_625.in
-- auto output

entry rhj [n1] [n2] [b]
	(ks1 : [n1][b]u8) (pL1 : [n1][]u8)
	(ks2 : [n2][b]u8) (pL2 : [n2][]u8)
	(maxDepth2 : i32) (bounds2 : []i64) (depths2 : []i32)
	(first_idx2 : []i64) (last_idx2 : []i64)
	(is1 : []i64) (is2 : []i64)
=
	let radix_size = first_idx2 |> length |> f64.i64 |> f64.log2 |> i32.f64
	let info2 : partitionInfo = {maxDepth = maxDepth2, bounds = bounds2, depths = depths2}
	let tab2 : radix_hashTable [i64.i32 radix_size] = {
		first_info_idx = first_idx2 :> [2**(i64.i32 radix_size)]i64,
		last_info_idx = last_idx2 :> [2**(i64.i32 radix_size)]i64
	}
	let res = radix_hash_join radix_size ks1 ks2 info2 tab2
	in (res.vs, res.ix, res.iy, pL1, pL2, is1, is2)