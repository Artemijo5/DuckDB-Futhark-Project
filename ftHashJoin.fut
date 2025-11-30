import "ftbasics"
--import "lib/github.com/diku-dk/sorts/merge_sort"

type byteSeq [bytes] = [bytes]u8

type~ partitionInfo = {maxDepth: i32, bounds: []idx_t.t, depths: []i32}

type radix_hashTable [rb] = {first_info_idx: [2**rb]idx_t.t, last_info_idx: [2**rb]idx_t.t} -- if idx == -1, partition is not present

-- Pre-Join Code

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
    in foldl (&&) (true) (map2 (==) r1[fb:lb+1] r2[fb:lb+1])

  def byteSeq_neq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
  : bool =
    let r1 = getRadix i j x1
    let r2 = getRadix i j x2
    let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
    let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
    in foldl (||) (false) (map2 (!=) r1[fb:lb+1] r2[fb:lb+1])

  def byteSeq_leq [b] (i: i32) (j: i32) (x1: byteSeq [b]) (x2: byteSeq [b])
  : bool =
    let r1 = getRadix i j x1
    let r2 = getRadix i j x2
    let fb = i64.i32 ((i32.i64 b) - (j/u8.num_bits) - 1)
    let lb = i64.i32 ((i32.i64 b) - (i/u8.num_bits) - 1)
    let loop_over =
      loop p = {def_lt = false, def_gt = false, byte = fb}
      while (!p.def_lt && !p.def_gt && p.byte<=lb) do
        let dlt = r1[p.byte] < r2[p.byte]
        let dgt = r1[p.byte] > r2[p.byte]
        in
        {
          def_lt = dlt,
          def_gt = dgt,
          byte = if !(dlt || dgt) then p.byte+1 else lb+1
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
      while (!p.def_lt && p.byte<=lb) do
        let dlt = r1[p.byte] < r2[p.byte]
        let dgt = r1[p.byte] > r2[p.byte]
        in
        {
          def_lt = dlt,
          byte = if !(dlt || dgt) then p.byte+1 else lb+1
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
      while (!p.def_lt && !p.def_gt && p.byte<=lb) do
        let dlt = r1[p.byte] < r2[p.byte]
        let dgt = r1[p.byte] > r2[p.byte]
        in
        {
          def_lt = dlt,
          def_gt = dgt,
          byte = if !(dlt || dgt) then p.byte+1 else lb+1
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
      while (!p.def_gt && p.byte<=lb) do
        let dlt = r1[p.byte] < r2[p.byte]
        let dgt = r1[p.byte] > r2[p.byte]
        in
        {
          def_gt = dgt,
          byte = if !(dlt || dgt) then p.byte+1 else lb+1
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
    let xps' = radix_sort_multistep
      block_size i j ((i32.i64 b)*u8.num_bits) (\bit xp -> byteSeq_getBit bit xp.0) xps
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
      |> filter (\(_, rs) -> foldl (||) (false) (rs |> map (\(r1, r2) -> r1 != r2)))
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


  def calc_partInfo [n] [b]
    (radix_size: i32)
    (pXs: [n](byteSeq [b]))
    (offset: idx_t.t)
    (size_thresh: idx_t.t)
    (max_depth: i32)
  : partitionInfo =
    let recursive_info : (partitionInfo, bool)
    = loop p = (getPartitionBounds 1 pXs 0 (radix_size-1), true)
    while (p.0.maxDepth < max_depth && p.1) do
      let bs = p.0.bounds
      let taidade = indices bs
        |> map (\i -> (bs[i], if (i<(length bs)-1) then bs[i+1] else (n), i))
        |> filter (\(lb, ub, _) -> ub-lb > size_thresh)
      let (inner_info : partitionInfo, _: idx_t.t, _: idx_t.t)
      = loop (q, ad, ox) = (p.0, 0, 0)
      for bounds in taidade do
        let m = bounds.1-bounds.0
        let x_bufen = pXs[bounds.0:bounds.1] :> [m](byteSeq [b])
        let new_i = radix_size*(p.0.maxDepth)
        let new_j = radix_size*(p.0.maxDepth+1) - 1
        let deeper_info = getPartitionBounds (p.0.maxDepth+1) x_bufen new_i new_j
        let insert_len = length deeper_info.bounds
        let stitch = bounds.2 + ox - ad
        let new_q = {
          maxDepth = p.0.maxDepth + 1,
          bounds = q.bounds[0:stitch] ++ (deeper_info.bounds |> map (\b -> b+bounds.0)) ++ q.bounds[stitch+1:(length q.bounds)],
          depths = q.depths[0:stitch] ++ deeper_info.depths ++ q.depths[stitch+1:(length q.depths)]
        }
        in (new_q, ad+1, ox + insert_len)
      in (inner_info, (length taidade) > 0)
    let info_len = length recursive_info.0.bounds
    let recursive_info_with_offset = {
      maxDepth = recursive_info.0.maxDepth,
      bounds = recursive_info.0.bounds |> map (\b -> b + offset) :> [info_len]idx_t.t,
      depths = recursive_info.0.depths :> [info_len]i32
    }
    in recursive_info_with_offset

  def calc_radixHashTab [n] [b]
    (radix_size : i32)
    (pXs : [n](byteSeq [b]))
    (x_info : partitionInfo)
    (scatter_psize : idx_t.t)
  : radix_hashTable [i64.i32 radix_size] =
    let m = length x_info.bounds
    let is_base = iota m
    let rs = i64.i32 radix_size
    let scatter_is_withMultiplicity = is_base
      |> map (\i ->  radix_to_idx (i32.i64 rs) pXs[x_info.bounds[i]])
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
    let first_partitionIndices = scatter (replicate (2**rs) (-1)) scatter_is_first is_base
    let last_partitionIndices = scatter (replicate (2**rs) (-1)) scatter_is_last is_base
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
  
  def radix_hash_join [nR] [nS] [b]
    (radix_size : i32)
    (tR : [nR](byteSeq [b]))
    (tS : [nS](byteSeq [b]))
    (pS : partitionInfo)
    (ht_S : radix_hashTable [i64.i32 radix_size])
--  : joinPairs_bsq [b] =
  =
    let n_pS = length pS.bounds
    -- find matches
    -- first check ht_S
    -- then do binary search
    let direct_flm_matches = tR
      |> map (\r -> radix_to_idx radix_size r)
      |> map (\ir -> (ht_S.first_info_idx[ir], ht_S.last_info_idx[ir]))
    let need_to_binary_search = any (\(fmatch,lmatch) -> fmatch!=lmatch) direct_flm_matches
    let partition_match_bounds =
      if !need_to_binary_search
      then direct_flm_matches |> map(.0)
      else
        let init_steps = direct_flm_matches |> map (\(fmatch,lmatch) -> lmatch-fmatch+1)
        let maxRange = i64.maximum init_steps
        let num_iter = 1 + (maxRange |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
        let (bsearch_match,_) =
         loop (bsearch_range,last_steps) = (direct_flm_matches |> map (.0),init_steps)
         for _ in (iota num_iter) do
          let this_steps = last_steps |> map (\step -> i64.max 1 (step/2))
          let cmps_ = map2 (\i (min_i, max_i) ->
              if (i<0) then (-1,tS[0],tS[0],0,0) else
              let prev_elem = if i==min_i then tS[min_i] else tS[pS.bounds[i-1]]
              let prev_depth = if i==min_i then 0 else pS.depths[i-1]
              let cur_elem = tS[pS.bounds[i]]
              let cur_depth = pS.depths[i]
              --let next_elem = if i>=(max_i-1) then tS[pS.depths[n_pS-1]] else tS[pS.bounds[i+1]]
              --let next_depth = if i>=(max_i-1) then 0 else pS.depths[i+1]
              in (i, prev_elem, cur_elem, prev_depth, cur_depth)
            ) bsearch_range direct_flm_matches
          let cmps = map4 (\rv (i,prev_sv,cur_sv,prev_depth,cur_depth) this_step (min_i,max_i) ->
            if i<0 then (-1) else
            if (radix_eq radix_size cur_depth rv cur_sv) then
              if (i==min_i || radix_neq radix_size prev_depth rv prev_sv)
              then i
              else i - this_step
            else if (radix_gt radix_size cur_depth rv cur_sv) then
              if (i==max_i) -- ommitted next_sv to make tuple leaner
              then (-1)
              else i64.min max_i (i+this_step)
            else -- radix_lt radix_size cur_depth rv cur_sv
              if (i==0 || radix_gt radix_size prev_depth rv prev_sv)
              then (-1)
              else i64.max min_i (i-this_step)
            ) tR cmps_ this_steps direct_flm_matches
          in (cmps,this_steps)
        in bsearch_match
    -- sequential search
    let max_partition_size = partition_match_bounds
      |> map (\i -> if i<0 then (-1,-1) else (pS.bounds[i], if i==n_pS-1 then nS else pS.bounds[i+1]))
      |> map (\(this_p, next_p) -> next_p-this_p)
      |> i64.maximum
    let searchIs = partition_match_bounds |> map (\i -> if i<0 then (-1) else pS.bounds[i])
    -- find match counts
    let counts_per_r =
      loop cms = replicate nR 0
      for j in (iota max_partition_size) do
        map3 (\i cc rv ->
          if i<0 || (i+j)>=nS then cc else
          let sv = tS[i+j]
          let isEq = (radix_eq ((i32.i64 b)*u8.num_bits) 1 rv sv)
          in if isEq then (cc+1) else cc
        ) (searchIs :> [nR]i64) (cms :> [nR]i64) tR
    let starting_pos = 
      map2 (\c z -> if c>0 then z else (-1))
        counts_per_r
        (exscan (+) 0 counts_per_r)
    let count_pairs = idx_t.sum counts_per_r
    let pairsWithMultiplicity = counts_per_r
      |> filter (\c -> c>1)
    let max_mult =
      if (length pairsWithMultiplicity > 0)
      then idx_t.maximum pairsWithMultiplicity
      else 1
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
    let (s_inds,_) =
      loop (s_inds_, cur_k) = r_inds |> map (\(k,ir) -> (searchIs[ir],k)) |> unzip
      for _ in (iota max_partition_size) do
        let is_k = map3 (\i_s (_,ir) ck ->
          if (ck<=0) then (i_s,ck) else
          let rv = tR[ir]
          let sv = tS[i_s]
          let isEq = (byteSeq_eq 0 ((i32.i64 b)-1) rv sv)
          in if isEq then
              if ck==1
              then (i_s, ck-1)
              else (i_s+1,ck-1)
            else (i_s+1,ck)
        ) s_inds_ r_inds cur_k
        in unzip is_k
    in
      {
        vs = r_inds |> map (\(_, ir) -> tR[ir]),
        ix = r_inds |> map (.1),
        iy = s_inds
      }

  def radix_hash_join_with_S_keys_unique [b]
    (radix_size : i32)
    (tR : [](byteSeq [b]))
    (tS : [](byteSeq [b]))
    (pS : partitionInfo)
    (ht_S : radix_hashTable [i64.i32 radix_size])
  : joinPairs_bsq [b] =
    let n_pS = length pS.bounds
    -- find matches
    -- first check ht_S
    -- then do binary search
    let (direct_flm_matches) = tR
      |> map (\r -> radix_to_idx radix_size r)
      |> map (\ir -> (ht_S.first_info_idx[ir], ht_S.last_info_idx[ir]))
    let need_to_binary_search = any (\(fmatch,lmatch) -> fmatch!=lmatch) direct_flm_matches
    let partition_match_bounds =
      if !need_to_binary_search
      then direct_flm_matches |> map (.0)
      else
        let init_steps = direct_flm_matches |> map (\(fm,lm) -> lm-fm+1)
        let maxRange = i64.maximum init_steps
        let num_iter = 1 + (maxRange |> f64.i64 |> f64.log2 |> f64.ceil |> i64.f64)
        let (pmb,_) =
          loop (search_ps,last_steps) = (direct_flm_matches |> map (.0), init_steps)
          for _ in (iota num_iter) do
            let this_steps = last_steps |> map (\step -> i64.max 1 ((step+1)/2))
            let cmps_ = search_ps
              |> map (\i ->
                if (i<0) then (i,tS[0],tS[0],0,0) else
                let prev_sv = if i==0 then tS[0] else tS[pS.bounds[i-1]]
                let cur_sv = tS[pS.bounds[i]]
                let prev_depth = if i==0 then pS.depths[0] else pS.depths[i-1]
                let cur_depth = pS.depths[i]
                in (i,prev_sv,cur_sv,prev_depth,cur_depth)
              )
            let cmps = map4 (\rv (i,pv,cv,pdepth,cdepth) (min_i,max_i) (this_step) ->
              if i<0 then i else
              if radix_eq radix_size cdepth rv cv
                then i
              else if radix_gt radix_size cdepth rv cv then
                if i==max_i
                then (-1)
                else i64.min max_i (i+this_step)
              else -- radix_lt radix_size cdepth rv cv
                if (i==min_i || radix_gt radix_size pdepth rv pv)
                then (-2)
                else i64.max min_i (i-this_step)
            ) tR cmps_ direct_flm_matches this_steps
            in (cmps, this_steps)
        in pmb
    -- sequential search
    let max_partition_size = partition_match_bounds
      |> map (\i -> if i<0 then (-1,-1) else (pS.bounds[i], if i==n_pS-1 then (length tS) else pS.bounds[i+1]))
      |> map (\(this_p, next_p) -> next_p-this_p)
      |> i64.maximum
    let match_in_iy = 
      loop matches = partition_match_bounds |> map (\i -> if i<0 then (-1) else pS.bounds[i])
      for _ in (iota max_partition_size) do
        map2 (\i rv ->
          if i<0 then (-1) else
          let sv = tS[i]
          let isEq = (byteSeq_eq 0 ((i32.i64 b)-1) rv sv)
          in if isEq then i else (i+1) -- guaranteed to converge since this is the unique func
        ) matches tR
    let count_pairs = countFor (>=0) match_in_iy
    let zuowei = map2
      (\m z -> if m>=0 then z else (-1))
      match_in_iy
      ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>=0 then 1 else 0)) )
    let ix_ = scatter (replicate count_pairs (-1)) zuowei (indices tR)
    let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
    let vs_ = gather (dummy_byteSeq b) tR ix_
    in {vs=vs_, ix=ix_, iy=iy_}
    --in (direct_flm_matches, partition_match_bounds)

-- ################################################################################################################
-- Testing
-- ################################################################################################################

  def test_with_unique =
    let xs1:[][]u8= [[0,1],[0,1],[0,1],[4,1],[1,2],[2,2],[3,2],[4,2],[0,3],[2,3],[4,3],[6,3],[1,4],[1,4],[1,4],[5,4]]
    let xs2:[][]u8= [[0,1],[1,1],[4,1],[5,1],[1,2],[2,2],[7,2],[9,2],[1,3],[3,3],[5,3],[7,3],[1,4],[2,4],[5,4],[6,4]]
    let inf2 = calc_partInfo 4 xs2 0 2 3
    let tab2 = calc_radixHashTab 4 xs2 inf2 256
    in radix_hash_join_with_S_keys_unique 4 xs1 xs2 inf2 tab2

  def test =
    let xs1:[][]u8= [[0,1],[0,1],[0,1],[4,1],[1,2],[2,2],[3,2],[4,2],[0,3],[2,3],[4,3],[6,3],[1,4],[1,4],[1,4],[5,4]]
    let xs2:[][]u8= [[0,1],[0,1],[4,1],[5,1],[2,2],[2,2],[2,2],[2,2],[1,3],[3,3],[5,3],[7,3],[1,4],[2,4],[5,4],[5,4]]
    let inf1 = calc_partInfo 4 xs1 0 2 3
    let tab1 = calc_radixHashTab 4 xs1 inf1 256
    in radix_hash_join 4 xs2 xs1 inf1 tab1

  def test1 =
    let xs1:[][]u8= [[0,1],[0,1],[0,1],[4,1],[1,2],[2,2],[3,2],[4,2],[0,3],[2,3],[4,3],[6,3],[1,4],[1,4],[1,4],[5,4]]
    let xs2:[][]u8= [[0,1],[1,1],[4,1],[5,1],[1,2],[2,2],[7,2],[9,2],[1,3],[3,3],[5,3],[7,3],[1,4],[2,4],[5,4],[6,4]]
    let inf2 = calc_partInfo 4 xs2 0 2 3
    let tab2 = calc_radixHashTab 4 xs2 inf2 256
    in radix_hash_join 4 xs1 xs2 inf2 tab2

  def more_test =
    let xs1:[][]u8=[
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,3],
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,7],
      [0,0,0,0,0,0,0,3],
      [0,0,0,0,0,0,0,3],
      [0,0,0,0,0,0,0,3],
      [0,0,0,0,0,0,0,9],
      [0,0,0,0,0,0,0,9],
    ]
    let xs2:[][]u8=[
      [0,0,0,0,0,0,0,3],
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,7],
      [0,0,0,0,0,0,0,10],
      [0,0,0,0,0,0,0,5],
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,8],
      [0,0,0,0,0,0,0,2],
      [0,0,0,0,0,0,0,9],
      [0,0,0,0,0,0,0,7]
    ]
    let (sxs1,_) = partition_and_deepen 256 256 16 xs1 (indices xs1) (2048) 2 2
    let (sxs2,_) = partition_and_deepen 256 256 16 xs2 (indices xs1) (2048) 2 2
    let inf2 = calc_partInfo 16 sxs2 0 2048 2
    let tab2 = calc_radixHashTab 16 sxs2 inf2 256
    in (sxs1, sxs2, radix_hash_join 16 sxs1 sxs2 inf2 tab2)
    
