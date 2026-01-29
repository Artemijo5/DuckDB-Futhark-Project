import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- radix partition works just fine
-- is it just that slow that it won't compile in sorts?

-- Radix-partition with 4&8-byte integer keys.
-- First one level, then recursive.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- ==
-- entry: part1_8 part1_12 part1_16 part2_8 part2_12 part2_16
-- random input { [10000000][4]u8 [10000000][0]u8 }
-- auto output
-- random input { [10000000][8]u8 [10000000][0]u8 }
-- auto output
-- random input { [10000000][4]u8 [10000000][4]u8 }
-- auto output
-- random input { [10000000][8]u8 [10000000][4]u8 }
-- auto output
-- random input { [10000000][4]u8 [10000000][8]u8 }
-- auto output
-- random input { [10000000][8]u8 [10000000][8]u8 }
-- auto output

entry part1_8 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	radix_part (i64.highest) ks pL 0 7 2

entry part1_12 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	radix_part (i64.highest) ks pL 0 11 2

entry part1_16 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	radix_part (i64.highest) ks pL 0 15 2

entry part2_8 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	let b_ = i32.i64 b in
	partition_and_deepen
		(i16.highest) (i64.highest) 8 ks pL 2048 b_ 2

entry part2_12 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	let b_ = i32.i64 b in
	partition_and_deepen
		(i16.highest) (i64.highest) 12 ks pL 2048 (3*(b_)/4) 2

entry part2_16 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	let b_ = i32.i64 b in
	partition_and_deepen
		(i16.highest) (i64.highest) 16 ks pL 2048 ((b_)/2) 2
