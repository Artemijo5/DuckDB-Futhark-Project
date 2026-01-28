import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Expansion, 4-byte integer keys.
--
-- ==
-- entry: smj2_i32
-- input @data/i32_0.in
-- auto output
-- input @data/i32_1.in
-- auto output
-- input @data/i32_2.in
-- auto output

entry smj2_i32 [n]
	(vs : [n]i32) (ix : [n]i64) (iy : [n]i64) (cm : [n]i64)
=
	let jTups : joinTup [n] i32 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let res = joinTups_to_joinPairs_InnerJoin jTups (-1)
	in (res.vs, res.ix, res.iy)

-- Expansion, 8-byte integer keys.
--
-- ==
-- entry: smj2_i64
-- input @data/i64_0.in
-- auto output
-- input @data/i64_1.in
-- auto output
-- input @data/i64_2.in
-- auto output

entry smj2_i64 [n]
	(vs : [n]i64) (ix : [n]i64) (iy : [n]i64) (cm : [n]i64)
=
	let jTups : joinTup [n] i64 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let res = joinTups_to_joinPairs_InnerJoin jTups (-1)
	in (res.vs, res.ix, res.iy)