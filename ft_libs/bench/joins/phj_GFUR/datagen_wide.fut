import "../../ftbasics"

-- Generate i32 data for wide join.
-- R : iota n, shuffled
-- S : duplicated iota n, shuffled
--
-- ==
-- entry: do_datagen
-- compiled input @data/dat_i32.in
-- auto output

entry do_datagen [n1] [n2]
	(numBytes : i64)
	(shuffle1 : [n1]i64)
	(shuffle2 : [n2]i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
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
	let xs2 = iota n2
		|> map (\i -> i%n1)
		|> bucket_sort 2 (i64.highest) shuffle2
		|> (.1)
		|> sized n2
		|> zip (replicate n2 (numBytes |> iota |> reverse))
		|> map (\(is, v) -> is
			|> map (\i ->
				u8.i64 ((i64.>>>) v (i * (i64.i32 u8.num_bits)))
			)
		) :> [n2][numBytes]u8
	in (xs1, xs2, pL1, pL2)