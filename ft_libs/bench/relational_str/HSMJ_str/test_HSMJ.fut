import "../../../ftbasics"
import "../../../ft_StrUtil"
import "../../../joins/ftHashJoin_str"

-- SMJ for strings.
-- ==
-- entry: hsmj_strs
-- input @data/strDat_nl.in
-- input @data/strDat_yl.in

entry hsmj_strs [b]
	(str_con1 : []u8) (str_idx1 : []i64)
	(str_con2 : []u8) (str_idx2 : []i64)
	(sxs1 : [][b]u8) (is1 : []i64)
	(sxs2 : [][b]u8) (is2 : []i64)
=
	let hash1 : sortedHash_str_GFUR [b] = {ks = sxs1, is = is1}
	let hash2 : sortedHash_str_GFUR [b] = {ks = sxs2, is = is2}
	let str_info1 : strInfo = {str_content = str_con1, str_idx = str_idx1}
	let str_info2 : strInfo = {str_content = str_con2, str_idx = str_idx2}
	let join = do_hashed_str_SMJ
		(arith_char_cmp)
		hash1
		hash2
		(i64.highest)
		(i64.highest)
		str_info1
		str_info2
	let gis1 = gather (-1) is1 join.ix
	let gis2 = gather (-1) is2 join.iy
	in (join.str_info.str_content, join.str_info.str_idx, gis1, gis2)