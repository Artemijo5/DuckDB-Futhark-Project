import "../../../ftbasics"
import "../../../ft_StrUtil"
import "../../../joins/ftSMJ_str"

-- Create sorted strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_strs
-- input @data/strDat.in
-- auto output

entry mk_strs [n] (lens: [n]i64) (con: []u8)
: ([]u8, [n]i64, [n]i64, []u8, [n*2]i64, [n*2]i64) =
	let str_idx1 = exscan (+) 0 lens
	let total_char_len = str_idx1[n-1] + lens[n-1]
	let str_content1 = con[0:total_char_len]
	let (sorted_content1, sorted_idx1, is1) = do_sort_str (arith_char_cmp) str_content1 str_idx1 (iota n) i64.highest
	let (sorted_content2, sorted_idx2)
		= do_gather_str
			i64.highest
			(iota n |> map (\i -> replicate 2 i) |> flatten)
			sorted_content1
			sorted_idx1
	let is2 = is1 |> map (\i -> [i, i+n]) |> flatten
	in (sorted_content1, sorted_idx1, is1, sorted_content2, sorted_idx2, is2)