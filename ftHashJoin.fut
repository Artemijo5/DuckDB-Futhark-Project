import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"
import "ftsort"


-- TODO
-- the logic used is to use futhark's pre-made radix-sort (might switch to custom) and isolate desired bits
-- currently doesn't work when partitioning by only 1 bit
-- might want to fix that (might not be necessary)

-- sortStruct & sortInfo are used to hold partition information
-- since partitioning is sorting-based, and results in a contiguous array

local def findPartitionBoundaries 'a
	(pXs: []a)
	(num_bits: i32)
	(get_bit: i32 -> a -> i32)
	(i: i32)
	(j: i32)
: [i64.i32 (2**(j-i))]idx_t.t
= replicate (i64.i32 (2**(j-i))) 0 -- TODO implement with hist

entry findPartitionBoundaries_short (pXs: []i16) (i: i32) (j: i32)
	= findPartitionBoundaries pXs i16.num_bits i16.get_bit i j
entry findPartitionBoundaries_int (pXs: []i32) (i: i32) (j: i32)
	= findPartitionBoundaries pXs i32.num_bits i32.get_bit i j
entry findPartitionBoundaries_long (pXs: []i64) (i: i32) (j: i32)
	= findPartitionBoundaries pXs i64.num_bits i64.get_bit i j
entry findPartitionBoundaries_float (pXs: []f32) (i: i32) (j: i32)
	= findPartitionBoundaries pXs f32.num_bits f32.get_bit i j
entry findPartitionBoundaries_double (pXs: []f64) (i: i32) (j: i32)
	= findPartitionBoundaries pXs f64.num_bits f64.get_bit i j

-- GFTR

local def radixPartRelation [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
  (i: i32)
  (j: i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let part_numBits = i32.min (num_bits+1-i) (j-i)
  let part_getBit = (\(a1: i32) -> (get_bit (a1+i)))
  let part_xys = blocked_radix_sort_by_key block_size (.0) part_numBits part_getBit xys
  let un_xys : ([n]a, [n][]u8) = unzip part_xys
  in {k = un_xys.0, pL = un_xys.1}

entry radixPartRelation_short [n] [b]
  (block_size: i16)
  (xs: sortStruct_short [n] [b])
  (i: i32)
  (j: i32)
 : sortStruct_short [n] [b]
  = radixPartRelation (block_size) (xs) (i16.num_bits) (i16.get_bit) (i) (j)
entry radixPartRelation_int [n] [b]
  (block_size: i16)
  (xs: sortStruct_int [n] [b])
  (i: i32)
  (j: i32)
 : sortStruct_int [n] [b]
  = radixPartRelation (block_size) (xs) (i32.num_bits) (i32.get_bit) (i) (j)
entry radixPartRelation_long [n] [b]
  (block_size: i16)
  (xs: sortStruct_long [n] [b])
  (i: i32)
  (j: i32)
 : sortStruct_long [n] [b]
  = radixPartRelation (block_size) (xs) (i64.num_bits) (i64.get_bit) (i) (j)
entry radixPartRelation_float [n] [b]
  (block_size: i16)
  (xs: sortStruct_float [n] [b])
  (i: i32)
  (j: i32)
 : sortStruct_float [n] [b]
  = radixPartRelation (block_size) (xs) (f32.num_bits) (f32.get_bit) (i) (j)
entry radixPartRelation_double [n] [b]
  (block_size: i16)
  (xs: sortStruct_double [n] [b])
  (i: i32)
  (j: i32)
 : sortStruct_double [n] [b]
  = radixPartRelation (block_size) (xs) (f64.num_bits) (f64.get_bit) (i) (j)

-- GFUR

local def radixPartColumn [n] 'a
  (incr: idx_t.t)
  (block_size: i16)
  (xs: [n]a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
  (i: i32)
  (j: i32)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let part_numBits = i32.min (num_bits+1-i) (j-i)
  let part_getBit = (\(a1: i32) -> (get_bit (a1+i)))
  let sorted_ixs = blocked_radix_sort_by_key block_size (.1) part_numBits part_getBit ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}

entry radixPartColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) (i: i32) (j: i32)
: sortInfo_short [n] =
  radixPartColumn incr block_size xs (i16.num_bits) (i16.get_bit) (i) (j)
entry radixPartColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) (i: i32) (j: i32)
: sortInfo_int [n] =
  radixPartColumn incr block_size xs (i32.num_bits) (i32.get_bit) (i) (j)
entry radixPartColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) (i: i32) (j: i32)
: sortInfo_long [n] =
  radixPartColumn incr block_size xs (i64.num_bits) (i64.get_bit) (i) (j)
entry radixPartColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) (i: i32) (j: i32)
: sortInfo_float [n] =
  radixPartColumn incr block_size xs (f32.num_bits) (f32.get_bit) (i) (j)
entry radixPartColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) (i: i32) (j: i32)
: sortInfo_double [n] =
  radixPartColumn incr block_size xs (f64.num_bits) (f64.get_bit) (i) (j)