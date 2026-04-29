import "../ftbasics"
import "strUtil"
import "../ft_partition"
import "../joins/ftSMJ"

-- | Sort hashed strings, together with indices.
def sort_hashed [b] (xs : [](byteSeq [b])) =
	radix_part 0 ((i32.i64 b)*u8.num_bits - 1) 2
		xs (indices xs)


-- | SMJ match-finding for hashed str's.
def hsmj_matchfinding_str [n1] [n2] [b]
	(merge_path_diagonals : i64)
	(hs1 : [n1](byteSeq [b]))
	(hs2 : [n2](byteSeq [b]))
: joinTup [n1] (byteSeq [b]) =
	let radix_bits = (i32.i64 b)*u8.num_bits - 1
	in smj_matchFinding
		(\h1 h2 -> all (id) (map2 (==) h1 h2))
		(\h1 h2 -> radix_cmp (>=) radix_bits (1,h1) (1,h2))
		(\h1 h2 -> radix_cmp (<=) radix_bits (1,h1) (1,h2))
		(\h1 h2 -> radix_cmp (>)  radix_bits (1,h1) (1,h2))
		(\h1 h2 -> radix_cmp (<)  radix_bits (1,h1) (1,h2))
		merge_path_diagonals
		hs1 hs2

-- | SMJ full Merge-Join routine (for hashed strings, Inner Join).
-- NOTE: need to call strJoin_filter afterwards
def do_InnerHSMJ_str [n1] [n2] [b]
	(merge_path_diagonals : i64)
	(hs1 : [n1](byteSeq [b]))
	(hs2 : [n2](byteSeq [b]))
: joinPairs_bsq [b] =
	hsmj_matchfinding_str merge_path_diagonals
		hs1 hs2
	|> smj_expand