import "../../../ftbasics"

-- Materialization stage of PHJ (GFTR).
--
-- ==
-- entry: do_materialization
-- compiled input @data/dat.in

entry do_materialization [b1] [b2]
	(vs : [][]u8)
	(ix : []i64)
	(iy : []i64)
	(pL1: [][b1]u8)
	(pL2: [][b2]u8)
=
	let g_pL1 = ix |> gather (replicate b1 0u8) pL1
	let g_pL2 = iy |> gather (replicate b2 0u8) pL2
	in (vs, g_pL1, g_pL2)