import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Match-finding stage of SMJ (GFUR).
--
-- ==
-- entry: do_matchfinding_i64
-- compiled input @data/dat_i64.in
-- auto output

entry do_matchfinding_i64 [n1] [n2]
	(xs1 : [n1]i64)
	(xs2 : [n2]i64)
	(is1: []i64)
	(is2: []i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let jTup = smj_matchFinding (==) (>=) (<=) (>) (<) ((n2 + 8191) / 8192) xs1 xs2
	in (jTup.vs, jTup.ix, jTup.iy, jTup.cm, is1, is2, pL1, pL2)