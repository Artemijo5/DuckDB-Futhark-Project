import "../ftbasics"
import "../lib/github.com/diku-dk/sorts/merge_sort"
import "../lib/github.com/diku-dk/segmented/segmented"

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

def partition_and_deepen 't [n] [b]
  (block_size: i16)
  (gather_psize: idx_t.t)
  (radix_size: i32)
  (xs: [n](byteSeq [b]))
  (pLs: [n]t)
  (size_thresh: idx_t.t)
  (max_depth_: i32)
  (bit_step: i32)
: ([n](byteSeq [b]), [n]t) =
  let max_J = (i32.i64 b)*u8.num_bits - 1
  let max_depth = i32.min max_depth_ (((i32.i64 b)*u8.num_bits + radix_size - 1)/radix_size)
  -- recursively subdivide partitions that are too large
  -- starting with the entire dataset as one too large partition
  let loop_over : ([n](byteSeq [b]), [n]t, [](idx_t.t, idx_t.t),  i32)
    = loop (pXs, pPs, taidade, dp) = (xs, pLs, [(0,n)], 0)
    while (length taidade > 0) && (dp<max_depth) do
        let nt = length taidade
        let new_i = radix_size*dp
        let new_j = i32.min max_J (new_i + radix_size - 1)
      -- get indices & lens of all taidade partitions
        let pinds = taidade |> map (.0)
        let plens = taidade |> map (.1)
      -- create gather/scatter indices via segmented iota
      -- as well as replicated taidade partition ids
        let pids = plens |> replicated_iota
        let nr = length pids
        let gather_idx = indices pids
          |> map (\i -> if i==0 then false else (pids[i-1]!=pids[i]))
          |> segmented_iota
          |> sized nr
          |> map2 (+) (pids |> map (\i -> pinds[i]) |> sized nr)
      -- gather partitions & apply repartitioning
        let xips = gather_idx
          |> map (\i -> (pXs[i], (pids[i], pPs[i])))
          |> unzip
        let (new_xips)
          = radix_part gather_psize xips.0 xips.1 new_i new_j bit_step
          |> (\ret -> zip ret.0 ret.1)
      -- if more than 1 partition, sort them by part id
        let (new_xs, new_pLs) =
          if nt == 1
          then new_xips |> map (\(x,(_,pL)) -> (x, pL)) |> unzip
          else new_xips
            |> merge_sort (\(_,(pid1,_)) (_,(pid2,_)) -> pid1<=pid2)
            |> map (\(x,(_,pL)) -> (x, pL))
            |> unzip
      -- identify if there are any new taidade partitions
        let curBounds = (getPartitionBounds dp new_xs new_i new_j).bounds
        let n_xinBufen = length curBounds
        let xin_taidade = if dp==max_depth-1 then []
          else indices curBounds
          |> map (\i -> if i==n_xinBufen-1 then (curBounds[i],nr) else (curBounds[i],curBounds[i+1]))
          |> map (\(inf,sup) -> sup-inf)
          |> zip (indices curBounds)
          |> map (\(i,s) -> (gather_idx[curBounds[i]], s))
          |> filter (\(_,s) -> s>size_thresh)
      -- scatter & move on to the next iteration
        let xin_pXs = scatter (copy pXs) gather_idx new_xs
        let xin_pPs = scatter (copy pPs) gather_idx new_pLs
        in (xin_pXs, xin_pPs, xin_taidade, dp+1)
  in (loop_over.0, loop_over.1)

type partitionedSet_GFTR [n] [b] [pL_b] = {ks: [n](byteSeq [b]), pL: [n](byteSeq [pL_b])}
type partitionedSet_GFUR [n] [b] = {ks: [n](byteSeq [b]), idx: [n]idx_t.t}


def calc_partInfo [n] [b]
  (radix_size: i32)
  (pXs: [n](byteSeq [b]))
  (offset: idx_t.t)
  (size_thresh: idx_t.t)
  (max_depth_: i32)
