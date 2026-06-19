import "../ftbasics"
import "../lib/github.com/diku-dk/segmented/segmented"
import "../lib/github.com/diku-dk/sorts/merge_sort"

-- Structs

	-- | Type to store a column of strings.
	-- Fields:
	-- - contents: the contents of all strings in a contiguous byte array
	-- - idxs: the index of the first char of each string in contents, increasing order
	type~ strInfo = {contents : []u8, idxs : []i64}

	-- | Type to store a transformed column of strings + the original payloads.
	type~ strInfo_sorted 't = {strs : strInfo , pLs : []t}

	-- | Type to store a transformed column of strings + the original indices.
	type~ sortInfo_str = strInfo_sorted i64
	-- | Type to store a transformed column of strings + the original payloads (as byte array).
	type~ sortStruct_str [b] = strInfo_sorted (byteSeq [b])

	-- | Type to store the join output between 2 str columns.
	type~ joinPairs_str = {strs : strInfo, ix : []i64, iy : []i64}

-- strInfo initialization
	
	-- | Create an empty strInfo.
	def mk_strInfo : strInfo = {contents = [], idxs = []}

	-- | Append strings into strInfo.
	-- If passLens is true, the idxs_ argument is treated as string lengths.
	-- If false, it is treated as the starting index of each new string in contents.
	-- NOTE : consumes original strInfo passed
	def appendStrs (passLens : bool) (idxs_ : []i64) (contents : []u8) (strs : *strInfo)
	: strInfo =
		let prev_len = length strs.contents
		let idxs = if passLens then exscan (+) 0 idxs_ else idxs_
			|> map (\i -> i + prev_len)
		in {contents = strs.contents ++ contents, idxs = strs.idxs ++ idxs}

	-- | Append a single string into strInfo
	def appendStr (str : []u8) (strs : *strInfo)
	: strInfo =
		{contents = strs.contents ++ str, idxs = strs.idxs ++ [length (strs.contents)]}

	-- | Fetch a range of strs.
	def fetchStrs (offs : i64) (limt : i64) (strs : strInfo)
	: strInfo =
		let inf = offs
		let sup = i64.min (offs+limt) (length strs.idxs)
		let fetch_idxs = strs.idxs[inf:sup]
		let inf_idx = fetch_idxs[0]
		let sup_idx = if sup<(length strs.idxs)
			then strs.idxs[sup]
			else length strs.contents
		let fetch_contents = strs.contents[inf_idx:sup_idx]
		in {
			contents = fetch_contents,
			idxs = fetch_idxs
				|> map (\i -> i - inf_idx)
		}

-- Character-wise Comparisons & Operations

	-- | Compares 2 characters by arithmetic value.
	-- Returns in i8:
	-- 0 if c1 == c2
	-- 1 if c1 > c2
	-- (-1) if c1 < c2
	def arith_cmp (c1: u8) (c2: u8)
	: i8 =
		if c1<c2 then (-1)
		else if c1>c2 then 1
		else 0

	-- | Wrapper for a character comparator that makes it ignore case.
	def mk_caseInsens 't
		(char_cmp : u8 -> u8 -> t)
	: (u8 -> u8 -> t) =
		(\c1 c2 ->
			let c1' = if (97<=c1 && 122>=c1) then (c1-32) else c1
			let c2' = if (97<=c2 && 122>=c2) then (c2-32) else c2
			in char_cmp c1' c2'
		)

	-- | Compares 2 characters by arithmetic value.
	-- Case insensitive (converts miniscules to capitals).
	-- Returns in i8:
	-- 0 if c1 == c2
	-- 1 if c1 > c2
	-- (-1) if c1 < c2
	def caseInsens_cmp : u8 -> u8 -> i8 = mk_caseInsens arith_cmp

	-- | Get the k'th character of the i'th string in strs.
	def get_kth_char
		(k : i64)
		(i : i64)
		(strs : strInfo)
	: u8 =
		if i<0 then 0 else
		let n = length strs.idxs
		let tlen = length strs.contents
		let inf = strs.idxs[i]
		let sup = if i==n-1 then tlen else strs.idxs[i+1]
		in
			if k < (sup-inf)
			then strs.contents[inf+k]
			else 0

