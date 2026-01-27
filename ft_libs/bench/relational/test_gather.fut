import "../../ftbasics"

-- Performance of clustered scatter - i32 values.
--
-- ==
-- entry: scatter1_i32
-- input { 1000i64 }
-- auto output
-- compiled input { 100000i64 }
-- auto output
-- compiled input { 10000000i64 }
-- auto output

entry scatter1_i32 (n : i64) =
	scatter (replicate n (-1)) (iota n) ((0i32..<(i32.i64 n)) :> [n]i32)

-- Performance of clustered scatter - i64 values.
--
-- ==
-- entry: scatter1_i64
-- input { 1000i64 }
-- auto output
-- compiled input { 100000i64 }
-- auto output
-- compiled input { 10000000i64 }
-- auto output

entry scatter1_i64 (n : i64) =
	scatter (replicate n (-1)) (iota n) (iota n)

-- TODO unclustered & segmented scatters

-- Performance of clustered gathers - i32 values.
--
-- ==
-- entry: clustered1_i32
-- compiled input { 1000i64 }
-- auto output
-- compiled input { 100000i64 }
-- auto output
-- compiled input { 10000000i64 }
-- auto output

entry clustered1_i32 (n : i64) =
	gather (-1) (0i32..<(i32.i64 n)) (iota n)

-- Performance of clustered gathers - i64 values.
--
-- ==
-- entry: clustered1_i64
-- compiled input { 1000i64 }
-- auto output
-- compiled input { 100000i64 }
-- auto output
-- compiled input { 10000000i64 }
-- auto output

entry clustered1_i64 (n : i64) =
	gather (-1) (iota n) (iota n)

-- Performance of unclustered gathers - i32 values.
--
-- ==
-- entry: unclustered1_i32
-- input @data/gatherIn_i32.in
-- auto output

entry unclustered1_i32 (vs : []i32) (is : []i64) =
	gather (-1) vs is

-- Performance of unclustered gathers - i64 values.
--
-- ==
-- entry: unclustered1_i64
-- input @data/gatherIn_i64.in
-- auto output

entry unclustered1_i64 (vs : []i64) (is : []i64) =
	gather (-1) vs is

-- Performance of segmented unclustered gathers - i64 values.
--
-- 
-- entry: unclustered2
-- input @data/gatherIn_i64.in
-- auto output

entry unclustered2 [n] (vs : []i64) (is : [n]i64) : [n]i64 =
	let psize_ = i64.max 1 (128000 / (i64.i32 ((64+u8.num_bits-1)/u8.num_bits) ))
	let max_iter = (n+psize_-1) / psize_
	in loop buff : [n]i64 = (replicate n 0) for j in (iota max_iter) do
		let inf = j*psize_
		let sup = i64.min n (inf + psize_)
		in buff
		  |> map2 (\j v ->
		    if (inf<=j && sup>j)
		    then vs[j]
		    else v
		  ) is