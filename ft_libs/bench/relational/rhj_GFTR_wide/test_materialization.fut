import "../../../ftbasics"

-- Materialization phase.
--
-- ==
-- entry: rhj
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
=
	let gpL1 = gather (replicate b 0) pL1 ix
	let gpL2 = gather (replicate b 0) pL2 iy
	in (gpL1, gpL2)