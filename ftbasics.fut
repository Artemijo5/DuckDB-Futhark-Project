-- For the present, implement some simple functionalities
-- 1. sort a column, return with og indices
-- 2. sort columns according to given indices (really a scatter operation)
-- 3. partition a column on function given
-- 4. partition columns according to given indices
-- 5. map-reduce aggregates
-- (read up on radix sort in futhark sorts library ++)

-- for now, supporting i32 only
-- TODO (probably first extenstion) extend to other types, probably using module types...

import "lib/github.com/diku-dk/sorts/radix_sort"

module type colData {
  type t

  val eq : t -> t -> bool
  val gt : t -> t -> bool
  val gte : t -> t -> bool
  val lt : t -> t -> bool
  val lte : t -> t -> bool

  val sort : []t -> []t
}
-- TODO implement modules for each type?
-- maybe not necessary? could just implement different sorts...

type sorted 't = {is: []i32, xs: []t}
type keyed 'k 't = {numKeys: i32, countByKey: []i32, ks: []k, is: []i32, xs: []t}
-- TODO implement for each type?
-- again, maybe not necessary?

def gather 'a (xs: []a) (is: []i32) =
  is |> map (\i -> xs[i])

def countFor 'a (p: a -> bool) (xs: []a) : i32 =
  i32.sum (xs |> map (p >-> i32.bool))

def sumFor 'a (p: a -> bool) (xs: []i32) : i64 =
  i64.sum (xs |> map (\x -> x*(p >-> i64.bool)))

-- TODO determine if this offers any advantage over countFor
def multiCount [lk] [lx] (ks: [lk]i32) (xs: [lx]i32) : i32 =
  let cxs : [][]i32 = xs
    |> map (\x -> (vals |> map (== x)))
    |> map (map i32.bool)
  let nxs : []i32 = replicate (length vals) 0
  in reduce (map2 (+)) nxs cxs

-- SORTING FUNCTIONS

-- sorts a column & couples it with the original indices
def sortColumn (xs: []i32) : ()[i32, i32] =
  let ixs : [](i32, i32) = xs |> zip (indices xs)
  let sorted_ixs : [](i32, i32) = blocked_radix_sort_int_by_key 256 (\ix -> ix.1) i32.num_bits i32.get_bit ixs
  in unzip sorted_ixs

-- sorts column on a function & couples it with the original indices
def sortColumnOn 'a (p: a -> i32) (xs: []a) : [](i32, a) =
  let ixs : [](i32, a) = xs |> zip (indices x)
  in blocked_radix_sort_int_by_key 256 (\ix -> p ix.1) i32.num_bits i32.get_bit ixs

-- sort column on given indices (via a gather operation)
def sortByIndices 'a (is: []i32) (ys: []a) : []a =
  is |> gather ys

-- PARTITIONING FUNCTIONS

-- sort indices on partition, return key-index pairs
def keyIndexPairs (ks: []i32) : ([]i32, []i32) =
  let is : []i32 = (indices ks) |> sortColumnOn (\i -> ks[i])
  in (ks, is)

-- partition and sort xs on key-index pairs
def keyByIndices 'a (kis: ([]i32, []i32)) (xs: []a) : [](i32, a) =
  xs |> sortByIndices kis.1 |> zip kis.0

-- couple xs with partition keys
def keyBy 'a (kf: a -> i32) (xs: []a) : [](i32, a) =
  xs |> zip (xs |> map kf)

-- partition and sort xs by key function
def orderBy 'a (kf: a -> i32) (xs: []a) : [](i32, a) =
  xs |> keyByIndices (keyIndexPairs (xs |> map kf))

-- return key-count pairs, for a known set of keys
def countByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
  keyValues |> map (\k -> keysFromXs |> countFor (== k)) |> zip keyValues
  --keysFromXs |> multiCount keyValues |> zip keysFromXs -- TODO ???
  

-- MAPRED FUNCTIONS

def sumByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
  keyValues |> map (\k -> keysFromXs |> sumFor (== k)) |> zip keyValues
