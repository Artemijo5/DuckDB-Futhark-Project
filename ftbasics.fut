local module param_idx_t (it: integral) = {
  type t = it.t
  def min = it.min
  def max = it.max
  def minimum = it.minimum
  def maximum = it.maximum
  def bool = it.bool
  def sum = it.sum
  def indices xs = (indices xs) |> map it.i64
  def indicesWithIncrement [n] 't (incr: i64) (_: [n]t) : [n](it.t) =
    let rng : [n]i64 = (incr..<(n+incr)) :> [n]i64
    in rng |> map it.i64
}
-- | Integer type used for indices.
module idx_t = param_idx_t i64

-- | Gather operation (based on futhark example).
def gather 't [ni] [n] (dummy_elem: t) (xs: [n]t) (is: [ni](idx_t.t)) =
  is |> map (\i -> if (i>=0 && i<n) then xs[i] else dummy_elem)
-- | Multi-pass gather operation (better cache-locality) - uses base array rather than dummy value.
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather_over_array [ni] [n] 'a
  (n_bits : i32) (psize : idx_t.t) (dest: [ni]a) (xs : [n]a) (is : [ni]idx_t.t)
=
  let psize_ = psize / (i64.i32 ((n_bits+u8.num_bits-1)/u8.num_bits) )
  let max_iter = (n+psize_-1) / psize_
  in loop buff = dest for j in (iota max_iter) do
    let inf = j*psize_
    let sup = idx_t.min n (inf + psize_)
    in buff
      |> zip (is)
      |> map (\(j,v) ->
        if (inf<=j && sup>j)
        then xs[j]
        else v
      )
-- | Multi-pass gather operation (better cache-locality).
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather [ni] [n] 'a
  (n_bits : i32) (psize : idx_t.t) (dummy_elem: a) (xs : [n]a) (is : [ni]idx_t.t)
=
  partitioned_gather_over_array n_bits psize (replicate ni dummy_elem) xs is
-- | Multi-pass scatter operation (better cache-locality).
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_scatter [nd] [n] 'a
  (n_bits: i32)
  (psize: idx_t.t) 
  (dest: *[nd]a)
  (is: [n]idx_t.t)
  (vs: [n]a)
: *[]a =
  let psize_ = psize / (i64.i32 ((n_bits+u8.num_bits-1)/u8.num_bits) )
  let max_iter = (nd+psize_-1) / psize_
  in loop buff = dest for j in (iota max_iter) do
    let inf = j*psize_
    let sup = idx_t.min nd (inf + psize_)
    let shifted_is = is |> map (\i -> i-inf)
    in buff with [inf:sup] = scatter (copy buff[inf:sup]) shifted_is vs


-- | Function to gather the payload columns of a relation after the join.
def gather_payloads [ni] [n] 't
  (num_bits: i32)
  (incr: idx_t.t)
  (psize: idx_t.t)
  (dummy_elem: t)
  (is: [ni]idx_t.t)
  (ys: [n]t)
=
  let offset_is = is |> map (\j -> j - incr)
  in partitioned_gather (num_bits) (psize) (dummy_elem) (ys) (offset_is)

  -- | Function to gather the payload columns of a relation after the join, over an array with previously gathered values.
def gather_payloads_GFUR [ni] [n] 't
  (n_bits: i32)
  (incr: idx_t.t)
  (psize: idx_t.t)
  (dummy_array: [ni]t)
  (is: [ni]idx_t.t)
  (ys: [n]t)
=
  let offset_is = is |> map (\j -> j - incr)
  in partitioned_gather_over_array (n_bits) (psize) (dummy_array) (ys) (offset_is)

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

-- argmin - for getting chunk replacement priorities in 2-pass sort (maybe better left to be sequential?)
def argmin [n] 't
    (lt: t -> t -> bool)
    (eq: t -> t -> bool)
    (highest: t)
    (ks: [n]t)
    : idx_t.t = 
  let ne = (n, highest)
  let iks = ks
    |> zip (idx_t.indices ks)
  let min_ik = reduce_comm(\(ix, vx) (iy, vy) ->
        if (vx `lt` vy) || ((vx `eq` vy) && (ix < iy))
          then (ix, vx)
          else (iy, vy)
      ) ne iks
  in min_ik.0

-- sequential map function
def seqmap [n] 't 'ot
  (f : t -> ot)
  (xs: [n]t)
 : [n]ot =
  if n==0 then ([] :> [n]ot) else
  loop buff = (replicate n (f xs[0])) for j in (1..<n) do
    buff with [j] = f xs[j]

--def seqmap2 [n] 't1 't2 'ot
--  (f : t1 -> t2 -> ot)
--  (xs: [n]t1)
--  (ys: [n]t2)
-- : [n]ot =
--  if n==0 then ([] :> [n]ot) else
--  loop buff = (replicate n (f xs[0] ys[0])) for j in (1..<n) do
--    buff with [j] = f xs[j] ys[j]

--def seqmap3 [n] 't1 't2 't3 'ot
--  (f : t1 -> t2 -> t3 -> ot)
--  (xs: [n]t1)
--  (ys: [n]t2)
--  (zs: [n]t3)
-- : [n]ot =
--  if n==0 then ([] :> [n]ot) else
--  loop buff = (replicate n (f xs[0] ys[0] zs[0])) for j in (1..<n) do
--    buff with [j] = f xs[j] ys[j] zs[j]

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
-- Substitutes the 'radix-partition' primitive.
-- Doing multiple bits per step vastly reduces scatter writes, though increases comparisons.
-- Note: radix size must be 1 byte at most!
def radix_sort_multistep [n] 't
  (block_size : idx_t.t)
  (i : i32)
  (j : i32)
  (num_bits : i32)
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
  in partitioned_scatter num_bits block_size (copy xs) scatter_idxs xs