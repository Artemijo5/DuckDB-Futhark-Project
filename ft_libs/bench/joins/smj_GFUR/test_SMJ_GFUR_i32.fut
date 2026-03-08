import "../../../ftbasics"
import "../../../ftsort"
import "../../../joins/ftSMJ"

-- Do entire SMJ pipeline.
--
-- ==
-- entry: do_SMJ_i32
-- compiled input @data/dat_i32.in

entry do_SMJ_i32 [n1] [n2] [b1] [b2]
	(xs1 : [n1]i32)
	(xs2 : [n2]i32)
	(pL1 : [n1][b1]u8)
	(pL2 : [n2][b2]u8)
=
	-- Transformation Phase
	let sxs1 = mergeSort_GFUR (<=) xs1
	let sxs2 = mergeSort_GFUR (<=) xs2
	let xs1' = sxs1.ks
	let is1 = sxs1.is
	let xs2' = sxs2.ks
	let is2 = sxs2.is
	-- Matching Phase
	let js = do_InnerSMJ (==) (>=) (>) (<) xs1' xs2'
	-- Materialization Phase
	let gis1 = js.ix |> gather 0 is1
	let gis2 = js.iy |> gather 0 is2
	let g_pL1 = gis1 |> gather (replicate b1 0u8) pL1
	let g_pL2 = gis2 |> gather (replicate b2 0u8) pL2
	in (js.vs, g_pL1, g_pL2)