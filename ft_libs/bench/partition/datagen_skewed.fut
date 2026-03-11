import "../../ftbasics"
import "../../ftSynthetic"

-- Generate data for join with skewed data.
-- Using the zipf distribution.
--
-- ==
-- entry: do_datagen
-- input @data/dat.in
-- auto output

entry do_datagen [n1] [n2]
	(shuffle1 : [n1]i64)
	(u_rand2 : [n2]f64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
	(numBytes : [1]i64)
	(radix_bits : [1]i32)
	(skew: [1]f64)
=
	let xs1 = iota n1
		|> bucket_sort 2 (i64.highest) shuffle1
		|> (.1)
		|> sized n1
		|> zip (replicate n1 (numBytes[0] |> iota |> reverse))
		|> map (\(is, v) -> is
			|> map (\i ->
				u8.i64 ((i64.>>>) v (i * (i64.i32 u8.num_bits)))
			)
		) :> [n1][numBytes[0]]u8
	let xs2 = u_rand2
		|> zipf_skewed skew[0] n1
		|> map (\i -> i%n1)
		|> sized n2
		|> zip (replicate n2 (numBytes[0] |> iota |> reverse))
		|> map (\(is, v) -> is
			|> map (\i ->
				u8.i64 ((i64.>>>) v (i * (i64.i32 u8.num_bits)))
			)
		) :> [n2][numBytes[0]]u8
	in (radix_bits, xs1, xs2, pL1, pL2)