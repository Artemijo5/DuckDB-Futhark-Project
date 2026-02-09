import "../../../ftbasics"

-- Performance of clustered scatter - i32 values.
--
-- ==
-- entry: scatter1_i32
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry scatter1_i32 (n : i64) =
	scatter (replicate n (-1)) (iota n) ((0i32..<(i32.i64 n)) :> [n]i32)

-- Performance of clustered scatter - i64 values.
--
-- ==
-- entry: scatter1_i64
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry scatter1_i64 (n : i64) =
	scatter (replicate n (-1)) (iota n) (iota n)

-- TODO unclustered & segmented scatters

-- Performance of clustered gathers - i32 values.
--
-- ==
-- entry: clustered1_i32
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry clustered1_i32 (n : i64) =
	gather (-1) (0i32..<(i32.i64 n)) (iota n)

-- Performance of clustered gathers - i64 values.
--
-- ==
-- entry: clustered1_i64
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry clustered1_i64 (n : i64) =
	gather (-1) (iota n) (iota n)

-- Performance of unclustered gathers - 4-byte & 8-byte values.
--
-- ==
-- entry: unclustered1
-- input @data/gatherIn_i32.in
-- auto output
-- input @data/gatherIn_i64.in
-- auto output

entry unclustered1 [b] (vs : [][b]u8) (is : []i64) =
	gather (replicate b 0) vs is

-- Performance of segmented unclustered gathers - i64 values.
--
-- ==
-- entry: unclustered2
-- input @data/gatherIn_i32.in
-- auto output
-- input @data/gatherIn_i64.in
-- auto output

entry unclustered2 [n] [b] (vs : [n][b]u8) (is : [n]i64) : [n][b]u8 =
	let psize_ = i64.max 1 (128000 / b)
	let max_iter = (n+psize_-1) / psize_
	in loop buff : [n][b]u8 = (replicate n (replicate b 0)) for j in (iota max_iter) do
		let inf = j*psize_
		let sup = i64.min n (inf + psize_)
		in buff
		  |> map2 (\j v ->
		    if (inf<=j && sup>j)
		    then vs[j]
		    else v
		  ) is