: partitionInfo =
  let max_depth = i32.min max_depth_ (((i32.i64 b)*u8.num_bits + radix_size - 1)/radix_size)
  let recursive_info : (partitionInfo, bool)
  = loop p = (getPartitionBounds 1 pXs 0 (radix_size-1), true)
  while (p.0.maxDepth < max_depth && p.1) do
    let bs = p.0.bounds
    let taidade = indices bs
      |> map (\i -> (bs[i], if (i<(length bs)-1) then bs[i+1] else (n), i))
      |> filter (\(lb, ub, _) -> ub-lb > size_thresh)
    let (inner_info : partitionInfo, _: idx_t.t, _: idx_t.t)
    = loop (q, ad, ox) = (p.0, 0, 0)
    -- TODO can probably be more parallelised, like with partition_and_deepen
    -- actually though, it's going to require stitching anyway (even if it might be better with exscans)
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

def byteSeq_to_prim [b] 't
  (x : byteSeq [b])
  (zero : t)
  (from_u8: u8 -> t)
  (shift_left : t -> i32 -> t)
  (bitwise_or : t -> t -> t)
: t =
  loop y : t = zero
  for j in (iota b) do
    let r = (from_u8 x[b-j-1]) `shift_left` ((i32.i64 j) * u8.num_bits)
    in y `bitwise_or` r

def byteSeq_to_u8 x
  = byteSeq_to_prim x 0 (id) (\bits shift -> bits << (u8.i32 shift)) (u8.|)
def byteSeq_to_u16 x
  = byteSeq_to_prim x 0 (u16.u8) (\bits shift -> bits << (u16.i32 shift)) (u16.|)
def byteSeq_to_u32 x
  = byteSeq_to_prim x 0 (u32.u8) (\bits shift -> bits << (u32.i32 shift)) (u32.|)
def byteSeq_to_u64 x
  = byteSeq_to_prim x 0 (u64.u8) (\bits shift -> bits << (u64.i32 shift)) (u64.|)

def prim_get_radix 't
  (radix_bits : i32)
  (x : t)
  (modulo_pow2 : t -> u64 -> t)
: t =
  let asPow : u64 = (1 << (u64.i32 radix_bits))
  in x `modulo_pow2` asPow
def u8_get_radix radix_bits x
  = prim_get_radix radix_bits x (\bits moj -> bits & (u8.u64 (moj-1)))
def u16_get_radix radix_bits x
  = prim_get_radix radix_bits x (\bits moj -> bits & (u16.u64 (moj-1)))
def u32_get_radix radix_bits x
  = prim_get_radix radix_bits x (\bits moj -> bits & (u32.u64 moj-1))
def u64_get_radix radix_bits x
  = prim_get_radix radix_bits x (\bits moj -> bits & (u64.u64 (moj-1)))

def prim_radix_gt 't
  (radix_bits : i32)
  (cur_depth : i32)
  (x1 : t)
  (x2 : t)
  (gt : t -> t -> bool)
  (modulo_pow2 : t -> u64 -> t)
: bool = 
  let (_,is_gt,_) =
  loop (def_lt, def_gt, bi) = (false,false,0) while (!def_gt && !def_lt && bi<cur_depth) do
    let r1 = prim_get_radix (radix_bits*(bi+1)) x1 (modulo_pow2)
    let r2 = prim_get_radix (radix_bits*(bi+1)) x2 (modulo_pow2)
    let dgt = r1 `gt` r2
    let dlt = r2 `gt` r1
    in (dlt,dgt,bi+1)
  in is_gt
def u8_radix_gt radix_bits cur_depth x1 x2
  = prim_radix_gt radix_bits cur_depth x1 x2 (>) (\bits moj -> bits & (u8.u64 (moj-1)))
def u16_radix_gt radix_bits cur_depth x1 x2
  = prim_radix_gt radix_bits cur_depth x1 x2 (>) (\bits moj -> bits & (u16.u64 (moj-1)))
def u32_radix_gt radix_bits cur_depth x1 x2
  = prim_radix_gt radix_bits cur_depth x1 x2 (>) (\bits moj -> bits & (u32.u64 (moj-1)))
def u64_radix_gt radix_bits cur_depth x1 x2
  = prim_radix_gt radix_bits cur_depth x1 x2 (>) (\bits moj -> bits & (moj-1))

def prim_radix_lt 't
  (radix_bits : i32)
  (cur_depth : i32)
  (x1 : t)
  (x2 : t)
  (lt : t -> t -> bool)
  (modulo_pow2 : t -> u64 -> t)
: bool = 
  let (is_lt,_,_) =
  loop (def_lt, def_gt, bi) = (false,false,0) while (!def_gt && !def_lt && bi<cur_depth) do
    let r1 = prim_get_radix (radix_bits*(bi+1)) x1 (modulo_pow2)
    let r2 = prim_get_radix (radix_bits*(bi+1)) x2 (modulo_pow2)
    let dlt = r1 `lt` r2
    let dgt = r2 `lt` r1
    in (dlt,dgt,bi+1)
  in is_lt
