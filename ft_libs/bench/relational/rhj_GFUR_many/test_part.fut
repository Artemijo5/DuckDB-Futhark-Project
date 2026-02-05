import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- Radix-partition keys & generate hash-partition strucures.
-- 
-- ==
-- entry: rpart
-- input @data/rhj_data.in
-- auto output

entry rpart [n]
	(ks1_ : [n][]u8)
	(is1_ : [n]i64)
	(pL1 : [n][]u8)
	(ks2_ : [n][]u8)
	(is2_ : [n]i64)
	(pL2 : [n][]u8)
=
	let (ks1,is1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ is1_ 5000 3 2
	let (ks2,is2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ is2_ 5000 3 2
	let info2 = calc_partInfo 16 ks2 0 5000 2
	let tab2 = calc_radixHashTab 16 ks2 info2 (i64.highest)
	in (
		ks1, is1, pL1, ks2, is2, pL2,
		info2.maxDepth, info2.bounds, info2.depths,
		tab2.first_info_idx, tab2.last_info_idx
	)