import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- Generated data is partitioned, and hash-join structures are computed.
-- 
-- ==
-- entry: rhj0
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry rhj0
	(n : i64)
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
	let pL1_ : [n][0]u8 = (replicate n [])
	let pL2_ : [2*n][0]u8 = (replicate (2*n) [])
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ pL1_ 4096 2 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ pL2_ 4096 2 2
	let info2 = calc_partInfo 16 ks2 0 4096 2
	let tab2 = calc_radixHashTab 16 ks2 info2 (i64.highest)
	in (
		ks1, pL1, ks2, pL2,
		info2.maxDepth, info2.bounds, info2.depths,
		tab2.first_info_idx, tab2.last_info_idx
	)