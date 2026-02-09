import "../../../ftbasics"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- Many-to-many join,
-- 
-- ==
-- entry: smj0_i32
-- input @data/datagen.in
-- auto output

entry smj0_i32 [n]
	(pL : [n][]u8)
	(num_keys : [1]i64)
=
	let mult= num_keys[0]
		|> f64.i64
		|> (f64./) (f64.i64 n)
		|> f64.ceil
		|> i64.f64
	let ks1 : [n]i32 = iota num_keys[0]
		|> map (\i -> replicate mult i)
		|> flatten
		|> (\arr -> arr[0:n])
		|> map (i32.i64)
		|> sized n
	let ks2 : [n]i32 = ks1
	in (ks1, pL, ks2, pL)

entry smj0_i64 [n]
	(pL : [n][]u8)
	(num_keys : [1]i64)
=
	let mult= num_keys[0]
		|> f64.i64
		|> (f64./) (f64.i64 n)
		|> f64.ceil
		|> i64.f64
	let ks1 : [n]i64 = iota num_keys[0]
		|> map (\i -> replicate mult i)
		|> flatten
		|> (\arr -> arr[0:n])
		|> sized n
	let ks2 : [n]i64 = ks1
	in (ks1, pL, ks2, pL)