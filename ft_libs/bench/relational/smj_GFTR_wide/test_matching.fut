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
-- input @data/i32_1_1.in
-- auto output
-- input @data/i32_1_2.in
-- auto output
-- input @data/i32_2_1.in
-- auto output
-- input @data/i32_2_2.in
-- auto output
-- input @data/i32_3_1.in
-- auto output
-- input @data/i32_3_2.in
-- auto output

entry smj1_i32
	(ks1 : []i32) (pL1 : [][]u8)
	(ks2 : []i32) (pL2 : [][]u8)
=
	let res = mergeJoin i32.num_bits ks1 ks2 0 0 (536870912) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, pL1, pL2)

-- Match-finding, 8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = n)
-- Narrow join (no payloads).
--

entry smj1_i64
	(ks1 : []i64) (pL1 : [][]u8)
	(ks2 : []i64) (pL2 : [][]u8)
=
	let res = mergeJoin i32.num_bits ks1 ks2 0 0 (1073741824) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, pL1, pL2)