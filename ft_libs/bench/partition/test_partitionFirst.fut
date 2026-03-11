import "../../ftbasics"
import "../../ft_partition"

-- Partition the right-side column.
-- This is to see, for instance, how skewness in the right-side influences performance.
-- If the right-side only is skewed, it is advantageous to partition the left-side one first.
-- ==
-- entry: do_partition1
-- input @data/dat.in
-- auto output

entry do_partition1 [n] [b]
	(radix_bits : [1]i32)
	(xs1 : [][]u8)
	(xs2 : [n][b]u8)
	(pL1 : [][]u8)
	(pL2 : [n][]u8)
=
	let size_thresh = 2 * n / (2 ** (i64.i32 radix_bits[0]))
	let maxDepth = i32.min 4 ((u8.num_bits * (i32.i64 b))/radix_bits[0])
	let (xs2', pL2') = partition_and_deepen 2 radix_bits[0] size_thresh maxDepth xs2 pL2
	in (radix_bits, xs1, xs2', pL1, pL2')