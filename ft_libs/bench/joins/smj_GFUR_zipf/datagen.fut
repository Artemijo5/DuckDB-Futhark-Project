import "../../../ftbasics"
import "../../../ftSynthetic"
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
	(pL : [n][]u8)
	(inds2 : [n]i64)
	(zipf_order : [n]i64)
	(zipf_prob : [n]f64)
	(zipf_skew : [1]f64)
=
	let ks1 : [n]i32 = iota n
		|> map (i32.i64)
	let ks2 : [n]i32 =
		let hot_keys = zipf_order
			|> zip (iota n)
			|> merge_sort (\(_,z1) (_,z2) -> z1<=z2)
			|> map (.0)
		in zipf_skewed (iota n) hot_keys n zipf_prob zipf_skew[0] (f64.ceil zipf_skew[0])
			|> map (i32.i64)
			|> merge_sort (<=)
	let inds1 = zipf_order
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
	(inds2 : [n]i64)
	(zipf_order : [n]i64)
	(zipf_prob : [n]f64)
	(zipf_skew : [1]f64)
=
	let ks1 : [n]i64 = iota n
	let ks2 : [n]i64 =
		let hot_keys = zipf_order
			|> zip (iota n)
			|> merge_sort (\(_,z1) (_,z2) -> z1<=z2)
			|> map (.0)
		in zipf_skewed (iota n) hot_keys n zipf_prob zipf_skew[0] (f64.ceil zipf_skew[0])
			|> merge_sort (<=)
	let inds1 = zipf_order
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	in (ks1, pL, ks2, pL)