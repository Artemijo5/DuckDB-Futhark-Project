local module param_idx_t (it: integral) = {
  type t = it.t
  def min = it.min
  def max = it.max
  def minimum = it.minimum
  def maximum = it.maximum
  def bool = it.bool
  def sum = it.sum
  def indices xs = (indices xs) |> map it.i64
  def indicesWithIncrement [n] 't (incr: i64) (xs: [n]t) : [n](it.t) =
    let rng : [n]i64 = (incr..<(n+incr)) :> [n]i64
    in rng |> map it.i64
}
-- | Integer type used for indices.
module idx_t = param_idx_t i64

-- | Gather operation (based on futhark example).
def gather 't [ni] [n] (dummy_elem: t) (xs: [n]t) (is: [ni](idx_t.t)) =
  is |> map (\i -> if (i>=0 && i<n) then xs[i] else dummy_elem)
-- | Multi-pass gather operation (better cache-locality).
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather [ni] [n] 'a (psize : idx_t.t) (dummy_elem: a) (xs : [n]a) (is : [ni]idx_t.t) =
  let m = (n+psize-1)/psize
  let dummy_array = (replicate ni dummy_elem) |> zip is
  let loop_over : {iter: idx_t.t, buff: [](idx_t.t, a)}
  = loop p = {iter=0, buff = dummy_array}
  while p.iter<m do
    let lower_bound = p.iter * psize
    let upper_bound = idx_t.min (n) (lower_bound + psize)
    let cur_xs = xs[lower_bound:upper_bound]
    let nextBuff = p.buff |> map (\(j, v) ->
      if (j>=lower_bound && j<upper_bound)
      then (j, cur_xs[j-lower_bound])
      else (j, v)
    )
    in {iter = p.iter+1, buff = nextBuff}
  in loop_over.buff |> map (\(i, v) -> v)
-- | Multi-pass gather operation (better cache-locality) - uses base array rather than dummy value.
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather_over_array [ni] [n] 'a (psize : idx_t.t) (dest: [ni]a) (xs : [n]a) (is : [ni]idx_t.t) =
  let m = (n+psize-1)/psize
  let loop_over : {iter: idx_t.t, buff: [](idx_t.t, a)}
  = loop p = {iter=0, buff = dest |> zip is}
  while p.iter<m do
    let lower_bound = p.iter * psize
    let upper_bound = idx_t.min (n) (lower_bound + psize)
    let cur_xs = xs[lower_bound:upper_bound]
    let nextBuff = p.buff |> map (\(j, v) ->
      if (j>=lower_bound && j<upper_bound)
      then (j, cur_xs[j-lower_bound])
      else (j, v)
    )
    in {iter = p.iter+1, buff = nextBuff}
  in loop_over.buff |> map (\(i, v) -> v)
-- | Multi-pass scatter operation (better cache-locality).
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_scatter [nd] [n] 'a
  (psize: idx_t.t) 
  (dest: *[nd]a)
  (is: [n]idx_t.t)
  (vs: [n]a)
: *[]a =
  let m = (nd+psize-1)/psize
  let loop_over : {iter: idx_t.t, buff: [nd]a}
  = loop p = {iter=0, buff = dest}
  while p.iter < m do
    let lower_bound = p.iter * psize
    let upper_bound = idx_t.min (nd) (lower_bound + psize)
    let cur_dest = copy p.buff[lower_bound:upper_bound] -- TODO copy needed?
    let cur_is = is |> map (\i ->
      if (i >= lower_bound && i < upper_bound)
      then (i-lower_bound)
      else -1
    )
    in {iter=p.iter+1, buff = p.buff with [lower_bound:upper_bound] = scatter cur_dest cur_is vs}
  in loop_over.buff


-- TODO could make a multi-pass map (would require dummy element like partitioned_gather)

-- | Exclusive scan operation (from Futhark by Example).
def exscan f ne xs =
  map2
    (\i x -> if i==0 then ne else x)
    (indices xs)
    (rotate (-1) (scan f ne xs))
    -- [ne] ++ (scan f ne xs[0:((length xs) - 1)]) -- TODO ???

