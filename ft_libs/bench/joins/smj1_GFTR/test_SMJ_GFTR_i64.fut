import "../../../ftbasics"
import "../../../ftsort"
import "../../../joins/ftSMJ"

-- Do entire SMJ pipeline.
--
-- ==
-- entry: do_SMJ_i64
-- compiled input @data/dat_i64.in

entry do_SMJ_i64 [n1] [n2] [b1] [b2]
	(xs1 : [n1]i64)
	(xs2 : [n2]i64)
	(pL1 : [n1][b1]u8)
	(pL2 : [n2][b2]u8)
=
	-- Transformation Phase
	let sxs1 = mergeSort_GFTR (<=) xs1 pL1
	let sxs2 = mergeSort_GFTR (<=) xs2 pL2
	let xs1' = sxs1.ks
	let pL1' = sxs1.pL
	-- Matching Phase
	let js = do_InnerSMJ1 (==) (>=) (>) (<) xs2 xs1'
	-- Materialization Phase
	let g_pL1 = js.iy |> gather (replicate b1 0u8) pL1'
	let g_pL2 = js.ix |> gather (replicate b2 0u8) pL2
	in (js.vs, g_pL1, g_pL2)