import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

-- Sorting functions for the different algorithms.
-- Namely, sorting with or without payloads, to compare
-- the GFUR & GFTR pipelines.
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39

-- Radix-sort is made to fit custom data.
-- Merge-sort using futhark library.

-- TODO can call clz in entry points to get MSB for radix-sort
-- TODO see if can use blocked_radix_sort from futhark library

-- Basic radix-sort funcs

	-- | Basic radix-sort
	def ft_radix_sort [n] 't
		(bit_step : i32)
	    (num_bits : i32)
	    (clz : t -> i32)
	    (get_bit : i32 -> t -> i32)
	    (xs : [n]t)
	: [n]t =
		let msb = xs |> map (clz) |> i32.minimum |> (i32.-) num_bits
	    in loop xs
		    for bit in (0..bit_step..<msb)
		    do radix_sort_multistep bit (i32.min (msb-1) (bit+bit_step-1)) get_bit xs

	-- | Radix-sort for signed integers.
	-- Based on futhark sorts library radix_sort_int
	def ft_radix_sort_int [n] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
	: [n]t =
	let get_bit' i x =
		-- Flip the most significant bit.
		let b = get_bit i x
		in if i == num_bits-1 then b ^ 1 else b
	in ft_radix_sort bit_step num_bits clz get_bit' xs

	-- | Radix-sort for floating-point data.
	-- Based on futhark sorts library radix_sort_int
	def ft_radix_sort_float [n] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
	: [n]t =
	let get_bit' i x =
		-- We flip the bit returned if:
		--
		-- 0) the most significant bit is set (this makes more negative
		--    numbers sort before less negative numbers), or
		--
		-- 1) we are asked for the most significant bit (this makes
		--    negative numbers sort before positive numbers).
		let b = get_bit i x
		in if get_bit (num_bits-1) x == 1 || i == num_bits-1
		then b ^ 1 else b
	in ft_radix_sort bit_step num_bits clz get_bit' xs

-- Wrapper types for GFTR & GFUR

	-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
	type sortStruct [n] [b] 't = {ks: [n]t, pL: [n][b]u8}

	-- | Type used to preserve original index information when sorting (GFUR).
	type sortInfo [n] 't = {ks: [n]t, is: [n]i64}

	-- TODO take these to ftRelational I suppose

	-- | Sorting information type (short)(GFTR).
	type sortStruct_i16 [n] [b] = sortStruct [n] [b] i16
	-- | Sorting information type (integer)(GFTR).
	type sortStruct_i32 [n] [b] = sortStruct [n] [b] i32
	-- | Sorting information type (long)(GFTR).
	type sortStruct_i64 [n] [b] = sortStruct [n] [b] i64
	-- | Sorting information type (float)(GFTR).
	type sortStruct_f32 [n] [b] = sortStruct [n] [b] f32
	-- | Sorting information type (double)(GFTR).
	type sortStruct_f64 [n] [b] = sortStruct [n] [b] f64

	-- | Sorting information type (short)(GFUR).
	type sortInfo_i16 [n] = sortInfo [n] i16
	-- | Sorting information type (integer)(GFUR).
	type sortInfo_i32 [n] = sortInfo [n] i32
	-- | Sorting information type (long)(GFUR).
	type sortInfo_i64 [n] = sortInfo [n] i64
	-- | Sorting information type (float)(GFUR).
	type sortInfo_f32 [n] = sortInfo [n] f32
	-- | Sorting information type (double)(GFUR).
	type sortInfo_f64 [n] = sortInfo [n] f64

-- Functions for GFTR sorting

	-- | Merge sort (GFTR).
	def mergeSort_GFTR [n] [b] 't
		(leq: t -> t -> bool)
		(xs : [n]t)
		(pL : [n][b]u8)
	: sortStruct [n] [b] t =
		let xys = zip xs pL
		let (sorted_xs, sorted_pL) = xys
			|> merge_sort (\(x1,_) (x2,_) -> x1 `leq` x2)
			|> unzip
		in {ks = sorted_xs, pL = sorted_pL}

	-- | Radix sort for signed integers (GFTR).
	def radixSort_int_GFTR [n] [b] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
		(pL : [n][b]u8)
	: sortStruct [n] [b] t =
		let xys = zip xs pL
		let (sorted_xs, sorted_pL) = xys
			|> ft_radix_sort_int bit_step num_bits (\(x,_) -> clz x) (\bi (x,_) -> get_bit bi x)
			|> unzip
		in {ks = sorted_xs, pL = sorted_pL}

	-- | Radix sort for floating-point data (GFTR).
	def radixSort_float_GFTR [n] [b] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
		(pL : [n][b]u8)
	: sortStruct [n] [b] t =
		let xys = zip xs pL
		let (sorted_xs, sorted_pL) = xys
			|> ft_radix_sort_float bit_step num_bits (\(x,_) -> clz x) (\bi (x,_) -> get_bit bi x)
			|> unzip
		in {ks = sorted_xs, pL = sorted_pL}

-- Functions for GFUR sorting

	-- | Merge sort (GFTR).
	def mergeSort_GFUR [n] 't
		(leq: t -> t -> bool)
		(xs : [n]t)
	: sortInfo [n] t =
		let xis = zip xs (indices xs)
		let (sorted_xs, sorted_is) = xis
			|> merge_sort (\(x1,_) (x2,_) -> x1 `leq` x2)
			|> unzip
		in {ks = sorted_xs, is = sorted_is}

	-- | Radix sort for signed integers (GFTR).
	def radixSort_int_GFUR [n] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
	: sortInfo [n] t =
		let xis = zip xs (indices xs)
		let (sorted_xs, sorted_is) = xis
			|> ft_radix_sort_int bit_step num_bits (\(x,_) -> clz x) (\bi (x,_) -> get_bit bi x)
			|> unzip
		in {ks = sorted_xs, is = sorted_is}

	-- | Radix sort for floating-point data (GFTR).
	def radixSort_float_GFUR [n] 't
		(bit_step : i32)
		(num_bits : i32)
	    (clz : t -> i32)
		(get_bit : i32 -> t -> i32)
		(xs : [n]t)
	: sortInfo [n] t =
		let xis = zip xs (indices xs)
		let (sorted_xs, sorted_is) = xis
			|> ft_radix_sort_float bit_step num_bits (\(x,_) -> clz x) (\bi (x,_) -> get_bit bi x)
			|> unzip
		in {ks = sorted_xs, is = sorted_is}