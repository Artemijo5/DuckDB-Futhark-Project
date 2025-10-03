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

def radix_part_step [n][b] 't 
  (block_size: idx_t.t)
  (xs: [n][b]u8)
  (pL: [n]t)
  (i: i32)
  (j: i32)
: ([n][b]u8, [n]t) =
  let xps = zip xs pL
  let xps' = radix_sort_multistep block_size i j (\bit xp -> byteSeq_getBit bit xp.0) xps
  in unzip xps'

def radix_part [n][b] 't
  (block_size: idx_t.t)
  (xs: [n][b]u8)
  (pL: [n]t)
  (i: i32)
  (j: i32)
  (bit_step: i32)
: ([n][b]u8, [n]t) =
  loop (xs, pL)
  for bit in (i..(i+bit_step)...j)
  do radix_part_step block_size xs pL bit (i32.min j (bit+bit_step-1))

def radixPartition_GFUR [n][b]
  (block_size: idx_t.t)
  (xs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (i: i32)
  (j: i32)
: ([n](byteSeq [b]), [n](idx_t.t)) =
  let is = (offset..<(offset + n)) :> [n]idx_t.t
  in radix_part block_size xs is i j 2

def radixPartition_GFTR [n][b][pL_b]
  (block_size: idx_t.t)
  (xs: [n](byteSeq [b]))
  (ys: [n](byteSeq [pL_b]))
  (i: i32)
  (j: i32)
: ([n](byteSeq [b]), [n](byteSeq [pL_b])) =
  radix_part block_size xs ys i j 2

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
def deepen_step_GFUR [m] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (curDepth: i32)
  (xi_bufen: ([m](byteSeq [b]), [m]idx_t.t))
  (size_thresh: idx_t.t)
: ([m](byteSeq [b]), [m]idx_t.t, [](idx_t.t, idx_t.t)) =
  let newDepth = curDepth+1
  let new_i = radix_size*curDepth
  let new_j = radix_size*(curDepth+1) - 1
  let xi_xinbufen = radix_part
    gather_psize
    xi_bufen.0
    xi_bufen.1
    new_i
    new_j
    2
  let newBounds = (getPartitionBounds newDepth xi_xinbufen.0 new_i new_j).bounds
  let taidade = -- partitions that exceed size_thresh, represented by lb (inclusive) & ub (exlusive)
    indices newBounds
    |> map (\i -> (newBounds[i], if (i<(length newBounds)-1) then newBounds[i+1] else (m)))
    |> filter (\(lb, ub) -> ub-lb > size_thresh)
  in (xi_xinbufen.0, xi_xinbufen.1, taidade)

def scan_and_deepen_GFUR [n] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: ([n](byteSeq [b]), [n]idx_t.t, partitionInfo) =
  let xis = (xs, ((offset..<(offset+n)) :> [n]idx_t.t) )
  let loop_over : {pXs: [n](byteSeq [b]), pIs: [n]idx_t.t, taidade: [](idx_t.t, idx_t.t), dp: i32}
  = loop p = {pXs = xis.0, pIs = xis.1, taidade = [(0,n)], dp = 0}
  while (length p.taidade > 0) && (p.dp<max_depth) do
    -- Iterate over xis
    -- deepen_step over taidade ranges, leave other ranges unchanged
    let inner_loop : {xbuff: [n](byteSeq [b]), ibuff: [n]idx_t.t, new_taidade: [](idx_t.t, idx_t.t)}
    = loop q = {xbuff = p.pXs, ibuff = p.pIs, new_taidade = []}
    for bounds in p.taidade do
      let m = bounds.1 - bounds.0
      let x_bufen = q.xbuff[bounds.0:bounds.1] :> [m](byteSeq [b])
      let i_bufen = q.ibuff[bounds.0:bounds.1] :> [m]idx_t.t
      let res = deepen_step_GFUR block_size gather_psize radix_size p.dp (x_bufen, i_bufen) size_thresh
      in {
        xbuff = (copy q.xbuff) with [bounds.0:bounds.1] = res.0,
        ibuff = (copy q.ibuff) with [bounds.0:bounds.1] = res.1,
        new_taidade = q.new_taidade ++ (res.2 |> map (\(lb, ub) -> (lb + bounds.0, ub + bounds.0)))
      }
    in {
      pXs = inner_loop.xbuff :> [n](byteSeq [b]),
      pIs = inner_loop.ibuff :> [n]idx_t.t,
      taidade = inner_loop.new_taidade,
      dp = p.dp + 1
    }
  -- Now, recursive partition info...
  -- TODO merge with main loop? if possible...
  let recursive_info : partitionInfo
  = loop p = getPartitionBounds 1 loop_over.pXs 0 (radix_size-1)
  while (p.maxDepth < loop_over.dp) do
    let depths = p.depths
    let bs = p.bounds
    let taidade = indices bs
      |> map (\i -> (bs[i], if (i<(length bs)-1) then bs[i+1] else (n), i))
      |> filter (\(lb, ub, i) -> ub-lb > size_thresh)
    let inner_info : partitionInfo
    = loop q = p
    for bounds in taidade do
      let m = bounds.1-bounds.0
      let x_bufen = loop_over.pXs[bounds.0:bounds.1] :> [m](byteSeq [b])
      let new_i = radix_size*(p.maxDepth)
      let new_j = radix_size*(p.maxDepth+1) - 1
      let deeper_info = getPartitionBounds (p.maxDepth+1) x_bufen new_i new_j
      let stitch = bounds.2
      in {
        totalBytes = i32.i64 b,
        radixSize = radix_size,
        maxDepth = p.maxDepth + 1,
        bounds = q.bounds[0:stitch] ++ (deeper_info.bounds |> map (\b -> b+bounds.0)) ++ q.bounds[stitch+1:(length q.bounds)],
        depths = q.depths[0:stitch] ++ deeper_info.depths ++ q.depths[stitch+1:(length q.depths)]
      }
    in inner_info
  in (
    loop_over.pXs,
    loop_over.pIs,
    recursive_info
  )


def main (max_depth : i32) =
  let size_thresh = 2
  let xs : [8](byteSeq [2]) = [[4,2],[2,1],[2,2],[3,5],[2,5],[3,1],[4,5],[3,7]]
  let res = scan_and_deepen_GFUR 256 256 4 xs 14 2 max_depth
  in res.2.bounds
