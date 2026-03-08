import "../../../ftbasics"
import "../../../ftSynthetic"

-- Generate i32 data for join with skewed data.
-- Using the zipf distribution.
--
-- ==
-- entry: do_datagen_i32
-- input @data/dat_i32.in
-- auto output

entry do_datagen_i32 [n1] [n2]
	(shuffle1 : [n1]i64)
	(u_rand2 : [n2]f64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
	(skew: [1]f64)
=
	let xs1 = iota n1
		|> map (i32.i64)
		|> bucket_sort 2 (i64.highest) shuffle1
		|> (.1)
	let xs2 = u_rand2
		|> zipf_skewed skew[0] n1
		|> map (\i -> i%n1)
		|> map (i32.i64)
	in (xs1, xs2, pL1, pL2)