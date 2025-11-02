import "ftbasics"
import "ft_StrUtil"
import "ftHashJoin"

let str_hashFunc [total_len] [n]
	(len_bytes : idx_t.t)
	(content_bytes : idx_t.t)
	(len_range_per_bucket : idx_t.t)
	(merge_chars : idx_t.t) -- maybe a good idea for 2? probably not for more though...
	(str_content:[total_len]u8)
	(str_idx:[n]idx_t.t)
	(i:idx_t.t)
:
	byteSeq [content_bytes+len_bytes]
=
	let i_len = (get_str_len total_len str_idx i)/len_range_per_bucket
	let len_byteSeq : [len_bytes]u8 = (iota len_bytes)
		|> map (\j_ -> 
			let j = len_bytes-j_-1
			in u8.i64 (((i64.>>>) i_len (j*(i64.i32 u8.num_bits))) & (i64.u8 u8.highest))
		)
	let con_byteSeq : [content_bytes]u8 = (iota content_bytes)
		|> map (\j_->
			let j = content_bytes-j_-1
			in loop ch=0 for k_ in (iota merge_chars) do
			-- TODO this just returns 0 for everything...
				let k = merge_chars-k_-1
				let to_shift_right : i32 =
					let denom = i32.i64 merge_chars
					in (u8.num_bits + denom - 1)/denom
				let filler = ( (u8.>>>) (get_kth_char str_content str_idx i ((i_len/2) + k)) (u8.i32 to_shift_right) )
					<< (u8.i32 ((i32.i64 k)*to_shift_right))
				in ch | filler
		)
	in (con_byteSeq ++ len_byteSeq) :> [content_bytes+len_bytes]u8

-- TODO make funcs and entry points
-- 1. create desired hash func using str_hashFunc
-- 2. apply hash func
-- 3. run Radix Hash Join on hashed str's (idea: hash table built on the len bytes ++... eg 2 radix bytes, 1 or 2 len bytes)
-- 4. gather str contents
-- 5. filter for str equality
