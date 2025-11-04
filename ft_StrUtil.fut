import "ftbasics"

def arith_char_cmp (c1: u8) (c2: u8)
: i32 =
	if c1<c2 then (-1)
	else if c1>c2 then 1
	else 0

def case_insensitive_char_cmp
	(char_cmp : u8 -> u8 -> i32)
: (u8 -> u8 -> i32) =
	(\c1 c2 ->
		let c1_ = if (97<=c1 && 122>=c1) then (c1-32) else c1
		let c2_ = if (97<=c2 && 122>=c2) then (c2-32) else c2
		in char_cmp c1_ c2_
	)

def get_kth_char [n] [total_len]
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
	(i : idx_t.t)
	(k : idx_t.t)
: u8 =
	let inf = str_idx[i]
	let sup = if i==n-1 then total_len else str_idx[i+1]
	in
		if k < (sup-inf)
		then str_content[inf+k]
		else 0

def get_str_len [n]
	(total_len : idx_t.t)
	(str_idx : [n]idx_t.t)
	(i : idx_t.t)
: idx_t.t =
	let inf = str_idx[i]
	let sup = if i==n-1 then total_len else str_idx[i+1]
	in (sup-inf)

def str_cmp_across_contents [n1] [n2] [total_len1] [total_len2]
	(str_content1 : [total_len1]u8)
	(str_idx1 : [n1]idx_t.t)
	(str_content2 : [total_len2]u8)
	(str_idx2 : [n2]idx_t.t)
	(i1 : idx_t.t)
	(i2 : idx_t.t)
	(char_cmp : u8 -> u8 -> i32)
: idx_t.t =
	let (inf1, inf2) = (str_idx1[i1], str_idx2[i2])
	let (l1, l2) = (get_str_len total_len1 str_idx1 i1, get_str_len total_len2 str_idx2 i2)
	let loop_over =
		loop (def_lt, def_gt, b) = (false, false, 0)
		while (!def_lt && !def_gt && b<(i64.min l1 l2)) do
			let cmp = char_cmp str_content1[inf1+b] str_content2[inf2+b]
			let dlt = cmp<0
			let dgt = cmp>0
			in (dlt, dgt, b+1)
	in
		if loop_over.0 then (-1)
		else if loop_over.1 then 1
		else (l1-l2)

def str_cmp_in_content [n] [total_len]
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
	(i1 : idx_t.t)
	(i2 : idx_t.t)
	(char_cmp : u8 -> u8 -> i32)
: idx_t.t =
	str_cmp_across_contents str_content str_idx str_content str_idx i1 i2 char_cmp

def do_gather_str [n] [ni] [total_len]
	(psize : idx_t.t)
	(gather_is : [ni]idx_t.t)
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
: ([]u8, [ni]idx_t.t) =
	let gather_lens = 
		let all_lens = (iota n)
			|> map (\i -> get_str_len total_len str_idx i)
		in partitioned_gather i64.num_bits psize 0 all_lens gather_is
	let zuowei = exscan (+) 0 gather_lens
	let output_size = idx_t.sum gather_lens
	let max_len = idx_t.maximum gather_lens
	let new_con =
		loop buff : [output_size]u8 = (replicate output_size 0)
		for j in (iota max_len) do
			let scatter_idxs = (iota ni)
				|> map (\i ->
					let len = gather_lens[i]
					in if len<=j then (-1) else (j+zuowei[i])
				)
			let scatter_chars =
				let raw_chars = (iota n)
					|> map (\i -> get_kth_char str_content str_idx i j)
				in partitioned_gather u8.num_bits psize 0 raw_chars gather_is
			in scatter (copy buff) scatter_idxs scatter_chars
	in (new_con, zuowei)

type~ strInfo = {str_content : []u8, str_idx : []idx_t.t}
type~ sortInfo_str = {str_info : strInfo, is : []idx_t.t}
type~ sortStruct_str [b] = {str_info : strInfo, pL : [][b]u8}
type~ joinPairs_str = {str_info : strInfo, ix : []idx_t.t, iy : []idx_t.t}

def gather_str [ni]
	(psize : idx_t.t)
	(gather_is : [ni]idx_t.t)
	(str_info : strInfo)
: strInfo =
	let (g_con, g_idx) = do_gather_str psize gather_is str_info.str_content str_info.str_idx
	in {str_content = g_con, str_idx = g_idx}