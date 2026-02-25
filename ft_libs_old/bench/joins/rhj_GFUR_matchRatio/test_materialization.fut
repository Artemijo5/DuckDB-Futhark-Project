import "../../../ftbasics"

-- Materialization phase.
--
-- ==
-- entry: rhj2
-- input @data/i32_100.in
-- input @data/i32_50.in
-- input @data/i32_25.in
-- input @data/i32_125.in

-- -- input @data/i32_625.in
-- auto output

entry rhj2 [b]
	(_ : [][]u8) (ix : []i64) (iy: []i64)
	(pL1 : [][b]u8) (pL2 : [][b]u8)
	(is1 : []i64) (is2 : []i64)
=
	let gis1 = gather (-1) is1 ix
	let gis2 = gather (-1) is2 iy
	let gpL1 = gather (replicate b 0) pL1 gis1
	let gpL2 = gather (replicate b 0) pL2 gis2
	in (gpL1, gpL2)