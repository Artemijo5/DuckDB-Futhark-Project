import "ftbasics"
import "ftHashJoin"

-- Uses the same Aggregator defined in SortGroupBy
-- only key-finding differs

-- TODO use loop inversion where necessary

local def do_find_key_counts [n] [key_no] [part_no] [b]
	(radix_size : i32)
	(k_ids : [key_no](byteSeq [b]))
	(part_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]i32)
	(part_rht : radix_hashTable [i64.i32 radix_size])
: [key_no](idx_t.t,idx_t.t) = -- returns partition index & size of each group
	k_ids
		|> map (\k -> 
			let part = rv_partitionMatchBounds radix_size k part_ks part_bounds part_depths part_rht
			in if part<0 then (-1,0) else
			let inf_ks = part_bounds[part]
			let sup_ks = if part==part_no-1 then n else part_bounds[part+1]
			let (count,_) =  rv_findPairCount k part_ks inf_ks sup_ks
			in (part, count)
		)

def partgroup_find_known_key_counts [n] [key_no] [part_no] [b]
	(radix_size : i32)
	(k_ids : [key_no](byteSeq [b]))
	(part_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]i32)
	(part_rht : radix_hashTable [i64.i32 radix_size])
: ([key_no]idx_t.t, [key_no]idx_t.t) = -- returns partition index & size of each group
	(do_find_key_counts radix_size k_ids part_ks part_bounds part_depths part_rht)
		|> unzip

-- NOTE 1
-- this only works if b is at most 8 bytes (so can be represented by long)
def partgroup_find_unknown_key_counts [n] [part_no] [b]
	(radix_size : i32)
	(part_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]i32)
	(part_rht : radix_hashTable [i64.i32 radix_size])
: ([]idx_t.t, []idx_t.t) = -- returns partition index & size of each group
	let nb = i32.i64 b
	-- tuning in case there are both positive and negative keys
	-- otherwise negatives would be represented as large positive values
	let as_long =
		let as_long_ = part_ks |> map (radix_to_idx (nb*u8.num_bits))
		let min_key = idx_t.minimum as_long_
		in as_long_ |> map (\v -> v - min_key)
	let max_key = idx_t.maximum as_long
	let key_range = (iota max_key)
		|> map (\lk ->
			loop bk = dummy_byteSeq b
			for j in (iota b) do
				bk with [b-j-1] = u8.i64 (((i64.>>>) lk (j*(i64.i32 (u8.num_bits)))) & (i64.u8 u8.highest))
		)
	in do_find_key_counts radix_size key_range part_ks part_bounds part_depths part_rht
		|> filter (\(p, c) -> p>=0 && c>0)
		|> unzip

--def partgroup_partitionByIndex [n] [group_no] [b]
--	(group_ks: [group_no](byteSeq [b]))
--	(ks : [n](byteSeq [b]))
-- : [n]idx_t.t =
--	ks |> map (\k ->
--		rv_find_match_if_exists k group_ks 0 n
--	)
def partgroup_partitionByIndex [n] [group_no] [b]
	(group_ks : [group_no](byteSeq [b]))
	(ks : [n](byteSeq [b]))
: [n]idx_t.t =
	loop per_k = (replicate n (-1)) for j in (iota group_ks) do
		let this_gid = group_ks[j]
		in map2 (\p i ->
				if p>=0 then p
				else if (foldl (&&) true (map2 (==) ks[i] this_gid)) then j
				else p
			)
			per_k
			(iota n)

-- are the following needed anywhere?
-- probably if I make a specialised Aggregator module (...)

def partitionSizes [part_no]
	(len : idx_t.t)
	(part_bounds : [part_no]idx_t.t)
: [part_no]idx_t.t =
	(iota part_no) |> map (\j ->
		let cur = part_bounds[j]
		let nex = if j==part_no-1 then len else part_bounds[j+1]
		in nex-cur
	)

def keys_per_partition [n] [key_no] [part_no] [b]
	(radix_size : i32)
	(k_ids : [key_no](byteSeq [b]))
	(sorted_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]i32)
: [part_no]idx_t.t =
	(iota part_no) |> map (\j ->
		countFor (\ki ->
			radix_eq radix_size part_depths[j] sorted_ks[part_bounds[j]] ki
		)
		k_ids
	)