-- | Function to count elements that satisfy a property.
def countFor 't (p: t -> bool) (xs: []t) : idx_t.t =
  idx_t.sum (xs |> map (p >-> idx_t.bool))

-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- Radix Sorting/Partitioning
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------

-- | Obtains a radix (of size 1 byte at most!).
def get_radix 't (i : i32) (j : i32) (get_bit : i32 -> t -> i32) (x : t) : u8 =
  let y = 0
  in
    loop y
    for bit in (0...(j-i))
    do u8.set_bit bit y (get_bit (i+bit) x)

-- | Performs a radix sort step, sorting over multiple bits at a time.
-- Based on radix-sort in futhark-by-example.
-- Substitutes the 'radix-partition' primitive, eg by doing 8 bits per step.
-- Doing multiple bits per step vastly reduces scatter writes, though increases comparisons.
-- Note: radix size must be 1 byte at most!
-- TODO can maybe be done in a more efficient way?
-- eg: find offset based on MSB, then add offset of MSB-1, etc...
-- after the final bit, would have to make difference between indices...
def radix_sort_multistep [n] 't
  (block_size : idx_t.t)
  (i : i32)
  (j : i32)
  (get_bit : i32 -> t -> i32)
  (xs : [n]t)
-- : [n]t =
  =
  let ij_bits = j-i+1
  let up_to = (1 << ij_bits) -- 2^(num_bits)
  let offs : i64 = 0
  let rs = xs |> map (get_radix i j get_bit)
  let idxs : [n]i64 = replicate n 0
  let loop_range = (0..<up_to) |> map (u8.i32)
  let ox = loop (offs, idxs) for (num : u8) in loop_range do
    -- "audience" ie relevant entries
    let guanzhong = rs
      |> map (\r -> r == num)
      |> map (i64.bool)
    -- addition to the offset
    let zengjia = reduce (+) 0 guanzhong
    -- "seats" ie indices in sorted array
    let zuowei = map2 (*) guanzhong (map (\z -> z+offs-1) (scan (+) 0 guanzhong))
    in (offs + zengjia, map2 (+) idxs zuowei)
  let scatter_idxs = ox.1
  in partitioned_scatter block_size (copy xs) scatter_idxs xs


-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- SMJ Partitions
-- & Merging functions
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- -------------------------------------------------------------------
-- Based on MergePath

