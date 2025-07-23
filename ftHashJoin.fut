import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"
import "ftsort"


-- TODO
-- the logic used is to use futhark's pre-made radix-sort (might switch to custom) and isolate desired bits
-- currently doesn't work when partitioning by only 1 bit
-- might want to fix that (might not be necessary)

-- sortStruct & sortInfo are used to hold partition information
-- since partitioning is sorting-based, and results in a contiguous array

local def findPartitionBoundaries 'a 'int_t
	(pXs: []a)
	(num_bits: i32)
	(get_bit: i32 -> a -> i32)
	(i: i32)
	(j: i32)
: [](i64, idx_t.t) =
  let radixBits = i32.min (num_bits+1-i) (j-i)
  let numParts = i64.i32 (2**(radixBits))
  --let fromMSB = i32.min 0 (num_bits - j + 1)
  --let getRadix (x: a) = x * (2**fromMSB) / (2**(fromMSB+i)) -- this only works for unsigned int
  -- could use it if first transforming to uint type...
  -- TODO figure out *efficient* method to obtain radix
  -- Ideas: 1. have all data in unsigned int form & bit-shift 2. use a bit mask (!)
  let getRadix (x: a) : i32 =
    let loop_over =
      loop p = (0, 0)
      while p.0<radixBits do
        (p.0+1, i32.set_bit p.0 p.1 (get_bit (p.0+i) x))
    in loop_over.1
  let radixs = pXs |> map getRadix |> map (i64.i32)
  -- based on Futhark by Example - Removing Duplicates
  in hist (idx_t.min) (length radixs) numParts radixs (indices radixs)
    |> zip (iota numParts)
    |> filter (\pair -> pair.1 < (length radixs))
-- TODO make appropriate types for access from the C API
-- TODO also, consider including partitionDepth parameter for recursive partitioning

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