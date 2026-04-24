import "../ftbasics"
import "strUtil"
import "../joins/ftSMJ"

-- | SMJ match-finding phase (for strings).
-- strsInfo's fields are passed separately to avoid size coercion issues.
-- Returns joinTup i64, where vs == ix.
def smj_matchfinding_str [n1] [n2]
	(char_cmp : u8 -> u8 -> i8)
	(merge_path_diagonals : i64)
	(strs1_content : []u8)
	(strs1_idxs : [n1]i64)
	(strs2_content : []u8)
	(strs2_idxs : [n2]i64)
: joinTup [n1] i64 =
	let strs1 = {contents = strs1_content, idxs = strs1_idxs}
	let strs2 = {contents = strs2_content, idxs = strs2_idxs}
	let this_cmp : i64 -> i64 -> i8 =
		str_cmp char_cmp strs1 strs2
	let s_eq  i1 i2 = (this_cmp i1 i2) == 0
	let s_geq i1 i2 = (this_cmp i1 i2) >= 0
	let s_leq i1 i2 = (this_cmp i1 i2) <= 0
	let s_gt  i1 i2 = (this_cmp i1 i2) >  0
	let s_lt  i1 i2 = (this_cmp i1 i2) <  0
	in smj_matchFinding s_eq s_geq s_leq s_gt s_lt merge_path_diagonals
		(indices strs1.idxs) (indices strs2.idxs)

-- | SMJ expansion phase (for strings, Inner Join).
-- Requires the left-side strInfo to be passed again.
def smj_expand_str [n] (strs1 : strInfo) (matches : joinTup [n] i64) : joinPairs_str =
	let res = matches |> smj_expand
	let res_strs = res.vs |> str_gather strs1
	in {strs = res_strs, ix = res.ix, iy = res.iy}

-- | SMJ full Merge-Join routine (for strings, Inner Join).
def do_InnerSMJ_str
	(char_cmp : u8 -> u8 -> i8)
	(merge_path_diagonals : i64)
	(strs1 : strInfo)
	(strs2 : strInfo)
: joinPairs_str =
	smj_matchfinding_str char_cmp
		merge_path_diagonals
		strs1.contents strs1.idxs
		strs2.contents strs2.idxs
	|> smj_expand_str strs1