import "../../../ftbasics"
import "../../../joins/ftHashJoin"

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
-- random input { [33554432][4]u8 [33554432][0]u8 }
-- auto output
-- random input { [33554432][8]u8 [33554432][0]u8 }
-- auto output
-- random input { [33554432][4]u8 [33554432][8]u8 }
-- auto output
-- random input { [33554432][8]u8 [33554432][8]u8 }
-- auto output
-- random input { [67108864][4]u8 [67108864][0]u8 }
-- auto output
-- random input { [67108864][8]u8 [67108864][0]u8 }
-- auto output
-- random input { [67108864][4]u8 [67108864][8]u8 }
-- auto output
-- random input { [67108864][8]u8 [67108864][8]u8 }
-- auto output
-- random input { [134217728][4]u8 [134217728][0]u8 }
-- auto output
-- random input { [134217728][8]u8 [134217728][0]u8 }
-- auto output
-- random input { [134217728][4]u8 [134217728][8]u8 }
-- auto output
-- random input { [134217728][8]u8 [134217728][8]u8 }
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
		(n / 65536 * 100 / 95)
		((b_)/2) 2

-- TODO

-- random input { [33554432][4]u8 [33554432][4]u8 }
-- auto output
-- random input { [33554432][8]u8 [33554432][4]u8 }
-- auto output

-- random input { [67108864][4]u8 [67108864][4]u8 }
-- auto output
-- random input { [67108864][8]u8 [67108864][4]u8 }
-- auto output

-- random input { [134217728][4]u8 [134217728][4]u8 }
-- auto output
-- random input { [134217728][8]u8 [134217728][4]u8 }
-- auto output

-- random input { [268435456][8]u8 [268435456][0]u8 }
-- auto output
-- random input { [268435456][4]u8 [268435456][4]u8 }
-- auto output
-- random input { [268435456][8]u8 [268435456][4]u8 }
-- auto output
-- random input { [268435456][4]u8 [268435456][8]u8 }
-- auto output
-- random input { [268435456][8]u8 [268435456][8]u8 }
-- auto output