import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Match-finding stage of SMJ (GFTR).
--
-- ==
-- entry: do_matchfinding_i32
-- input @data/dat_i32.in
-- auto output

entry do_matchfinding_i32 [n1] [n2]
	(xs1 : [n1]i32)
	(xs2 : [n2]i32)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let jTup = smj_matchFinding (==) (>=) (>) (<) xs1 xs2
	in (jTup.vs, jTup.ix, jTup.iy, jTup.cm, pL1, pL2)