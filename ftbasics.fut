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
  

-- MAPRED FUNCTIONS

--def sumByKey (keyValues: []i32) (keysFromXs: []i32) : [](i32, i32) =
--  keyValues |> map (\k -> keysFromXs |> sumFor (== k)) |> zip keyValues
