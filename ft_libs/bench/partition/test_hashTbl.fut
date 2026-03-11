import "../../ftbasics"
import "../../ft_partition"

-- Produce the hash table of the partitioned column.
-- ==
-- entry: do_hashTbl
-- input @data/dat.in
-- auto output

entry do_hashTbl
	(radix_bits : [1]i32)
	(xs1 : [][]u8)
	(xs2 : [][]u8)
	(pL1 : [][]u8)
	(pL2 : [][]u8)
	(maxDepth : [1]i32)
	(bounds : []i64)
	(depths : []i32)
=
	let pInfo : partitionInfo = {maxDepth = maxDepth[0], bounds = bounds, depths = depths}
	let pTbl = calc_radixHashTab radix_bits[0] pInfo xs2
	in (radix_bits, xs1, xs2, pL1, pL2
		, maxDepth, bounds, depths,
		pTbl.first_info_idx, pTbl.last_info_idx)