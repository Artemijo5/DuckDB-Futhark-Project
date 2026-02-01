import "../../../ftbasics"
import "../../../ft_StrUtil"
import "../../../joins/ftSMJ_str"

-- SMJ for strings.
-- ==
-- entry: smj_strs
-- input @data/avg10.in
-- auto output
-- input @data/avg35.in
-- auto output
-- input @data/avg100.in
-- auto output
-- input @data/det10.in
-- auto output
-- input @data/det35.in
-- auto output
-- input @data/det100.in
-- auto output

entry smj_strs
	(str_con1 : []u8) (str_idx1 : []i64) (is1 : []i64)
	(str_con2 : []u8) (str_idx2 : []i64) (is2 : []i64)
=
	let info1 = {str_content = str_con1, str_idx = str_idx1}
	let info2 = {str_content = str_con2, str_idx = str_idx2}
	let join = smj_str (arith_char_cmp) info1 info2 0 0 (i64.highest) (i64.highest)
	let gis1 = gather (-1) is1 join.ix
	let gis2 = gather (-1) is2 join.iy
	in (join.str_info.str_content, join.str_info.str_idx, gis1, gis2)