def u8_radix_lt radix_bits cur_depth x1 x2
  = prim_radix_lt radix_bits cur_depth x1 x2 (<) (\bits moj -> bits & (u8.u64 (moj-1)))
def u16_radix_lt radix_bits cur_depth x1 x2
  = prim_radix_lt radix_bits cur_depth x1 x2 (<) (\bits moj -> bits & (u16.u64 (moj-1)))
def u32_radix_lt radix_bits cur_depth x1 x2
  = prim_radix_lt radix_bits cur_depth x1 x2 (<) (\bits moj -> bits & (u32.u64 (moj-1)))
def u64_radix_lt radix_bits cur_depth x1 x2
  = prim_radix_lt radix_bits cur_depth x1 x2 (<) (\bits moj -> bits & (moj-1))

def prim_radix_eq 't
  (radix_bits : i32)
  (cur_depth : i32)
  (x1 : t)
  (x2 : t)
  (eq : t -> t -> bool)
  (modulo_pow2 : t -> u64 -> t)
: bool =
  let r1 = prim_get_radix (radix_bits*cur_depth) x1 (modulo_pow2)
  let r2 = prim_get_radix (radix_bits*cur_depth) x2 (modulo_pow2)
  in r1 `eq` r2
def u8_radix_eq radix_bits cur_depth x1 x2
  = prim_radix_eq radix_bits cur_depth x1 x2 (==) (\bits moj -> bits & (u8.u64 (moj-1)))
def u16_radix_eq radix_bits cur_depth x1 x2
  = prim_radix_eq radix_bits cur_depth x1 x2 (==) (\bits moj -> bits & (u16.u64 (moj-1)))
def u32_radix_eq radix_bits cur_depth x1 x2
  = prim_radix_eq radix_bits cur_depth x1 x2 (==) (\bits moj -> bits & (u32.u64 (moj-1)))
def u64_radix_eq radix_bits cur_depth x1 x2
  = prim_radix_eq radix_bits cur_depth x1 x2 (==) (\bits moj -> bits & (moj-1))

def rv_partitionMatchBounds [nR] [pR] 't
  (as_index_ : t -> i64)
  (get_radix_ : i32 -> t -> t)
  (radix_eq_ : i32 -> i32 -> t -> t -> bool)
  (radix_gt_ : i32 -> i32 -> t -> t -> bool)
  (radix_lt_ : i32 -> i32 -> t -> t -> bool)
  (radix_size: i32)
  (rv: t)
  (tS: [nR]t)
  (bounds_S: [pR]idx_t.t)
  (depths_S: [pR]i32)
  (ht_S: radix_hashTable [i64.i32 radix_size])
:idx_t.t =
  let j = as_index_ (get_radix_ radix_size rv)
  let spi = ht_S.first_info_idx[j]
  in
    if spi < 0 then (-1) else
    if spi==ht_S.last_info_idx[j] then (spi) else
    -- binary search for matching partition
    let end_spi = ht_S.last_info_idx[j]
    let init_step = idx_t.max 1 ((end_spi-spi)/2)
    let (bsearch,_) =
      loop (heshi, step) = (spi, init_step) while step>0 do
        let cur_depth = depths_S[heshi]
        let prev_depth = if heshi==spi then cur_depth else depths_S[heshi-1]
        let next_depth = if heshi==end_spi then cur_depth else depths_S[heshi+1]
        let cur_S = tS[bounds_S[heshi]]
        let prev_S = if heshi==spi then cur_S else tS[bounds_S[heshi-1]]
        let next_S = if heshi==end_spi then cur_S else tS[bounds_S[heshi+1]]
        in
          if (radix_eq_ radix_size cur_depth rv cur_S)
          then (heshi, 0) -- found match
          else if (radix_lt_ radix_size cur_depth rv cur_S) then
            if (heshi==spi || radix_gt_ radix_size prev_depth rv prev_S)
            then (-1, 0)
            else (heshi-step, idx_t.max 1 (step/2))
          else -- if (radix_gt_ radix_size cur_depth rv cur_S) then
            if (heshi==end_spi || radix_lt_ radix_size next_depth rv next_S)
            then (-1, 0)
            else (heshi+step, idx_t.max 1 (step/2))
    in bsearch

