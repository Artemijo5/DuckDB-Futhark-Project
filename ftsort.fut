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

def partFunc_sequential_search [n] 't
  (a_i: idx_t.t)
  (bi: idx_t.t)
  (as: [n]t)
  (bs: [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: (idx_t.t, idx_t.t) =
  let ai = if a_i < n then a_i else n-1
  let b_prev = if bi > 0 then bs[bi-1] else bs[0]
  let seqSearch : {k: idx_t.t, ak: idx_t.t, bk: idx_t.t} =
    if as[ai] `gt` b_prev then
      loop p = {k=0, ak=ai, bk=bi}
      while (p.k < n-bi) do
        let i_a = ai-p.k
        let i_b = bi+p.k
        in
          if (as[i_a-1] `leq` bs[i_b])
          then {k = n-bi, ak = i_a, bk = i_b} --break
          else {k = p.k+1, ak = ai, bk = bi} -- continue
    else
      loop p = {k=0, ak=ai, bk=bi}
      while (p.k < bi-1) do
        let hi_a = ai+p.k+1
        let i_a = if hi_a < n then hi_a else n-1
        let i_b = bi - p.k - 1
        in
          if (as[i_a] `gt` bs[i_b])
          then {k = bi-1, ak = i_a, bk = i_b+1} -- break
          else {k = p.k+1, ak = ai, bk = bi} -- continue
    in (seqSearch.ak, seqSearch.bk)

def partitionFunc [n] 't
  (partitions: idx_t.t)
  (upToThread: idx_t.t)
  (as : [n]t)
  (bs : [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: [upToThread](idx_t.t, idx_t.t) =
  let lowest = if (as[0] `leq` bs[0]) then as[0] else bs[0]
  --let highest = if (as[n-1] `gt` bs[n-1]) then as[n-1] else bs[n-1]
  let ts : [upToThread-1]idx_t.t = 1..<upToThread
  let pre_ret : [upToThread-1](idx_t.t, idx_t.t) = ts
    |> map (\t ->
      let index : idx_t.t = t*(n+n)/partitions
      let loop_over : {flag: bool, a_top: idx_t.t, b_top: idx_t.t, a_bottom: idx_t.t} =
        loop p = {
          flag = true,
          a_top = if (index > n) then n else index,
          b_top = if (index > n) then (index - n) else 0,
          a_bottom = if (index > n) then (index - n - 1) else 0
        }
        while p.flag do
          let offset = (p.a_top - p.a_bottom)/2
          let ai = (p.a_top - offset)
          let bi = idx_t.max 0 (p.b_top + offset)
          let b_prev = if bi>0 then bs[bi-1] else lowest
          --let a_prev =if ai <= 0 then lowest else if ai > na then highest else as[ai-1]
          --let b_prev = if bi <= 0 then lowest else if bi > nb then highest else bs[bi-1]
          in
            -- Corner Cases
              if ai <= 0-- First Partition, or reached beginning of as
                then {flag=false, a_top=0, b_top=bi, a_bottom=p.a_bottom}
              else if bi >= n -- Reached end of Bs
                then
                  let a_ind = idx_t.min (ai) (n-1)
                  in
                    if (bs[n-1] `leq` as[a_ind])
                    then {flag=false, a_top=ai, b_top=n, a_bottom=p.a_bottom} -- break
                    else {flag=true, a_top=p.a_top, b_top = n-1-offset, a_bottom = p.a_bottom} -- continue
              else if (ai >= n) -- Reached end of As
                then
                  let b_ind = idx_t.min (bi) (n-1)
                  in
                    if (as[n-1] `leq` bs[b_ind])
                    then {flag=false, a_top=n, b_top=bi, a_bottom=p.a_bottom} -- break
                    else {flag=true, a_top=n-1, b_top=p.b_top, a_bottom=p.a_bottom} -- continue
              else if (p.a_top <= p.a_bottom) then -- resort to sequential search
                let (sai, sbi) = partFunc_sequential_search ai bi as bs (leq) (gt)
                in {flag=false, a_top=sai, b_top=sbi, a_bottom=p.a_bottom} -- break
              else
            -- Regular Cases
            if (as[ai] `gt` b_prev) then
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
  let ret = [(0,0)] ++ pre_ret :> [upToThread](idx_t.t, idx_t.t)
  in ret

def mergeFunc_seq [n] 't
  (threads: idx_t.t)
  (as: [n]t)
  (bs: [n]t)
  (zero: t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: ((idx_t.t,idx_t.t), [n](i8, idx_t.t, t)) =
  let partitionBounds : [threads+1](idx_t.t, idx_t.t) = (partitionFunc (2*threads) (threads+1) as bs leq gt)
  let buff : [n](i8, idx_t.t, t) = 
    let lbuff : (idx_t.t, [n](i8, idx_t.t, t)) =
      loop p = (0, replicate (n) (-1, -1, zero))
      while p.0 < (threads) do
        let thr = p.0
        let lbs = partitionBounds[thr]
        let ubs = partitionBounds[thr+1]
        let ind_bottom = lbs.0 + lbs.1
        let ind_top = ubs.0 + ubs.1
        let at = zip3 (replicate (ubs.0-lbs.0) 0) ((lbs.0)..<(ubs.0)) as[lbs.0:ubs.0]
        let bt = zip3 (replicate (ubs.1-lbs.1) 1) ((lbs.1)..<(ubs.1)) bs[lbs.1:ubs.1]
        let merger = (at ++ bt)
          |> merge_sort (\x1 x2 ->
            if (x1.0==x2.0)
            then (x1.1<=x2.1) -- if same relation, keep index order
            else (x1.2 `leq` x2.2) -- among relations, compare values
          )
          :> [ind_top - ind_bottom](i8,idx_t.t,t)
        let newSections = p.1 with [ind_bottom:ind_top] = merger[0:(ind_top-ind_bottom)]
        in (thr+1, newSections)
    in lbuff.1
  in (partitionBounds[threads], buff)

def mergeFunc_par [n] 't
  (threads: idx_t.t)
  (as: [n]t)
  (bs: [n]t)
  (zero: t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: ((idx_t.t,idx_t.t), [n](i8, idx_t.t, t)) =
  let perThread = 1 + n/threads
  let partitionBounds : [threads+1](idx_t.t, idx_t.t) = partitionFunc (2*threads) (threads+1) as bs leq gt
  let ts = iota threads
  let sections : [threads][perThread](idx_t.t,(i8,idx_t.t,t)) = ts
    |> map(\thr ->
      let secBuff = replicate perThread (-1, -1, zero)
      let lbs = partitionBounds[thr]
      let ubs = partitionBounds[thr+1]
      let card = (ubs.0+ubs.1) - (lbs.0+lbs.1)
      let at = zip3 (replicate (ubs.0-lbs.0) 0) ((lbs.0)..<(ubs.0)) as[lbs.0:ubs.0]
      let bt = zip3 (replicate (ubs.1-lbs.1) 1) ((lbs.1)..<(ubs.1)) bs[lbs.1:ubs.1]
      let merger = (at ++ bt)
        |> merge_sort (\x1 x2 ->
          if (x1.0==x2.0)
          then (x1.1<=x2.1) -- if same relation, keep index order
          else (x1.2 `leq` x2.2) -- among relations, compare values
        )
        :> [card](i8,idx_t.t,t)
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
  in (partitionBounds[threads], scatter (replicate (n) (-1, -1, zero)) ims.0 ims.1)

-- TODO change rs to i8 since only 2 relations are really merged...
-- | Type used to preserve relation & index information for Sort Merge Join.
type mergeInfo [len] 't = {rs: [len]i8, is: [len]idx_t.t, vs: [len]t}
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
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
  (inParallel: bool)
: mergeInfo [na+nb] t =
  let loop_over : {lbs: (idx_t.t, idx_t.t), buffer: [na+nb](i8, idx_t.t, t)}
  = loop p = {lbs = (0,0), buffer = replicate (na+nb) (-1, -1, zero)}
  while (p.lbs.0 < na-window_size && p.lbs.1 < nb-window_size) do
    let ubs = (p.lbs.0 + window_size, p.lbs.1 + window_size)
    let ind_bottom = p.lbs.0 + p.lbs.1
    let ind_top = ind_bottom + window_size
    let at = a_list[p.lbs.0:ubs.0] :> [window_size]t
    let bt = b_list[p.lbs.1:ubs.1] :> [window_size]t
    let merger : ((idx_t.t, idx_t.t), [window_size](i8, idx_t.t, t)) =
      if inParallel
      then mergeFunc_par threads at bt zero leq gt
      else mergeFunc_seq threads at bt zero leq gt
    let shifted_merger = merger.1
      |> map (\(r,i,v) -> (r, if r==0 then i+p.lbs.0 else i+p.lbs.1, v))
      :> [window_size](i8, idx_t.t, t)
    let next_lbs = (merger.0.0 + p.lbs.0, merger.0.1 + p.lbs.1)
    let next_buff = p.buffer with [ind_bottom:ind_top] = (copy shifted_merger)
    in {lbs = next_lbs, buffer = next_buff}
  let finFrom = loop_over.lbs
  let lenAs = na - finFrom.0
  let lenBs = nb - finFrom.1
  let at_f = a_list[finFrom.0:na] :> [lenAs]t
  let zat_f : [lenAs](i8, idx_t.t, t)
    = zip3
      (replicate lenAs 0)
      (((finFrom.0)..<na) :> [lenAs]idx_t.t)
      at_f
  let bt_f = b_list[finFrom.1:nb] :> [lenBs]t
  let zbt_f : [lenBs](i8, idx_t.t, t)
   = zip3
      (replicate lenBs 1)
      (((finFrom.1)..<nb) :> [lenBs]idx_t.t)
      bt_f
  let merger_f = (zat_f ++ zbt_f)
    |> merge_sort (\x1 x2 ->
      if (x1.0==x2.0)
      then (x1.1<=x2.1) -- if same relation, keep index order
      else (x1.2 `leq` x2.2) -- among relations, compare values
    )
    :> [lenAs + lenBs](i8, idx_t.t, t)
  let final_buffer = loop_over.buffer with [(finFrom.0 + finFrom.1):na+nb] = merger_f[0:lenAs+lenBs]
  let split_buffer = final_buffer |> unzip3
  in {rs = split_buffer.0, is = split_buffer.1, vs = split_buffer.2}

entry mergeSorted_short [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i16)
  (b_list: [nb]i16)
  (inParallel: bool)
:  mergeInfo_short [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 (<=) (>) inParallel

entry mergeSorted_int [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i32)
  (b_list: [nb]i32)
  (inParallel: bool)
:  mergeInfo_int [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 (<=) (>) inParallel

entry mergeSorted_long [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]i64)
  (b_list: [nb]i64)
  (inParallel: bool)
:  mergeInfo_long [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 (<=) (>) inParallel

entry mergeSorted_float [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]f32)
  (b_list: [nb]f32)
  (inParallel: bool)
:  mergeInfo_float [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 (<=) (>) inParallel

entry mergeSorted_double [na] [nb]
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (a_list: [na]f64)
  (b_list: [nb]f64)
  (inParallel: bool)
:  mergeInfo_double [na+nb] =
  merge_pipeline window_size threads a_list b_list 0 (<=) (>) inParallel
  

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