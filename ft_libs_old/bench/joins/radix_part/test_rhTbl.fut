import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: rhTbl
-- input @data/part_data.in

entry rhTbl [n] [b] (ks : [n][b]u8) (maxDepth : i32) (bounds : []i64) (depths : []i32) =
	let info : partitionInfo = {maxDepth=maxDepth, bounds=bounds, depths=depths}
	let tab = calc_radixHashTab 16 ks info (i64.highest)
	in (tab.first_info_idx, tab.last_info_idx)