import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"
import "ftSMJ"

-- TODO entry points
-- probably make a content+idx record type

let arith_char_cmp (c1: u8) (c2: u8)
: i32 =
	if c1<c2 then (-1)
	else if c1>c2 then 1
	else 0

let get_kth_char [n] [total_len]
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

let get_str_len [n]
	(total_len : idx_t.t)
	(str_idx : [n]idx_t.t)
	(i : idx_t.t)
: idx_t.t =
	let inf = str_idx[i]
	let sup = if i==n-1 then total_len else str_idx[i+1]
	in (sup-inf)

let str_cmp_across_contents [n1] [n2] [total_len1] [total_len2]
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

let str_cmp_in_content [n] [total_len]
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
	(i1 : idx_t.t)
	(i2 : idx_t.t)
	(char_cmp : u8 -> u8 -> i32)
: idx_t.t =
	str_cmp_across_contents str_content str_idx str_content str_idx i1 i2 char_cmp

let gather_str [n] [ni] [total_len] 't
	(psize : idx_t.t)
	(gather_is : [ni]idx_t.t)
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
: ([]u8, [ni]idx_t.t) =
	let gather_lens = 
		let all_lens = (iota n)
			|> map (\i -> get_str_len total_len str_idx i)
		in partitioned_gather psize 0 all_lens gather_is
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
				in partitioned_gather psize 0 raw_chars gather_is
			in scatter (copy buff) scatter_idxs scatter_chars
	in (new_con, zuowei)

let sort_str [n] [total_len] 't
	(char_cmp : u8 -> u8 -> i32)
	(str_content : [total_len]u8)
	(str_idx : [n]idx_t.t)
	(ys : [n]t)
	(dummy_pL : t)
	(psize : idx_t.t)
: ([total_len]u8, [n]idx_t.t, [n]t) =
	let str_leq : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_in_content str_content str_idx i1 i2 char_cmp)<=0)
	let sorted_idxs = merge_sort (str_leq) (iota n)
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
	let gathered_ys = partitioned_gather psize dummy_pL ys sorted_idxs
	in (sorted_str_con, sorted_str_idx, gathered_ys)	

-- after this is called, still need to gather strings from r using vs
let smj_str [n1] [n2] [total_len1] [total_len2] 't
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
		= (\i1 i2 -> (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)==0)
	let str_gt : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)>0)
	let str_lt : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> (str_cmp_across_contents str_content1 str_idx1 str_content2 str_idx2 i1 i2 char_cmp)<0)
	in
		inner_SMJ (-1) (iota n1) (iota n2) offset_R offset_S partitionSize scatter_psize (str_eq) (str_gt) (str_lt)