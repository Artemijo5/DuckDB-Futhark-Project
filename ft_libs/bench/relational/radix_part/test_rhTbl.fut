import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: rhTbl_8
-- input @data/partitioned_8.in
-- auto output

entry rhTbl_8 [n] [b] (ks : [n][b]u8) (maxDepth : i32) (bounds : []i64) (depths : []i32) =
	let info : partitionInfo = {maxDepth=maxDepth, bounds=bounds, depths=depths}
	let tab = calc_radixHashTab 8 ks info (i64.highest)
	in (tab.first_info_idx, tab.last_info_idx)

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: rhTbl_12
-- input @data/partitioned_12.in
-- auto output

entry rhTbl_12 [n] [b] (ks : [n][b]u8) (maxDepth : i32) (bounds : []i64) (depths : []i32) =
	let info : partitionInfo = {maxDepth=maxDepth, bounds=bounds, depths=depths}
	let tab = calc_radixHashTab 12 ks info (i64.highest)
	in (tab.first_info_idx, tab.last_info_idx)

-- Construct partitionInfo for partitioned dataset.
--
-- ==
-- entry: rhTbl_16
-- input @data/partitioned_16.in
-- auto output

entry rhTbl_16 [n] [b] (ks : [n][b]u8) (maxDepth : i32) (bounds : []i64) (depths : []i32) =
	let info : partitionInfo = {maxDepth=maxDepth, bounds=bounds, depths=depths}
	let tab = calc_radixHashTab 16 ks info (i64.highest)
	in (tab.first_info_idx, tab.last_info_idx)