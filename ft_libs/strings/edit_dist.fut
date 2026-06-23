-- Implementations of Hamming and Levenschtein distance between 2 strings.

import "../ftbasics"
import "../lib/github.com/diku-dk/segmented/segmented"

import "strUtil"

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


	-- | Parallel Hamming distance computations.
	-- For unequal strlen, the absolute difference is added to the result.
	def hamming_dists [n]
		(char_cmp : u8 -> u8 -> i8)
		(strs1 : strInfo)
		(strs2 : strInfo)
		(is1 : [n]i64)
		(is2 : [n]i64)
	: [n]i64 =
		-- string lengths
		let lens1 = is1 |> map (\i1 -> get_str_len i1 strs1)
		let lens2 = is2 |> map (\i2 -> get_str_len i2 strs2)
		let lens = map2 (i64.max) lens1 lens2
		-- gather & compare the strings
		in iota n |> expand_outer_red
			(\i -> lens[i])
			(\i ind ->
				let leftSide = get_kth_char ind is1[i] strs1
				let rightSide = get_kth_char ind is2[i] strs2
				in char_cmp leftSide rightSide
					|> (i8.abs >-> i64.i8)
			)
			(+) 0

	-- | Parallel Hamming distance computations,
	-- with strings in the same superstring.
	def hamming_codists [n]
		(char_cmp : u8 -> u8 -> i8)
		(strs : strInfo)
		(is1 : [n]i64)
		(is2 : [n]i64)
	: [n]i64 = hamming_dists
		char_cmp strs strs is1 is2

	-- | Levenschtein distance between 2 strings.
	-- Code based on Wikipedia.
	def lev_dist
		(char_cmp : u8 -> u8 -> i8)
		(strs1 : strInfo)
		(strs2 : strInfo)
		(i1 : i64)
		(i2 : i64)
	: i64 =
		let len1 = get_str_len i1 strs1
		let len2 = get_str_len i2 strs2
		let v0 = iota (len2+1)
		let v1 = replicate (len2+1) 0
		let (final_v0,_) = loop (l_v0,l_v1) : ([len2+1]i64, [len2+1]i64) = (v0,v1)
		for j<len1 do
			let l_v1' = (copy l_v1) with [0] = j+1
			let leftSide = get_kth_char j i1 strs1
			let new_v1 = loop inner_v1 = l_v1'
				for i<len2 do
					let rightSide = get_kth_char i i2 strs2
					let ineq = char_cmp leftSide rightSide
						|> (i8.abs >-> bool.i8)
					let del_cost = l_v0[i+1] + 1
					let ins_cost = inner_v1[i] + 1
					let sub_cost = (if ineq
						then l_v0[i] + 1
						else l_v0[i])
					let newval = i64.min sub_cost
						(i64.min ins_cost del_cost)
					in (copy inner_v1) with [i+1]=newval
			in (new_v1, l_v0)
		in final_v0 |> last

	-- | Check if the Levenschtein distance is within a certain threshold.
	def lev_dist_within
		(char_cmp : u8 -> u8 -> i8)
		(thresh : i64)
		(strs1 : strInfo)
		(strs2 : strInfo)
		(i1 : i64)
		(i2 : i64)
	: bool =
		let len1 = get_str_len i1 strs1
		let len2 = get_str_len i2 strs2
		let v0 = iota (len2+1)
		let v1 = replicate (len2+1) 0
		let (final_v0,_,_) = loop (l_v0,l_v1,j) : ([len2+1]i64, [len2+1]i64, i64) = (v0,v1,0)
		while j<len1 do
			let l_v1' = (copy l_v1) with [0] = j+1
			let leftSide = get_kth_char j i1 strs1
			let new_v1 = loop inner_v1 = l_v1'
				for i<len2 do
					let rightSide = get_kth_char i i2 strs2
					let ineq = char_cmp leftSide rightSide
						|> (i8.abs >-> bool.i8)
					let del_cost = l_v0[i+1] + 1
					let ins_cost = inner_v1[i] + 1
					let sub_cost = (if ineq
						then l_v0[i] + 1
						else l_v0[i])
					let newval = i64.min sub_cost
						(i64.min ins_cost del_cost)
					in (copy inner_v1) with [i+1]=newval
			let next_j = if all (>thresh) new_v1 then len1 else j+1
			in (new_v1, l_v0, next_j)
		in final_v0 |> (last >-> (<= thresh))

	def lev_dists_within
		(char_cmp : u8 -> u8 -> i8)
		(thresh : i64)
		(strs1 : strInfo)
		(strs2 : strInfo)
	: []bool =
		let n = length strs1.idxs
		let lens1 = iota n
			|> map (\i -> get_str_len i strs1)
		let lens2 = iota n
			|> map (\i -> get_str_len i strs2)
		let segm_is = lens2
			|> map (\l -> l+1)
			|> exscan (+) 0
		let segm_flags = lens2 |> expand
			(\l -> l+1)
			(\_ ind -> ind==0)
		let vs0 = iota n |> expand 
			(\i -> lens2[i] + 1)
			(\_ ind -> ind)
		let vs1 = iota n |> expand
			(\i -> lens2[i] + 1)
			(\_ _ -> 0i64)
		let (final_vs0,_,_) = loop (l_vs0,l_vs1,j) = (vs0,vs1,0)
		while j<(i64.maximum lens1) do
			let l_vs1' = scatter (copy l_vs1)
				segm_is (replicate n (j+1))
			let leftSides = iota n
				|> map (\i1 -> get_kth_char j i1 strs1)
			let new_vs1 = loop inner_vs1 = l_vs1'
				for i<(i64.maximum lens2) do
					let rightSides = iota n
						|> map (\i2 -> get_kth_char i i2 strs2)
					let ineqs = map2 (char_cmp) leftSides rightSides
						|> (i8.abs >-> bool.i8)
					let del_costs = zip (iota n) lens2
						|> map (\i2 l2 -> if l2>i2
							then l_vs0[segm_is[i2]+i+1]
							else l_vs0[segm_is[i2]+l2]
						)
					let ins_costs = zip (iota n) lens2
						|> map (\i2 l2 -> if l2>i2
							then inner_vs1[segm_is[i2]+i]
							else inner_vs1[segm_is[i2]+l2-1]
						)
					let ins_costs = zip3 (iota n2) lens2 ineqs
						|> map (\i2 l2 ineq -> 
							if ineq
							then l_vs0[segm_is[i2]+ (i64.min i (l2-1))] + 1
							else l_vs0[segm_is[i2]+ (i64.min i (l2-1))]
						)
					let newvals = map3
						(\del_cost ins_cost sub_cost ->
							i64.min sub_cost (i64.min del_cost ins_cost)
						) del_costs ins_costs sub_costs
					-- TODO need to adjust indices for strlen (...)
					in scatter (copy inner_vs1)
						(segm_is |> map (\ind -> ind+i+1))
						newvals
			-- TODO cont (...)
			let next_j = if all (>thresh) new_vs1 then (i64.maximum lens1) else j+1
			in (new_vs1, l_vs0, next_j)
		in final_vs0 |> (last >-> (<= thresh))
