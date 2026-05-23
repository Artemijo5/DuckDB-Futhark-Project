-- Suppose the following problem:
--
-- xs : [n]t -- values sorted by a key function
-- k_is : []i64 -- for each valid key value, the first index of its segment in xs
-- Segments can also have 0 length (ie consecutive equal values in k_is)
--
-- We want to produce an array ks : [n]i64,
-- such that for each element in xs, we have its key value,
-- represented by its corresponding index in k_is.
--
-- This can occur in applications involving for example:
-- hash-partioning;
-- spatial indexing;
-- string representation.

-- Test 3 different methods for this problem:
-- 1. using the scan hof with the (i64.min) operator.
-- 2. using the expand function from segmented.
-- 3. using a custom expand function.
-- ==
-- entry: mk_segments_scan mk_segments_expand mk_segments_my_expand
-- input {16i64 [0i64, 4i64, 5i64, 9i64, 12i64, 15i64]}
-- output {[0i64,0i64,0i64,0i64,1i64,2i64,2i64,2i64,2i64,3i64,3i64,3i64,4i64,4i64,4i64,5i64]}
-- input {16i64 [0i64, 0i64, 4i64, 5i64, 9i64, 9i64, 9i64, 12i64, 15i64, 15i64]}
-- output {[1i64,1i64,1i64,1i64,2i64,3i64,3i64,3i64,3i64,6i64,6i64,6i64,7i64,7i64,7i64,9i64]}

entry mk_segments_scan
	(n : i64)
	(k_is : []i64)
: [n]i64 =
	-- "Scatter" each key in k_is to its index.
	-- In the case of consecutive equal values,
	-- we only want the last one (since previous ones have 0 elements).
	-- We can use a histogram for this.
	let ks_blank = hist (i64.max) (-1) n k_is (indices k_is)
	-- Scan to fill out i64.lowest positions with the last positive value.
	in ks_blank |> scan (i64.max) (-1)

import "../lib/github.com/diku-dk/segmented/segmented"

entry mk_segments_expand [k]
	(n : i64)
	(k_is : [k]i64)
: [n]i64 =
	-- Obtain the size of each segment
	let k_sz = iota k
		|> map (\i ->
			if i==k-1
			then n - k_is[i]
			else k_is[i+1] - k_is[i]
		)
	-- Expand
	in iota k
		|> expand
			(\i -> k_sz[i])
			(\i _ -> i)
		|> sized n

-- | Alternative implementation of expand.
def my_expand [n] 't1 't2
	(sz  : t1 -> i64)
	(get : t1 -> i64 -> t2)
	(xs  : [n]t1)
: []t2 =
	let szs = xs |> map (sz >-> (i64.max 0))
	let prefix_szs = szs |> scan (+) 0
		|> map2 (\sz_at pref -> pref-sz_at) szs
	let total_sz = reduce (+) 0 szs
	let repl_iota = hist (i64.max) (-1) total_sz prefix_szs (iota n)
		|> scan (i64.max) (-1)
	let segm_iota = iota total_sz
		|> map2 (\r_i i -> i - prefix_szs[r_i]) repl_iota
	in map2 (\r_i s_i -> get xs[r_i] s_i) repl_iota segm_iota

entry mk_segments_my_expand [k]
	(n : i64)
	(k_is : [k]i64)
: [n]i64 =
	-- Obtain the size of each segment
	let k_sz = iota k
		|> map (\i ->
			if i==k-1
			then n - k_is[i]
			else k_is[i+1] - k_is[i]
		)
	-- Expand
	in iota k
		|> my_expand
			(\i -> k_sz[i])
			(\i _ -> i)
		|> sized n