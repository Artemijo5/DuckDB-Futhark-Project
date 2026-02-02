import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- Datagen, 4-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Wide-join - payloads vary from 4-8 bytes.
-- Generated data is partitioned, and hash-join structures are computed.
-- 
-- ==
-- entry: rhj0_100 rhj0_50 rhj0_25 rhj0_125 rhj0_625
-- random input { [134217728]f16 [134217728][16]u8 [134217728][16]u8 }
-- auto output

entry rhj0_matchRatio [n]
	(matchRate : f16)
	(prob : [n]f16)
	(pL1_ : [n][]u8)
	(pL2_ : [n][]u8)
=
	let ks1_ : [n][4]u8 =   (iota n)
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
	let ks2_ : [2*n][4]u8 = (iota n)
		|> map2
		|> map2 (\p v ->
			if p<matchRate then v else (n + v)
		) prob
		|> map (\i -> [(i/256/256/256)%256,(i/256/256)%256,(i/256)%256,i%256])
		|> map (map u8.i64)
		|> sized n
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ pL1_ 5000 2 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ pL2_ 5000 2 2
	let info2 = calc_partInfo 16 ks2 0 5000 2
	let tab2 = calc_radixHashTab 16 ks2 info2 (i64.highest)
	in (
		ks1, pL1, ks2, pL2,
		info2.maxDepth, info2.bounds, info2.depths,
		tab2.first_info_idx, tab2.last_info_idx
	)

entry rhj0_100 (matchRate : f16) (prob : [n]f16) (pL1 : [n][]u8) (pL2 : [n][]u8)
= rhj0_matchRatio 1.0 prob pL1 pL2

entry rhj0_50 (matchRate : f16) (prob : [n]f16) (pL1 : [n][]u8) (pL2 : [n][]u8)
= rhj0_matchRatio 0.5 prob pL1 pL2

entry rhj0_25 (matchRate : f16) (prob : [n]f16) (pL1 : [n][]u8) (pL2 : [n][]u8)
= rhj0_matchRatio 0.25 prob pL1 pL2

entry rhj0_125 (matchRate : f16) (prob : [n]f16) (pL1 : [n][]u8) (pL2 : [n][]u8)
= rhj0_matchRatio 0.125 prob pL1 pL2

entry rhj0_625 (matchRate : f16) (prob : [n]f16) (pL1 : [n][]u8) (pL2 : [n][]u8)
= rhj0_matchRatio 0.0625 prob pL1 pL2