def u8_rv_partitionMatchBounds radix_size rv tS bounds_S depths_S ht_S =
  let this_as_index = (i64.u8)
  let this_get_radix = (u8_get_radix)
  let this_eq = (u8_radix_eq)
  let this_gt = (u8_radix_gt)
  let this_lt = (u8_radix_lt)
  in rv_partitionMatchBounds (this_as_index) (this_get_radix) (this_eq) (this_gt) (this_lt) radix_size rv tS bounds_S depths_S ht_S
def u16_rv_partitionMatchBounds radix_size rv tS bounds_S depths_S ht_S =
  let this_as_index = (i64.u16)
  let this_get_radix = (u16_get_radix)
  let this_eq = (u16_radix_eq)
  let this_gt = (u16_radix_gt)
  let this_lt = (u16_radix_lt)
  in rv_partitionMatchBounds (this_as_index) (this_get_radix) (this_eq) (this_gt) (this_lt) radix_size rv tS bounds_S depths_S ht_S
def u32_rv_partitionMatchBounds radix_size rv tS bounds_S depths_S ht_S =
  let this_as_index = (i64.u32)
  let this_get_radix = (u32_get_radix)
  let this_eq = (u32_radix_eq)
  let this_gt = (u32_radix_gt)
  let this_lt = (u32_radix_lt)
  in rv_partitionMatchBounds (this_as_index) (this_get_radix) (this_eq) (this_gt) (this_lt) radix_size rv tS bounds_S depths_S ht_S
def u64_rv_partitionMatchBounds radix_size rv tS bounds_S depths_S ht_S =
  let this_as_index = (i64.u64)
  let this_get_radix = (u64_get_radix)
  let this_eq = (u64_radix_eq)
  let this_gt = (u64_radix_gt)
  let this_lt = (u64_radix_lt)
  in rv_partitionMatchBounds (this_as_index) (this_get_radix) (this_eq) (this_gt) (this_lt) radix_size rv tS bounds_S depths_S ht_S

#[noinline]
def radix_hash_join_u8 [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u8)
  let tS_int = tS |> map (byteSeq_to_u8)
  let n_pS = length pS.bounds
  let (first_match_per_r, counts_per_r) = tR_int
    |> map (\rv->
      let heshi = u8_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,cm,_) =
        loop (j,duome,i)=(inf,0,inf) while (i<sup) do
          let is_eq = rv == tS_int[i]
          let next_j = if (duome>0 || is_eq) then j else j+1
          let next_duome = if is_eq then duome+1 else duome
          in (next_j, next_duome, i+1)
      in if sm<sup then (sm,cm) else (-1,0)
    ) |> unzip
  let r_inds =
    let base_inds = replicated_iota counts_per_r
    let seg_inds = indices base_inds
      |> map (\i -> if i==0 then false else base_inds[i-1]!=base_inds[i])
      |> segmented_iota
      |> map (\k -> k+1)
    in zip seg_inds base_inds
  let s_inds = r_inds
    |> map (\(k, ir) ->
      let rv = tR_int[ir]
      let inf_s_idx = first_match_per_r[ir]
      let (kth_match,_) =
        loop (j,m) = (inf_s_idx-1,0) while m<k do
          let is_eq = rv == tS_int[j+1]
          in if is_eq then (j+1,m+1) else (j+1,m)
      in kth_match
    )
  in
    {
      vs = r_inds |> map (\(_, ir) -> tR[ir]),
      ix = r_inds |> map (.1),
      iy = s_inds
    }
#[noinline]
def radix_hash_join_u16 [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u16)
  let tS_int = tS |> map (byteSeq_to_u16)
  let n_pS = length pS.bounds
  let (first_match_per_r, counts_per_r) = tR_int
    |> map (\rv->
      let heshi = u16_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,cm,_) =
        loop (j,duome,i)=(inf,0,inf) while (i<sup) do
          let is_eq = rv == tS_int[i]
          let next_j = if (duome>0 || is_eq) then j else j+1
          let next_duome = if is_eq then duome+1 else duome
          in (next_j, next_duome, i+1)
      in if sm<sup then (sm,cm) else (-1,0)
    ) |> unzip
  let r_inds =
    let base_inds = replicated_iota counts_per_r
    let seg_inds = indices base_inds
      |> map (\i -> if i==0 then false else base_inds[i-1]!=base_inds[i])
      |> segmented_iota
      |> map (\k -> k+1)
    in zip seg_inds base_inds
  let s_inds = r_inds
    |> map (\(k, ir) ->
      let rv = tR_int[ir]
      let inf_s_idx = first_match_per_r[ir]
      let (kth_match,_) =
        loop (j,m) = (inf_s_idx-1,0) while m<k do
          let is_eq = rv == tS_int[j+1]
          in if is_eq then (j+1,m+1) else (j+1,m)
      in kth_match
    )
  in
    {
      vs = r_inds |> map (\(_, ir) -> tR[ir]),
      ix = r_inds |> map (.1),
      iy = s_inds
    }