-- Comparisons & Operations on the scale of strings.

	-- | Get the length of the i'th string in strs.
	def get_str_len
		(i : i64)
		(strs : strInfo)
	: i64 =
		if i<0 then (-1) else
		let n = length strs.idxs
		let tlen = length strs.contents
		let inf = strs.idxs[i]
		let sup = if i==n-1 then tlen else strs.idxs[i+1]
		in (sup-inf)

	-- | Compare 2 strings in different strInfo structs.
	-- Returns in i8:
	-- 0 if str1 == str2
	-- 1 if str1 > str2
	-- (-1) if str1 < str2
	def str_cmp
		(char_cmp : u8 -> u8 -> i8)
		(strs1 : strInfo)
		(strs2 : strInfo)
		(i1 : i64)
		(i2 : i64)
	: i8 =
		let len1 = get_str_len i1 strs1
		let len2 = get_str_len i2 strs2
		let (res,_) =
			loop (cmp,j) = (0i8,0i64) while cmp==0 && j<(i64.min len1 len2) do
				let c1 = strs1.contents[strs1.idxs[i1]+j]
				let c2 = strs2.contents[strs2.idxs[i2]+j]
				in (char_cmp c1 c2, j+1)
		in if res!=0 then res else
		if len1<len2 then -1 else if len1>len2 then 1 else 0

	-- | Compare 2 strings in the same strInfo struct.
	-- Returns in i8:
	-- 0 if str1 == str2
	-- 1 if str1 > str2
	-- (-1) if str1 < str2
	def coStr_cmp
		(char_cmp : u8 -> u8 -> i8)
		(strs : strInfo)
		(i1 : i64)
		(i2 : i64)
	: i8 =
		if i1==i2 then 0 else
		str_cmp char_cmp strs strs i1 i2

-- Gather & Sort Operations

	-- | Gather strings by index.
	def str_gather
		(strs : strInfo)
		(is : []i64)
	: strInfo =
		let gather_lens = is |> map (\i -> get_str_len i strs)
		let new_idxs = gather_lens |> exscan (+) 0
		let new_cons = is
			|> map (\i -> strs.idxs[i])
			|> zip gather_lens
			|> expand (.0) (\(_,i) ind -> strs.contents[i+ind])
		in {contents = new_cons, idxs = new_idxs}

	-- | Sort strings, returning their indices.
	def str_index_sort 't
		(char_cmp : u8 -> u8 -> i8)
		(strs : strInfo)
	: []i64 = strs.idxs
		|> indices
		|> merge_sort (\i1 i2 -> coStr_cmp char_cmp strs i1 i2 <= 0)

	-- | Sort strings.
	def str_sort 't
		(char_cmp : u8 -> u8 -> i8)
		(strs : strInfo)
		(pLs : []t)
	: strInfo_sorted t =
		let (sorted_pLs, sorted_is) = strs.idxs
			|> indices
			|> zip pLs
			|> merge_sort (\(_,i1) (_,i2) -> coStr_cmp char_cmp strs i1 i2 <= 0)
			|> unzip
		let sorted_strs = sorted_is |> str_gather strs
		in {strs = sorted_strs, pLs = sorted_pLs}


-- Splitting

	-- | Split a string into multiple, using a list of delimiters.
	def str_split [n] (delim : []u8) (str : [n]u8)
	: strInfo =
		if n==0 then {contents = [], idxs = []} else
		if (length delim) == 0 then {contents=str,idxs=[0]} else
		let isDelim = str
			|> map (\c -> delim |> any (\dc -> c==dc))
		let str_con = str
			|> zip isDelim
			|> filter (\(isD,_) -> !isD)
			|> map (.1)
		let str_idx = isDelim
			|> map (not >-> i64.bool)
			|> segmented_reduce (+) 0 ((copy isDelim) with [0] = true) -- str lengths
			|> exscan (+) 0
		-- get rid of multiples
		let str_idx' = str_idx
			|> group_boundaries (!=)
			|> zip str_idx
			|> filter (.1)
			|> map (.0)
		in {contents = str_con, idxs = str_idx'}

	-- | Split all strings in an strInfo, using a list of delimiters.
	def str_multiSplit (delim : []u8) (strs : strInfo)
	: strInfo =
		if (length delim) == 0 then strs else
		-- Unite into 1 string, separated by the first delim.
		let lens_plusOne = strs.idxs
			|> indices
			|> map (\i -> get_str_len i strs)
			|> map (\l -> l+1)
		-- insert first delimiter between all str's
		let str = strs.idxs
			|> zip lens_plusOne
			|> expand (.0) (\(_,i) ind ->
				if ind==0
				then delim[0]
				else strs.contents[i+ind-1]
			)
		in str_split delim str
		

