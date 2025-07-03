import "lib/github.com/diku-dk/sorts/radix_sort"
import "lib/github.com/diku-dk/sorts/merge_sort"


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

-- | Type used to preserve original index information when sorting.
type sortInfo [len] 't = {is: [len](idx_t.t), xs: [len]t}
-- | Sorting information type (short).
type sortInfo_short [n] = sortInfo [n] i16
-- | Sorting information type (integer).
type sortInfo_int [n] = sortInfo [n] i32
-- | Sorting information type (long).
type sortInfo_long [n] = sortInfo [n] i64
-- | Sorting information type (float).
type sortInfo_float [n] = sortInfo [n] f32
-- | Sorting information type (double).
type sortInfo_double [n] = sortInfo [n] f64


-- | Gather operation (based on futhark example).
def gather 't [ni] [n] (dummy_elem: t) (xs: [n]t) (is: [ni](idx_t.t)) =
  is |> map (\i -> if (i>0 && i<n) then xs[i] else dummy_elem)
-- | Multi-pass gather operation (better cache-locality).
-- Based on 2007 paper 'Efficient gather and scatter operations on graphics processors'
-- by Bingsheng He et al.
def partitioned_gather [ni] [n] 't (psize : idx_t.t) (dummy_elem: t) (xs : [n]t) (is : [ni]idx_t.t) =
  let m = (ni+psize-1)/psize
  let dummy_array = (replicate ni dummy_elem)
  let loop_over : {iter: idx_t.t, buff: []t}
  = loop p = {iter=0, buff = dummy_array}
  while p.iter<m do
    let lower_bound = p.iter * psize
    let upper_bound = idx_t.min (ni) (lower_bound + psize)
    let cur_gather = gather (dummy_elem) (xs) (is[lower_bound:upper_bound])
    in {iter = p.iter+1, buff = p.buff with [lower_bound:upper_bound] = cur_gather}
  in loop_over.buff
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
    let cur_is = is |> map (\i -> if (i >= lower_bound && i < upper_bound) then i else -1)
    in {iter=p.iter+1, buff = scatter (p.buff) cur_is vs}
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

-- | Abstract type for column data.
-- Presumed to hold an ordered type.
-- Supports operations for comparison and sorting.
module type colData = {
  -- | The type held by the column's items.
  type t

  val ==: t -> t -> bool
  val !=: t -> t -> bool
  val > : t -> t -> bool
  val < : t -> t -> bool
  val >=: t -> t -> bool
  val <=: t -> t -> bool

  val min : t -> t -> t
  val max : t -> t -> t
  val minimum [n] : [n]t -> t
  val maximum [n] : [n]t -> t

  val sort [n] : idx_t.t -> [n]t -> sortInfo [n] t
  val blocked_sort [n] : idx_t.t -> i16 -> [n]t -> sortInfo [n] t
}

-- | Abstract type for column data that supports arithmetic operations.
-- Built on top of colData.
module type numData = {
  include colData
  -- Primitive Constructors:
  val i8 : i8 -> t
  val i16: i16 -> t
  val i32: i32 -> t
  val i64: i64 -> t
  val f16: f16 -> t
  val f32: f32 -> t
  val f64: f64 -> t
  val bool : bool -> t
  -- Arithmetic Operators
  val + : t -> t -> t
  val - : t -> t -> t
  val * : t -> t -> t
  val / : t -> t -> t
  val % : t -> t -> t
  val **: t -> t -> t

  val zero : t
  val one : t
  val neg : t -> t

  val sum [n] : [n]t -> t
  val product [n] : [n]t -> t
}

-- | Type for integer column data.
-- Implements numData with an integral type.
module intData (T: integral) : numData with t = T.t = {
  type t = T.t

  def (==) = (T.==)
  def (!=) = (T.!=)
  def (> ) = (T.> )
  def (< ) = (T.< )
  def (>=) = (T.>=)
  def (<=) = (T.<=)

  def min = T.min
  def max = T.max
  def minimum = T.minimum
  def maximum = T.maximum

  def sort [n] (incr: idx_t.t) (xs : [n](T.t)) =
    let ixs = xs |> zip (xs |> idx_t.indicesWithIncrement incr)
    let s_ixs = merge_sort_by_key (\ix -> ix.1) (<=) ixs
    let tup = unzip s_ixs
    in {is = tup.0, xs = tup.1}

  def blocked_sort [n] (incr: idx_t.t) (block_size: i16) (xs : [n](T.t)) =
    let ixs = xs |> zip (xs |> idx_t.indicesWithIncrement incr)
    let s_ixs = blocked_radix_sort_int_by_key block_size (\ix -> ix.1) T.num_bits T.get_bit ixs
    let tup = unzip s_ixs
    in {is = tup.0, xs = tup.1}

  def i8 = T.i8
  def i16= T.i16
  def i32= T.i32
  def i64= T.i64
  def f16= T.f16
  def f32= T.f32
  def f64= T.f64
  def bool = T.bool

  def (+) = (T.+)
  def (-) = (T.-)
  def (*) = (T.*)
  def (/) = (T./)
  def (%) = (T.%)
  def (**)= (T.**)

  def zero= T.i64 0
  def one = T.i64 1
  def neg = T.neg

  def sum = T.sum
  def product = T.product
}

-- | Type for float column data.
-- Implements numData with a float type.
module fltData (T: float) : numData with t = T.t = {
  type t = T.t

  def (==) = (T.==)
  def (!=) = (T.!=)
  def (> ) = (T.> )
  def (< ) = (T.< )
  def (>=) = (T.>=)
  def (<=) = (T.<=)

  def min = T.min
  def max = T.max
  def minimum = T.minimum
  def maximum = T.maximum

  def sort [n] (incr: idx_t.t) (xs : [n](T.t)) =
    let ixs = xs |> zip (xs |> idx_t.indicesWithIncrement incr)
    let s_ixs = merge_sort_by_key (\ix -> ix.1) (<=) ixs
    let tup = unzip s_ixs
    in {is = tup.0, xs = tup.1}

  def blocked_sort [n] (incr: idx_t.t) (block_size: i16) (xs : [n](T.t)) =
    let ixs = xs |> zip (xs |> idx_t.indicesWithIncrement incr)
    let s_ixs = blocked_radix_sort_float_by_key block_size (\ix -> ix.1) T.num_bits T.get_bit ixs
    let tup = unzip s_ixs
    in {is = tup.0, xs = tup.1}

  def i8 = T.i8
  def i16= T.i16
  def i32= T.i32
  def i64= T.i64
  def f16= T.f16
  def f32= T.f32
  def f64= T.f64
  def bool = T.bool

  def (+) = (T.+)
  def (-) = (T.-)
  def (*) = (T.*)
  def (/) = (T./)
  def (%) = (T.%)
  def (**)= (T.**)

  def zero= T.i64 0
  def one = T.i64 1
  def neg = T.neg

  def sum = T.sum
  def product = T.product
}