#[noinline]
def radix_hash_join_u32 [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u32)
  let tS_int = tS |> map (byteSeq_to_u32)
  let n_pS = length pS.bounds
  let (first_match_per_r, counts_per_r) = tR_int
    |> map (\rv->
      let heshi = u32_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,cm,_) =
        loop (j,duome,i)=(inf,0,inf) while (i<sup) do
          let is_eq = rv == tS_int[i]
          let next_j = if (duome>0 || is_eq) then j else j+1
          let next_duome = if is_eq then duome+1 else duome
          in (next_j, next_duome, i+1)
      in if sm<sup then (sm,cm) else (-1,0)
    ) |> unzip
  let r_inds =
    let base_inds = replicated_iota counts_per_r
    let seg_inds = indices base_inds
      |> map (\i -> if i==0 then false else base_inds[i-1]!=base_inds[i])
      |> segmented_iota
      |> map (\k -> k+1)
    in zip seg_inds base_inds
  let s_inds = r_inds
    |> map (\(k, ir) ->
      let rv = tR_int[ir]
      let inf_s_idx = first_match_per_r[ir]
      let (kth_match,_) =
        loop (j,m) = (inf_s_idx-1,0) while m<k do
          let is_eq = rv == tS_int[j+1]
          in if is_eq then (j+1,m+1) else (j+1,m)
      in kth_match
    )
  in
    {
      vs = r_inds |> map (\(_, ir) -> tR[ir]),
      ix = r_inds |> map (.1),
      iy = s_inds
    }
#[noinline]
def radix_hash_join_u64 [nR] [nS] [b]
 (radix_size : i32)
 (tR : [nR](byteSeq [b]))
 (tS : [nS](byteSeq [b]))
 (pS : partitionInfo)
 (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u64)
  let tS_int = tS |> map (byteSeq_to_u64)
  let n_pS = length pS.bounds
  let (first_match_per_r, counts_per_r) = tR_int
    |> map (\rv->
      let heshi = u64_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,cm,_) =
        loop (j,duome,i)=(inf,0,inf) while (i<sup) do
          let is_eq = rv == tS_int[i]
          let next_j = if (duome>0 || is_eq) then j else j+1
          let next_duome = if is_eq then duome+1 else duome
          in (next_j, next_duome, i+1)
      in if sm<sup then (sm,cm) else (-1,0)
    ) |> unzip
  let r_inds =
    let base_inds = replicated_iota counts_per_r
    let seg_inds = indices base_inds
      |> map (\i -> if i==0 then false else base_inds[i-1]!=base_inds[i])
      |> segmented_iota
      |> map (\k -> k+1)
    in zip seg_inds base_inds
  let s_inds = r_inds
    |> map (\(k, ir) ->
      let rv = tR_int[ir]
      let inf_s_idx = first_match_per_r[ir]
      let (kth_match,_) =
        loop (j,m) = (inf_s_idx-1,0) while m<k do
          let is_eq = rv == tS_int[j+1]
          in if is_eq then (j+1,m+1) else (j+1,m)
      in kth_match
    )
  in
    {
      vs = r_inds |> map (\(_, ir) -> tR[ir]),
      ix = r_inds |> map (.1),
      iy = s_inds
    }

