import "ftbasics"
import "ft_StrUtil"
import "ftHashJoin"

type~ str_hash_info [b] = {hash_vals : [](byteSeq [b]), str_idx : []idx_t.t}
type~ partitionedSet_str_GFTR [b] [pL_b] = {ks: str_hash_info [b], pL: [](byteSeq [pL_b])}

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
				let this_char = if merge_chars>1 && case_sens && this_char_>=97 && this_char<=122
					then (if this_char_ & 1 == 0 then this_char_-1 else this_char_+1)
					else this_char_
				let filler = (this_char & bitmask) << ( u8.i32 ( (i32.i64 k) * (u8.num_bits - (u8.num_bits / mc)) ) )
				in ch + filler
		)
	in (con_byteSeq ++ len_byteSeq) :> [content_bytes+len_bytes]u8

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
	let (is, hvs) = (indices str_info.str_idx)
		|> map (\j ->
			let hv = str_hashFunc
				len_bytes content_bytes
				len_range_per_bucket merge_chars
				from_subdiv num_subdiv case_sens
				str_info.str_content str_info.str_idx
				j
			in (j, hv)
		)
		|> unzip
	in {hash_vals = hvs, str_idx = is}

-- following used for GFTR only...
def partition_and_deepen_str [b] 't
  (block_size)
  (gather_psize)
  (radix_size)
  (xs : str_hash_info [b])
  (pL : []t)
  (size_thresh)
  (max_depth)
  (bit_step)
: (str_hash_info [b], []t) =
	let pL_ = indices pL
		|> map (\i -> (xs.str_idx[i], pL[i]))
	let pr = partition_and_deepen
		block_size gather_psize radix_size
		xs.hash_vals
		pL_
		size_thresh max_depth bit_step
	let (is, ps) = unzip pr.1
	in ({hash_vals = pr.0, str_idx = is}, ps)

def str_hash_join [b]
	(radix_size : i32)
	(tR : str_hash_info [b])
	(tS : str_hash_info [b])
	(pS : partitionInfo)
	(ht_S : radix_hashTable [i64.i32 radix_size])
	(char_cmp : u8 -> u8 -> i32)
	(str_info_R : strInfo)
	(str_info_S : strInfo)
: joinPairs_str =
	let ignore_psize = idx_t.max (length str_info_R.str_content) (length str_info_S.str_content)
	let jp = radix_hash_join radix_size tR.hash_vals tS.hash_vals pS ht_S
	let idx_R = gather (-1) tR.str_idx jp.ix
	let idx_S = gather (-1) tS.str_idx jp.iy
	let str_R = gather_str ignore_psize idx_R str_info_R
	let str_S = gather_str ignore_psize idx_S str_info_S
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
		|> filter (\i ->
			str_eq i i
		)
		|> map (\i ->
			(i, jp.ix[i], jp.iy[i])
		)
		|> unzip3
	let filt_str = gather_str ignore_psize filt_idx str_R
	in {str_info = filt_str, ix = filt_ix, iy = filt_iy}

-- TODO make funcs and entry points
-- 1. create desired hash func using str_hashFunc
-- 2. apply hash func
-- 3. run Radix Hash Join on hashed str's (idea: hash table built on the len bytes ++... eg 2 radix bytes, 1 or 2 len bytes)
-- 4. gather str contents
-- 5. filter for str equality
