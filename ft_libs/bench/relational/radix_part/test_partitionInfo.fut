import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: partInfo_8
-- input @data/partitioned_8.in
-- auto output

entry partInfo_8 [n] [b] (ks : [n][b]u8) (_ : [n][]u8) =
	let pInfo = calc_partInfo 8 ks 0 2048 (i32.i64 b)
	in (ks, pInfo.maxDepth, pInfo.bounds, pInfo.depths)

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: partInfo_12
-- input @data/partitioned_12.in
-- auto output

entry partInfo_12 [n] [b] (ks : [n][b]u8) (_ : [n][]u8) =
	let pInfo = calc_partInfo 12 ks 0 2048 (i32.i64 (3*b/4))
	in (ks, pInfo.maxDepth, pInfo.bounds, pInfo.depths)

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: partInfo_16
-- input @data/partitioned_16.in
-- auto output

entry partInfo_16 [n] [b] (ks : [n][b]u8) (_ : [n][]u8) =
	let pInfo = calc_partInfo 16 ks 0 2048 (i32.i64 (b/2))
	in (ks, pInfo.maxDepth, pInfo.bounds, pInfo.depths)