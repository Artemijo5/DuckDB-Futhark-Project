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

	-- Levenschtein distance between 2 strings.
	-- Code based on Wikipedia.
	--
	-- TODO fix (...)
	def lev_dists [n]
		(char_cmp : u8 -> u8 -> i8)
		(strs1 : strInfo)
		(strs2 : strInfo)
		(is1 : [n]i64)
		(is2 : [n]i64)
	: [n]i64 =
		-- gathered strs
		let g_strs1 = str_gather strs1 is1
		let g_strs2 = str_gather strs2 is2
		let simple_lens = length g_strs2.contents
		let total_lens = simple_lens + n
		-- string lengths
		let lens1 = is1 |> map (\i1 -> get_str_len i1 strs1)
		let lens2 = is2 |> map (\i2 -> get_str_len i2 strs2)
		let offs = lens2
			|> map (\i -> i+1)
			|> exscan (+) 0
		let flags = scatter (replicate total_lens false) offs (replicate n true)
		-- working rows
		let vs0 = lens2
			|> expand (\i -> i+1)
				(\_ ind -> ind)
			|> sized total_lens
		let vs1 = flags |> map (i64.bool)
		-- loop
		let (final_vs0,_) = loop (l_vs0, l_vs1) = (vs0,vs1)
		for j<1+(i64.maximum lens1) do
			-- deletion cost
			let delCost = l_vs0
				|> rotate 1
				|> zip (flags |> rotate 1)
				|> filter ((.0) >-> not)
				|> map (\(_,dc) -> dc+1)
				|> sized simple_lens
			-- insertion cost
			let insCost = l_vs1
				|> zip (flags |> rotate 1)
				|> filter ((.0) >-> not)
				|> map (\(_,dc) -> dc+1)
				|> sized simple_lens
			-- substitution cost
			let subCost = iota n
				|> expand (\i -> lens2[i])
					(\i ind -> 
						let leftSide = get_kth_char j i g_strs1
						let rightSide = get_kth_char ind i g_strs2
						let ineq = char_cmp leftSide rightSide
							|> bool.i8
						in ineq
					)
				|> sized simple_lens
				|> zip (
					l_vs0
						|> zip (flags |> rotate 1)
						|> filter ((.0) >-> not)
						|> map (.1)
						|> sized simple_lens
					)
				|> map (\(v0, ineq) -> if ineq then v0+1 else v0)
			let new_v1s = map3 (\dc ic sc -> i64.min dc (i64.min ic sc))
					delCost insCost subCost
			let new_v1s' = flags |> rotate 1 |> map ((not) >-> i64.bool)
				|> exscan (+) 0
				|> zip (flags |> rotate 1 |> map (not))
				|> trace
				|> map (\(f,i) -> if f then new_v1s[i] else 0)
			in (new_v1s', l_vs0)
		in final_vs0 |> zip (flags |> rotate 1)
			|> filter (.0) |> map (.1) |> sized n

				


		