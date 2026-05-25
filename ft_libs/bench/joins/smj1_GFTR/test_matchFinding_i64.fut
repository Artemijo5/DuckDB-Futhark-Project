import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Match-finding stage of SMJ (GFTR).
--
-- ==
-- entry: do_matchfinding_i64
-- compiled input @data/dat_i64.in
-- auto output

entry do_matchfinding_i64 [n1] [n2]
	(sxs1 : [n1]i64)
	(xs2 : [n2]i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let jTup = smj1_matchFinding (==) (>=) (>) (<) xs2 sxs1
	in (jTup.vs, jTup.ix, jTup.iy, jTup.cm, pL1, pL2)