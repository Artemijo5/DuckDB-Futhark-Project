-- Alternative implementations of some functions from diku-dk/segmented extension,
-- using binary search instead of segmented scans.

import "ftbasics"
import "merge_path"

def b_replicated_iota [n]
	(szs : [n]i64)
: []i64 =
	let prefix_szs = exscan (+) 0 szs
	let total_sz = i64.sum szs
	in bsearch_last_merge_path
		(>=) (<=) (>) (<)
		((n + 2047) / 2048)
		(iota total_sz)
		prefix_szs

def b_repl_segm_iota [n]
	(szs : [n]i64)
: ([]i64, []i64) =
	let prefix_szs = exscan (+) 0 szs
	let total_sz = i64.sum szs
	let repl_iota =  bsearch_last_merge_path
		(>=) (<=) (>) (<)
		((n + 2047) / 2048)
		(iota total_sz)
		prefix_szs
	let segm_iota = map2 (\repl_i ind -> ind - prefix_szs[repl_i])
		repl_iota
		(iota total_sz)
	in (repl_iota, segm_iota)

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

def b_segmented_exscan [n] 't
	(op : t -> t -> t)
	(antiop : t -> t -> t)
	(ne : t)
	(flags : [n]bool)
	(as : [n]t)
: [n]t = as
	|> b_segmented_scan op antiop ne flags
	|> map2 (\v acc -> acc `antiop` v) as

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

def b_expand [n] 't1 't2
	(sz  : t1 -> i64)
	(get : t1 -> i64 -> t2)
	(xs  : [n]t1)
: []t2 =
	let (repl_is, segm_is) = xs
		|> map (sz)
		|> b_repl_segm_iota
	in map2 (\x ind -> get x ind)
		(repl_is |> map (\i -> xs[i]))
		segm_is