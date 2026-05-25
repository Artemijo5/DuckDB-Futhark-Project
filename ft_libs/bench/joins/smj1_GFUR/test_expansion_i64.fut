import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Expansion stage of SMJ (GFUR).
--
-- ==
-- entry: do_expansion_i64
-- compiled input @data/dat_i64.in
-- auto output

entry do_expansion_i64 [n1] [n2]
	(vs : [n2]i64)
	(ix : [n2]i64)
	(iy : [n2]i64)
	(cm : [n2]i64)
	(is1: []i64)
	(pL1: [n1][]u8)
	(pL2: [n2][]u8)
=
	let matches : joinTup [n2] i64 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let jPrs = matches |> smj_expand
	in (jPrs.vs, jPrs.iy, jPrs.ix, is1, pL1, pL2)