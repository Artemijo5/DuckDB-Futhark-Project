import "lib/github.com/diku-dk/sorts/merge_sort"
import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

-- GFTR SORTING
-- TODO see ftError.fut

-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
type sortStruct [n] [b] 'a = {k: [n]a, pL: [n][b]u8}

type sortStruct_short [n] [b] = sortStruct [n] [b] i16
type sortStruct_int [n] [b] = sortStruct [n] [b] i32
type sortStruct_long [n] [b] = sortStruct [n] [b] i64
type sortStruct_float [n] [b] = sortStruct [n] [b] f32
type sortStruct_double [n] [b] = sortStruct [n] [b] f64

local def radixSortRelation_signed_integral [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = blocked_radix_sort_int_by_key block_size (\xy -> xy.0) num_bits get_bit xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
local def radixSortRelation_signed_float [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = blocked_radix_sort_float_by_key block_size (\xy -> xy.0) num_bits get_bit xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
local def mergeSortRelation [n] [b] 'a
  (xs: sortStruct [n] [b] a)
  (leq: a -> a -> bool)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = merge_sort_by_key (\xy -> xy.0) (leq) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}

entry radixSortRelation_short [n] [b]
  (block_size: i16)
  (xs: sortStruct_short [n] [b])
 : sortStruct_short [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i16.num_bits) (i16.get_bit)
entry radixSortRelation_int [n] [b]
  (block_size: i16)
  (xs: sortStruct_int [n] [b])
 : sortStruct_int [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i32.num_bits) (i32.get_bit)
entry radixSortRelation_long [n] [b]
  (block_size: i16)
  (xs: sortStruct_long [n] [b])
 : sortStruct_long [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i64.num_bits) (i64.get_bit)
entry radixSortRelation_float [n] [b]
  (block_size: i16)
  (xs: sortStruct_float [n] [b])
 : sortStruct_float [n] [b]
  = radixSortRelation_signed_float (block_size) (xs) (f32.num_bits) (f32.get_bit)
entry radixSortRelation_double [n] [b]
  (block_size: i16)
  (xs: sortStruct_double [n] [b])
 : sortStruct_double [n] [b]
  = radixSortRelation_signed_float (block_size) (xs) (f64.num_bits) (f64.get_bit)

entry mergeSortRelation_short [n] [b]
  (xs: sortStruct_short [n] [b])
: sortStruct_short [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_int [n] [b]
  (xs: sortStruct_int [n] [b])
: sortStruct_int [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_long [n] [b]
  (xs: sortStruct_long [n] [b])
: sortStruct_long [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_float [n] [b]
  (xs: sortStruct_float [n] [b])
: sortStruct_float [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_double [n] [b]
  (xs: sortStruct_double [n] [b])
: sortStruct_double [n] [b]
  = mergeSortRelation (xs) (<=)

-- GFUR SORTING

-- | Type used to preserve original index information when sorting (GFUR).
type sortInfo [len] 't = {is: [len](idx_t.t), xs: [len]t}
-- | Sorting information type (short)(GFUR).
type sortInfo_short [n] = sortInfo [n] i16
-- | Sorting information type (integer)(GFUR).
type sortInfo_int [n] = sortInfo [n] i32
-- | Sorting information type (long)(GFUR).
type sortInfo_long [n] = sortInfo [n] i64
-- | Sorting information type (float)(GFUR).
type sortInfo_float [n] = sortInfo [n] f32
-- | Sorting information type (double)(GFUR).
type sortInfo_double [n] = sortInfo [n] f64

local def radixSortColumn_signed_integral [n] 'a
  (incr: idx_t.t)
  (block_size: i16)
  (xs: [n]a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = blocked_radix_sort_int_by_key block_size (\ixs -> ixs.1) num_bits get_bit ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}
local def radixSortColumn_signed_float [n] 'a 
  (incr: idx_t.t)
  (block_size: i16)
  (xs: [n]a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = blocked_radix_sort_float_by_key block_size (\ixs -> ixs.1) num_bits get_bit ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}
local def mergeSortColumn [n] 'a
  (incr: idx_t.t)
  (xs: [n]a)
  (leq: a -> a -> bool)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = merge_sort_by_key (\ixs -> ixs.1) (leq) ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}

-- Order a payload column given the reordered indices.
local def orderByIndices [n] 't (block_size: idx_t.t) (dummy_elem: t) (is: [n](idx_t.t)) (ys: [n]t) : [n]t =
  partitioned_gather block_size dummy_elem ys is

-- | Sort a column of short type (GFUR).
entry radixSortColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) : sortInfo_short [n] =
  radixSortColumn_signed_integral incr block_size xs (i16.num_bits) (i16.get_bit)
-- | Sort a column of integer type (GFUR).
entry radixSortColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) : sortInfo_int [n] =
  radixSortColumn_signed_integral incr block_size xs (i32.num_bits) (i32.get_bit)
-- | Sort a column of long type (GFUR).
entry radixSortColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) : sortInfo_long [n] =
  radixSortColumn_signed_integral incr block_size xs (i64.num_bits) (i64.get_bit)
-- | Sort a column of float type (GFUR).
entry radixSortColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) : sortInfo_float [n] =
  radixSortColumn_signed_float incr block_size xs (f32.num_bits) (f32.get_bit)
-- | Sort a column of double type (GFUR).
entry radixSortColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) : sortInfo_double [n] =
  radixSortColumn_signed_float incr block_size xs (f64.num_bits) (f64.get_bit)

-- | Sort a column of short type (GFUR).
entry mergeSortColumn_short [n] (incr: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of integer type (GFUR).
entry mergeSortColumn_int [n] (incr: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of long type (GFUR).
entry mergeSortColumn_long [n] (incr: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of float type (GFUR).
entry mergeSortColumn_float [n] (incr: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of double type (GFUR).
entry mergeSortColumn_double [n] (incr: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
  mergeSortColumn incr xs (<=)

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) : [n]i16 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) : [n]i32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) : [n]i64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) : [n]f32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) : [n]f64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys


-- argmin - for getting chunk replacement priorities
local def argmin [n] 't
    (lt: t -> t -> bool)
    (eq: t -> t -> bool)
    (highest: t)
    (ks: [n]t)
    : idx_t.t = 
  let ne = (n, highest)
  let iks = ks
    |> zip (idx_t.indices ks)
  let min_ik = reduce_comm(\(ix, vx) (iy, vy) ->
        if (vx `lt` vy) || ((vx `eq` vy) && (ix < iy))
          then (ix, vx)
          else (iy, vy)
      ) ne iks
  in min_ik.0

-- | Obtain the index of the smallest element from a list of shorts.
entry argmin_short [n] (ks: [n]i16) : idx_t.t =
  argmin (<) (==) (i16.highest) ks
-- | Obtain the index of the smallest element from a list of integers.
entry argmin_int [n] (ks: [n]i32) : idx_t.t =
  argmin (<) (==) (i32.highest) ks
-- | Obtain the index of the smallest element from a list of longs.
entry argmin_long [n] (ks: [n]i64) : idx_t.t =
  argmin (<) (==) (i64.highest) ks
-- | Obtain the index of the smallest element from a list of floats.
entry argmin_float [n] (ks: [n]f32) : idx_t.t =
  argmin (<) (==) (f32.highest) ks
-- | Obtain the index of the smallest element from a list of doubles.
entry argmin_double [n] (ks: [n]f64) : idx_t.t =
  argmin (<) (==) (f64.highest) ks