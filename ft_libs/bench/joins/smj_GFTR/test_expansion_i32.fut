import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Expansion stage of SMJ (GFTR).
--
-- ==
-- entry: do_expansion_i32
-- compiled input @data/dat_i32.in
-- auto output

entry do_expansion_i32 [n1] [n2]
	(vs : [n1]i32)
	(ix : [n1]i64)
	(iy : [n1]i64)
	(cm : [n1]i64)
	(pL1: [n1][]u8)
	(pL2: [n2][]u8)
=
	let matches : joinTup [n1] i32 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let jPrs = matches |> smj_expand
	in (jPrs.vs, jPrs.ix, jPrs.iy, pL1, pL2)