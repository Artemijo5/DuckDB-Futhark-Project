import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Expansion, 4-byte integer keys.
--
-- ==
-- entry: smj2_i32
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

entry smj2_i32 [n]
	(vs : [n]i32) (ix : [n]i64) (iy : [n]i64) (cm : [n]i64)
	(is1 : []i64) (is2 : []i64) (pL1 : [][]u8) (pL2 : [][]u8)
=
	let jTups : joinTup [n] i32 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let res = joinTups_to_joinPairs_InnerJoin jTups (-1)
	in (res.vs, res.ix, res.iy, is1, is2, pL1, pL2)