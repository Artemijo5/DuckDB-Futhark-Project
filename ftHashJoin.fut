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

-- based on radix-sort in futhark-by-example
def radix_part_step [n][b] 't (block_size: idx_t.t) (xs: [n][b]u8) (pL: [n]t) (i: i32): ([n][b]u8, [n]t) =
  let bits = map (\x -> byteSeq_getBit i x) xs
  let bits_neg = map (1 - ) bits
  let offs = reduce (+) 0 bits_neg
  let idxs0 = map2 (*) bits_neg (scan (+) 0 bits_neg)
  let idxs1 = map2 (*) bits (map (+offs) (scan (+) 0 bits))
  let idxs2 = map2 (+) idxs0 idxs1
  let idxs = map (\x -> x-1) idxs2
  let xs' = partitioned_scatter block_size (copy xs) (map i64.i32 idxs) xs
  let pL' = partitioned_scatter block_size (copy pL) (map i64.i32 idxs) pL
  in (xs', pL')

def radix_part [n][b] 't (block_size: idx_t.t) (xs: [n][b]u8) (pL: [n]t) (i: i32) (j: i32): ([n][b]u8, [n]t) =
  loop (xs, pL) for bit in (i...j) do radix_part_step block_size xs pL bit

def radixPartition_GFUR [n][b]
  (block_size: idx_t.t)
  (xs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (i: i32)
  (j: i32)
: ([n](byteSeq [b]), [n](idx_t.t)) =
  let is = (offset..<(offset + n)) :> [n]idx_t.t
  in radix_part block_size xs is i j

def radixPartition_GFTR [n][b][pL_b]
  (block_size: idx_t.t)
  (xs: [n](byteSeq [b]))
  (ys: [n](byteSeq [pL_b]))
  (i: i32)
  (j: i32)
: ([n](byteSeq [b]), [n](byteSeq [pL_b])) =
  radix_part block_size xs ys i j

def getPartitionBounds [n] [b]
  (curDepth: i32)
  (pXs: [n](byteSeq [b]))
  (i: i32)
  (j: i32)
: partitionInfo =
  let boundIndices = (1..<n)
    |> map (\ind -> (pXs[ind], pXs[ind-1]))
    |> map (\(x1, x2) -> (getRadix i j x1, getRadix i j x2))
    |> map (\(x1, x2) -> zip x1 x2)
    |> zip (1..<n)
    |> filter (\(ind, rs) -> any (\(r1, r2) -> r1 != r2) rs)
    |> map (.0)
  in {
    totalBytes = i32.i64 b,
    radixSize = j-i+1,
    maxDepth = curDepth,
    bounds = [0] ++ boundIndices,
    depths = replicate (1+ (length boundIndices)) curDepth
  }

def getPartitionRadix [n] [b]
  (radix_size: i32)
  (curDepth: i32)
  (boundIdx: idx_t.t)
  (pXs: [n](byteSeq [b]))
: byteSeq [b] =
  let totalRadix = i32.min (radix_size*curDepth) ((i32.i64 b)*u8.num_bits)
  in getRadix 0 (totalRadix-1) pXs[boundIdx]

-- Repartitioning
-- TODO
-- see pre-made code in repartition.txt (...) modify that for new partitioning func

def main =
  let xs : [8](byteSeq [2]) = [[4,2],[2,1],[2,2],[3,5],[2,5],[3,1],[4,5],[3,7]]
  let nxs = radixPartition_GFUR 256 xs 14 0 4
  in (getPartitionBounds 1 nxs.0 0 4).bounds
