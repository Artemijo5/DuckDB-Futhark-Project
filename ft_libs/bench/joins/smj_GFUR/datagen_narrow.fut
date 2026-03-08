import "../../../ftbasics"

-- Generate i32 data for narrow join.
-- R : iota n, shuffled
-- S : duplicated iota n, shuffled
--
-- ==
-- entry: do_datagen_i32
-- input @data/dat_i32.in
-- auto output

entry do_datagen_i32 [n1] [n2]
	(shuffle1 : [n1]i64)
	(shuffle2 : [n2]i64)
=
	let xs1 = iota n1
		|> map (i32.i64)
		|> bucket_sort 2 (i64.highest) shuffle1
		|> (.1)
	let xs2 = iota n2
		|> map (\i -> i%n1)
		|> map (i32.i64)
		|> bucket_sort 2 (i64.highest) shuffle2
		|> (.1)
	let pL1 : [][]u8 = replicate n1 [] :> [n1][0]u8
	let pL2 : [][]u8 = replicate n2 [] :> [n2][0]u8
	in (xs1, xs2, pL1, pL2)