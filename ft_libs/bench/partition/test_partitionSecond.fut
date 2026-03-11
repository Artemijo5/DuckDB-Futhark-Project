import "../../ftbasics"
import "../../ft_partition"

-- Partition the second (here, left-side) column.
-- ==
-- entry: do_partition2
-- input @data/dat.in

entry do_partition2
	(radix_bits : [1]i32)
	(xs1 : [][]u8)
	(xs2 : [][]u8)
	(pL1 : [][]u8)
	(pL2 : [][]u8)
	(maxDepth : [1]i32)
	(bounds : []i64)
	(depths : []i32)
	(fBounds: []i64)
	(lBounds: []i64)
=
	let pInfo : partitionInfo = {maxDepth = maxDepth[0], bounds = bounds, depths = depths}
	let (xs1', pL1') = partition_preconfigured 2 radix_bits[0] xs2 pInfo xs1 pL1
	in (radix_bits, xs1', xs2, pL1', pL2
		, maxDepth, bounds, depths,
		fBounds, lBounds)
	