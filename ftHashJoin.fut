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

type radix_hashTable [rb] = {idx: [2**rb]idx_t.t, isDeep: [2**rb]bool} -- if idx == -1, partition is not present

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
def deepen_step 't [m] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (curDepth: i32)
  (x_bufen: ([m](byteSeq [b]), [m]t))
  (size_thresh: idx_t.t)
: ([m](byteSeq [b]), [m]t, [](idx_t.t, idx_t.t)) =
  let newDepth = curDepth+1
  let new_i = radix_size*curDepth
  let new_j = radix_size*(curDepth+1) - 1
  let x_xinbufen = radix_part
    gather_psize
    x_bufen.0
    x_bufen.1
    new_i
    new_j
    2
  let newBounds = (getPartitionBounds newDepth x_xinbufen.0 new_i new_j).bounds
  let taidade = -- partitions that exceed size_thresh, represented by lb (inclusive) & ub (exlusive)
    indices newBounds
    |> map (\i -> (newBounds[i], if (i<(length newBounds)-1) then newBounds[i+1] else (m)))
    |> filter (\(lb, ub) -> ub-lb > size_thresh)
  in (x_xinbufen.0, x_xinbufen.1, taidade)

def partition_and_deepen 't [n] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (pL: [n]t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: ([n](byteSeq [b]), [n]t) =
  let xps = (xs, pL)
  let loop_over : {pXs: [n](byteSeq [b]), pPs: [n]t, taidade: [](idx_t.t, idx_t.t), dp: i32}
  = loop p = {pXs = xps.0, pPs = xps.1, taidade = [(0,n)], dp = 0}
  while (length p.taidade > 0) && (p.dp<max_depth) do
    -- Iterate over xis
    -- deepen_step over taidade ranges, leave other ranges unchanged
    let inner_loop : {xbuff: [n](byteSeq [b]), pbuff: [n]t, new_taidade: [](idx_t.t, idx_t.t)}
    = loop q = {xbuff = p.pXs, pbuff = p.pPs, new_taidade = []}
    for bounds in p.taidade do
      let m = bounds.1 - bounds.0
      let x_bufen = q.xbuff[bounds.0:bounds.1] :> [m](byteSeq [b])
      let p_bufen = q.pbuff[bounds.0:bounds.1] :> [m]t
      let res = deepen_step block_size gather_psize radix_size p.dp (x_bufen, p_bufen) size_thresh
      in {
        xbuff = (copy q.xbuff) with [bounds.0:bounds.1] = res.0,
        pbuff = (copy q.pbuff) with [bounds.0:bounds.1] = res.1,
        new_taidade = q.new_taidade ++ (res.2 |> map (\(lb, ub) -> (lb + bounds.0, ub + bounds.0)))
      }
    in {
      pXs = inner_loop.xbuff :> [n](byteSeq [b]),
      pPs = inner_loop.pbuff :> [n]t,
      taidade = inner_loop.new_taidade,
      dp = p.dp + 1
    }
  in (
    loop_over.pXs,
    loop_over.pPs
  )

