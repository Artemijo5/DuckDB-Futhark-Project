import "../../../ftbasics"
import "../../../ftSynthetic"
import "../../../lib/github.com/diku-dk/sorts/merge_sort"
import "../../../joins/ftHashJoin_old"

-- Datagen, 4-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-n
-- 2nd relation's foreign keys follow Zipf distribution.
-- 
-- ==
-- entry: rhj0
-- input @data/datagen.in
-- auto output

entry rhj0 [n]
	(pL1 : [n][]u8)
	(pL2 : [n][]u8)
	(inds2 : [n]i64) -- can "cheat" by using zipf_order as inds1
	(zipf_order : [n]i64)
	(zipf_prob : [n]f64)
	(zipf_skew : [1]f64)
=
	let ks1 : [n][4]u8 =   iota n
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
	let ks2 : [n][4]u8 =
		let hot_keys = zipf_order
			|> zip (iota n)
			|> merge_sort (\(_,z1) (_,z2) -> z1<=z2)
			|> map (.0)
		in zipf_skewed (iota n) hot_keys n zipf_prob zipf_skew[0] (f64.ceil zipf_skew[0])
			|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
			|> map (map u8.i64)
	let inds1 = zipf_order
	let is1 = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	let is2 = inds2
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1<=ind2)
		|> map (.0)
	in (ks1, is1, pL1, ks2, is2, pL2)