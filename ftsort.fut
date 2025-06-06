import "lib/github.com/diku-dk/sorts/merge_sort"
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

-- ------------------------------------------------------------------- MERGE JOIN

-- TODO figure out .......................

def partitionFunc [na] [nb] 't
  (threads: idx_t.t)
  (as : [na]t)
  (bs : [nb]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: [threads](idx_t.t, idx_t.t) =
  let lowest = if (as[0] `leq` bs[0]) then as[0] else bs[0]
  let highest = if (as[na-1] `gt` bs[nb-1]) then as[na-1] else bs[nb-1]
  let ts = iota threads
  let ret : [threads](idx_t.t, idx_t.t) = ts
    |> map (\t ->
      let index : idx_t.t = t*(na+nb)/threads
      let loop_over : {flag: bool, a_top: idx_t.t, b_top: idx_t.t, a_bottom: idx_t.t} =
        loop p = {
          flag = true,
          a_top = if (index > na) then na else index,
          b_top = if (index > na) then (index - na) else 0,
          a_bottom = if (index > na) then (index - na - 1) else 0
        }
        while p.flag do
          let offset = (p.a_top - p.a_bottom)/2
          let ai = (p.a_top - offset) -- hypothetical ai
          let bi = (p.b_top + offset) -- hypothetical bi
          --let a_prev =if ai <= 0 then lowest else if ai > na then highest else as[ai-1]
          --let b_prev = if bi <= 0 then lowest else if bi > nb then highest else bs[bi-1]
          in
            -- Corner Cases
            if (ai <= 0 || ai >= na || bi <= 0 || bi >= nb || p.a_top < p.a_bottom) then
              let ar = if ai<=0 then 0 else if ai>= na then na-1 else ai
              let br = if bi<=0 then 0 else if bi>= nb then nb-1 else bi
              let loop_on = !(ai <= 0 || p.a_top < p.a_bottom)
              in {flag = loop_on, a_top = ar, b_top = br, a_bottom = p.a_bottom}
            else
            -- Regular Cases
            if (as[ai] `gt` bs[bi-1]) then
              if (as[ai-1] `leq` bs[bi]) then
                {
                  flag = false, -- break
                  a_top = ai,
                  b_top = bi,
                  a_bottom = p.a_bottom
                }
              else {
                flag = true,
                a_top = ai - 1,
                b_top = bi + 1,
                a_bottom = p.a_bottom
              }
            else {
              flag = true,
              a_top = p.a_top,
              b_top = p.b_top,
              a_bottom = ai + 1
            }
        in (loop_over.a_top, loop_over.b_top)
      )
  in ret

def mergeFunc_seq [na] [nb] 't
  (threads: idx_t.t)
  (as: [na]t)
  (bs: [nb]t)
  (zero: t)
  (lowest: t)
  (highest: t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: ((idx_t.t,idx_t.t), [na+nb](idx_t.t, idx_t.t, t)) =
  let perThread = (na+nb)/threads
  let partitionBounds : [threads](idx_t.t, idx_t.t) = partitionFunc threads as bs leq gt
  let buff : [na+nb](idx_t.t, idx_t.t, t) = 
    let lbuff : (idx_t.t, [na+nb](idx_t.t, idx_t.t, t)) =
      loop p = (0, replicate (na+nb) (-1, -1, zero))
      while p.0 < (threads-1) do
        let thr = p.0
        let lbs = partitionBounds[thr]
        let ubs = if thr+1<threads then partitionBounds[thr+1] else lbs
        let ind_bottom = lbs.0 + lbs.1
        let ind_top = ubs.0 + ubs.1
        let at = zip3 (replicate (ubs.0-lbs.0) 0) ((lbs.0)..<(ubs.0)) as[lbs.0:ubs.0]
        let bt = zip3 (replicate (ubs.1-lbs.1) 1) ((lbs.1)..<(ubs.1)) bs[lbs.1:ubs.1]
        let merger = (at ++ bt)
          |> (merge_sort_by_key (\tup -> tup.2)) (leq) :> [ind_top-ind_bottom](idx_t.t,idx_t.t,t)
        let newSections = p.1 with [ind_bottom:ind_top] = merger[0:(ind_top-ind_bottom)]
        in (thr+1, newSections)
    in lbuff.1
  in (partitionBounds[threads-1], buff)

def mergeFunc_par [na] [nb] 't
  (threads: idx_t.t)
  (as: [na]t)
  (bs: [nb]t)
  (zero: t)
  (lowest: t)
  (highest: t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: ((idx_t.t,idx_t.t), [na+nb](idx_t.t, idx_t.t, t)) =
  let perThread = 1 + (na+nb)/threads
  let partitionBounds : [threads](idx_t.t, idx_t.t) = partitionFunc threads as bs leq gt
  let ts = iota threads
  let sections : [threads][perThread](idx_t.t,(idx_t.t,idx_t.t,t)) = ts
    |> map(\thr ->
      let secBuff = replicate perThread (-1, -1, zero)
      let lbs = partitionBounds[thr]
      let ubs = if thr+1<threads then partitionBounds[thr+1] else lbs
      let card = (ubs.0+ubs.1) - (lbs.0+lbs.1)
      let at = zip3 (replicate (ubs.0-lbs.0) 0) ((lbs.0)..<(ubs.0)) as[lbs.0:ubs.0]
      let bt = zip3 (replicate (ubs.1-lbs.1) 1) ((lbs.1)..<(ubs.1)) bs[lbs.1:ubs.1]
      let merger = (at ++ bt)
        |> (merge_sort_by_key (\tup -> tup.2)) (leq) :> [card](idx_t.t,idx_t.t,t)
      let sec = secBuff with [0:card] = merger[0:card]
      let is_naive = iota perThread |> map (\i -> i+(lbs.0 + lbs.1))
      let is = if (card == perThread-1) then is_naive with [card] = -1 else is_naive
      in sec |> zip is
    )
    -- could also use ts[0:(threads-1)] and add the last one manually (...)
    -- issue lies in merging using 1 less thread than partitioning
    -- anyway, with a high enough number of threads, maybe it won't matter
    -- really, a cyclic buffer doesn't actually solve anything...
  let ims = sections |> flatten |> unzip
  in (partitionBounds[threads-1], scatter (replicate (na+nb) (-1, -1, zero)) ims.0 ims.1)

-- TODO change rs to i8 since only 2 relations are really merged...
-- | Type used to preserve relation & index information for Sort Merge Join.
type mergeInfo [len] 't = {rs: [len]idx_t.t, is: [len]idx_t.t, vs: [len]t}
-- | Sort Merge information type (short).
type mergeInfo_short [len] = mergeInfo [len] i16
-- | Sort Merge information type (int).
type mergeInfo_int [len] = mergeInfo [len] i32
-- | Sort Merge information type (long).
type mergeInfo_long [len] = mergeInfo [len] i64
-- | Sort Merge information type (float).
type mergeInfo_float [len] = mergeInfo [len] f32
-- | Sort Merge information type (double).
type mergeInfo_double [len] = mergeInfo [len] f64

def merge_pipeline [na] [nb] 't
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]t)
  (b_list: [nb]t)
  (zero: t)
  (lowest: t)
  (highest: t)
  (leq: t-> t -> bool)
  (gt: t -> t -> bool)
: mergeInfo [na+nb] t =
  let loop_over : {lbs: (idx_t.t,idx_t.t), buffer: [na+nb](idx_t.t, idx_t.t, t)} =
    loop p = {lbs = (0,0), buffer = replicate (na+nb) (-1, -1, zero)}
    while (p.lbs.0 < na-window_size || p.lbs.1 < nb-window_size) do
      let lb_a = p.lbs.0
      let lb_b = p.lbs.1
      let card_a = idx_t.min window_size (na - lb_a)
      let card_b = idx_t.min window_size (nb - lb_b)
      let lb_merge = lb_a + lb_b
      let ub_merge = lb_merge + card_a + card_b
      let as = a_list[lb_a : (lb_a + card_a)]
      let bs = b_list[lb_b : (lb_b + card_b)]
      let res = mergeFunc_par threads as bs zero lowest highest leq gt
      let corrected_res = res.1
        |> map(\(r, i, v) ->
          if r==0 then (r, i+lb_a, v)
          else if r==1 then (r, i+lb_b, v)
          else (r, i, v)
        )
      let corrected_ubs = (res.0.0 + lb_a, res.0.1 + lb_b)
      in {lbs = corrected_ubs, buffer = p.buffer with [lb_merge:ub_merge] = corrected_res}
  let flb_a = loop_over.lbs.0
  let flb_b = loop_over.lbs.1
  let f_as = zip3 (replicate (na-flb_a) 0) (flb_a..<na) a_list[flb_a:na]
  let f_bs = zip3 (replicate (nb-flb_b) 1) (flb_b..<nb) b_list[flb_b:nb]
  let finalMerge = (f_as ++ f_bs)
    |> (merge_sort_by_key (\tup -> tup.2)) (leq) :> [(na+nb)-(flb_a+flb_b)](idx_t.t,idx_t.t,t)
  let finalBuffer = loop_over.buffer with [(flb_a+flb_b):(na+nb)] = finalMerge
  let fBs = finalBuffer |> unzip3
  in {rs = fBs.0, is = fBs.1, vs = fBs.2}
  -- TODO
  --  ADD COMMENTS this is kind of a mess...


entry mergeSorted_short [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i16)
  (b_list: [nb]i16)
:  mergeInfo_short [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 i16.lowest i16.highest (<=) (>)

entry mergeSorted_int [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i32)
  (b_list: [nb]i32)
:  mergeInfo_int [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 i32.lowest i32.highest (<=) (>)

entry mergeSorted_long [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i64)
  (b_list: [nb]i64)
:  mergeInfo_long [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 i64.lowest i64.highest (<=) (>)

entry mergeSorted_float [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]f32)
  (b_list: [nb]f32)
:  mergeInfo_float [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 f32.lowest f32.highest (<=) (>)

entry mergeSorted_double [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]f64)
  (b_list: [nb]f64)
:  mergeInfo_double [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 f64.lowest f64.highest (<=) (>)

  

-- TODO random experiment
type mergeTup [len] 't = {is: [len](idx_t.t), xs: [len]t, cs: [len](idx_t.t)}
type mergeTup_short [n] = mergeTup [n] i16
type mergeTup_int [n] = mergeTup [n] i32
type mergeTup_long [n] = mergeTup [n] i64
type mergeTup_float [n] = mergeTup [n] f32
type mergeTup_double [n] = mergeTup [n] f64

local def mergeTups [w1] [w2] 't (neq: t -> t -> bool)  (xs : [w1]t) (ys : [w2]t) : mergeTup [w1] t =
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

entry mergeTups_short [w1] [w2]  (xs : [w1]i16) (ys : [w2]i16) : mergeTup_short [w1] =
  mergeTups (!=) xs ys
entry mergeTups_int [w1] [w2]  (xs : [w1]i32) (ys : [w2]i32) : mergeTup_int [w1] =
  mergeTups (!=) xs ys
entry mergeTups_long [w1] [w2]  (xs : [w1]i64) (ys : [w2]i64) : mergeTup_long [w1] =
  mergeTups (!=) xs ys
entry mergeTups_float [w1] [w2]  (xs : [w1]f32) (ys : [w2]f32) : mergeTup_float [w1] =
  mergeTups (!=) xs ys
entry mergeTups_double [w1] [w2]  (xs : [w1]f64) (ys : [w2]f64) : mergeTup_double [w1] =
  mergeTups (!=) xs ys