def partition_and_deepen_GFTR [n] [b] [pL_b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (pL: [n](byteSeq [pL_b]))
  (size_thresh: idx_t.t)
  (max_depth: i32)
: ([n](byteSeq [b]), [n](byteSeq [pL_b])) =
  partition_and_deepen block_size gather_psize radix_size xs pL size_thresh max_depth

def partition_and_deepen_GFUR [n] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: ([n](byteSeq [b]), [n]idx_t.t) =
  let is = (offset..<(offset+n)) :> [n]idx_t.t
  in partition_and_deepen block_size gather_psize radix_size xs is size_thresh max_depth

def calc_partitions_from_partitioned_set 't [n] [b]
  (radix_size: i32)
  (pXs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth: i32)
: partitionInfo =
  let recursive_info : (partitionInfo, bool)
  = loop p = (getPartitionBounds 1 pXs 0 (radix_size-1), true)
  while (p.0.maxDepth < max_depth && p.1) do
    let depths = p.0.depths
    let bs = p.0.bounds
    let taidade = indices bs
      |> map (\i -> (bs[i], if (i<(length bs)-1) then bs[i+1] else (n), i))
      |> filter (\(lb, ub, i) -> ub-lb > size_thresh)
    let inner_info : partitionInfo
    = loop q = p.0
    for bounds in taidade do
      let m = bounds.1-bounds.0
      let x_bufen = pXs[bounds.0:bounds.1] :> [m](byteSeq [b])
      let new_i = radix_size*(p.0.maxDepth)
      let new_j = radix_size*(p.0.maxDepth+1) - 1
      let deeper_info = getPartitionBounds (p.0.maxDepth+1) x_bufen new_i new_j
      let stitch = bounds.2
      in {
        totalBytes = i32.i64 b,
        radixSize = radix_size,
        maxDepth = p.0.maxDepth + 1,
        bounds = q.bounds[0:stitch] ++ (deeper_info.bounds |> map (\b -> b+bounds.0)) ++ q.bounds[stitch+1:(length q.bounds)],
        depths = q.depths[0:stitch] ++ deeper_info.depths ++ q.depths[stitch+1:(length q.depths)]
      }
    in (inner_info, (length taidade) > 0)
  let recursive_info_with_offset = {
    totalBytes = recursive_info.0.totalBytes,
    radixSize = recursive_info.0.radixSize,
    maxDepth = recursive_info.0.maxDepth,
    bounds = recursive_info.0.bounds |> map (\b -> b + offset),
    depths = recursive_info.0.depths
  }
  in recursive_info_with_offset

def create_hash_table_from_partitioned_set [n] [b]
  (pXs : [n](byteSeq [b]))
  (x_info : partitionInfo)
  (numPartitions: idx_t.t) -- something weird is going on with the scope ...
  (scatter_psize : idx_t.t)
: radix_hashTable [i64.i32 x_info.radixSize] =
  let m = numPartitions
  let bs = copy (x_info.bounds) :> [m]idx_t.t
  let rs = i64.i32 x_info.radixSize
  let scatter_is_withMultiplicity = bs
    |> map (\i -> get_radix 0 (i32.i64 rs-1) (byteSeq_getBit) pXs[i])
    |> map (i64.u8)
  let is_first_last =
    if x_info.maxDepth == 1
    then replicate m (true, true)
    else iota m
      |> map (\i ->
        let cur_i = scatter_is_withMultiplicity[i]
        let pre_i = if i==0 then -1 else scatter_is_withMultiplicity[i-1]
        let is_first = if cur_i == pre_i then false else true
        let pos_i = if i==(m-1) then -1 else scatter_is_withMultiplicity[i+1]
        let is_last = if cur_i == pos_i then false else true
        in (is_first, is_last)
      )
  let scatter_is = (iota m)
    |> zip is_first_last
    |> map (\((is_first, is_last), i) -> if is_first then scatter_is_withMultiplicity[i] else -1)
  let partitionIndices = partitioned_scatter scatter_psize (replicate (2**rs) (-1)) scatter_is bs
  let isPartitionDeep = (partitioned_scatter scatter_psize (replicate (2**rs) (true, true)) scatter_is is_first_last)
    |> map (\(is_first, is_last) -> !(is_first && is_last))
  in {idx = partitionIndices, isDeep = isPartitionDeep}

-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- Join Code
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################

type joinTup [n] 't = {vs: [n]t, ix: [n]idx_t.t, iy: [n]idx_t.t, cm: [n]idx_t.t}

def uncooked_joinTup [nR] 't
  (tR: [nR]t)
  (offset_R: idx_t.t)
: joinTup [nR] t =
  {
    vs = copy tR,
    ix = idx_t.indicesWithIncrement (offset_R) (tR),
    iy = replicate nR (-1),
    cm = replicate nR 0
  }

-- TODO rewrite - make simpler ...
def find_joinTuples [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (extParallelism: idx_t.t)
  (neq: t -> t -> bool)
  (gt: t -> t -> bool)
: joinTup [nR] t =
  let numIter = (nR+extParallelism-1) / extParallelism
  let pre_joinTup = uncooked_joinTup (tR) (offset_R)
  let joinLoop : {iter: idx_t.t, result: joinTup [nR] t}
    = loop p = {iter = 0, result = pre_joinTup}
    while p.iter < numIter do
      let start = p.iter*extParallelism
      let iter_size = idx_t.min (extParallelism) (nR - start)
      let iter_R = tR[start : start + iter_size] :> [iter_size]t
      in -- skip if R and S do not overlap
        let r_min = iter_R[0]
        let r_max = iter_R[iter_size-1]
        let s_min = if nS>0 then tS[0] else r_min
        let s_max = if nS>0 then tS[nS-1] else r_max
        in
          if (r_min `gt` s_max) then {iter= numIter, result= p.result} -- S < iter_R -> break
          else if (s_min `gt` r_max) then {iter= p.iter+1, result= p.result} -- S > iter_R -> continue
      else -- default case
      let ircs = zip3 (idx_t.indicesWithIncrement (offset_R+start) iter_R) (iter_R) (replicate iter_size 0)
      let iscs = zip3 (idx_t.indicesWithIncrement (offset_S) tS) tS (replicate nS 1)
      let ms = ircs -- TODO optimisation: if iscs below a certain size, use loop rather than reduce (?)
        |> map (\(i, x, c) ->
          let redTup = reduce_comm (\(i1, y1, c1) (i2, y2, c2) ->
              if ((y1 `neq` x) || i1 < 0) && ((y2 `neq` x) || i2 < 0) then (-1, x, 0)
              else if ((y1 `neq` x) || i1 < 0) then (i2, y2, c2)
              else if ((y2 `neq` x) || i2 < 0) then (i1, y1, c1)
              else (idx_t.min i1 i2, x, c1+c2)
            )
            (-1, x, 0)
            iscs
          in (redTup.0, redTup.2) -- keep only necessary info - iy & cm
        )
        |> unzip
      -- TODO figure out consumption tactics...
      let nextVs = p.result.vs
      let nextIx = p.result.ix
      let nextIy = (copy p.result.iy) with [start:start+iter_size] = ms.0
      let nextCm = (copy p.result.cm) with [start:start+iter_size] = ms.1
      in {
        iter = p.iter+1,
        result = {vs = nextVs, ix = nextIx, iy = nextIy, cm = nextCm}
      }
  in joinLoop.result

-- TODO make this use O(1) hash table, and binary search for mutually deep partitions
def partitionMatchBounds [n] [pR] [pS] 't
  (tR: [n]t)
  (bounds_R: [pR]idx_t.t)
  (depths_R: [pR]i32)
  (tS: [pS]t)
  (bounds_S: [pS]idx_t.t)
  (depths_S: [pS]i32)
  (partitions_S: partitionInfo)
  (extParallelism: idx_t.t)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
: [pR](idx_t.t, idx_t.t) =
  -- TODO binary search from here
  replicate pR (0,pS)
  

def main (max_depth : i32) =
  let size_thresh = 2
  let xs : [8](byteSeq [2]) = [[4,2],[2,1],[2,2],[3,5],[2,5],[3,1],[4,5],[3,7]]
  let res = partition_and_deepen_GFUR 256 256 4 xs 0 2 max_depth
  --in res.2.bounds
  let res_info = calc_partitions_from_partitioned_set 4 (res.0) 0 2 max_depth
  let numPartitions = length (res_info.bounds)
  let hashTbl = create_hash_table_from_partitioned_set (res.0) res_info numPartitions 256
  --in (res.0, hashTbl.idx, hashTbl.isDeep)
  in hashTbl.isDeep
