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

type radix_hashTable [rb] = {first_info_idx: [2**rb]idx_t.t, last_info_idx: [2**rb]idx_t.t} -- if idx == -1, partition is not present

def dummy_byteSeq (b: idx_t.t)
: byteSeq [b] =
  replicate b 0

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

def radix_eq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  in all (\(x1, x2) -> x1 == x2) (zip r1 r2)

def radix_neq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  in any (\(x1, x2) -> x1 != x2) (zip r1 r2)

def radix_leq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let loop_over =
    loop p = {def_lt = false, def_gt = false, byte = 0}
    while (!p.def_lt && !p.def_gt && p.byte<b) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in (loop_over.def_lt || (!loop_over.def_gt))

def radix_lt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let loop_over =
    loop p = {def_lt = false, byte = 0}
    while (!p.def_lt && p.byte<b) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in loop_over.def_lt

def radix_geq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let loop_over =
    loop p = {def_lt = false, def_gt = false, byte = 0}
    while (!p.def_lt && !p.def_gt && p.byte<b) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in (loop_over.def_gt || (!loop_over.def_lt))

def radix_gt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let loop_over =
    loop p = {def_gt = false, byte = 0}
    while (!p.def_gt && p.byte<b) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else b
      }
  in loop_over.def_gt

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
  (scatter_psize : idx_t.t)
: radix_hashTable [i64.i32 x_info.radixSize] =
  let m = length x_info.bounds
  let is_base = iota m
  let rs = i64.i32 x_info.radixSize
  let scatter_is_withMultiplicity = is_base
    |> map (\i -> get_radix 0 (i32.i64 rs-1) (byteSeq_getBit) pXs[ x_info.bounds[i] ])
    |> map (i64.u8)
  let is_first_last =
    if x_info.maxDepth == 1
    then replicate m (true, true)
    else is_base
      |> map (\i ->
        let cur_i = scatter_is_withMultiplicity[i]
        let pre_i = if i==0 then -1 else scatter_is_withMultiplicity[i-1]
        let is_first = (cur_i != pre_i)
        let pos_i = if i==(m-1) then -1 else scatter_is_withMultiplicity[i+1]
        let is_last = (cur_i != pos_i)
        in (is_first, is_last)
      )
  let scatter_is_first = map2 (\(is_first, _) i -> if is_first then scatter_is_withMultiplicity[i] else -1) is_first_last is_base
  let scatter_is_last = map2 (\(_, is_last) i -> if is_last then scatter_is_withMultiplicity[i] else -1) is_first_last is_base
  let first_partitionIndices = partitioned_scatter scatter_psize (replicate (2**rs) (-1)) scatter_is_first is_base
  let last_partitionIndices = partitioned_scatter scatter_psize (replicate (2**rs) (-1)) scatter_is_last is_base
  --let isPartitionDeep = (partitioned_scatter scatter_psize (replicate (2**rs) (true, true)) scatter_is is_first_last)
  --  |> map (\(is_first, is_last) -> !(is_first && is_last))
  --in {info_idx = partitionIndices, isDeep = isPartitionDeep}
  in {first_info_idx = first_partitionIndices, last_info_idx = last_partitionIndices}

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

-- | The pairs obtained from joining byteseq arrays x&y.
-- vs : the values of each pair
-- ix : the respective index in x
-- iy : the respective index in y
type~ joinPairs_bsq [b] = {vs: [](byteSeq [b]), ix: []idx_t.t, iy: []idx_t.t}

-- TODO test ...
def partitionMatchBounds [n] [b] [pR] [pS] 't
  (radix_size: i32)
  (tR : [n](byteSeq [b]))
  (tS : [n](byteSeq [b]))
  (bounds_R: [pR]idx_t.t) -- without offset
  (bounds_S: [pS]idx_t.t) -- without offset
  (depths_R: [pR]i32)
  (depths_S: [pS]i32)
  (ht_Rs : radix_hashTable [i64.i32 radix_size])
  (ht_Ss : radix_hashTable [i64.i32 radix_size])
  (extParallelism: idx_t.t)
