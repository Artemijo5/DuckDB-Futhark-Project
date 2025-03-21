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

def gather 't (xs: []t) (is: []i32) =
  is |> map (\i -> xs[i])

def countFor 't (p: t -> bool) (xs: []t) : i32 =
  i32.sum (xs |> map (p >-> i32.bool))

--def sumFor 't (p: t -> bool) (xs: []t) =
--  reduce (+) 0 (xs |> map (\x -> x*(p >-> t.bool))) -- TODO bool most likely needs to be called by a module...

-- find greatest element that satisfies property
-- TODO test if it works
def argmaxFor 't (p: t -> bool) (xs: []t) : i32 =
  let tup = 
    reduce_comm (\(ix, vx) (iy, vy) ->
        if (p x) && (vx > vy || (vx == vy && ix > iy))
          then (ix, vx)
        else if (p y) then (iy, vy)
        else (-1, vy)
      )
      (t.lowest, -1)
      (xs |> zip (indices xs))
  in tup.1
  

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
