import "../../ftbasics"
import "../../ftSynthetic"

-- Generate i32 data for join with skewed data.
-- Using the zipf distribution.
--
-- ==
-- entry: do_datagen
-- input @data/dat_i32.in
-- auto output

entry do_datagen [n1] [n2]
	(numBytes : i64)
	(shuffle1 : [n1]i64)
	(u_rand2 : [n2]f64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
	(skew: [1]f64)
: ([n1][numBytes]u8, [n2][numBytes]u8, [n1][]u8, [n2][]u8) =
	let xs1 = iota n1
		|> bucket_sort 2 (i64.highest) shuffle1
		|> (.1)
		|> sized n1
		|> zip (replicate n1 (numBytes |> iota |> reverse))
		|> map (\(is, v) -> is
			|> map (\i ->
				u8.i64 ((i64.>>>) v (i * (i64.i32 u8.num_bits)))
			)
		) :> [n1][numBytes]u8
	let xs2 = u_rand2
		|> zipf_skewed skew[0] n1
		|> map (\i -> i%n1)
		|> sized n2
		|> zip (replicate n2 (numBytes |> iota |> reverse))
		|> map (\(is, v) -> is
			|> map (\i ->
				u8.i64 ((i64.>>>) v (i * (i64.i32 u8.num_bits)))
			)
		) :> [n2][numBytes]u8
	in (xs1, xs2, pL1, pL2)