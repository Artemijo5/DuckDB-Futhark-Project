import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"
import "ft_StrUtil"
import "ftSMJ"

type~ str_hash_info [b] = [](byteSeq [b])
type~ sortedHash_str_GFUR [b] = {ks: str_hash_info [b], is : []i64}
-- type~ partitionedSet_str_GFTR [b] [pL_b] = {ks: str_hash_info [b], is : []i64, pL: [](byteSeq [pL_b])}

local def hash_leq [b] (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_lt = false, def_gt = false, byte = 0}
    while (!p.def_lt && !p.def_gt && p.byte<b) do
      let dlt = x1[p.byte] < x2[p.byte]
      let dgt = x1[p.byte] > x2[p.byte]
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in (loop_over.def_lt || (!loop_over.def_gt))

local def hash_eq [b] (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
   foldl (&&) (true) (map2 (==) x1 x2)

local def hash_lt [b] (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_lt = false, byte = 0}
    while (!p.def_lt && p.byte<b) do
      let dlt = x1[p.byte] < x2[p.byte]
      let dgt = x1[p.byte] > x2[p.byte]
      in
      {
        def_lt = dlt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in loop_over.def_lt

local def hash_gt [b] (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_gt = false, byte = 0}
    while (!p.def_gt && p.byte<b) do
      let dlt = x1[p.byte] < x2[p.byte]
      let dgt = x1[p.byte] > x2[p.byte]
      in
      {
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in loop_over.def_gt


def str_hashFunc [total_len] [n]
	(len_bytes : idx_t.t)
	(content_bytes : idx_t.t)
	(len_range_per_bucket : idx_t.t)
	(merge_chars : idx_t.t) -- maybe a good idea for 2? probably not for more though...
	(from_subdiv : idx_t.t)
	(num_subdiv : idx_t.t)
	(case_sens : bool)
	(str_content:[total_len]u8)
	(str_idx:[n]idx_t.t)
	(i:idx_t.t)
:
	byteSeq [content_bytes+len_bytes]
=
	let i_len_ = (get_str_len total_len str_idx i)
	let i_len = i_len_ / len_range_per_bucket
	let len_byteSeq : [len_bytes]u8 = (iota len_bytes)
		|> map (\j_ -> 
			let j = len_bytes-j_-1
			in u8.i64 (((i64.>>>) i_len (j*(i64.i32 u8.num_bits))) & (i64.u8 u8.highest))
		)
	let mc = i32.i64 merge_chars
	let bitmask = u8.i32 ((i32.>>>) (i32.u8 u8.highest) (u8.num_bits - (u8.num_bits / mc)))
	let con_byteSeq : [content_bytes]u8 = (iota content_bytes)
		|> map (\j_->
			let j = content_bytes-j_-1
			in loop ch=0 for k_ in (iota merge_chars) do
				let k = merge_chars-k_-1
				let this_char_ = get_kth_char str_content str_idx i ((from_subdiv*i_len_/num_subdiv) + j*merge_chars + k)
				let this_char = if merge_chars>1 && case_sens && this_char_>=97 && this_char_<=122
					then (if this_char_ & 1 == 0 then this_char_-1 else this_char_+1)
					else this_char_
				let filler = (this_char & bitmask) << ( u8.i32 ( (i32.i64 k) * (u8.num_bits - (u8.num_bits / mc)) ) )
				in ch + filler
		)
	in (len_byteSeq ++ con_byteSeq) :> [content_bytes+len_bytes]u8

def get_hashed_strs
	(len_bytes : idx_t.t)
	(content_bytes : idx_t.t)
	(len_range_per_bucket : idx_t.t)
	(from_subdiv : idx_t.t)
	(num_subdiv : idx_t.t)
	(case_sens : bool)
	(merge_chars : idx_t.t)
	(str_info : strInfo)
: str_hash_info [content_bytes + len_bytes] =
	(indices str_info.str_idx)
		|> map (\j ->
			let hv : [content_bytes + len_bytes]u8 = str_hashFunc
				len_bytes content_bytes
				len_range_per_bucket merge_chars
				from_subdiv num_subdiv case_sens
				str_info.str_content str_info.str_idx
				j
			in hv
		)

def test_get_hashed_strs =
	let strs = "The quick brown fox jumps over the fast lazy dog."
	let idxs = (indices strs) |> filter (\i -> i==0 || strs[i-1]==32)
	let sInf : strInfo = {str_content = strs, str_idx = idxs}
	in get_hashed_strs 1 1 1 0 1 true 2 sInf

def do_sort_hashed_strs [b] 't
	(xs : str_hash_info [b])
  (pL : []t)
: (str_hash_info [b], []t) =
	let pL_ = indices pL
	let sorted = merge_sort (\(x1,_) (x2,_) -> hash_leq x1 x2) (zip xs pL)
	in unzip sorted

def do_sort_hashed_strs_GFUR [b]
	(xs : str_hash_info [b])
: sortedHash_str_GFUR [b] =
	let (sxs, sis) = do_sort_hashed_strs xs (indices xs)
	in {ks = sxs, is = sis}


-- type~ joinPairs_str = {str_info : strInfo, ix : []idx_t.t, iy : []idx_t.t}
def do_hashed_str_SMJ [b]
	(char_cmp : u8 -> u8 -> i32)
	(tR : sortedHash_str_GFUR [b])
	(tS : sortedHash_str_GFUR [b])
	(partition_size : i64)
	(scatter_psize : i64)
	(str_info_R : strInfo)
	(str_info_S : strInfo)
: joinPairs_str =
	let jp = inner_SMJ (i32.i64 b) (replicate b 0) tR.ks tS.ks 0 0 partition_size scatter_psize (hash_eq) (hash_gt) (hash_lt)
	let idx_R = gather (-1) tR.is jp.ix
	let idx_S = gather (-1) tS.is jp.iy
	let ignore_psize = idx_t.max (length str_info_R.str_content) (length str_info_S.str_content)
	let str_R = gather_str scatter_psize idx_R str_info_R
	let str_S = gather_str scatter_psize idx_S str_info_S
	let str_eq : idx_t.t -> idx_t.t -> bool
		= (\i1 i2 -> 
			if
				(get_str_len (length str_R.str_content) str_R.str_idx i1)
				!=
				(get_str_len (length str_S.str_content) str_S.str_idx i2)
			then false
			else (
				str_cmp_across_contents
					str_R.str_content str_R.str_idx
					str_S.str_content str_S.str_idx
					i1 i2 char_cmp
				)==0
		)
	let (filt_idx, filt_ix, filt_iy) = (indices jp.vs)
		|> filter (\i -> str_eq i i)
		|> map (\i ->
			(i, jp.ix[i], jp.iy[i])
		)
		|> unzip3
	let filt_str = gather_str ignore_psize filt_idx str_R
	in {str_info = filt_str, ix = filt_ix, iy = filt_iy}

def test_smj (case_sens : bool) =
	let str1_ = "The quick brown fox jumps over the lazy dog"
	let str2_ = "I wonder if that dog was jumped over by the brown fox"
	let idx1_ = (indices str1_) |> filter (\i -> i==0 || str1_[i-1]==32)
	let idx2_ = (indices str2_) |> filter (\i -> i==0 || str2_[i-1]==32)
	let str1 = str1_ |> filter (\c -> c!=32)
	let str2 = str2_ |> filter (\c -> c!=32)
	let idx1 = idx1_ |> zip (indices idx1_) |> map (\(ii,i) -> i-ii)
	let idx2 = idx2_ |> zip (indices idx2_) |> map (\(ii,i) -> i-ii)
	let info1 : strInfo = {str_content = str1, str_idx = idx1}
	let info2 : strInfo = {str_content = str2, str_idx = idx2}
	let hs1 = get_hashed_strs 1 1 1 0 1 case_sens 2 info1
	let hs2 = get_hashed_strs 1 1 1 0 1 case_sens 2 info2
	let shs1 = do_sort_hashed_strs_GFUR hs1
	let shs2 = do_sort_hashed_strs_GFUR hs2
	let ccmp = (\c1 c2 -> if case_sens then (arith_char_cmp c1 c2) else ((case_insensitive_char_cmp arith_char_cmp) c1 c2))
	in do_hashed_str_SMJ ccmp shs1 shs2 256 256 info1 info2
