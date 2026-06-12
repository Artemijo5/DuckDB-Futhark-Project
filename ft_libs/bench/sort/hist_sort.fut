import "../../ftbasics"
import "../../lib/github.com/diku-dk/segmented/segmented"

-- How fast can we sort a column + payloads?
--
-- ==
-- entry: do_hist_sort
-- compiled input @data/dat_i64.in

def hist_sort [n]
	(xs : [n]i64)
: ([n]i64, [n]i64) =
	let min_key = xs |> i64.minimum
	let k = 1 + (xs |> i64.maximum) - min_key
	let xs' = (copy xs) |> map (\x -> x - min_key)
	let multiplicities = hist (+) 0i64 k xs' (replicate n 1i64)
	let max_mult = multiplicities |> i64.maximum
	let dest_first_is = multiplicities
		|> exscan (+) 0
	let dest_vs = iota k
		|> expand (\i -> multiplicities[i])
			(\i _ -> i)
		|> sized n
	let (final_is,_) = loop (d_is, cur_xs)
	= (replicate n (-1), xs') for j<max_mult do
		let dest_cur_is = map2 (\mult ind ->
			if mult<=j then (-1) else ind+j
		) multiplicities dest_first_is
		let hist_is = cur_xs
			|> map (\x -> if x<0 then x else dest_cur_is[x])
		let d_is' = hist_lean (i64.min) n n
			hist_is (iota n)
		|> map2 (\alt neu -> if neu>=n then alt else neu) d_is
		let next_xs = if j==max_mult-1
			then cur_xs
			else scatter (copy cur_xs) d_is' (replicate n (-1))
		in (d_is', next_xs)
	in (dest_vs |> map (\v -> v+min_key), final_is)

entry do_hist_sort [n]
	(xs: [n]i64)
	(_: [n][]u8)
	(_ : [1]i32)
=
	hist_sort xs
