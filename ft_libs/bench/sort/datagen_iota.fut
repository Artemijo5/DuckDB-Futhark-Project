import "../../ftbasics"

def shuffle_t [n] 't
	(from_i64 : i64 -> t)
	(shuffle : [n]i64)
	(pL : [n][]u8)
	(bitstep : [1]i32)
	(maxval : [1]i64)
=
	let vals = iota n
		|> map (\v -> v%maxval[0])
		|> map (from_i64)
	let xs = vals
		|> bucket_sort 2 (i64.highest) shuffle
		|> (.1)
	in (xs, pL, bitstep)

-- Generate a column of i32 shuffled iota values.
--
-- ==
-- entry: shuffle_i32
-- compiled input @data/dat_i32.in
-- auto output

entry shuffle_i32 shuffle pL bitstep maxval : ([]i32,[][]u8,[1]i32)
	= shuffle_t (i32.i64) shuffle pL bitstep maxval

-- Generate a column of i64 shuffled iota values.
--
-- ==
-- entry: shuffle_i64
-- compiled input @data/dat_i64.in
-- auto output

entry shuffle_i64 shuffle pL bitstep maxval : ([]i64,[][]u8,[1]i32)
	= shuffle_t (id) shuffle pL bitstep maxval