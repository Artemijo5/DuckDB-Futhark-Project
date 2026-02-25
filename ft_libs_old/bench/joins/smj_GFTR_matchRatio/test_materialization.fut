import "../../../ftbasics"

-- Materialization (gather payloads).
--
-- ==
-- entry: smj3_i32
-- input @data/i32_100.in
-- input @data/i32_50.in
-- input @data/i32_25.in
-- input @data/i32_125.in

-- -- input @data/i32_625.in

entry smj3_i32 [n] [b]
	(_ : [n]i32) (ix : [n]i64) (iy : [n]i64)
	(pL1 : [][b]u8) (pL2 : [][b]u8)
=
	let gpL1 = gather (replicate b 0) pL1 ix
	let gpL2 = gather (replicate b 0) pL2 iy
	in (gpL1, gpL2)