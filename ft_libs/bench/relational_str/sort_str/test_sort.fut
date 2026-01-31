import "../../../ft_StrUtil"
import "../../../joins/ftSMJ_str"

-- Sorting for strings.
-- ==
-- entry: sort_strs
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

entry sort_strs (str_con : []u8) (str_idx : []i64)
= do_sort_str (arith_char_cmp) str_con str_idx (indices str_idx) (i64.highest)