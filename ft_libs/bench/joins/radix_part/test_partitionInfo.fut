import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: partInfo
-- input @data/part_data.in
-- auto output

entry partInfo [n] [b] (ks : [n][b]u8) (_ : [n][]u8) =
	let pInfo = calc_partInfo 16 ks 0 (n / 65536) (i32.i64 (b/2))
	in (ks, pInfo.maxDepth, pInfo.bounds, pInfo.depths)