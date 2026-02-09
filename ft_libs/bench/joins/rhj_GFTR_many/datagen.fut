import "../../../ftbasics"

-- Datagen, 4-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-n
-- Many-to-many join.
-- 
-- ==
-- entry: rhj0
-- input @data/datagen.in
-- auto output

entry rhj0 [n]
	(pL1 : [n][]u8)
	(pL2 : [n][]u8)
	(num_keys : [1]i64)
=
	let mult= num_keys[0]
		|> f64.i64
		|> (f64./) (f64.i64 n)
		|> f64.ceil
		|> i64.f64
	let ks1 : [n][4]u8 = iota num_keys[0]
		|> map (\i -> replicate mult i)
		|> flatten
		|> (\arr -> arr[0:n])
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
		|> sized n
	let ks2 : [n][4]u8 = ks1
	in (ks1, pL1, ks2, pL2)