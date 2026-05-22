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
-- 2. using the expand function from the segmented extension.
-- 3. using a binary search.
-- ==
-- entry: mk_segments_scan mk_segments_expand mk_segments_bsearch
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

-- | Binary search:
-- for each value v in vs,
-- find the largest index i,
-- such that v >= xs[i].
def bsearch_last [nvs] [n] 't
	(geq: t -> t -> bool)
	(lt : t -> t -> bool)
	(min_is : [nvs]i64)
	(max_is : [nvs]i64) -- exclusive
	(xs : [n]t)
	(vs : [nvs]t)
: [nvs]i64 = vs |> map3 (\i_min i_max v ->
	if i_min<0 || (v `lt` xs[i_min]) then (-1) else
	let (found_at,_) = loop (i, last_step) = (i_min, i_max-i_min)
	while i>=0 && i>=i_min && i<i_max &&
		!( (v `geq` xs[i]) && ( i==(i_max-1) || (v `lt` xs[i+1]) ) )
	do
		-- check for kv>=cv && kv<nv is done in loop conditions
		-- so inside loop assume that isn't the case
		let this_step = (last_step+1)/2 in
		if (v `lt` xs[i]) then
			(i64.max i_min (i-this_step), this_step)
		else
			(i64.min (i_max-1) (i+this_step), this_step)
	in found_at
) min_is max_is

entry mk_segments_bsearch [k]
	(n : i64)
	(k_is : [k]i64)
: [n]i64 =
	-- For each index i in iota n,
	-- find the largest index i,
	-- such that i >= k_is[j].
	iota n |> bsearch_last
		(>=) (<)
		(replicate n 0)
		(replicate n k)
		k_is