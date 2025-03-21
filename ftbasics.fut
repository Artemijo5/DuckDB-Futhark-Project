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

-- TODO figure out how to do the following with automatic type inferrence...

def untyped_sumFor 't
    (sum: []t -> t)
    (mult: t -> i32 -> t) -- this one might be a pain...
    (p: t -> bool) (xs: []t) : t =
  sum (xs |> map (\x -> (x `mult` ((p >-> i32.bool) x)) ))

-- find greatest element that satisfies property
def untyped_argmaxFor 't
    (lowest: t)
    (gt: t -> t -> bool)
    (eq: t -> t ->bool)
    (p: t -> bool) (xs: []t) : i32 =
  let tup = 
    reduce_comm (\(ix, vx) (iy, vy) ->
        if (p vx) && !(p vy) then (ix, vx) else
        if !(p vx) && (p vy) then (iy, vy) else
        if !((p vx) || (p vy)) then (-1, vx) else
        if (vx `gt` vy) || ((vx `eq` vy) && (ix > iy)) then (ix, vx)
        else (iy, vy)
      )
      (-1, lowest)
      (xs |> zip ((indices xs) |> map (i32.i64)))
  in tup.0

def argmaxFor 't (p: t -> bool) (xs: []t) : t =
  -- TODO placeholder
  -1
  

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
