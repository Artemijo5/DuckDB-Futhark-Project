-- Alternative implementations of some functions from diku-dk/segmented extension,
-- without using segmented scans.

import "ftbasics"
import "merge_path"

-- | Alternative replicated_iota using binary search.
def b_replicated_iota [n]
	(szs : [n]i64)
: []i64 =
	let prefix_szs = exscan (+) 0 szs
	let total_sz = i64.sum szs
	in bsearch_last_merge_path
		(>=) (<)
		((n+2047)/2048)
		(iota total_sz)
		(prefix_szs)

-- | Alternative replicated_iota using dict_encoding.
def s_replicated_iota [n]
	(szs : [n]i64)
: []i64 =
	let present_is = iota n
		|> filter (\i -> szs[i]>0)
	let prefix_szs = szs |> exscan (+) 0
	let total_sz = i64.sum szs
	let repl_iota = scatter (replicate total_sz false)
			(present_is |> map (\i -> prefix_szs[i]))
			(present_is |> map (\_ -> true))
		|> dict_encoding
	in repl_iota |> map (\i -> present_is[i])

-- | Alternative repl_segm_iota using binary search.
def b_repl_segm_iota [n]
	(szs : [n]i64)
: ([]i64, []i64) =
	let prefix_szs = exscan (+) 0 szs
	let total_sz = i64.sum szs
	let repl_iota = bsearch_last_merge_path
		(>=) (<)
		((n+2047)/2048)
		(iota total_sz)
		(prefix_szs)
	let segm_iota = map2 (\repl_i ind -> ind - prefix_szs[repl_i])
		repl_iota
		(iota total_sz)
	in (repl_iota, segm_iota)

-- | Alternative repl_segm_iota using dict_encoding.
def s_repl_segm_iota [n]
	(szs : [n]i64)
: ([]i64, []i64) =
	let present_is = iota n
		|> filter (\i -> szs[i]>0)
	let prefix_szs = szs |> exscan (+) 0
	let total_sz = i64.sum szs
	let repl_iota = scatter (replicate total_sz false)
			(present_is |> map (\i -> prefix_szs[i]))
			(present_is |> map (\_ -> true))
		|> dict_encoding
		|> map (\i -> present_is[i])
	let segm_iota = map2 (\repl_i ind -> ind - prefix_szs[repl_i])
		repl_iota
		(iota total_sz)
	in (repl_iota, segm_iota)

-- | Alternative segmented_scan using binary search.
-- NOTE: requires a reverse of op, antiop.
-- Such that: if op x y = z, then antiop z x = y, antiop z y = x.
-- For example min, max can't be used with this.
-- (+) can be used with (-), (*) can be used with (/).
def b_segmented_scan [n] 't
	(op : t -> t -> t)
	(antiop : t -> t -> t)
	(ne : t)
	(flags : [n]bool)
	(as : [n]t)
: [n]t =
	let prefix_scan = as |> exscan op ne
	let group_prefixes_small = bsearch_first_merge_path
			(==) (>=) (<=) (>) (<)
			((n + 2047) / 2048)
			(flags |> filter (id) |> indices)
			(flags |> dict_encoding)
		|> map (\i -> prefix_scan[i])
	let group_prefixes = flags
		|> dict_encoding
		|> map (\i -> group_prefixes_small[i])
	in as
		|> scan op ne
		|> map2 (\pref tot -> tot `antiop` pref) group_prefixes

-- | Exclusive segmented_scan using binary search.
-- NOTE: requires a reverse of op, antiop.
-- Such that: if op x y = z, then antiop z x = y, antiop z y = x.
-- For example min, max can't be used with this.
-- (+) can be used with (-), (*) can be used with (/).
def b_segmented_exscan [n] 't
	(op : t -> t -> t)
	(antiop : t -> t -> t)
	(ne : t)
	(flags : [n]bool)
	(as : [n]t)
: [n]t = as
	|> b_segmented_scan op antiop ne flags
	|> map2 (\v acc -> acc `antiop` v) as

-- | Alternative segmented_reduce using binary search.
-- NOTE: requires a reverse of op, antiop.
-- Such that: if op x y = z, then antiop z x = y, antiop z y = x.
-- For example min, max can't be used with this.
-- (+) can be used with (-), (*) can be used with (/).
def b_segmented_reduce [n] 't
	(op : t -> t -> t)
	(antiop : t -> t -> t)
	(ne : t)
	(flags : [n]bool)
	(as : [n]t)
: []t = as
	|> b_segmented_scan op antiop ne flags
	|> zip (flags |> rotate 1)
	|> filter (.0)
	|> map (.1)

-- | Alternative segmented_reduce using a histogram.
def h_segmented_reduce [n] 't
	(op : t -> t -> t)
	(ne : t)
	(flags : [n]bool)
	(as : [n]t)
: []t =
	let ks = flags |> dict_encoding
	let k = if n==0 then 0 else (last ks) + 1
	in hist op ne k ks as

-- | Alternative expand based on previous alternative repl_segm_iota's.
def b_expand [n] 't1 't2
	(sz  : t1 -> i64)
	(get : t1 -> i64 -> t2)
	(xs  : [n]t1)
: []t2 =
	let (repl_is, segm_is) = xs
		|> map (sz >-> (i64.max 0))
	--	|> b_repl_segm_iota
		|> s_repl_segm_iota
	in map2 (\x ind -> get x ind)
		(repl_is |> map (\i -> xs[i]))
		segm_is