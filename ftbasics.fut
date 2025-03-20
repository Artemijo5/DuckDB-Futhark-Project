-- For the present, implement some simple functionalities
-- 1. sort a column, return with og indices
-- 2. sort columns according to given indices (really a scatter operation)
-- 3. partition a column on function given
-- 4. partition columns according to given indices
-- 5. map-reduce aggregates
-- (read up on radix sort in futhark sorts library ++)

-- for now, supporting i32 only
-- TODO (probably first extenstion) extend to other types, probably using module types...
-- will probably need a specification of sort for each type (...)

import "lib/github.com/diku-dk/sorts/radix_sort"

type sortTup [len] 't = {is: [len]i32, xs: [len]t}
type keyTup [lk] [lx] 'k 't = {numKeys: i32, keyCounts: [lk]i32, ks: [lk]k, is: [lx]i32, xs: [lx]t}
-- TODO implement for each type?
-- again, maybe not necessary?

def gather 'a (xs: []a) (is: []i32) =
  is |> map (\i -> xs[i])

def countFor 'a (p: a -> bool) (xs: []a) : i32 =
  i32.sum (xs |> map (p >-> i32.bool))

def sumFor 'a (p: a -> bool) (xs: []a) =
  sum (xs |> map (\x -> x*(p >-> bool))) -- TODO does multiplication work? test...

def argmaxFor 'a (p: a -> bool) (xs: []a) : i32 =
  -- TODO
  0

-- TODO determine if this offers any advantage over countFor
def multiCount [lk] [lx] (ks: [lk]) (xs: [lx]) : i32 =
  let cxs : [lx][lk]i32 = xs
    |> map (\x -> (vals |> map (== x)))
    |> map (map i32.bool)
  let nxs : [lk]i32 = replicate lk 0
  in reduce (map2 (+)) nxs cxs

-- SORTING FUNCTIONS

-- sorts a column & couples it with the original indices
entry def sortColumn [] (xs: []i32) : sortTup i32 =
  let ixs : [](i32, i32) = xs |> zip (indices xs)
  let sorted_ixs : [](i32, i32) = blocked_radix_sort_int_by_key 256 (\ix -> ix.1) i32.num_bits i32.get_bit ixs
  let tup : ([]i32, []i32) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

-- sorts column on a function & couples it with the original indices
def sortColumnOn [] 't (p: a -> i32) (xs: []t) : sortTup t =
  let ixs : [](i32, t) = xs |> zip (indices xs)
  let sorted_ixs : [](i32, t) = blocked_radix_sort_int_by_key 256 (\ix -> p ix.1) i32.num_bits i32.get_bit ixs
  let tup : ([]i32, []t) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

-- sort column on given indices (via a gather operation)
entry def orderByIndices 'a (is: []i32) (ys: []a) : []a =
  is |> gather ys

-- PARTITIONING FUNCTIONS

-- sort indices on partition, return key-index pairs
-- TODO maybe would be more logically consistent to return a sortTup?
def keyIndexPairs 'k (ks: []k) : ([]k, []i32) =
  let is : []i32 = indices |> sortColumnOn (\i -> ks[i]) 
  let sorted_ks : []k = ks |> orderByIndices is
  in (sorted_ks, is)

-- partition and sort xs on key-index pairs
def keyByIndices 'a (kis: ([]i32, []i32)) (xs: []a) : []a =
  xs |> orderByIndices kis.1

-- return key-count pairs, for a known set of keys
def countByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
  keyValues
    |> map (\k -> keysFromXs |> countFor (== k))
    |> zip keyValues
  --keysFromXs |> multiCount keyValues |> zip keysFromXs -- TODO ???

-- set partitions for a key column xs by key function
-- NOTE: this requires knowledge of key values
-- TODO figure out how to make entry point, since one of the args is a function
-- TODO will range work with polymorphic k?
def keyBy 'k 't 
  (kf : t -> k)
  (xs : []t) 
: keyTup 'k 't =
  let ks : []k = xs |> map kf
  let kis : ([]k, []i32) = keyIndexPairs ks
  let kvals : []k = kis.0[0]..kis.1[-1] -- TODO collect distinct keys instead, and use this in a keyByInteger specialised function
  let ordered_xs : ([]t) = xs |> keyByIndices kis
  let keyCounts = kis.0 |> countByKey kvals
  in {numKeys = length kvals,
      keyCounts = keyCounts,
      ks = kvals,
      is = kis.1,
      xs = ordered_xs}

def keyAndSelectBy 'k 't 
  (kf : t -> k)
  (kvals : []k)
  (xs : []t) 
: keyTup 'k 't =
  let ks : []k = xs |> map kf
  let kis : ([]k, []i32) = keyIndexPairs ks
    |> filter (\k -> kvals |> (countFor (== k) > 0))
  let ordered_xs : ([]t) = xs |> keyByIndices kis
  let keyCounts = kis.0 |> countByKey kvals
  in {numKeys = length kvals,
      keyCounts = keyCounts,
      ks = kvals,
      is = kis.1,
      xs = ordered_xs}

-- intervals are minimum-inclusive, maximum-exclusive - the last element is the maximum allowed value, inclusive
-- TODO will need to take care in case it belongs to no interval...
entry def keyByInterval [lk] 't
  (orderedMinima : [lk]t)
  (xs : []t)
: keyTup 't 't =
  let determineKey (x: t) : t -> i32 =
    let i = orderedMinima |> argmaxFor (<= x)
    in if (i >= 0 && x <= orderedMinima[lk-1])
      then min(i, lk-1)
      else -1
  in keyAndSelectBy determineKey (iota lk-1) xs
  
  

-- MAPRED FUNCTIONS

def sumByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
  keyValues |> map (\k -> keysFromXs |> sumFor (== k)) |> zip keyValues
