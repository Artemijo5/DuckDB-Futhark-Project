import "ftbasics"

type byteSeq [bytes] = [bytes]u8

type~ partitionInfo = {maxDepth: i32, bounds: []idx_t.t, depths: []i32}

-- TODO consider using Option type here (?)
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
  let rdx = getRadix 0 (radix_bits-1) bsq
  let radix_bytes = i64.i32 ((radix_bits + u8.num_bits - 1)/(u8.num_bits))
  in
    loop y : idx_t.t = 0
    for j in (0..<radix_bytes) do
      let r = (i64.u8 rdx[b-j-1]) << (j*(i64.i32 u8.num_bits))
      in y | r

def byteSeq_eq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  in all (id) (map2 (==) r1[fb:lb+1] r2[fb:lb+1])

def byteSeq_neq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  in any (id) (map2 (!=) r1[fb:lb+1] r2[fb:lb+1])

def byteSeq_leq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let loop_over =
    loop p = {def_lt = false, def_gt = false, byte = fb}
    while (!p.def_lt && !p.def_gt && p.byte<lb) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else lb
      }
  in (loop_over.def_lt || (!loop_over.def_gt))

def byteSeq_lt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let loop_over =
    loop p = {def_lt = false, byte = fb}
    while (!p.def_lt && p.byte<lb) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        byte = if !(dlt || dgt) then p.byte+1 else lb
      }
  in loop_over.def_lt

def byteSeq_geq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let loop_over =
    loop p = {def_lt = false, def_gt = false, byte = fb}
    while (!p.def_lt && !p.def_gt && p.byte<lb) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_lt = dlt,
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else lb
      }
  in (loop_over.def_gt || (!loop_over.def_lt))

