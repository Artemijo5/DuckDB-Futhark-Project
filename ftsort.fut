import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

-- Local modules for sorting functions
local module shortSorter = intData i16
local module intSorter = intData i32
local module longSorter = intData i64
local module floatSorter = fltData f32
local module doubleSorter = fltData f64
-- Order a payload column given the reordered indices.
local def orderByIndices 't (is: [](idx_t.t)) (ys: []t) : []t =
  is |> gather ys

-- | Sort a column of short type.
entry sortColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) : sortInfo_short [n] =
  shortSorter.blocked_sort incr block_size xs
-- | Sort a column of integer type.
entry sortColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) : sortInfo_int [n] =
  intSorter.blocked_sort incr block_size xs
-- | Sort a column of long type.
entry sortColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) : sortInfo_long [n] =
  longSorter.blocked_sort incr block_size xs
-- | Sort a column of float type.
entry sortColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) : sortInfo_float [n] =
  floatSorter.blocked_sort incr block_size xs
-- | Sort a column of double type.
entry sortColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) : sortInfo_double [n] =
  doubleSorter.blocked_sort incr block_size xs

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short [n] [ni] (incr: idx_t.t) (is: [ni](idx_t.t)) (ys: [n]i16) : [ni]i16 =
  let offset_is : [ni](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int [n] [ni] (incr: idx_t.t) (is: [ni](idx_t.t)) (ys: [n]i32) : [ni]i32 =
  let offset_is : [ni](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long [n] [ni] (incr: idx_t.t) (is: [ni](idx_t.t)) (ys: [n]i64) : [ni]i64 =
  let offset_is : [ni](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float [n] [ni] (incr: idx_t.t) (is: [ni](idx_t.t)) (ys: [n]f32) : [ni]f32 =
  let offset_is : [ni](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double [n] [ni] (incr: idx_t.t) (is: [ni](idx_t.t)) (ys: [n]f64) : [ni]f64 =
  let offset_is : [ni](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is


-- argmin - for getting chunk replacement priorities
local def argmin [n] 't
    (lt: t -> t -> bool)
    (eq: t -> t -> bool)
    (highest: t)
    (ks: [n]t)
    : idx_t.t = 
  let ne = (n, highest)
  let iks = ks
    |> zip (idx_t.indices ks)
  let min_ik = reduce_comm(\(ix, vx) (iy, vy) ->
        if (vx `lt` vy) || ((vx `eq` vy) && (ix < iy))
          then (ix, vx)
          else (iy, vy)
      ) ne iks
  in min_ik.0

-- | Obtain the index of the smallest element from a list of shorts.
entry argmin_short [n] (ks: [n]i16) : idx_t.t =
  argmin (<) (==) (i16.highest) ks
-- | Obtain the index of the smallest element from a list of integers.
entry argmin_int [n] (ks: [n]i32) : idx_t.t =
  argmin (<) (==) (i32.highest) ks
-- | Obtain the index of the smallest element from a list of longs.
entry argmin_long [n] (ks: [n]i64) : idx_t.t =
  argmin (<) (==) (i64.highest) ks
-- | Obtain the index of the smallest element from a list of floats.
entry argmin_float [n] (ks: [n]f32) : idx_t.t =
  argmin (<) (==) (f32.highest) ks
-- | Obtain the index of the smallest element from a list of doubles.
entry argmin_double [n] (ks: [n]f64) : idx_t.t =
  argmin (<) (==) (f64.highest) ks


-- TODO figure out GPU Merge Path
-- till then, make a 'naive' join with BNLJ logic...

entry partitionFunc [na] [nb] (threads: i64) (as : [na]i32) (bs : [nb]i32) : [threads](i64, i64) =
  let Adiag = replicate threads na
  let Bdiag = replicate threads nb
  let ts = iota threads
  let ret = (zip3 ts Adiag Bdiag) |> map
    (\(i, ad, bd) ->
      let index = i*(na + nb)/threads
      let atop = if index > na then na else index
      let btop = if index > na then index-na else 0
      let abottom = btop
      let bigTuple : {a_top: i64, b_top: i64, a_bottom: i64, flag: bool, a_i : i64, b_i : i64}
        = loop p = {a_top=atop, b_top=btop, a_bottom=abottom, flag=true, a_i=na, b_i=nb}
        while p.flag do (
          let offset = (p.a_top - p.a_bottom)/2
          let ai = p.a_top - offset
          let bi = p.b_top + offset
          -- what to do with corner cases???
          let acur = if (0<=ai && ai<na) then as[ai] else if ai<0 then i32.lowest else i32.highest
          let aprev = if 0<ai then as[ai-1] else i32.lowest
          let bcur = if (0<=bi && bi<nb) then bs[bi] else if bi<0 then i32.lowest else i32.highest
          let bprev = if 0<bi then bs[bi-1] else i32.lowest
          in if acur > bprev then
            if aprev <= bcur then
              {a_top=p.a_top, b_top=p.b_top, a_bottom=p.a_bottom, flag=false, a_i=ai, b_i=bi}
            else
              {a_top=ai-1, b_top=bi+1, a_bottom=p.a_bottom, flag=true, a_i=na, b_i=nb}
          else
            {a_top=p.a_top, b_top=p.b_top, a_bottom=ai+1, flag=true, a_i=na, b_i=nb}
        )
      in (i, bigTuple.a_i, bigTuple.b_i)
    )
  in ret |> map (\(t, ad, bd) -> (ad, bd))

entry mergeFunc [w1] [w2]  (xs : [w1]i32) (ys : [w2]i32) : [w1](i64, i32, i64) =
  let ixcs = zip3 (indices xs) xs (replicate (w1) 0)
  let iycs = zip3 (indices ys) ys (replicate (w2) 1)
  in ixcs
    |> map (\(i, x, c) ->
      reduce_comm (\(i1, y1, c1) (i2, y2, c2) ->
        if (y1 != x || i1 >= w2) && (y2 != x || i2 >= w2) then (w2, x, 0)
        else if (i1>=w2 || y1!=x) then (i2, y2, c2)
        else if (i2>=w2 || y2!=x) then (i1, y1, c1)
        else ((if (i1<i2 || i2>=w2) then i1 else i2), x, c1+c2)
      )
      (w2, x, 0)
      iycs
    )

--entry mergeTupsToPairs (ixcs : [](i64, i32, i64)) : [](i32, i64, i64) = 
--  let incrementalCounts = scan (+) 0 (ixcs |> map (\ixc -> ixc.2))
--  let numPairs = incrementalCounts[-1]
--  let newArr = replicate numPairs (i32.0, i64.-1, i64.-1)
  -- for each index of newArr, find the first element of x such that incrementalCounts
  -- AAAERGH I'll have to look at the paper!!!!!!