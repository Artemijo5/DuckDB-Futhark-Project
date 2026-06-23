-- Alternative Implementation for String Comparison
-- using segmented reduction.
-- Fully parallel, however cannot be passed as an operator to parallel hof's.
-- Moreover, materializes strings (might be a problem in loops...)

import "../ftbasics"
import "strUtil"
import "../lib/github.com/diku-dk/segmented/segmented"

-- Wrapper for expand_outer_reduce
-- handling the case of 1 point.
local def expand_outer_red [n] 't
	sz get op ne (xs : [n]t)
= if n>1
	then xs |> expand_outer_reduce sz get op ne
	else xs |> expand (sz >-> (i64.max 1))
		(\x ind -> if (sz x)==0 then ne else get x ind)
		|> reduce op ne
		|> replicate n

-- | Alternative implementation for string comparison,
-- using segmented parallelism.
-- Has to materialize the strings.
def str_cmp_alt [n]
	(char_cmp : u8 -> u8 -> i8)
	(strs1 : strInfo)
	(strs2 : strInfo)
	(is1 : [n]i64)
	(is2 : [n]i64)
: [n]i8 =
	let lens1 = is1 |> map (\i1 -> get_str_len i1 strs1)
	let lens2 = is2 |> map (\i2 -> get_str_len i2 strs2)
	in iota n
		|> zip3 lens1 lens2
		|> expand_outer_red
			(\(l1,l2,_) ->
				i64.max 0 (i64.max l1 l2)
			)
			(\(l1,l2,i) k ->
				let i1 = if l1<0 then (-1) else strs1.idxs[is1[i]]
				let i2 = if l2<0 then (-1) else strs2.idxs[is2[i]]
				let c1 = if i1<0 then 0 else
					if l1>k then strs1.contents[i1+k] else 0
				let c2 = if i2<0 then 0 else
					if l2>k then strs2.contents[i2+k] else 0
				in char_cmp c1 c2
			)
			(\res1 res2 ->
				if res1!=0 then res1 else res2
			)
			0

-- bsearch_first for strings using str_cmp_alt
-- TODO fix (...)
def str_bsearch_first [nvs]
	(char_cmp : u8 -> u8 -> i8)
	(min_is : [nvs]i64)
	(max_is : [nvs]i64)
	(xs : strInfo)
	(vs : strInfo) -- must have at least nvs strings
: []i64 =
	let num_iter = map2 (-) max_is min_is
		|> i64.maximum
		|> f64.i64
		|> f64.log
		|> f64.ceil
		|> i64.f64
		|> (i64.+) 1
	let (foundAt,_) = loop (is_at,steps) = (min_is,map2 (-) max_is min_is)
	for j<num_iter do
		let next_steps = steps |> map (\step -> (step+1)/2)
		let isStillIn = map2 (\cur_i cur_step -> cur_i>=0 && cur_step>0)
			is_at steps
		let isAtStart = map2 (==) is_at min_is
			|> map2 (||) (isStillIn |> map (not))
		let isAtEnd = map2 (\cur_i max_i -> cur_i==max_i-1) is_at max_is
		let gt_prev = str_cmp_alt char_cmp vs xs
			(iota nvs |> map (\i -> if isStillIn[i] then i else (-1)))
			(
				map2 (\cur_i atStart ->
					if atStart then (-1) else cur_i
				) is_at isAtStart
			)
			|> map (>0)
			|> trace
		let gt_cur = str_cmp_alt char_cmp vs xs
			(iota nvs |> map (\i -> if isStillIn[i] then i else (-1)) |> trace)
			is_at
			|> map (>0)
			|> trace
		let (next_is,next_steps') = map5 (\i gt_p gt_c step atEnd ->
				if gt_c && atEnd then (-1,0) else
				if gt_c then (i+step,step) else
				if gt_p then (i,0) else
				(i-step,step)
			)
			is_at gt_prev gt_cur next_steps isAtEnd
			|> trace
			|> unzip
		let next_is' = map3 (\next_i min_i max_i ->
			if next_i<min_i then (-1) else
			i64.min next_i (max_i-1)
		) next_is min_is max_is
		in (next_is',next_steps') |> trace
	let check_correctness = str_cmp_alt char_cmp xs vs
		foundAt (iota nvs)
		|> map (==0)
	let foundAt' = map2 (\iAt isCorrect -> if isCorrect then iAt else (-1))
		foundAt check_correctness
	in foundAt'


-- bsearch_last for strings using str_cmp_alt

-- bsearch_range for strings using str_cmp_alt


-- Merge-Path for strings using str_cmp_alt.
-- Diagonals use only the first character of each string.

