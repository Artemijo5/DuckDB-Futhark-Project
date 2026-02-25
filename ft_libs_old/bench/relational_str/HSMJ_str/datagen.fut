import "../../../ftbasics"
import "../../../ft_StrUtil"
import "../../../joins/ftHashJoin_str"

-- Create sorted strings of different avg lengths.
-- Also differs on whether deterministic or not.
--
-- ==
-- entry: mk_str_nLen mk_str_yLen
-- input @data/strDat.in
-- auto output

def mk_str [n] (useLen : bool) (lens: [n]i64) (con: []u8) =
	let lenBytes = if useLen then 1 else 0
	let str_idx1 = exscan (+) 0 lens
	let total_char_len = str_idx1[n-1] + lens[n-1]
	let str_content1 = con[0:total_char_len]
	let str_info1 : strInfo = {str_content = str_content1, str_idx = str_idx1}
	let hashed_strs1 = get_hashed_strs lenBytes (4-lenBytes) 1 1 5 true 2 str_info1
	let hsorted1 = do_sort_hashed_strs_GFUR hashed_strs1
	let (sxs1, is1) = (hsorted1.ks, hsorted1.is)
	let (str_content2, str_idx2)
		= do_gather_str
			i64.highest
			(iota n |> map (\i -> replicate 2 i) |> flatten)
			str_content1
			str_idx1
	let sxs2 = sxs1 |> map (\sx -> replicate 2 sx) |> flatten
	let is2 = is1 |> map (\i -> [2*i, 2*i+1]) |> flatten
	in (str_content1, str_idx1, str_content2, str_idx2, sxs1, is1, sxs2, is2)
	
entry mk_str_nLen lens con = mk_str false lens con
entry mk_str_yLen lens con = mk_str true lens con