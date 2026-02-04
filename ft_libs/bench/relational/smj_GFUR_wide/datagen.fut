import "../../../ftbasics"
import "../../../lib/github.com/diku-dk/sorts/merge_sort"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- 
-- ==
-- entry: smj0_i32
-- input @data/datagen.in
-- auto output

entry smj0_i32 [n]
	(inds1 : [n]i64)
	(inds2 : [2*n]i64)
	(pL : [2*n][]u8)
=
	let ks1 = iota n
		|> map (i32.i64)
	let ks2 = iota n
		|> map (\i -> replicate 2 i)
		|> flatten
		|> map (i32.i64)
		|> sized (2*n)
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota (2*n))
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	in (ks1, is1, pL, ks2, is2, pL[0:n])

entry smj0_i64 [n]
	(inds1 : [n]i64)
	(inds2 : [2*n]i64)
	(pL : [2*n][]u8)
=
	let ks1 = iota n
	let ks2 = iota n
		|> map (\i -> replicate 2 i)
		|> flatten
		|> sized (2*n)
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota (2*n))
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	in (ks1, is1, pL, ks2, is2, pL[0:n])