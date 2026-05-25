import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Expansion stage of SMJ (GFTR).
--
-- ==
-- entry: do_expansion_i64
-- compiled input @data/dat_i64.in
-- auto output

entry do_expansion_i64 [n1] [n2]
	(vs : [n1]i64)
	(ix : [n1]i64)
	(iy : [n1]i64)
	(cm : [n1]i64)
	(pL1: [n1][]u8)
	(pL2: [n2][]u8)
=
	let matches : joinTup [n2] i64 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let jPrs = matches |> smj_expand
	in (jPrs.vs, jPrs.iy, jPrs.ix, pL1, pL2)