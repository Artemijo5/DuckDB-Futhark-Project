import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"
import "../../../lib/github.com/diku-dk/sorts/merge_sort" 

-- Datagen, 4-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Wide-join - payloads vary from 4-8 bytes.
-- Generated data is partitioned, and hash-join structures are computed.
-- 
-- ==
-- entry: rhj0
-- input @data/datagen.in
-- auto output

entry rhj0 [n]
	(inds1 : [n]i64)
	(inds2 : [2*n]i64)
	(pL1 : [n][]u8)
	(pL2 : [2*n][]u8)
=
	let ks1_ : [n][4]u8 =   iota n
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
	let ks2_ : [2*n][4]u8 = iota n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
		|> sized (2*n)
	let is1_ = inds1
		|> zip (iota n)
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	let is2_ = inds2
		|> zip (iota (2*n))
		|> merge_sort (\(_,ind1) (_,ind2) -> ind1 <= ind2)
		|> map (.0)
	let (ks1,is1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ is1_ 5000 2 2
	let (ks2,is2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ is2_ 5000 2 2
	let info2 = calc_partInfo 16 ks2 0 5000 2
	let tab2 = calc_radixHashTab 16 ks2 info2 (i64.highest)
	
	in (
		ks1, pL1, ks2, pL2,
		info2.maxDepth, info2.bounds, info2.depths,
		tab2.first_info_idx, tab2.last_info_idx,
		is1, is2
	)