def radix_hash_join [b]
  (radix_size : i32)
  (tR : [](byteSeq [b]))
  (tS : [](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
=
  if b==1 then radix_hash_join_u8 radix_size tR tS pS ht_S
  else if b==2 then radix_hash_join_u16 radix_size tR tS pS ht_S
  else if b<=4 then radix_hash_join_u32 radix_size tR tS pS ht_S
  else radix_hash_join_u64 radix_size tR tS pS ht_S

#[noinline]
def radix_hash_join_with_S_keys_unique_u8 [nR] [nS] [b]
  (radix_size : i32)
  (tR : [nR](byteSeq [b]))
  (tS : [nS](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u8)
  let tS_int = tS |> map (byteSeq_to_u8)
  let n_pS = length pS.bounds
  let match_in_iy = tR_int
    |> map (\rv->
      let heshi = u8_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,_) =
        loop (j,zhaodaole)=(inf,false) while (j<sup) && (!zhaodaole) do
          let is_eq = rv == tS_int[j]
          in if is_eq then (j,true) else (j+1,false)
      in if sm<sup then sm else (-1)
    )
  let count_pairs = countFor (>=0) match_in_iy
  let zuowei = map2
    (\m z -> if m>=0 then z else (-1))
    match_in_iy
    ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>=0 then 1 else 0)) )
  let ix_ = scatter (replicate count_pairs (-1)) zuowei (iota nR)
  let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
  let vs_ = gather (dummy_byteSeq b) tR ix_
  in {vs=vs_, ix=ix_, iy=iy_}
#[noinline]
def radix_hash_join_with_S_keys_unique_u16 [nR] [nS] [b]
  (radix_size : i32)
  (tR : [nR](byteSeq [b]))
  (tS : [nS](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u16)
  let tS_int = tS |> map (byteSeq_to_u16)
  let n_pS = length pS.bounds
  let match_in_iy = tR_int
    |> map (\rv->
      let heshi = u16_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,_) =
        loop (j,zhaodaole)=(inf,false) while (j<sup) && (!zhaodaole) do
          let is_eq = rv == tS_int[j]
          in if is_eq then (j,true) else (j+1,false)
      in if sm<sup then sm else (-1)
    )
  let count_pairs = countFor (>=0) match_in_iy
  let zuowei = map2
    (\m z -> if m>=0 then z else (-1))
    match_in_iy
    ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>=0 then 1 else 0)) )
  let ix_ = scatter (replicate count_pairs (-1)) zuowei (iota nR)
  let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
  let vs_ = gather (dummy_byteSeq b) tR ix_
  in {vs=vs_, ix=ix_, iy=iy_}
#[noinline]
def radix_hash_join_with_S_keys_unique_u32 [nR] [nS] [b]
  (radix_size : i32)
  (tR : [nR](byteSeq [b]))
  (tS : [nS](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u32)
  let tS_int = tS |> map (byteSeq_to_u32)
  let n_pS = length pS.bounds
  let match_in_iy = tR_int
    |> map (\rv->
      let heshi = u32_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,_) =
        loop (j,zhaodaole)=(inf,false) while (j<sup) && (!zhaodaole) do
          let is_eq = rv == tS_int[j]
          in if is_eq then (j,true) else (j+1,false)
      in if sm<sup then sm else (-1)
    )
  let count_pairs = countFor (>=0) match_in_iy
  let zuowei = map2
    (\m z -> if m>=0 then z else (-1))
    match_in_iy
    ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>=0 then 1 else 0)) )
  let ix_ = scatter (replicate count_pairs (-1)) zuowei (iota nR)
  let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
  let vs_ = gather (dummy_byteSeq b) tR ix_
  in {vs=vs_, ix=ix_, iy=iy_}
