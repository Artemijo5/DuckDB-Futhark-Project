import "../../../ft_StrUtil"
import "../../../joins/ftHashJoin_str"

-- Hash-sorting for strings.
-- ==
-- entry: hsort_strs_noLen hsort_strs_withLen
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

entry hsort_strs_noLen (str_con : []u8) (str_idx : []i64) =
	let str_info : strInfo = {str_content = str_con, str_idx = str_idx}
	let hashed_strs = get_hashed_strs 0 4 1 1 5 true 2 str_info
	let hsorted = do_sort_hashed_strs_GFUR hashed_strs
	in do_gather_str (i64.highest) hsorted.is str_con str_idx

entry hsort_strs_withLen (str_con : []u8) (str_idx : []i64) =
	let str_info : strInfo = {str_content = str_con, str_idx = str_idx}
	let hashed_strs = get_hashed_strs 1 3 1 2 5 true 2 str_info
	let hsorted = do_sort_hashed_strs_GFUR hashed_strs
	in do_gather_str (i64.highest) hsorted.is str_con str_idx