import "../../../ftbasics"

-- Materialization phase.
--
-- ==
-- entry: rhj2
-- input @data/data1_1.in
-- auto output
-- input @data/data1_2.in
-- auto output
-- input @data/data2_1.in
-- auto output
-- input @data/data2_2.in
-- auto output
-- input @data/data3_1.in
-- auto output
-- input @data/data3_2.in
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