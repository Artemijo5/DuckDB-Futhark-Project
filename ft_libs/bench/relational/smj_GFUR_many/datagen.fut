import "../../../ftbasics"
import "../../../lib/github.com/diku-dk/sorts/merge_sort"

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
	(inds1 : [n]i64)
	(inds2 : [n]i64)
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
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	in (ks1, is1, pL, ks2, is2, pL)

entry smj0_i64 [n]
	(pL : [n][]u8)
	(inds1 : [n]i64)
	(inds2 : [n]i64)
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
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	in (ks1, is1, pL, ks2, is2, pL)