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
    let nextBuff = p.buff |> map (\(j, v) -> if (j>=lower_bound && j<upper_bound) then (j, cur_xs[j-lower_bound]) else (j, v))
    in {iter = p.iter+1, buff = nextBuff}
  in loop_over.buff |> map (\(i, v) -> v)
-- | Multi-pass gather operation (better cache-locality) - uses base array rather than dummy value.
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather_over_array [ni] [n] 'a (psize : idx_t.t) (dummy_array: [ni]a) (xs : [n]a) (is : [ni]idx_t.t) =
  let m = (n+psize-1)/psize
  let loop_over : {iter: idx_t.t, buff: [](idx_t.t, a)}
  = loop p = {iter=0, buff = dummy_array |> zip is}
  while p.iter<m do
    let lower_bound = p.iter * psize
    let upper_bound = idx_t.min (n) (lower_bound + psize)
    let cur_xs = xs[lower_bound:upper_bound]
    let nextBuff = p.buff |> map (\(j, v) -> if (j>=lower_bound && j<upper_bound) then (j, cur_xs[j-lower_bound]) else (j, v))
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
    let cur_is = is |> map (\i -> if (i >= lower_bound && i < upper_bound) then (i-lower_bound) else -1)
    in {iter=p.iter+1, buff = p.buff with [lower_bound:upper_bound] = scatter cur_dest cur_is vs}
  in loop_over.buff


-- TODO could make a multi-pass map (would require dummy element like partitioned_gather)

-- | Exclusive scan operation (from Futhark by Example).
def exscan f ne xs =
  map2
    (\i x -> if i==0 then ne else x)
    (indices xs)
    (rotate (-1) (scan f ne xs))

-- | Function to count elements that satisfy a property.
def countFor 't (p: t -> bool) (xs: []t) : idx_t.t =
  idx_t.sum (xs |> map (p >-> idx_t.bool))