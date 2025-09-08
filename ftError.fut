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

-- based on radix-sort in futhark-by-example
def radix_part_step [n][b] 't (xs: [n][b]u8) (pL: [n]t) (i: i32): ([n][b]u8, [n]t) =
  let bits = map (\x -> byteSeq_getBit i x) xs
  let bits_neg = map (1 - ) bits
  let offs = reduce (+) 0 bits_neg
  let idxs0 = map2 (*) bits_neg (scan (+) 0 bits_neg)
  let idxs1 = map2 (*) bits (map (+offs) (scan (+) 0 bits))
  let idxs2 = map2 (+) idxs0 idxs1
  let idxs = map (\x -> x-1) idxs2
  let xs' = scatter (copy xs) (map i64.i32 idxs) xs
  let pL' = scatter (copy pL) (map i64.i32 idxs) pL
  in (xs', pL')

def radix_part [n][b] 't (xs: [n][b]u8) (pL: [n]t) (i: i32) (j: i32): ([n][b]u8, [n]t) =
  loop (xs, pL) for bit in (i...j) do radix_part_step xs pL bit

def main =
  let xs : [8](byteSeq [2]) = [[4,2],[2,1],[2,2],[3,5],[2,5],[3,1],[4,5],[3,7]]
  let ys = (iota 8)
  in radix_part xs ys 0 4