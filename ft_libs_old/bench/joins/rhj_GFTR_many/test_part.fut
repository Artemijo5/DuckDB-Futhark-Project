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
	(pL1_ : [n][]u8)
	(ks2_ : [n][]u8)
	(pL2_ : [n][]u8)
=
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ pL1_ 5000 3 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ pL2_ 5000 3 2
	let info2 = calc_partInfo 16 ks2 0 5000 2
	let tab2 = calc_radixHashTab 16 ks2 info2 (i64.highest)
	in (
		ks1, pL1, ks2, pL2,
		info2.maxDepth, info2.bounds, info2.depths,
		tab2.first_info_idx, tab2.last_info_idx
	)