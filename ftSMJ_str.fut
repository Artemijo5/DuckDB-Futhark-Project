import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"
import "ft_StrUtil"
import "ftSMJ"

-- TODO entry points
-- probably make a content+idx record type

def do_sort_str [n] [total_len] 't
	(char_cmp : u8 -> u8 -> i32)
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
	(ys : [n]t)
	(psize : idx_t.t)
: ([total_len]u8, [n]idx_t.t, [n]t) =
	let str_leq : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_in_content str_content str_idx i1 i2 char_cmp)<=0)
	let (sorted_idxs, sorted_ys) = (merge_sort (\(i1, _) (i2, _) -> str_leq i1 i2) (zip (iota n) ys))
		|> unzip
	let (sorted_str_con, sorted_str_idx) =
		let sorted_str_lens = 
			let lens = (iota n)
				|> map (\i -> get_str_len total_len str_idx i)
			in partitioned_gather psize 0 lens sorted_idxs
		let max_len = idx_t.maximum sorted_str_lens
		let zuowei = exscan (+) 0 sorted_str_lens
		let new_con =
			loop buff : [total_len]u8 = (replicate total_len 0)
			for j in (iota max_len) do
				let scatter_idxs = (iota n)
					|> map (\i ->
						let len = sorted_str_lens[i]
						in if len<=j then (-1) else (j+zuowei[i])
					)
				let scatter_chars =
					let unsorted_chars = (iota n)
						|> map (\i -> get_kth_char str_content str_idx i j)
					in partitioned_gather psize 0 unsorted_chars sorted_idxs
				in scatter (copy buff) scatter_idxs scatter_chars
		in (new_con, zuowei)
	in (sorted_str_con, sorted_str_idx, sorted_ys)

-- after this is called, still need to gather strings from r using vs
def do_smj_str [n1] [n2] [total_len1] [total_len2] 't
	(char_cmp : u8 -> u8 -> i32)
	(str_content1 : [total_len1]u8)
	(str_idx1 : [n1]idx_t.t)
	(str_content2 : [total_len2]u8)
	(str_idx2 : [n2]idx_t.t)
	(offset_R: idx_t.t)
	(offset_S: idx_t.t)
	(partitionSize: idx_t.t)
	(scatter_psize: idx_t.t)
: joinPairs_long = -- {vs = ix: r index, iy: s index}
	let str_eq : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> 
			if (get_str_len total_len1 str_idx1 i1) != (get_str_len total_len2 str_idx2 i2)
			then false
			else (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)==0
		)
	let str_gt : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)>0)
	let str_lt : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)<0)
	in
		inner_SMJ (-1) (iota n1) (iota n2) offset_R offset_S partitionSize scatter_psize (str_eq) (str_gt) (str_lt)

-- TODO entry points for sort...

def smj_str
	(char_cmp : u8 -> u8 -> i32)
	(str_info1 : strInfo)
	(str_info2 : strInfo)
	(offset_R : idx_t.t)
	(offset_S : idx_t.t)
	(partitionSize: idx_t.t)
	(scatter_psize: idx_t.t)
: joinPairs_str =
	let jp = do_smj_str
		char_cmp
		str_info1.str_content str_info1.str_idx
		str_info2.str_content str_info2.str_idx
		offset_R
		offset_S
		partitionSize
		scatter_psize
	let jp_str = gather_str scatter_psize jp.ix str_info1
	in {str_info = jp_str, ix = jp.ix, iy = jp.iy}
