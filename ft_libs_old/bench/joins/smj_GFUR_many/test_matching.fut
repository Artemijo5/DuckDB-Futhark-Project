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
-- input @data/smjdat.in
-- auto output

entry smj1_i32
	(ks1 : []i32) (is1 : []i64) (pL1 : [][]u8)
	(ks2 : []i32) (is2 : []i64) (pL2 : [][]u8)
=
	let res = mergeJoin i32.num_bits ks1 ks2 0 0 i64.highest (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, is1, is2, pL1, pL2)

-- Match-finding, 8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = n)
-- Narrow join (no payloads).
--

entry smj1_i64
	(ks1 : []i64) (is1 : []i64) (pL1 : [][]u8)
	(ks2 : []i64) (is2 : []i64) (pL2 : [][]u8)
=
	let res = mergeJoin i32.num_bits ks1 ks2 0 0 i64.highest (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, is1, is2, pL1, pL2)