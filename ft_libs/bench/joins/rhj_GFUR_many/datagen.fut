import "../../../ftbasics"
import "../../../lib/github.com/diku-dk/sorts/merge_sort"

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
	(inds1 : [n]i64)
	(inds2 : [n]i64)
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
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let ks2 : [n][4]u8 = ks1
	in (ks1, is1, pL1, ks2, is2, pL2)