import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

-- Local modules for sorting functions
local module shortSorter = intData i16
local module intSorter = intData i32
local module longSorter = intData i64
local module floatSorter = fltData f32
local module doubleSorter = fltData f64
-- Order a payload column given the reordered indices.
local def orderByIndices 't (block_size: idx_t.t) (is: [](idx_t.t)) (ys: []t) : []t =
  partitioned_gather block_size ys is

-- | Sort a column of short type.
entry radixSortColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) : sortInfo_short [n] =
  shortSorter.blocked_sort incr block_size xs
-- | Sort a column of integer type.
entry radixSortColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) : sortInfo_int [n] =
  intSorter.blocked_sort incr block_size xs
-- | Sort a column of long type.
entry radixSortColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) : sortInfo_long [n] =
  longSorter.blocked_sort incr block_size xs
-- | Sort a column of float type.
entry radixSortColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) : sortInfo_float [n] =
  floatSorter.blocked_sort incr block_size xs
-- | Sort a column of double type.
entry radixSortColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) : sortInfo_double [n] =
  doubleSorter.blocked_sort incr block_size xs

-- | Sort a column of short type.
entry mergeSortColumn_short [n] (incr: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
  shortSorter.sort incr xs
-- | Sort a column of integer type.
entry mergeSortColumn_int [n] (incr: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
  intSorter.sort incr xs
-- | Sort a column of long type.
entry mergeSortColumn_long [n] (incr: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
  longSorter.sort incr xs
-- | Sort a column of float type.
entry mergeSortColumn_float [n] (incr: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
  floatSorter.sort incr xs
-- | Sort a column of double type.
entry mergeSortColumn_double [n] (incr: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
  doubleSorter.sort incr xs

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) : [n]i16 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size offset_is ys
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) : [n]i32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size offset_is ys
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) : [n]i64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size offset_is ys
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) : [n]f32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size offset_is ys
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) : [n]f64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size offset_is ys


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

-- TODO naive for now, improve based on papers...
-- doesn't work as I expected... will need to copy paper ig...
--entry mergeFunc [na] [nb] (threads: i64) (as: [na]i32) (bs: [nb]i32) : [na+nb]i32 =
--  let perThread = (na+nb+threads-1)/threads
--  let partitionPoints : [threads](i64, i64) = partitionFunc threads as bs
--  let ts = iota threads
--  let da : [threads][perThread]i32 = replicate threads (replicate perThread 0)
--  let mda = da
--    |> zip ts
--    |> map (\(ti, ua) ->
--      let arr_loop : {j: idx_t.t, a_j: idx_t.t, b_j: idx_t.t, buff: [perThread]i32}
--        = loop p = {j=0, a_j=partitionPoints[ti].0, b_j=partitionPoints[ti].1, buff = replicate perThread 0}
--        while (p.j < perThread  && p.j + perThread*ti < (na+nb)) do ( -- this actually needs to handle partition indices ...
--          if (as[p.a_j] >= bs[p.b_j])
--          then {j = p.j + 1, a_j = p.a_j + 1, b_j = p.b_j, buff = p.buff with [p.j] = as[p.a_j]}
--          else {j = p.j + 1, a_j = p.a_j, b_j = p.b_j + 1, buff = p.buff with [p.j] = bs[p.b_j]}
--        )
--      in arr_loop.buff
--    )
--  let final_arr = (flatten mda)[0:(na+nb)]
--  in final_arr

-- TODO random experiment
type mergeInfo [len] 't = {is: [len](idx_t.t), xs: [len]t, cs: [len](idx_t.t)}
type mergeInfo_short [n] = mergeInfo [n] i16
type mergeInfo_int [n] = mergeInfo [n] i32
type mergeInfo_long [n] = mergeInfo [n] i64
type mergeInfo_float [n] = mergeInfo [n] f32
type mergeInfo_double [n] = mergeInfo [n] f64

local def mergeTups [w1] [w2] 't (neq: t -> t -> bool)  (xs : [w1]t) (ys : [w2]t) : mergeInfo [w1] t =
  let ixcs = zip3 (indices xs) xs (replicate (w1) 0)
  let iycs = zip3 (indices ys) ys (replicate (w2) 1)
  let ms = ixcs
    |> map (\(i, x, c) ->
      reduce_comm (\(i1, y1, c1) (i2, y2, c2) ->
        if ((y1 `neq` x) || i1 < 0) && ((y2 `neq` x) || i2 < 0) then (-1, x, 0)
        else if (i1<0 || (y1 `neq` x)) then (i2, y2, c2)
        else if (i2<0 || (y2 `neq` x)) then (i1, y1, c1)
        else ((if (i1<i2 || i2<0) then i1 else i2), x, c1+c2)
      )
      (-1, x, 0)
      iycs
    )
    |> unzip3
  in {is = ms.0, xs = ms.1, cs = ms.2}

entry mergeTups_short [w1] [w2]  (xs : [w1]i16) (ys : [w2]i16) : mergeInfo_short [w1] =
  mergeTups (!=) xs ys
entry mergeTups_int [w1] [w2]  (xs : [w1]i32) (ys : [w2]i32) : mergeInfo_int [w1] =
  mergeTups (!=) xs ys
entry mergeTups_long [w1] [w2]  (xs : [w1]i64) (ys : [w2]i64) : mergeInfo_long [w1] =
  mergeTups (!=) xs ys
entry mergeTups_float [w1] [w2]  (xs : [w1]f32) (ys : [w2]f32) : mergeInfo_float [w1] =
  mergeTups (!=) xs ys
entry mergeTups_double [w1] [w2]  (xs : [w1]f64) (ys : [w2]f64) : mergeInfo_double [w1] =
  mergeTups (!=) xs ys