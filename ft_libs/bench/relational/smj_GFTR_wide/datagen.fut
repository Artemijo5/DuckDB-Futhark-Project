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
	(pL : [n][]u8)
=
	let ks1 : [n/2]i32 = iota (n/2)
		|> map (i32.i64)
	let ks2 : [n]i32 = iota (n/2)
		|> map (\i -> replicate 2 i)
		|> flatten
		|> map (i32.i64)
		|> sized n
	in (ks1, pL, ks2, pL[0:n])

entry smj0_i64 [n]
	(pL : [n][]u8)
=
	let ks1 : [n/2]i64 = iota (n/2)
	let ks2 : [n]i64 = iota (n/2)
		|> map (\i -> replicate 2 i)
		|> flatten
		|> sized n
	in (ks1, pL, ks2, pL[0:n])