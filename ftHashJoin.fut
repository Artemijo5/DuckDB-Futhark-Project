import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

-- LOGIC
-- keys are passed from C in the form of byte arrays (arrays of u8)
-- partitioning parameters:
-- 1. total number of bits
-- 2. number of bits per partition
-- 3. maximum desired depth <= ceil(total bits / partition bits)
-- Partition data
-- 1. starting index
-- 2. partition depth
-- 3. ig total bits & bits per partition as well
-- TODO
-- consider how data are passed from C (all at once or just relevant radixes?)
-- consider how are relations partitioned (both entirely or one entirely and the other concurrently with the join? prob the former)

type byteSeq [bytes] = [bytes]u8

type~ partitionInfo = {totalBytes: i32, radixSize: i32, maxDepth: i32, bounds: []idx_t.t, depths: []i32}

def byteSeq_getBit [b] (i: i32) (x: byteSeq [b])
: i32 =
  let whichByte = (i32.i64 b) - (i/u8.num_bits) - 1
  let whichBit = i%u8.num_bits
  in x[whichByte]
    |> u8.get_bit whichBit

def getRadix [b] (i: i32) (j: i32) (x: byteSeq [b])
: byteSeq [b] =
  let firstByte = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let lastByte = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let firstBit = i%u8.num_bits
  let lastBit = j%u8.num_bits
  let first_bitMask = u8.highest << (u8.i32 firstBit)
  let last_bitMask = (u8.>>>) u8.highest(u8.i32 (u8.num_bits - lastBit - 1))
  let mod_x = x
    |> zip (iota b)
    |> map (\(ind, v) -> if (ind<=firstByte && ind>=lastByte) then (ind, v) else (ind, 0))
    |> map (.1)
  let mod1_x = (copy mod_x) with [firstByte] = mod_x[firstByte] & first_bitMask
  in (copy mod1_x) with [lastByte] = mod1_x[lastByte] & last_bitMask


--def analyseBits [n] [b] (xs: [n](byteSeq[b])) : [n][b*(i64.i32 u8.num_bits)]i32 =
--  (replicate n (iota (b*(i64.i32 u8.num_bits))))
--    |> map (map i32.i64)
--    |> zip (iota n)
--    |> map (\(i, vs) ->
--      vs
--        |> map (\v -> byteSeq_getBit v xs[i])
--    )

-- TODO find alternative solution?
def adjustOddBits [b] (j: i32) (x: byteSeq [b])
: byteSeq [b] =
  let whichByte = (i32.i64 b) - (j/u8.num_bits) - 1
  let whichBit = j%u8.num_bits
  let bitMask = (u8.>>>) u8.highest (u8.i32 (u8.num_bits - whichBit - 1))
  in (copy x) with [whichByte] = (x[whichByte] & bitMask)

entry radixPartRelation_GFUR [n] [b]
  (block_size: i16)
  (xs: [n](byteSeq [b]))
  (i: i32)
  (j: i32)
 : [n](byteSeq [b]) =
  let part_numBits = j-i+1
  let part_getBit = (\(a1: i32) -> (byteSeq_getBit (a1+i)))
  in
    if part_numBits%2 == 0
    then blocked_radix_sort block_size part_numBits part_getBit  xs
    else blocked_radix_sort_by_key block_size (adjustOddBits (j)) (part_numBits+1) part_getBit  xs

entry radixPartRelation_GFTR [n] [b] [pL_b]
  (block_size: i16)
  (xs: [n](byteSeq [b]))
  (ys: [n](byteSeq [pL_b]))
  (i: i32)
  (j: i32)
 : ([n](byteSeq [b]), [n](byteSeq [pL_b])) =
  let part_numBits = j-i+1
  let part_getBit = (\(a1: i32) -> (byteSeq_getBit (a1+i)))
  let xys = zip xs ys
  in
    if part_numBits%2 == 0
    then unzip (blocked_radix_sort_by_key block_size (.0) part_numBits part_getBit xys)
    else unzip (blocked_radix_sort_by_key block_size (\xy -> adjustOddBits (j) xy.0) (part_numBits+1) part_getBit xys)

entry getPartitionBounds_first [n] [b]
  (pXs: [n](byteSeq [b]))
  (i: i32)
  (j: i32)
: partitionInfo =
  let boundIndices = (1..<n)
    |> map (\i -> (pXs[i], pXs[i-1]))
    |> map (\(x1, x2) -> (getRadix i j x1, getRadix i j x2))
    |> map (\(x1, x2) -> zip x1 x2)
    |> zip (1..<n)
    |> filter (\(i, rs) -> any (\(r1, r2) -> r1 != r2) rs)
    |> map (.0)
  in {
    totalBytes = i32.i64 b,
    radixSize = j-i+1,
    maxDepth = 0,
    bounds = [0] ++ boundIndices,
    depths = replicate (1+ (length boundIndices)) 0
  }