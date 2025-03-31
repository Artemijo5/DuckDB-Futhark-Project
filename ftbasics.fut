-- For the present, implement some simple functionalities
-- 1. sort a column, return with og indices
-- 2. sort columns according to given indices (really a scatter operation)
-- 3. partition a column on function given
-- 4. partition columns according to given indices
-- 5. map-reduce aggregates
-- (read up on radix sort in futhark sorts library ++)

import "lib/github.com/diku-dk/sorts/radix_sort"


local module param_idx_t (it: integral) = {
  type t = it.t
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


-- | Gather operation.
def gather 't (xs: []t) (is: [](idx_t.t)) =
  is |> map (\i -> xs[i])
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
    let s_ixs = blocked_radix_sort_int_by_key 256 (\ix -> ix.1) T.num_bits T.get_bit ixs
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
    let s_ixs = blocked_radix_sort_float_by_key 256 (\ix -> ix.1) T.num_bits T.get_bit ixs
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


--def untyped_sumFor 't
--    (sum: []t -> t)
--    (mult: t -> i32 -> t) -- this one might be a pain...
--    (p: t -> bool) (xs: []t) : t =
--  sum (xs |> map (\x -> (x `mult` ((p >-> i32.bool) x)) ))

-- find greatest element that satisfies property
--def int_argmaxFor 't
--    (lowest: t)
--    (gt: t -> t -> bool)
--    (eq: t -> t ->bool)
--    (p: t -> bool) (xs: []t) : i32 =
--  let tup = 
--    reduce_comm (\(ix, vx) (iy, vy) ->
--        if (p vx) && !(p vy) then (ix, vx) else
--        if !(p vx) && (p vy) then (iy, vy) else
--        if !((p vx) || (p vy)) then (-1, vx) else
--        if (vx `gt` vy) || ((vx `eq` vy) && (ix > iy)) then (ix, vx)
--        else (iy, vy)
--      )
--      (-1, lowest)
--      (xs |> zip ((indices xs) |> map (i32.i64)))
--  in tup.0
  
-- TODO determine if this offers any advantage over countFor
--def multiCount [lk] [lx] 't (ks: [lk]t) (xs: [lx]t) : i32 =
--  let cxs : [lx][lk]i32 = xs
--    |> map (\x -> (vals |> map (== x)))
--    |> map (map t.bool) -- TODO bool most likely needs to be called by a module...
--  let nxs : [lk]i32 = replicate lk 0
--  in reduce (map2 (+)) nxs cxs
  

-- MAPRED FUNCTIONS

--def sumByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
--  keyValues |> map (\k -> keysFromXs |> sumFor (== k)) |> zip keyValues