: [](idx_t.t, idx_t.t, idx_t.t) =
  let ht_R = ht_Rs.first_info_idx
  let ht_S = ht_Ss.first_info_idx
  let pow2 = i64.i32 (2**radix_size)
  in (iota pow2)
    |> map (\(j: i64) ->
      let rpi = ht_R[j]
      let matches =
        let spi = ht_S[j]
        in
          -- case 1 - partition not present in either of the sets -> no match
          if (rpi == -1 || spi == -1)
          then (-1, -1)
          -- case 2 - partition present in both sets, at most one deep -> match with first & last match
          else if (depths_R[spi] == 1 || depths_S[spi] == 1)
          then (spi, ht_Ss.last_info_idx[j])
          -- case 3 - partition deep in both R and S -> binary search for first and last match
          else
            let cur_R = tR[bounds_R[rpi]]
            let last_potential_spi = ht_Ss.last_info_idx[j]
            -- do binary search in range [spi, last_potential_spi]
            let init_step = idx_t.min 1 ((last_potential_spi - spi)/2)
            let bsearch_first =
              loop (first_spi, step) = (spi, init_step)
              while step>0 do
                let cur_S = tS[bounds_S[first_spi]]             
                let cur_depth = i32.min depths_R[rpi] depths_S[first_spi]              
                let last_bit = radix_size*cur_depth - 1
                let prev_S = if (first_spi > spi) then tS[bounds_S[first_spi-1]] else cur_S
                let prev_depth = if (first_spi > spi) then (i32.min depths_R[rpi] depths_S[first_spi-1]) else cur_depth
                let prev_last_bit = radix_size*prev_depth - 1
                let next_S = if (first_spi < last_potential_spi) then tS[bounds_S[first_spi+1]] else cur_S
                let next_depth = if (first_spi < last_potential_spi) then (i32.min depths_R[rpi] depths_S[first_spi+1]) else cur_depth
                let next_last_bit = radix_size*next_depth - 1
                in if
                  (radix_eq 0 last_bit cur_R cur_S) && (first_spi==spi || radix_neq 0 prev_last_bit cur_R prev_S)
                then
                  (first_spi, 0)
                else if
                  (radix_gt 0 last_bit cur_R cur_S)
                then
                  if first_spi==last_potential_spi then (-1, 0) else
                  if (radix_lt 0 next_last_bit cur_R next_S) then (-1, 0) else
                  (first_spi + step, idx_t.min 1 (step/2))
                else -- radix_lt 0 cur_bit cur_R cur_S
                  if first_spi==spi then (-1, 0) else
                  if (radix_gt 0 prev_last_bit cur_R prev_S) then (-1, 0) else
                  (first_spi - step, idx_t.min 1 (step/2))
            let bsearch_last =
              if bsearch_first.0 == -1 then (-1, 0) else
              loop (last_spi, step) = (last_potential_spi, init_step)
              while step>0 do
                let cur_S = tS[bounds_S[last_spi]]             
                let cur_depth = i32.min depths_R[rpi] depths_S[last_spi]              
                let last_bit = radix_size*cur_depth - 1
                let prev_S = if (last_spi > spi) then tS[bounds_S[last_spi-1]] else cur_S
                let prev_depth = if (last_spi > spi) then (i32.min depths_R[rpi] depths_S[last_spi-1]) else cur_depth
                let prev_last_bit = radix_size*prev_depth - 1
                let next_S = if (last_spi < last_potential_spi) then tS[bounds_S[last_spi+1]] else cur_S
                let next_depth = if (last_spi < last_potential_spi) then (i32.min depths_R[rpi] depths_S[last_spi+1]) else cur_depth
                let next_last_bit = radix_size*next_depth - 1
                in if
                  (radix_eq 0 last_bit cur_R cur_S) && (last_spi==last_potential_spi || radix_neq 0 next_last_bit cur_R next_S)
                then
                  (last_spi, 0)
                else if
                  (radix_gt 0 last_bit cur_R cur_S)
                then
                  if last_spi==last_potential_spi then (-1, 0) else
                  if (radix_lt 0 next_last_bit cur_R next_S) then (-1, 0) else
                  (last_spi + step, idx_t.min 1 (step/2))
                else -- radix_lt 0 cur_bit cur_R cur_S
                  if last_spi==spi then (-1, 0) else
                  if (radix_gt 0 prev_last_bit cur_R prev_S) then (-1, 0) else
                  (last_spi - step, idx_t.min 1 (step/2))
            in (bsearch_first.0, bsearch_last.0)
          in (rpi, matches.0, matches.1)
    )
    |> filter (\(_, f, l) -> (f != -1) && (l != -1))

def find_joinTuples [nR] [nS] [b]
  (tR: [nR](byteSeq [b]))
  (tS: [nS](byteSeq [b]))
  (scatter_psize: idx_t.t)
: joinPairs_bsq [b] =
  let numPairs = tR
    |> map (\rv -> tS |> countFor (\sv -> rv==sv))
    |> idx_t.sum
  let matchmaking = loop
    (dest_vs: [numPairs](byteSeq [b]), dest_ix: [numPairs]idx_t.t, dest_iy: [numPairs]idx_t.t, offs: idx_t.t)
    = ((replicate numPairs (dummy_byteSeq b)), (replicate numPairs 0), (replicate numPairs 0), 0)
    for j in (iota nS) do
      let sv = tS[j]
      let matchArr = tR |> map (\rv -> rv == sv)
      let match_count = matchArr |> countFor (id)
      let scatter_idxs = map2
        (\og offs -> if og then offs else -1)
        matchArr
        (exscan (+) 0 (matchArr |> map (idx_t.bool)))
      let newVs = replicate match_count sv
      let newIy = replicate match_count j
      let newIx = partitioned_scatter scatter_psize (replicate match_count 0) scatter_idxs (iota nR)
      in (
        (copy dest_vs) with [offs:offs+match_count] = newVs,
        (copy dest_ix) with [offs:offs+match_count] = newIx,
        (copy dest_iy) with [offs:offs+match_count] = newIy,
        offs + match_count
      )
  in {vs = matchmaking.0, ix = matchmaking.1, iy = matchmaking.2}

-- TODO
-- function that does the join start-to-finish (...)
-- entry points (...)
  

def main (max_depth : i32) =
  let size_thresh = 2
  let xs : [8](byteSeq [2]) = [[4,2],[2,1],[2,2],[3,5],[2,5],[3,1],[4,5],[3,7]]
  let res = partition_and_deepen_GFUR 256 256 4 xs 0 2 max_depth
  --in res.2.bounds
  let res_info = calc_partitions_from_partitioned_set 4 (res.0) 0 2 max_depth
  let hashTbl = create_hash_table_from_partitioned_set (res.0) res_info 256
  in (res.0, res_info.bounds, hashTbl)
  --in hashTbl.isDeep