#[noinline]
def radix_hash_join_with_S_keys_unique_u64 [nR] [nS] [b]
  (radix_size : i32)
  (tR : [nR](byteSeq [b]))
  (tS : [nS](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
: joinPairs_bsq [b] =
  let tR_int = tR |> map (byteSeq_to_u64)
  let tS_int = tS |> map (byteSeq_to_u64)
  let n_pS = length pS.bounds
  let match_in_iy = tR_int
    |> map (\rv->
      let heshi = u64_rv_partitionMatchBounds radix_size rv tS_int pS.bounds pS.depths ht_S
      let inf = if heshi<=0 then 0 else pS.bounds[heshi]
      let sup = if heshi==n_pS-1 then nS else pS.bounds[heshi+1]
      let (sm,_) =
        loop (j,zhaodaole)=(inf,false) while (j<sup) && (!zhaodaole) do
          let is_eq = rv == tS_int[j]
          in if is_eq then (j,true) else (j+1,false)
      in if sm<sup then sm else (-1)
    )
  let count_pairs = countFor (>=0) match_in_iy
  let zuowei = map2
    (\m z -> if m>=0 then z else (-1))
    match_in_iy
    ( exscan (+) 0 (match_in_iy |> map (\iy -> if iy>=0 then 1 else 0)) )
  let ix_ = scatter (replicate count_pairs (-1)) zuowei (iota nR)
  let iy_ = scatter (replicate count_pairs (-1)) zuowei match_in_iy
  let vs_ = gather (dummy_byteSeq b) tR ix_
  in {vs=vs_, ix=ix_, iy=iy_}

def radix_hash_join_with_S_keys_unique [b]
  (radix_size : i32)
  (tR : [](byteSeq [b]))
  (tS : [](byteSeq [b]))
  (pS : partitionInfo)
  (ht_S : radix_hashTable [i64.i32 radix_size])
=
  if b==1 then radix_hash_join_with_S_keys_unique_u8 radix_size tR tS pS ht_S
  else if b==2 then radix_hash_join_with_S_keys_unique_u16 radix_size tR tS pS ht_S
  else if b<=4 then radix_hash_join_with_S_keys_unique_u32 radix_size tR tS pS ht_S
  else radix_hash_join_with_S_keys_unique_u64 radix_size tR tS pS ht_S

-- ################################################################################################################
-- Composite size types
-- ################################################################################################################

def byteSeq_to_u48 x : (u16, u32) =
  let b = length x
  let ms_slot = byteSeq_to_u16 ((replicate (6-b) 0) ++ x[0:b-4])
  let ls_slot = byteSeq_to_u32 x[4:(length x)]
  in (ms_slot, ls_slot)
def byteSeq_to_u80 x : (u16, u64) =
  let b = length x
  let ms_slot = byteSeq_to_u16 ((replicate (10-b) 0) ++ x[0:b-8])
  let ls_slot = byteSeq_to_u64 x[2:(length x)]
  in (ms_slot, ls_slot)
def byteSeq_to_u96 x : (u32, u64) =
  let b = length x
  let ms_slot = byteSeq_to_u32 ((replicate (12-b) 0) ++ x[0:b-8])
  let ls_slot = byteSeq_to_u64 x[b-8:b]
  in (ms_slot, ls_slot)
def byteSeq_to_u112 x : (u16,u32,u64) =
  let b = length x
  let ms_slot = byteSeq_to_u16 ((replicate (14-b) 0) ++ x[0:b-12])
  let ns_slot = byteSeq_to_u32 x[b-12:b-8]
  let ls_slot = byteSeq_to_u64 x[b-8:b]
  in (ms_slot,ns_slot,ls_slot)
def byteSeq_to_u128 x : (u64, u64) =
  let b = length x
  let ms_slot = byteSeq_to_u64 ((replicate (16 - b) 0) ++ x[0:b-8])
  let ls_slot = byteSeq_to_u64 x[b-8:b]
  in (ms_slot, ls_slot)

def u48_get_radix radix_bits (x : (u16,u32)) : (u16,u32) =
  let ls_radix_bits = i32.min radix_bits u32.num_bits
  let ms_radix_bits = i32.max (radix_bits-u32.num_bits) 0
  let ls_r = u32_get_radix ls_radix_bits x.1
  let ms_r = u16_get_radix ms_radix_bits x.0
  in (ms_r, ls_r)
def u80_get_radix radix_bits (x : (u16,u64)) : (u16,u64) =
  let ls_radix_bits = i32.min radix_bits u64.num_bits
  let ms_radix_bits = i32.max (radix_bits-u64.num_bits) 0
  let ls_r = u64_get_radix ls_radix_bits x.1
  let ms_r = u16_get_radix ms_radix_bits x.0
  in (ms_r, ls_r)
def u96_get_radix radix_bits (x : (u32,u64)) : (u32,u64) =
  let ls_radix_bits = i32.min radix_bits u64.num_bits
  let ms_radix_bits = i32.max (radix_bits-u64.num_bits) 0
  let ls_r = u64_get_radix ls_radix_bits x.1
  let ms_r = u32_get_radix ms_radix_bits x.0
  in (ms_r, ls_r)
def u112_get_radix radix_bits (x : (u16,u32,u64)) : (u16,u32,u64) =
  let ls_radix_bits = i32.min radix_bits u64.num_bits
  let ns_radix_bits = i32.min (i32.max (radix_bits-u64.num_bits) 0) u32.num_bits
  let ms_radix_bits = i32.max (radix_bits-u32.num_bits-u64.num_bits) 0
  let ls_r = u64_get_radix ls_radix_bits x.2
  let ns_r = u32_get_radix ns_radix_bits x.1
  let ms_r = u16_get_radix ms_radix_bits x.0
  in (ms_r,ns_r,ls_r)
def u128_get_radix radix_bits (x : (u64,u64)) : (u64,u64) =
  let ls_radix_bits = i32.min radix_bits u64.num_bits
  let ms_radix_bits = i32.max (radix_bits-u64.num_bits) 0
  let ls_r = u64_get_radix ls_radix_bits x.1
  let ms_r = u64_get_radix ms_radix_bits x.0
  in (ms_r, ls_r)

def u128_radix_eq radix_bits cur_depth (x1:(u64,u64)) (x2:(u64,u64))
: bool =
  let ls_depth = i32.min cur_depth ((radix_bits + u64.num_bits - 1)/(radix_bits))
  let ms_depth = cur_depth - ls_depth
  let eq1 = u64_radix_eq radix_bits ls_depth x1.1 x2.1
  let eq2 = if ms_depth>0 then (u64_radix_eq radix_bits ms_depth x1.0 x2.0) else true
  in eq1&&eq2
def u128_radix_gt radix_bits cur_depth (x1:(u64,u64)) (x2:(u64,u64))
: bool =
  let ls_depth = i32.min cur_depth ((radix_bits + u64.num_bits - 1)/(radix_bits))
  let ms_depth = cur_depth - ls_depth
  let ls_gt = u64_radix_gt radix_bits ls_depth x1.1 x2.1
  in if ls_gt then true else
    let ls_eq = u64_radix_eq radix_bits ls_depth x1.1 x2.1
    let ms_lt = if ms_depth>0 then (u64_radix_gt radix_bits ms_depth x1.0 x2.0) else false
    in (ms_lt && ls_eq)
def u128_radix_lt radix_bits cur_depth (x1:(u64,u64)) (x2:(u64,u64))
: bool =
  let ls_depth = i32.min cur_depth ((radix_bits + u64.num_bits - 1)/(radix_bits))
  let ms_depth = cur_depth - ls_depth
  let ls_lt = u64_radix_lt radix_bits ls_depth x1.1 x2.1
  in if ls_lt then true else
    let ls_eq = u64_radix_eq radix_bits ls_depth x1.1 x2.1
    let ms_lt = if ms_depth>0 then (u64_radix_lt radix_bits ms_depth x1.0 x2.0) else false
    in (ms_lt && ls_eq)

-- ################################################################################################################

def test1 =
  let xs1:[][]u8= [[0,1],[0,1],[0,1],[4,1],[1,2],[2,2],[3,2],[4,2],[0,3],[2,3],[4,3],[6,3],[1,4],[1,4],[1,4],[5,4]]
  let xs2:[][]u8= [[0,1],[0,1],[4,1],[5,1],[2,2],[2,2],[2,2],[2,2],[1,3],[3,3],[5,3],[7,3],[1,4],[2,4],[5,4],[5,4]]
  let inf1 = calc_partInfo 8 xs1 0 2 3
  let tab1 = calc_radixHashTab 8 xs1 inf1 256
  in radix_hash_join 8 xs2 xs1 inf1 tab1

def test2 (depth : i32) =
  let xs1_:[][]u8= [[4,1],[0,3],[1,4],[1,4],[1,4],[5,4],[2,3],[4,3],[6,3],[1,2],[2,2],[4,2],[3,2],[0,1],[0,1],[0,1]]
  let xs2_:[][]u8= [[4,1],[5,1],[0,1],[0,1],[5,4],[5,4],[1,4],[2,4],[2,2],[2,2],[2,2],[2,2],[5,3],[7,3],[1,3],[3,3]]
  let (xs1,is1) = partition_and_deepen (i16.highest) (i64.highest) 8 xs1_ (indices xs1_) 0 depth 2
  let (xs2,is2) = partition_and_deepen (i16.highest) (i64.highest) 8 xs2_ (indices xs2_) 0 depth 2
  let inf1 = calc_partInfo 8 xs1 0 0 depth
  let tab1 = calc_radixHashTab 8 xs1 inf1 256
  in (radix_hash_join 8 xs2 xs1 inf1 tab1, xs1, xs2, inf1, tab1.first_info_idx[0:10], tab1.last_info_idx[0:10])
