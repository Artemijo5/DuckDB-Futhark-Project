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

-- TODO find alternative solution?
-- currently needed because blocked_radix_sort for odd #bits uses 1 more bit
def adjustOddBits [b] (j: i32) (x: byteSeq [b])
: byteSeq [b] =
  let whichByte = (i32.i64 b) - (j/u8.num_bits) - 1
  let whichBit = j%u8.num_bits
  let bitMask = (u8.>>>) u8.highest (u8.i32 (u8.num_bits - whichBit - 1))
  in (copy x) with [whichByte] = (x[whichByte] & bitMask)

def radixPartRelation_GFUR [n] [b]
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

def radixPartRelation_GFTR [n] [b] [pL_b]
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

local type~ partitionLoopTup [b] = {iter: idx_t.t, pBounds1: []idx_t.t, pDepths1: []i32, pXs1: [](byteSeq [b])}

-- TODO
-- this works in repl
-- doesn't compile
-- (...) remake so that it does
-- maybe use from C ??? since futhark doesn't seem to like this ...
def reorganisePartitions_GFUR [n] [b]
  (block_size: i16)
  (pInfo: partitionInfo)
  (pXs: [n](byteSeq [b]))
  (maxPartitionSize: idx_t.t)
  (stopAtDepth: i32)
: ([n](byteSeq [b]), partitionInfo) =
  let outer_loop : ([][]u8, partitionInfo, bool)
  = loop p_outer = (pXs, pInfo, true)
  while (p_outer.1.maxDepth < stopAtDepth && p_outer.2) do
    let pXs0 = p_outer.0
    let pInfo0 = p_outer.1
    let bounds0 = pInfo0.bounds
    let np0 = length bounds0
    let criticalBoundIdxs = iota np0
      |> map (\i ->
        if i<(np0-1)
        then (bounds0[i], bounds0[i+1])
        else (bounds0[i], n)
      )
      |> zip (iota (np0))
      |> filter (\(i, (b1, b2)) -> (b2-b1)>maxPartitionSize)
      |> map (.0)
    let n_cb = length criticalBoundIdxs
    in
      if (n_cb == 0) then (pXs0, pInfo0, false) else
      let newDepth = pInfo0.maxDepth + 1
      let new_i = pInfo0.radixSize*newDepth
      let new_j = i32.min (new_i+pInfo0.radixSize-1) ((i32.i64 b)*u8.num_bits-1)
      let piHead = copy bounds0[0:criticalBoundIdxs[0]]
      let piDepths = copy pInfo0.depths[0:criticalBoundIdxs[0]]
      let xHead = copy pXs0[0: bounds0[criticalBoundIdxs[0]]]
      let loop_over : partitionLoopTup [b]
      = loop p = {iter=0, pBounds1=piHead, pDepths1=piDepths, pXs1 = xHead}
      while p.iter<n_cb do
        let startIdx = bounds0[criticalBoundIdxs[p.iter]]
        let endIdx = if (criticalBoundIdxs[p.iter]<(np0-1)) then bounds0[criticalBoundIdxs[p.iter]+1] else n
        let nextStartIdx = if (p.iter<(n_cb-1)) then bounds0[criticalBoundIdxs[p.iter+1]] else n
        let pXs_repartition = radixPartRelation_GFUR block_size pXs0[startIdx:endIdx] new_i new_j
        let repartitionInfo_raw = getPartitionBounds newDepth pXs_repartition new_i new_j
        let repartitionBounds = repartitionInfo_raw.bounds |> map (\ind -> ind + startIdx)
        let next_Xs = pXs0[endIdx:nextStartIdx]
        let nextBoundsFrom = if (p.iter<np0) then criticalBoundIdxs[p.iter]+1 else 0
        let nextBoundsTo = if (p.iter<(n_cb-1)) then criticalBoundIdxs[p.iter+1] else if (p.iter<np0) then np0 else 0
        let nextBounds = bounds0[nextBoundsFrom:nextBoundsTo]
        in {
          iter = p.iter+1,
          pBounds1 = p.pBounds1 ++ repartitionBounds ++ nextBounds,
          pDepths1 = p.pDepths1 ++ (replicate (length repartitionBounds) newDepth) ++ pInfo0.depths[nextBoundsFrom:nextBoundsTo],
          pXs1 = p.pXs1 ++ pXs_repartition ++ next_Xs
        }
      in (
        loop_over.pXs1,
        {
          totalBytes = pInfo0.totalBytes,
          radixSize = pInfo0.radixSize,
          maxDepth = newDepth,
          bounds = loop_over.pBounds1,
          depths = loop_over.pDepths1
        },
        true
      )
    in (outer_loop.0 :> [n][b]u8, outer_loop.1)

def main (init_j: i32) (fin_size: i64) =
  let xs1 = (iota 16) |> map (u8.i64) |> map (\x -> 15 - x) |> map (\x -> if (x%2 == 0) then (x+1) else (x-1))
  let xs2 = (iota 16) |> map (u8.i64) |> map (/2)
  let xs3 = ((iota 8) |> map (u8.i64) |> map (*2)) ++ ((iota 8) |> map (u8.i64) |> map (\x -> x*2+1)) :> [16]u8
  let xs : [16](byteSeq [3]) = map3 (\x1 x2 x3 -> [x1, x2, x3]) xs3 xs1 xs2--xs1 xs2 (replicate 16 0)
  let ys = radixPartRelation_GFUR 256 xs 0 init_j
  let bs = getPartitionBounds 0 ys 0 init_j
  in reorganisePartitions_GFUR 256 bs ys fin_size (3*16)