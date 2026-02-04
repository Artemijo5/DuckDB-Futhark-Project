import "../../../ftbasics"

-- Create random strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_strs
-- input @data/strDat.in
-- auto output

entry mk_strs [n] (lens: [n]i64) (con: []u8)
: ([]u8, [n]i64) =
	let str_idxs = exscan (+) 0 lens
	let total_char_len = str_idxs[n-1] + lens[n-1]
	let str_content = con[0:total_char_len]
	in (str_content, str_idxs)