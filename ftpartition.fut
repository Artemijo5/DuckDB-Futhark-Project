import "ftbasic"
import "ftsort"

type keyTup [lk] [lx] 'k 't = {numKeys: i32, keyCounts: [lk]i32, ks: [lk]k, is: [lx]i32, xs: [lx]t}

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
