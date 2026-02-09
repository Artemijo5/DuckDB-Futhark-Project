import "../../../ftbasics"
import "../../../joins/ftHashJoin_old"

-- radix partition works just fine
-- so why does radix sort not compile?
-- TODO determine which tests are necessary to avoid hoarding too many GB's per bench

-- Radix-partition with 16-bit radix size.
-- Keys & payloads vary between 4 bytes & 8 bytes.
-- part1 only does the first pass, like radix-sort on the first bits.
-- part2 has a threshold to perform a second pass for some data.
--
-- ==
-- entry: part1 part2
-- input @data/part_data.in
-- auto output

entry part1 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	radix_part (i64.highest) ks pL 0 15 2

entry part2 [n] [b] [pL_b] (ks : [n][b]u8) (pL : [n][pL_b]u8) =
	let b_ = i32.i64 b in
	partition_and_deepen
		(i16.highest)
		(i64.highest)
		16
		ks
		pL
		(n / 65536)
		((b_)/2)
		2