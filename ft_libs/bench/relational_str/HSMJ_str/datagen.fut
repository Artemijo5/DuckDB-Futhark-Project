import "../../../ft_StrUtil"
import "../../../joins/ftHashJoin_str"

-- Create sorted strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_str_10 mk_str_35 mk_str_100 mk_str_det10 mk_str_det35 mk_str_det100
-- random input { [33554432]f32 [33554432]u8 [33554432]f32 [33554432]u8 }
-- auto output

def mk_str_avgLen [n] (avgLen : i64)
	(p_isBreak1 : [n]f32) (str_con1 : [n]u8)
	(p_isBreak2 : [n]f32) (str_con2 : [n]u8)
=
	let prob_break = (f32.i64 n) / (f32.i64 avgLen)
	let str_idx1 = p_isBreak1
		|> map (\p -> p <= prob_break)
		|> zip (iota n)
		|> filter (\(i,isBreak) -> isBreak || i==0)
		|> map (.0)
	let str_idx2 = p_isBreak2
		|> map (\p -> p <= prob_break)
		|> zip (iota n)
		|> filter (\(i,isBreak) -> isBreak || i==0)
		|> map (.0)
	let str_info1 : strInfo = {str_content = str_con1, str_idx = str_idx1}
	let hashed_strs1 = get_hashed_strs 0 4 1 1 5 true 2 str_info1
	let hsorted1 = do_sort_hashed_strs_GFUR hashed_strs1
	let (sxs1, is1) = (hsorted1.ks, hsorted1.is)
	let str_info2 : strInfo = {str_content = str_con2, str_idx = str_idx2}
	let hashed_strs2 = get_hashed_strs 0 4 1 1 5 true 2 str_info2
	let hsorted2 = do_sort_hashed_strs_GFUR hashed_strs2
	let (sxs2, is2) = (hsorted2.ks, hsorted2.is)
	in (str_con1, str_idx1, str_con2, str_idx2, sxs1, is1, sxs2, is2)

def mk_str_detLen [n] (detLen : i64) (_ : [n]f32) (str_con1 : [n]u8) (_ : [n]f32) (str_con2 : [n]u8)
=
	let str_idx = (0..detLen..<n)
	let str_info1 : strInfo = {str_content = str_con1, str_idx = str_idx}
	let hashed_strs1 = get_hashed_strs 0 4 1 1 5 true 2 str_info1
	let hsorted1 = do_sort_hashed_strs_GFUR hashed_strs1
	let (sxs1, is1) = (hsorted1.ks, hsorted1.is)
	let str_info2 : strInfo = {str_content = str_con2, str_idx = str_idx}
	let hashed_strs2 = get_hashed_strs 0 4 1 1 5 true 2 str_info2
	let hsorted2 = do_sort_hashed_strs_GFUR hashed_strs2
	let (sxs2, is2) = (hsorted2.ks, hsorted2.is)
	in (str_con1, str_idx, str_con2, str_idx, sxs1, is1, sxs2, is2)

entry mk_str_10 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_avgLen 10 p_isBreak1 str_con1 p_isBreak2 str_con2
entry mk_str_35 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_avgLen 35 p_isBreak1 str_con1 p_isBreak2 str_con2
entry mk_str_100 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_avgLen 100 p_isBreak1 str_con1 p_isBreak2 str_con2

entry mk_str_det10 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_detLen 10 p_isBreak1 str_con1 p_isBreak2 str_con2
entry mk_str_det35 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_detLen 35 p_isBreak1 str_con1 p_isBreak2 str_con2
entry mk_str_det100 p_isBreak1 str_con1 p_isBreak2 str_con2
	= mk_str_detLen 100 p_isBreak1 str_con1 p_isBreak2 str_con2