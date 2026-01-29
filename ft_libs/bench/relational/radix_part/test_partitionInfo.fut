import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: partInfo
-- input @data/partitioned_1.in
-- auto output
-- input @data/partitioned_2.in
-- auto output
-- input @data/partitioned_3.in
-- auto output

entry partInfo [n] [b] (ks : [n][b]u8) (_ : [n][]u8) =
	let pInfo = calc_partInfo 16 ks 0 2048 (i32.i64 (b/2))
	in (ks, pInfo.maxDepth, pInfo.bounds, pInfo.depths)