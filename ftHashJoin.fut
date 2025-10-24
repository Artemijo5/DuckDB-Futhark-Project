import "ftbasics"

type byteSeq [bytes] = [bytes]u8

type~ partitionInfo = {maxDepth: i32, bounds: []idx_t.t, depths: []i32}

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
  let j_ = i32.min j ((i32.i64 b)*u8.num_bits - 1) in
  let firstByte = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let lastByte_ = i64.i32 ((i32.i64 b) - (j_/u8.num_bits) - 1)
  let lastByte = i64.min lastByte_ (b*(i64.i32 u8.num_bits) - 1)
  let firstBit = i%u8.num_bits
  let lastBit = j_%u8.num_bits
  let first_bitMask = u8.highest << (u8.i32 firstBit)
  let last_bitMask = (u8.>>>) u8.highest(u8.i32 (u8.num_bits - lastBit - 1))
  let mod_x = map2 (\ind v -> if (ind<=firstByte && ind>=lastByte) then v else 0) (iota b) x
  let mod1_x = (copy mod_x) with [firstByte] = mod_x[firstByte] & first_bitMask
  in (copy mod1_x) with [lastByte] = mod1_x[lastByte] & last_bitMask

def radix_to_idx [b] (radix_bits: i32) (bsq: byteSeq [b])
: idx_t.t =
  map2
    (\byte ind -> ((i64.u8 byte) * (1 << (ind*(i64.i32 u8.num_bits)) ) ) )
    (getRadix 0 (radix_bits-1) bsq)
    (indices bsq |> map (\i -> b-i-1))
  |> i64.sum

def byteSeq_eq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  in all (id) (map2 (==) r1 r2)

def byteSeq_neq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  in any (id) (map2 (!=) r1 r2)

def byteSeq_leq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
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

def byteSeq_lt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
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

def byteSeq_geq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
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

def byteSeq_gt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
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

def radix_eq [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  byteSeq_eq 0 (num_bits*depth-1) x1 x2

def radix_neq [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  byteSeq_neq 0 (num_bits*depth-1) x1 x2

def radix_leq [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_lt = false, def_gt = false, curDepth = 0}
    while (!p.def_lt && !p.def_gt && p.curDepth < depth) do
      let dlt = byteSeq_lt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      let dgt = byteSeq_gt (num_bits*p.curDepth) (num_bits*(p.curDepth)-1) x1 x2
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        curDepth = if !(dlt || dgt) then p.curDepth+1 else depth
      }
  in (loop_over.def_lt || (!loop_over.def_gt))

def radix_lt [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_lt = false, curDepth = 0}
    while (!p.def_lt && p.curDepth < depth) do
      let dlt = byteSeq_lt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      let dgt = byteSeq_gt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      in
      {
        def_lt = dlt,
        curDepth = if !(dlt || dgt) then p.curDepth+1 else depth
      }
  in loop_over.def_lt

def radix_geq [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_lt = false, def_gt = false, curDepth = 0}
    while (!p.def_lt && !p.def_gt && p.curDepth < depth) do
      let dlt = byteSeq_lt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      let dgt = byteSeq_gt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        curDepth = if !(dlt || dgt) then p.curDepth+1 else depth
      }
  in (loop_over.def_gt || (!loop_over.def_lt))

