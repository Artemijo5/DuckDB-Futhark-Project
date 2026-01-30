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
-- input @data/i32_625.in
-- auto output

entry smj1_i32 [n1] [n2]
	(ks1 : [n1]i32) (pL1 : [n1][]u8)
	(ks2 : [n2]i32) (pL2 : [n2][]u8)
=
	let xs1 : sortStruct_int [n1] [0] = {k = ks1, pL = pL1}
	let xs2 : sortStruct_int [n2] [0] = {k = ks2, pL = pL2}
	let sks1 = (mergeSortRelation xs1 (<=)).k
	let sks2 = (mergeSortRelation xs2 (<=)).k
	let res = mergeJoin i32.num_bits sks1 sks2 0 0 (536870912) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm, sks1.pL, sks2.pL)