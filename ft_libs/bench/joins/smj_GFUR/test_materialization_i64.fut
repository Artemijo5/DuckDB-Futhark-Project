import "../../../ftbasics"

-- Materialization stage of SMJ (GFUR).
--
-- ==
-- entry: do_materialization_i64
-- compiled input @data/dat_i64.in

entry do_materialization_i64 [b1] [b2]
	(vs : []i64)
	(ix : []i64)
	(iy : []i64)
	(is1: []i64)
	(is2: []i64)
	(pL1: [][b1]u8)
	(pL2: [][b2]u8)
=
	let gis1 = ix |> gather 0 is1
	let gis2 = iy |> gather 0 is2
	let g_pL1 = gis1 |> gather (replicate b1 0u8) pL1
	let g_pL2 = gis2 |> gather (replicate b2 0u8) pL2
	in (vs, g_pL1, g_pL2)