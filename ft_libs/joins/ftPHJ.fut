import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"
import "../ft_partition"

-- Partitioned Hash Join
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39
-- & References

-- Method:
-- After applying partitioning, we transform the radices to unsigned integer primitives.
-- (this requires that the radices are no more than 64 bits)
-- This is done because
-- a. binary search operations & comparisons on primitives are faster than on nested arrays.
-- b. allows to shrink the data size to the minimum required.
-- Multi-level radices can also be processed as just one number via bit rearrangement.

-- | Parametric module to perform hash join after partitioning.
-- Radices processed as unsigned integers of appropriate size.
module prim_PHJ (U : integral) = {
	type t = U.t

	local def zero : t = U.i32 0
	local def one  : t = U.i32 1
	local def ones : t = U.highest

	local def num_bits : i32 = U.num_bits
	local def num_bytes: i64 = i64.i32 (num_bits / u8.num_bits)

	local def from_u8  : u8  -> t = (U.u8)
	local def from_i32 : i32 -> t = (U.i32)
	local def to_i64 = (U.to_i64)

	local def eq = (U.==)
	local def gt = (U.>)
	local def lt = (U.<)
	local def geq = (U.>=)
	local def leq = (U.<=)

	local def lshift : t -> i32 -> t = (\x bi -> bi |> from_i32 |> (U.<<) x)
	local def rshift : t -> i32 -> t = (\x bi -> bi |> from_i32 |> (U.>>>) x)
	local def bitwise_or  : t -> t -> t = (U.|)
	local def bitwise_and : t -> t -> t = (U.&)

	-- | Convert byteSeq into unsigned integer of type t
	local def from_byteSeq [b] (x : byteSeq [b]) : t =
		loop y : t = zero
		for j<num_bytes do
			let r = (from_u8 x[b-j-1]) `lshift` ((i32.i64 j) * u8.num_bits)
			in y `bitwise_or` r

	-- | Gets a single level of the radix.
	local def get_simple_radix
		(radix_bits : i32)
		(cur_level  : i32)
		(x : t)
	: t =
		let lx = x `rshift` (radix_bits*cur_level)
		let mask = ones `rshift` (num_bits - radix_bits)
		in lx `bitwise_and` mask

	-- | Obtains a multi-level radix, with bits re-arranged by level.
	local def get_radix
		(radix_bits : i32)
		(depth : i32)
		(x : t)
	: t =
		loop y : t = zero
		for j<depth do
			let r = (x |> get_simple_radix radix_bits j)
				`lshift` (radix_bits*(depth-j-1))
			in y `bitwise_or` r

	-- | Radix comparisons (a single primitive comparison between re-arranged bits)
	local def radix_cmp
		(cmp : t -> t -> bool)
		(radix_bits : i32)
		(depth : i32)
		(x1 : t)
		(x2 : t)
	: bool =
		let r1 = get_radix radix_bits depth x1
		let r2 = get_radix radix_bits depth x2
		in r1 `cmp` r2

	-- | Radix ==
	local def radix_eq radix_bits depth x1 x2 = radix_cmp (eq) radix_bits depth x1 x2
	-- | Radix >
	local def radix_gt radix_bits depth x1 x2 = radix_cmp (gt) radix_bits depth x1 x2
	-- | Radix <
	local def radix_lt radix_bits depth x1 x2 = radix_cmp (lt) radix_bits depth x1 x2
	-- | Radix >=
	local def radix_geq radix_bits depth x1 x2 = radix_cmp (geq) radix_bits depth x1 x2
	-- | Radix <=
	local def radix_leq radix_bits depth x1 x2 = radix_cmp (leq) radix_bits depth x1 x2

	-- | Complex binary search for finding matching partitions.
	-- Finds the matching partition in xs for each v, if it exists.
	-- f_init, l_init, maxRange obtained from hash table.
	def radix_bsearch [nv]
		(radix_bits : i32)
		(bounds : []i64)
		(depths : []i32)
		(maxRange : i64)
		(f_init : [nv]i64) -- first potential match
		(l_init : [nv]i64) -- last potential match
		(xs : []t)
		(vs : [nv]t)
	: [nv]i64 =
		let maxIter = 1 + (maxRange |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
		let max3 (i1 : i64) (i2 : i64) (i3 : i64) = i64.max i1 (i64.max i2 i3)
		in if maxIter==1 then f_init else
		let (foundAt, _) =
			loop (is, last_steps) = (f_init, map2 (\fi li -> li - fi + 1) f_init l_init)
			for _ in iota maxIter do
				let this_steps = last_steps |> map (\ls -> (ls+1)/2)
				let searchAt = is
					|> map2 (\fi i ->
						let prev_elem = xs[bounds[max3 0 fi (i-1)]]
						let this_elem = xs[bounds[max3 0 fi i]]
						in (i, prev_elem, this_elem)
					) f_init
					|> map5 (\kv fi li step (i,pv,cv) ->
						if i<0 then (-1) else
						if radix_eq radix_bits depths[i] kv cv
							then i
						else if radix_gt radix_bits depths[i] kv cv then
							if i==li
							then (-1)
							else i64.min li (i+step)
						else
							if i==fi || radix_gt radix_bits depths[i-1] kv pv
							then (-1)
							else i64.max fi (i-step)
					) vs f_init l_init this_steps
				in (searchAt, this_steps)
		in foundAt

	-- | Locate the matching partition of xs for each radix in vs
	def hash_bsearch [nv]
		(radix_bits : i32)
		(xInfo : partitionInfo)
		(xTbl  : radix_hashTable [i64.i32 radix_bits])
		(xs : []t)
		(vs : [nv]t)
	: [nv]i64 =
		-- Get the potentially matching partitions from partitionInfo
		let (f_init, l_init) = vs
			|> map (\v -> v |> get_radix radix_bits 1 |> to_i64)
			|> map (\i -> (xTbl.first_info_idx[i], xTbl.last_info_idx[i]))
			|> unzip
		-- Get the max partition range among radices present in values
		-- Use the fact that vs are grouped by 1st-level radix to take distinct
		let maxRange = f_init
			|> group_boundaries (!=)
			|> zip3 f_init l_init
			|> filter (.2)
			|> map (\(fi,li,_) -> if fi<0 then 0 else li-fi+1)
			|> i64.maximum
		in radix_bsearch
			radix_bits xInfo.bounds xInfo.depths maxRange f_init l_init xs vs

	-- TODO
	-- test binary searches
	-- following sequential searches within each partition (together with bsearch: match-finding phase)
	-- expansion phase (also does seq search)
	-- etc

}