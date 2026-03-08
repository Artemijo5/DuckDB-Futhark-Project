import "../ftbasics"
import "strUtil"
import "../ft_partition"
import "../joins/ftPHJ"

-- | Function to hash the i'th string from a strInfo.
--
-- Encodes (bytes * 2**compression) characters into a byteSeq [bytes].
-- If use_len, the last of those characters is replaced with the radix of str_len.
-- Can also divide the length by len_divide.
--
-- Characters are taken starting from the (from_subdiv/num_subdiv)*strlen character of the string.
-- Their order is reversed so that they can be used like radices.
-- If use_len, the len goes at the end.
def hash_str
	(case_insens : bool)
	(use_len : bool)
	(len_divide : i64)
	(compression: i64)
	(num_subdiv : i64)
	(from_subdiv: i64)
	(bytes: i64)
	(strs : strInfo)
	(i : i64)
: byteSeq [bytes] =
	let bitmask = (mk_radix_bitmask 0 (8 / (2 ** (i32.i64 compression))) 1)[0]
	let str_len = get_str_len i strs
	let first_k = (str_len * from_subdiv) / num_subdiv
	let all_chars = ((bytes * (2**compression) - 1)..(bytes * (2**compression) - 2)...0)
		|> sized (bytes * (2 ** compression))
		|> map (\k ->
			if use_len then
				if k==0 then (-1)
				else k + first_k - 1
			else k + first_k)
		|> map (\k ->
			if k < 0
			then u8.i64 (str_len / len_divide)
			else let c = get_kth_char k i strs in
				-- if case-insensitive, map to all-caps
				if case_insens && compression==0 && c>=97 && c<=122
				then c-32 else
				-- if case-sensitive & compressed, interpolate miniscules
				if !case_insens && compression>0 && c>=97 && c<=122
				then if c%2==0 then c-1 else c+1
				else c
		)
	in
		if compression==0
		then all_chars |> sized bytes
		else all_chars
			|> unflatten
			|> sized bytes
			|> map (\cs ->
				cs
				|> map (\c -> c & bitmask)
				|> zip (cs |> indices)
				|> foldl (\(_,c1) (i2,c2) ->
					(i2, c1 | (c2 << (u8.i64 (i2 * (8/(2 ** compression)))))))
				(0i64,0u8)
				|> (.1)
			)

-- | Function to hash all strings of a strInfo.
-- See hash_str for details.
def hash_strs
	(case_insens : bool)
	(use_len : bool)
	(len_divide : i64)
	(compression: i64)
	(num_subdiv : i64)
	(from_subdiv: i64)
	(bytes: i64)
	(strs : strInfo)
= strs.idxs
	|> indices
	|> map (hash_str case_insens use_len len_divide compression num_subdiv from_subdiv bytes strs)


-- These hash values can be used for PHJ (or potentially SMJ).
-- However, must filter the result after expansion.

-- | After join expansion, filter the results.
-- Check for true string equality among pairs.
--
-- NOTE: requires GFUR join (or to maintain initial indices during partitioning).
def strJoin_filter [b]
	(char_cmp : u8 -> u8 -> i8)
	(strs1 : strInfo)
	(strs2 : strInfo)
	(is1 : []i64)
	(is2 : []i64)
	(pairs : joinPairs (byteSeq [b]))
: joinPairs_str =
	let n = length pairs.ix
	let (filt_ix, filt_iy) = zip (pairs.ix :> [n]i64) (pairs.iy :> [n]i64)
		|> map (\(i1,i2) -> (is1[i1],is2[i2]))
		|> map (\(i1,i2) -> str_cmp char_cmp strs1 strs2 i1 i2)
		|> map (\res -> res==0)
		|> zip (iota n)
		|> filter (.1)
		|> map (\(i,_) -> (is1[pairs.ix[i]], is2[pairs.iy[i]]))
		|> unzip
	let filt_strs = filt_ix |> str_gather strs1
	in {strs = filt_strs, ix = filt_ix, iy = filt_iy}
