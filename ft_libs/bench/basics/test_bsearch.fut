import "../../ftbasics"
import "../../merge_path"

-- Test binary search versus merge_path
-- ==
-- entry: do_iota do_bsearch do_merge_path
-- input {33554432i64}
-- input {67108864i64}
-- input {134217728i64}

entry do_iota n =
	let a = iota n
	let b = iota n
	in (a,b)

entry do_bsearch n =
	bsearch_range
		(==) (>=) (>) (<)
		(replicate n 0)
		(replicate n n)
		(iota n)
		(iota n)

entry do_merge_path n =
	bsearch_range_merge_path
	   	(==) (>=) (<=) (>) (<)
	   	((n + 8191)/8192)
	    (iota n)
	    (iota n)