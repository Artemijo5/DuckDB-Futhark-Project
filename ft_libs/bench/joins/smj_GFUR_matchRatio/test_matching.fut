import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Match-finding, 4-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = n)
-- Narrow join (no payloads).
--
-- ==
-- entry: smj1_i32
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

entry smj1_i32
	(ks1 : []i32) (is1 : []i64) (pL1 : [][]u8)
	(ks2 : []i32) (is2 : []i64) (pL2 : [][]u8)
=
	let res = mergeJoin i32.num_bits ks1 ks2 0 0 (i64.highest) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, is1, is2, pL1, pL2)