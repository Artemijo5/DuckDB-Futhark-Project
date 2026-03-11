import "../../ftbasics"
import "../../ft_partition"

-- Produce the partitionInfo of the partitioned column.
-- ==
-- entry: do_partInfo
-- input @data/dat.in
-- auto output

entry do_partInfo [n] [b]
	(radix_bits : [1]i32)
	(xs1 : [][]u8)
	(xs2 : [n][b]u8)
	(pL1 : [][]u8)
	(pL2 : [][]u8)
=
	let size_thresh = 2 * n / (2 ** (i64.i32 radix_bits[0]))
	let maxDepth = i32.min 4 ((u8.num_bits * (i32.i64 b))/radix_bits[0])
	let pInfo = calc_partInfo radix_bits[0] 0 size_thresh maxDepth xs2
	in (radix_bits, xs1, xs2, pL1, pL2
		, [pInfo.maxDepth] |> sized 1, pInfo.bounds, pInfo.depths)