def byteSeq_gt [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
: bool =
  let r1 = getRadix i j x1
  let r2 = getRadix i j x2
  let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
  let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
  let loop_over =
    loop p = {def_gt = false, byte = fb}
    while (!p.def_gt && p.byte<lb) do
      let dlt = r1[p.byte] < r2[p.byte]
      let dgt = r1[p.byte] > r2[p.byte]
      in
      {
        def_gt = dgt,
        byte = if !(dlt || dgt) then p.byte+1 else lb
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

def deepen_step 't [m] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (curDepth: i32)
  (x_bufen: ([m](byteSeq [b]), [m]t))
  (size_thresh: idx_t.t)
  (bit_step: i32)
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
    bit_step
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
  (bit_step: i32)
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
      let res = deepen_step block_size gather_psize radix_size p.dp (x_bufen, p_bufen) size_thresh bit_step
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

def rv_partitionMatchBounds [nR] [b] [pR]
  (radix_size: i32)
  (rv: byteSeq [b])
  (tS: [nR](byteSeq [b]))
  (bounds_S: [pR]idx_t.t)
  (depths_S: [pR]i32)
  (ht_S: radix_hashTable [i64.i32 radix_size])
: idx_t.t = -- returns matching partition in S if there is one, otherwise -1
  let j = radix_to_idx radix_size rv
  let spi = ht_S.first_info_idx[j]
  in
    if spi < 0 then (-1) else
    if depths_S[spi] == 1 then (spi) else
    -- binary search for first and last matching partitions
    let end_spi = ht_S.last_info_idx[j]
    let init_step = idx_t.max 1 ((end_spi-spi)/2)
    let bsearch =
      loop (heshi, step) = (spi, init_step) while step>0 do
        let cur_depth = depths_S[heshi]
        let prev_depth = if heshi==spi then cur_depth else depths_S[heshi-1]
        let next_depth = if heshi==end_spi then cur_depth else depths_S[heshi+1]
        let cur_S = tS[bounds_S[heshi]]
        let prev_S = if heshi==spi then cur_S else tS[bounds_S[heshi-1]]
        let next_S = if heshi==end_spi then cur_S else tS[bounds_S[heshi+1]]
        in
          if
            (radix_eq radix_size cur_depth rv cur_S)
            &&
            (heshi == spi || radix_neq radix_size prev_depth rv prev_S)
          then (heshi, 0) -- found first match
          else if (radix_eq radix_size cur_depth rv cur_S) -- found match range but not first
          then (heshi-step, idx_t.max 1 (step/2))
          else if (radix_lt radix_size cur_depth rv cur_S) then
            if (heshi == spi || radix_gt radix_size prev_depth rv prev_S)
            then (-1, 0)
            else (heshi-step, idx_t.max 1 (step/2))
          else -- if (radix_gt radix_size cur_depth rv cur_S) then
            if (heshi == end_spi || radix_lt radix_size next_depth rv next_S)
            then (-1, 0)
            else (heshi+step, idx_t.max 1 (step/2))
    in bsearch.0

def rv_findPairCount [nS] [b]
  (rv: byteSeq [b])
  (tS: [nS](byteSeq [b]))
: (idx_t.t, idx_t.t)  = -- returns pair count & index of first match
  let cj =
    loop (count, j, i0) = (0, 0, -1) while j<nS do
      if all (id) (map2 (==) rv tS[j])
      then (count+1, j+1, if i0<0 then j else i0)
      else (count, j+1, i0)
  in (cj.0, cj.2)

def rv_find_kth_match [nS] [b]
  (rv: byteSeq [b])
  (tS: [nS](byteSeq [b]))
  (k: idx_t.t)
: idx_t.t =
  let cj =
    loop (count, j) = (0, 0) while (count<k && j<nS) do
      if all (id) (map2 (==) rv tS[j])
      then (count+1, j+1)
      else (count, j+1)
  in (cj.1-1)

def rv_find_match_if_exists [nS] [b]
  (rv: byteSeq [b])
  (tS: [nS](byteSeq [b]))
: idx_t.t =
  let cj =
    loop (count, j) = (0, 0) while (count<1 && j<nS) do
      if all (id) (map2 (==) rv tS[j])
      then (count+1, j+1)
      else (count, j+1)
  in 
    if ( cj.1<nS || (all (id) (map2 (==) rv tS[nS-1])) )
    then cj.1-1
    else -1

def radix_hash_join [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let n_pS = length pS.bounds
  -- tuples of: first matching partition in S, last matching partition in S
  let heshi = (iota nR)
    |> map (\i ->
      rv_partitionMatchBounds radix_size tR[i] tS pS.bounds pS.depths ht_S
    )
  let (counts_per_r, first_match_per_r) = unzip
    (
      map2 (\i fm ->
          if fm<0 then (0,-1) else
          let rv = tR[i]
          let inf_s_idx = if fm >= 0 then pS.bounds[fm] else 0
          let sup_s_idx = if fm==n_pS-1 then nS else pS.bounds[fm+1]
          let cur_S = tS[inf_s_idx:sup_s_idx]
          in 
            if fm >= 0
            then 
              let (cm, mi) = rv_findPairCount rv cur_S
              in (cm, mi+inf_s_idx)
            else (0,-1)
        )
        (iota nR)
        heshi
    )
  let starting_pos = 
    map2 (\c z -> if c>0 then z else (-1))
      counts_per_r
      (exscan (+) 0 counts_per_r)
  let count_pairs = idx_t.sum counts_per_r
  let pairsWithMultiplicity = counts_per_r
    |> zip starting_pos
    |> filter (\(_, c) -> c>1)
  let max_mult =
    if (length pairsWithMultiplicity > 0)
    then pairsWithMultiplicity
      |> map (\(_,c) -> c)
      |> idx_t.maximum
    else 1
  -- TODO test
  let r_inds : [count_pairs](idx_t.t, idx_t.t)
    = loop curBuff = (scatter (replicate count_pairs 0) starting_pos (iota nR))
      |> zip (replicate count_pairs (1))
    for iter in (1..<max_mult) do
      let this_scatter_idxs = iota nR
        |> map (\i ->
          if counts_per_r[i]<=iter
          then (-1)
          else (starting_pos[i]+iter)
        )
      in scatter (copy curBuff) this_scatter_idxs (zip (replicate nR (iter+1)) (iota nR))
  let s_inds = r_inds
    |> map (\(k, ir) ->
      let rv = tR[ir]
      let fm = heshi[ir]
      let inf_s_idx = first_match_per_r[ir]
      let sup_s_idx = if fm==n_pS-1 then nS else pS.bounds[fm+1]
      let cur_S = tS[inf_s_idx:sup_s_idx]
      in (rv_find_kth_match rv cur_S k ) + inf_s_idx
    )
  in
    {
      vs = r_inds |> map (\(_, ir) -> tR[ir]),
      ix = r_inds |> map (.1),
      iy = s_inds
    }

def radix_hash_join_with_S_keys_unique [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let n_pS = length pS.bounds
  -- tuples of: first matching partition in S, last matching partition in S
  let heshi = (iota nR)
    |> map (\i ->
      rv_partitionMatchBounds radix_size tR[i] tS pS.bounds pS.depths ht_S
    )
  let match_in_iy = map2
    (\j fm ->
      let rv = tR[j]
      let inf_s_idx = if fm >= 0 then pS.bounds[fm] else 0
      let sup_s_idx = if fm==n_pS-1 then nS else pS.bounds[fm+1]
      let cur_S = tS[inf_s_idx:sup_s_idx]
      let si = rv_find_match_if_exists rv cur_S
      in
        if si>0
        then si+inf_s_idx
        else -1
    )
    (iota nR)
    heshi
  let count_pairs = countFor (>0) match_in_iy
  let zuowei = map2
    (\m z -> if m>0 then z else (-1))
    match_in_iy
    ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>0 then 1 else 0)) )
  let ix_ = scatter (replicate count_pairs (-1)) zuowei (iota nR)
  let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
  let vs_ = gather (dummy_byteSeq b) tR ix_
  in {vs=vs_, ix=ix_, iy=iy_}
