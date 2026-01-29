import "../../../ftbasics"

-- Materialization (gather payloads).
--
-- ==
-- entry: smj3_i32
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

entry smj3_i32 [n]
	(_ : [n]i32) (ix : [n]i64) (iy : [n]i64)
	(is1 : []i64) (is2 : []i64)
	(pL1 : [][]u8) (pL2 : [][]u8)
=
	let gis1 = gather is1 ix
	let gpL1 = gather pL1 gis1
	let gis2 = gather is2 iy
	let gpL2 = gather pL2 gis2
	in (gpL1, gpL2)