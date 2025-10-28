import "ftbasics"
import "ftHashJoin"

-- Uses the same Aggregator defined in SortGroupBy
-- only key-finding differs

-- TODO test

-- Good idea to filter this afterwards
def partgroup_find_known_key_counts [n] [key_no] [part_no] [b]
	(radix_size : [part_no]idx_t.t)
	(k_ids : [key_no](byteSeq [b]))
	(sorted_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]idx_t.t)
	(part_rht : radix_hashTable [i64.i32 radix_size])
: ([key_no]idx_t.t, [key_no]idx_t.t) = -- returns partition index & size of each group
	k_ids	|> map (\k -> 
		let part = rv_partitionMatchBounds radix_size k sorted_ks part_bounds part_depths part_rht
		in if part<0 then (-1,0) else
		let inf_ks = part_bounds[part]
		let sup_ks = if part==part_no-1 then n else part_bounds[part+1]
		let cur_ks = sorted_ks[inf_ks:sup_ks]
		let count =  rv_findPairCount k cur_ks
		in (part, count)
	)

def partitionSizes [part_no]
	(len : idx_t.t)
	(part_bounds : [part_no]idx_t.t)
: [part_no]idx_t.t =
	(iota part_no) |> map (\j ->
		let cur = part_bounds[j]
		let nex = if j==part_no-1 then len else part_bounds[j+1]
		in nex-cur
	)

-- TODO (?)
def partgroup_find_unknown_key_ranges [n] [key_no] [part_no] [b]
	(radix_size : [part_no]idx_t.t)
	(k_ids : [key_no](byteSeq [b]))
	(sorted_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
: ([part_no](byteSeq [b]), [part_no](byteSeq [b])) = -- returns minimum & maximum index in each partition
	let ib = i32.i64 b
	let nbits = u8.num_bits
	let which_part = 
		loop buff = (replicate [n] (-1))
		for j in (iota part_no) do
			let inf = part_bounds[j]
			let sup = if j==part_no-1 then n else part_bounds[j+1]
			in buff with [inf:sup] = (replicate (sup-inf) j)
	let minima = hist
		(\x1 x2 -> if byteSeq_lt radix_size (ib*nbits-1) x1 x2 then x1 else x2)
		(replicate b u8.highest)
		part_no
		which_part
		sorted_ks
	let maxima = hist
		(\x1 x2 -> if byteSeq_gt radix_size (ib*nbits-1) x1 x2 then x1 else x2)
		(dummy_byteSeq b)
		part_no
		which_part
		sorted_ks
	in (minima, maxima)

-- TODO (?)
def partgroup_find_unknown_keys [n] [key_no] [part_no] [b]
	(radix_size : [part_no]idx_t.t)
	(k_ids : [key_no](byteSeq [b]))
	(sorted_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]idx_t.t)
: [](byteSeq [b]) =
	[]

-- Good idea to filter this afterwards
def keys_per_partition [key_no] [part_no]
	(radix_size : [part_no]idx_t.t)
	(k_ids : [key_no](byteSeq [b]))
	(sorted_ks : [n](byteSeq [b]))
	(part_bounds : [part_no]idx_t.t)
	(part_depths : [part_no]idx_t.t)
: [part_no]idx_t.t =
	(iota part_no) |> map (\j ->
		countFor (\ki ->
			radix_eq radix_size part_depths[j] sorted_ks[part_bounds[j]] ki
		)
		k_ids
	)

def partgroup_partitionByIndex [n] [group_no] [b]
	(group_ks: [group_no](byteSeq [b]))
	(ks : [n](byteSeq [b]))
: [n]idx_t.t =
	ks |> map (\k ->
		rv_find_match_if_exists k group_ks
	)