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

def gather 'a (xs: []a) (is: []i32) =
  is |> map (\i -> xs[i])

-- TODO does this already exist in some futhark library?
def countFor 'a (p: a -> bool) (xs: []a) =
  i64.sum (xs |> map (p >-> i64.bool))

def sumFor 'a (p: a -> bool) (xs: []i32) =
  i64.sum (xs |> map (\x -> x*(p >-> i64.bool)))

-- TODO test if sth like this works in REPL, and consider if it's really advantageous over the 'simpler' count approach
def multiCount i32 (vals: []i32) (ks: []i32) =
  let cxs : [][]bool = xs |> map (\x -> (vals |> map (== xs)))
    in let nxs : []i32 = replicate (length vals) 0
      in reduce (map2 (+)) nxs cxs

-- SORTING FUNCTIONS

-- sorts a column & couples it with the original indices
def sortColumn (xs: []i32) : [](i32, i32) =
  let ixs : [](i32, i32) = xs |> zip (indices xs)
    in blocked_radix_sort_int_by_key 256 (\ix -> ix.1) i32.num_bits i32.get_bit ixs

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
  -- TODO perhaps this could be somehow done better with a function that returns an array of booleans?
  -- so count can scan for many keys in only one pass of the keysFromX
  keyValues |> map (\k -> keysFromXs |> countFor (== k)) |> zip keyValues
  --keysFromXs |> multiCount keyValues |> zip keysFromXs -- TODO ???
  

-- MAPRED FUNCTIONS

def sumByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
  keyValues |> map (\k -> keysFromXs |> sumFor (== k)) |> zip keyValues
