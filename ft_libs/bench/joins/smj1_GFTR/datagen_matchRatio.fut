import "../../../ftbasics"

-- Generate i32 data for join with match ratio <= 100%.
--
-- ==
-- entry: do_datagen_i32
-- input @data/dat_i32.in
-- auto output

entry do_datagen_i32 [n1] [n2]
	(shuffle1 : [n1]i64)
	(shuffle2 : [n2]i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
	(probs : [n2]f16)
	(ratio : [1]f16)
=
	let xs1 = (0..2..<(2*n1))
		|> sized n1
		|> map (i32.i64)
		|> bucket_sort 2 (i64.highest) shuffle1
		|> (.1)
	let xs2 = (0..2..<(2*n2))
		|> sized n2
		|> map (\i -> i%(2*n1))
		|> map (i32.i64)
		|> bucket_sort 2 (i64.highest) shuffle2
		|> (.1)
		|> map2 (\prob x -> if prob>ratio[0] then x+1 else x) probs
	in (xs1, xs2, pL1, pL2)