def partFunc_sequential_search [n] 't
  (a_i: idx_t.t)
  (bi: idx_t.t)
  (as: [n]t)
  (bs: [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: (idx_t.t, idx_t.t) =
  let ai = if a_i < n then a_i else n-1
  let b_prev = if bi > 0 then bs[bi-1] else bs[0]
  let seqSearch : {k: idx_t.t, ak: idx_t.t, bk: idx_t.t} =
    if as[ai] `gt` b_prev then
      loop p = {k=0, ak=ai, bk=bi}
      while (p.k < n-bi) do
        let i_a = ai-p.k
        let i_b = bi+p.k
        in
          if (as[i_a-1] `leq` bs[i_b])
          then {k = n-bi, ak = i_a, bk = i_b} --break
          else {k = p.k+1, ak = ai, bk = bi} -- continue
    else
      loop p = {k=0, ak=ai, bk=bi}
      while (p.k < bi-1) do
        let hi_a = ai+p.k+1
        let i_a = if hi_a < n then hi_a else n-1
        let i_b = bi - p.k - 1
        in
          if (as[i_a] `gt` bs[i_b])
          then {k = bi-1, ak = i_a, bk = i_b+1} -- break
          else {k = p.k+1, ak = ai, bk = bi} -- continue
    in (seqSearch.ak, seqSearch.bk)

def partitionFunc [n] 't
  (partitions: idx_t.t)
  (fromThread: idx_t.t)
  (upToThread: idx_t.t)
  (as : [n]t)
  (bs : [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: [upToThread-fromThread](idx_t.t, idx_t.t) =
  let lowest = if (as[0] `leq` bs[0]) then as[0] else bs[0]
  let firstThr = idx_t.max (fromThread) (1)
  --let highest = if (as[n-1] `gt` bs[n-1]) then as[n-1] else bs[n-1]
  let ts : [upToThread-firstThr]idx_t.t = (firstThr)..<(upToThread)
  let pre_ret : [upToThread-firstThr](idx_t.t, idx_t.t) = ts
    |> map (\t ->
      let index : idx_t.t = t*(n+n)/partitions
      let loop_over : {flag: bool, a_top: idx_t.t, b_top: idx_t.t, a_bottom: idx_t.t} =
        loop p = {
          flag = true,
          a_top = if (index > n) then n else index,
          b_top = if (index > n) then (index - n) else 0,
          a_bottom = if (index > n) then (index - n - 1) else 0
        }
        while p.flag do
          let offset = (p.a_top - p.a_bottom)/2
          let ai = (p.a_top - offset)
          let bi = idx_t.max 0 (p.b_top + offset)
          let b_prev = if bi>0 then bs[bi-1] else lowest
          --let a_prev =if ai <= 0 then lowest else if ai > na then highest else as[ai-1]
          --let b_prev = if bi <= 0 then lowest else if bi > nb then highest else bs[bi-1]
          in
            -- Corner Cases
              if ai <= 0-- First Partition, or reached beginning of as
                then {flag=false, a_top=0, b_top=bi, a_bottom=p.a_bottom}
              else if bi >= n -- Reached end of Bs
                then
                  let a_ind = idx_t.min (ai) (n-1)
                  in
                    if (bs[n-1] `leq` as[a_ind])
                    then {flag=false, a_top=ai, b_top=n, a_bottom=p.a_bottom} -- break
                    else {flag=true, a_top=p.a_top, b_top = n-1-offset, a_bottom = p.a_bottom} -- continue
              else if (ai >= n) -- Reached end of As
                then
                  let b_ind = idx_t.min (bi) (n-1)
                  in
                    if (as[n-1] `leq` bs[b_ind])
                    then {flag=false, a_top=n, b_top=bi, a_bottom=p.a_bottom} -- break
                    else {flag=true, a_top=n-1, b_top=p.b_top, a_bottom=p.a_bottom} -- continue
              else if (p.a_top <= p.a_bottom) then -- resort to sequential search
                let (sai, sbi) = partFunc_sequential_search ai bi as bs (leq) (gt)
                in {flag=false, a_top=sai, b_top=sbi, a_bottom=p.a_bottom} -- break
              else
            -- Regular Cases
            if (as[ai] `gt` b_prev) then
              if (as[ai-1] `leq` bs[bi]) then
                {
                  flag = false, -- break
                  a_top = ai,
                  b_top = bi,
                  a_bottom = p.a_bottom
                }
              else {
                flag = true,
                a_top = ai - 1,
                b_top = bi + 1,
                a_bottom = p.a_bottom
              }
            else {
              flag = true,
              a_top = p.a_top,
              b_top = p.b_top,
              a_bottom = ai + 1
            }
        in (loop_over.a_top, loop_over.b_top)
      )
  let ret =
    (
      if fromThread>0
      then pre_ret
      else [(0,0)] ++ pre_ret
    )
    :> [upToThread-fromThread](idx_t.t, idx_t.t)
  in ret

-- Returns the same as partitionFunc but allows control of max parallelism
-- TODO can be made to strictly access certain subarrays at once, but the loop to do that will be quite convoluted in futhark
def windowed_partitionFunc [n] 't
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (tR: [n]t)
  (tS: [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
 : [partitionsPerWindow*numberOfWindows](idx_t.t, idx_t.t) =
  let totalNo = partitionsPerWindow*numberOfWindows
  let loop_over : {iter: idx_t.t, result: [totalNo](idx_t.t, idx_t.t)}
    = loop p = {iter=0, result = (replicate totalNo (n, n))}
    while p.iter<numberOfWindows do
      let start = p.iter*partitionsPerWindow
      let end = start+partitionsPerWindow
      let nextParts = partitionFunc (totalNo) (start) (end) tR tS (leq) (gt) :> [end-start](idx_t.t, idx_t.t)
      in {iter = p.iter+1, result = p.result with [start:end] = nextParts}
  in loop_over.result :> [partitionsPerWindow*numberOfWindows](idx_t.t, idx_t.t)