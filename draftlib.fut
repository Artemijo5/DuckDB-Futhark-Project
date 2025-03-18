-- For the present, implement some simple functionalities
-- 1. sort a column, return with og indices
-- 2. sort columns according to given indices (really a scatter operation)
-- 3. partition a column on function given
-- 4. partition columns according to given indices
-- 5. map-reduce aggregates
-- (read up on radix sort in futhark sorts library ++)

import "lib/github.com/diku-dk/sorts/radix_sort"

def gather 'a (xs: []a) (is: []i32) =
  is |> map (\i -> xs[i])

-- SORTING FUNCTIONS

-- sorts a column & couples it with the original indices
def sortColumn 'a (xs: []a) : [](i32, a) =
  -- TODO placeholder
  xs |> zip (indices xs)

-- sorts column on a function & couples it with the original indices
def sortColumnOn 'a 'b (p: a -> b) (xs: []a) : [](i32, a) =
  -- TODO placeholder
  xs |> zip (indices xs)

-- sort column on given indices
def sortByIndices 'a (is: []i32) (ys: []a) : []a =
  -- TODO optimise?
  scatter (replicate (length ys) ys[0]) is ys

-- PARTITIONING FUNCTIONS

-- sort indices on partition, return key-index pairs
-- TODO specify size?
def keyIndexPairs (ks: []i32) : ([]i32, []i32) =
  is : []i32 = (indices ks) |> sortByIndices ks
  (ks, is)

-- partition and sort on key-index pairs
def keyByIndices 'a (kis: ([]i32, []i32)) (xs: []a) : [](i32, a) =
  (kis).1 |> gather xs |> zip (kis).0

-- logically partition xs on given key function, sorting on keys
def keyBy 'a (kf: a -> i32) (xs: []a) : [](i32, a) =
  --xs |> keyByIndices (keyIndexPairs (xs |> map kf))
  ks : []i32 = xs |> map kf
  xs |> zip ks |> sortByIndices ks
  -- might not be needed unless operating on a single column - since the intermediate kis are needed to shuffle other columns...

-- MAPRED FUNCTIONS

-- TODO