def radix_gt [b] (num_bits: i32) (depth: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let loop_over =
    loop p = {def_gt = false, curDepth = 0}
    while (!p.def_gt && p.curDepth < depth) do
      let dlt = byteSeq_lt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      let dgt = byteSeq_gt (num_bits*p.curDepth) (num_bits*(p.curDepth+1)-1) x1 x2
      in
      {
        def_gt = dgt,
        curDepth = if !(dlt || dgt) then p.curDepth+1 else depth
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
  let j_ = i32.min j ((i32.i64 b)*u8.num_bits - 1) in
  loop (xs, pL)
  for bit in (i..(i+bit_step)...(j_))
  do radix_part_step block_size xs pL bit (i32.min j_ (bit+bit_step-1))

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
    |> filter (\(_, rs) -> any (\(r1, r2) -> r1 != r2) rs)
    |> map (.0)
  in {
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

type partitionedSet_GFTR [n] [b] [pL_b] = {ks: [n](byteSeq [b]), pL: [n](byteSeq [pL_b])}
type partitionedSet_GFUR [n] [b] = {ks: [n](byteSeq [b]), idx: [n]idx_t.t}


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

def partitionMatchBounds [nR] [nS] [b] [pR] [pS] 't
  (radix_size: i32)
  (tR : [nR](byteSeq [b]))
  (tS : [nS](byteSeq [b]))
  (bounds_R: [pR]idx_t.t) -- without offset
  (bounds_S: [pS]idx_t.t) -- without offset
  (depths_R: [pR]i32)
  (depths_S: [pS]i32)
  (ht_Ss : radix_hashTable [i64.i32 radix_size])
: [](idx_t.t, idx_t.t, idx_t.t) = -- returns: r partition, first s match, last s match
  let ht_S = ht_Ss.first_info_idx
  in (indices bounds_R)
    |> map (\(rpi: i64) ->
      let j = radix_to_idx radix_size tR[bounds_R[rpi]]
      let matches =
        let spi = ht_S[j]
        in
          -- case 1 - partition not present in either of the sets -> no match
          if (spi == -1)
          then (-1, -1)
          -- case 2 - partition present in both sets, at most one deep -> match with first & last match
          else if (depths_R[rpi] == 1 || depths_S[spi] == 1)
          then (spi, ht_Ss.last_info_idx[j])
          -- case 3 - partition deep in both R and S -> binary search for first and last match
          else
            let cur_R = tR[bounds_R[rpi]]
            let last_potential_spi = ht_Ss.last_info_idx[j]
            -- do binary search in range [spi, last_potential_spi]
            let init_step = idx_t.max 1 ((last_potential_spi - spi)/2)
            let bsearch_first =
              loop (first_spi, step) = (spi, init_step)
              while step>0 do
                let cur_S = tS[bounds_S[first_spi]]             
                let cur_depth = i32.min depths_R[rpi] depths_S[first_spi]              
                let prev_S = if (first_spi > spi) then tS[bounds_S[first_spi-1]] else cur_S
                let prev_depth = if (first_spi > spi) then (i32.min depths_R[rpi] depths_S[first_spi-1]) else cur_depth
                let next_S = if (first_spi < last_potential_spi) then tS[bounds_S[first_spi+1]] else cur_S
                let next_depth = if (first_spi < last_potential_spi) then (i32.min depths_R[rpi] depths_S[first_spi+1]) else cur_depth
                in if
                  (radix_eq radix_size cur_depth cur_R cur_S) && (first_spi==spi || radix_neq radix_size prev_depth cur_R prev_S)
                then
                  (first_spi, 0)
                else if
                  (radix_gt radix_size cur_depth cur_R cur_S)
                then
                  if first_spi==last_potential_spi then (-1, 0) else
                  if (radix_lt radix_size next_depth cur_R next_S) then (-1, 0) else
                  (first_spi + step, idx_t.max 1 (step/2))
                else -- radix_lt radix_size cur_depths cur_R cur_S
                  if first_spi==spi then (-1, 0) else
                  if (radix_gt radix_size prev_depth cur_R prev_S) then (-1, 0) else
                  (first_spi - step, idx_t.max 1 (step/2))
            let bsearch_last =
              if bsearch_first.0 < 0 then (bsearch_first.0, 0) else
              loop (last_spi, step) = (last_potential_spi, init_step)
              while step>0 do
                let cur_S = tS[bounds_S[last_spi]]             
                let cur_depth = i32.min depths_R[rpi] depths_S[last_spi]              
                let prev_S = if (last_spi > spi) then tS[bounds_S[last_spi-1]] else cur_S
                let prev_depth = if (last_spi > spi) then (i32.min depths_R[rpi] depths_S[last_spi-1]) else cur_depth
                let next_S = if (last_spi < last_potential_spi) then tS[bounds_S[last_spi+1]] else cur_S
                let next_depth = if (last_spi < last_potential_spi) then (i32.min depths_R[rpi] depths_S[last_spi+1]) else cur_depth
                in if
                  (radix_eq radix_size cur_depth cur_R cur_S) && (last_spi==last_potential_spi || radix_neq radix_size next_depth cur_R next_S)
                then
                  (last_spi, 0)
                else if
                  (radix_gt radix_size cur_depth cur_R cur_S)
                then
                  if last_spi==last_potential_spi then (-1, 0) else
                  if (radix_lt radix_size next_depth cur_R next_S) then (-1, 0) else
                  (last_spi + step, idx_t.max 1 (step/2))
                else -- radix_lt radix_size cur_depths cur_R cur_S
                  if last_spi==spi then (-1, 0) else
                  if (radix_gt radix_size prev_depth cur_R prev_S) then (-1, 0) else
                  (last_spi - step, idx_t.max 1 (step/2))
            in (bsearch_first.0, bsearch_last.0)
          in (rpi, matches.0, matches.1)
    )
    |> filter (\(_, f, l) -> (f >= 0) && (l >= 0))

def do_find_joinPairs [nR] [nS] [b]
  (tR: [nR](byteSeq [b]))
  (tS: [nS](byteSeq [b]))
  (offset_R : idx_t.t)
  (offset_S : idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_bsq [b] =
  if nR > 6 then
    let matchmaking = loop
      (dest_vs: [](byteSeq [b]), dest_ix: []idx_t.t, dest_iy: []idx_t.t) = ([], [], [])
      for j in (iota nS) do
        let sv = tS[j]
        let matchArr = tR |> map (\rv -> all (id) (map2 (==) rv sv))
        let match_count = matchArr |> countFor (id)
        let scatter_idxs = map2
          (\og offs -> if og then offs else -1)
          matchArr
          (exscan (+) 0 (matchArr |> map (idx_t.bool)))
        let newVs = replicate match_count sv
        let newIy = replicate match_count j
        let newIx = partitioned_scatter scatter_psize (replicate match_count 0) scatter_idxs (iota nR)
        in (
          dest_vs ++ newVs,
          dest_ix ++ newIx,
          dest_iy ++ newIy,
        )
    in {vs = matchmaking.0, ix = matchmaking.1 |> map (\ind -> ind + offset_R), iy = matchmaking.2 |> map (\ind -> ind + offset_S)}
  else
    let seq_matches = loop
    (dest_vs: [](byteSeq [b]), dest_ix: []idx_t.t, dest_iy: []idx_t.t) = ([], [], [])
    for i in (iota nR) do
      let rv = tR[i]
      let matches =
        loop (this_iy: []idx_t.t) = [] for j in (iota nS) do
        let sv = tS[j]
        in
          if (all (id) (map2 (==) rv sv))
          then this_iy ++ [j]
          else this_iy
      let match_count = length matches
      let new_vs = replicate match_count rv
      let new_ix = replicate match_count i
      in (dest_vs ++ new_vs, dest_ix ++ new_ix, dest_iy ++ matches)
    in {vs = seq_matches.0, ix = seq_matches.1 |> map (\ind -> ind + offset_R), iy = seq_matches.2 |> map (\ind -> ind + offset_S)}

def find_joinPairs [nR] [nS] [b]
  (tR: [nR](byteSeq [b]))
  (tS: [nS](byteSeq [b]))
  (offset_R : idx_t.t)
  (offset_S : idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_bsq [b] =
  if nS <= nR
  then do_find_joinPairs tR tS offset_R offset_S scatter_psize
  else
    let switched = do_find_joinPairs tS tR offset_S offset_R scatter_psize
    in {vs = switched.vs, ix = switched.iy, iy = switched.ix}

def join_hashPartitions [nR] [nS] [b]
  (pR : [nR](byteSeq [b]))
  (pS : [nS](byteSeq [b]))
  (r_info : partitionInfo)
  (s_info : partitionInfo)
  (r_partition_matches : [](idx_t.t, idx_t.t, idx_t.t))
  (scatter_psize: idx_t.t)
: joinPairs_bsq [b] =
  let ir_len = length r_info.bounds
  let is_len = length s_info.bounds
  in
    loop p = {vs = [], ix = [], iy = []}
    for p_match in r_partition_matches do
      let r_lb = r_info.bounds[p_match.0]
      let r_ub = if (p_match.0 < ir_len-1) then r_info.bounds[p_match.0 + 1] else nR
      let r_part = pR[r_lb:r_ub]
      let tR_matches : joinPairs_bsq [b] =
        loop q = {vs = [], ix = [], iy = []}
        for s_part_ii in ((p_match.1)...(p_match.2)) do
          let s_lb = s_info.bounds[s_part_ii]
          let s_ub = if (s_part_ii < is_len-1) then s_info.bounds[s_part_ii+1] else nS
          let s_part = pS[s_lb:s_ub]
          let new_q = find_joinPairs r_part s_part r_lb s_lb scatter_psize
          in {vs = q.vs ++ new_q.vs, ix = q.ix ++ new_q.ix, iy = q.iy ++ new_q.iy}
      in {vs = p.vs ++ tR_matches.vs, ix = p.ix ++ tR_matches.ix, iy = p.iy ++ tR_matches.iy}
