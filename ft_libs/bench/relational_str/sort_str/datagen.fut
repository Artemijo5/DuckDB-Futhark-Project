-- Create random strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_str_10 mk_str_35 mk_str_100 mk_str_det10 mk_str_det35 mk_str_det100
-- random input { [33554432]f32 [33554432]u8 }
-- auto output

def mk_str_avgLen [n] (avgLen : i64) (p_isBreak : [n]f32) (str_con : [n]u8)
: ([n]u8, []i64) =
	let prob_break = (f32.i64 n) / (f32.i64 avgLen)
	let str_idx = p_isBreak
		|> map (\p -> p <= prob_break)
		|> zip (iota n)
		|> filter (\(i,isBreak) -> isBreak || i==0)
		|> map (.0)
	in (str_con, str_idx)

def mk_str_detLen [n] (detLen : i64) (_ : [n]f32) (str_con : [n]u8)
: ([n]u8, []i64) =
	let str_idx = (0..detLen..<n)
	in (str_con, str_idx)

entry mk_str_10 p_isBreak str_con = mk_str_avgLen 10 p_isBreak str_con
entry mk_str_35 p_isBreak str_con = mk_str_avgLen 35 p_isBreak str_con
entry mk_str_100 p_isBreak str_con = mk_str_avgLen 100 p_isBreak str_con

entry mk_str_det10 p_isBreak str_con = mk_str_detLen 10 p_isBreak str_con
entry mk_str_det35 p_isBreak str_con = mk_str_detLen 35 p_isBreak str_con
entry mk_str_det100 p_isBreak str_con = mk_str_detLen 100 p_isBreak str_con