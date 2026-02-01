import "../../../ft_StrUtil"
import "../../../joins/ftSMJ_str"

-- Create sorted strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_str_10 mk_str_35 mk_str_100 mk_str_det10 mk_str_det35 mk_str_det100
-- random input { [32000]f32 [32000]u8 [32000]f32 [32000]u8 }
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
	let (sorted_c1, sorted_i1, ix) = do_sort_str (arith_char_cmp) str_con1 str_idx1 (indices str_idx1) (i64.highest)
	let (sorted_c2, sorted_i2, iy) = do_sort_str (arith_char_cmp) str_con2 str_idx2 (indices str_idx2) (i64.highest)
	in (sorted_c1, sorted_i1, ix, sorted_c2, sorted_i2, iy)

def mk_str_detLen [n] (detLen : i64) (_ : [n]f32) (str_con1 : [n]u8) (_ : [n]f32) (str_con2 : [n]u8)
=
	let str_idx = (0..detLen..<n)
	let (sorted_c1, sorted_i1, ix) = do_sort_str (arith_char_cmp) str_con1 str_idx (indices str_idx) (i64.highest)
	let (sorted_c2, sorted_i2, iy) = do_sort_str (arith_char_cmp) str_con2 str_idx (indices str_idx) (i64.highest)
	in (sorted_c1, sorted_i1, ix, sorted_c2, sorted_i2, iy)

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