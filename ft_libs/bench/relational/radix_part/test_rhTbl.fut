import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: rhTbl
-- input @data/partitioned_1.in
-- auto output
-- input @data/partitioned_2.in
-- auto output
-- input @data/partitioned_3.in
-- auto output

entry rhTbl [n] [b] (ks : [n][b]u8) (maxDepth : i32) (bounds : []i64) (depths : []i32) =
	let info : partitionInfo = {maxDepth=maxDepth, bounds=bounds, depths=depths}
	let tab = calc_radixHashTab 16 ks info (i64.highest)
	in (tab.first_info_idx, tab.last_info_idx)