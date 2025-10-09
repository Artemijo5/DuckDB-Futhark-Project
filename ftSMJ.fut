import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

-- Sorting Functions

-- 1. Radix Sort
-- TODO make bit_step accessible from entry points
-- TODO implement blocked algorithm like in sorts library?

def my_radix_sort [n] 't
  (block_size : idx_t.t)
  (bit_step : i32)
  (num_bits : i32)
  (get_bit : i32 -> t -> i32)
  (xs : [n]t)
: [n]t =
  loop xs
  for bit in (0..bit_step..<num_bits)
  do radix_sort_multistep block_size bit (i32.min num_bits (bit+bit_step-1)) get_bit xs

-- Based on futhark sorts library radix_sort_int
def my_radix_sort_int [n] 't
  (block_size : idx_t.t)
  (bit_step : i32)
  (num_bits : i32)
  (get_bit : i32 -> t -> i32)
  (xs : [n]t)
: [n]t =
  let get_bit' i x =
    -- Flip the most significant bit.
    let b = get_bit i x
    in if i == num_bits-1 then b ^ 1 else b
  in my_radix_sort block_size bit_step num_bits get_bit' xs

-- Based on futhark sorts library radix_sort_float
def my_radix_sort_float [n] 't
  (block_size : idx_t.t)
  (bit_step : i32)
  (num_bits : i32)
  (get_bit : i32 -> t -> i32)
  (xs : [n]t)
: [n]t =
  let get_bit' i x =
    -- We flip the bit returned if:
    --
    -- 0) the most significant bit is set (this makes more negative
    --    numbers sort before less negative numbers), or
    --
    -- 1) we are asked for the most significant bit (this makes
    --    negative numbers sort before positive numbers).
    let b = get_bit i x
    in if get_bit (num_bits-1) x == 1 || i == num_bits-1
       then b ^ 1 else b
  in my_radix_sort block_size bit_step num_bits get_bit' xs


-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- Sorting Code
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################

-- GFTR SORTING
-- TODO see ftError.fut

-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
type sortStruct [n] [b] 'a = {k: [n]a, pL: [n][b]u8}

type sortStruct_short [n] [b] = sortStruct [n] [b] i16
type sortStruct_int [n] [b] = sortStruct [n] [b] i32
type sortStruct_long [n] [b] = sortStruct [n] [b] i64
type sortStruct_float [n] [b] = sortStruct [n] [b] f32
type sortStruct_double [n] [b] = sortStruct [n] [b] f64

local def radixSortRelation_signed_integral [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = my_radix_sort_int (i64.i16 block_size) 2 num_bits (\i xy -> get_bit i xy.0) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
local def radixSortRelation_signed_float [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = my_radix_sort_float (i64.i16 block_size) 2 num_bits (\i xy -> get_bit i xy.0) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
local def mergeSortRelation [n] [b] 'a
  (xs: sortStruct [n] [b] a)
  (leq: a -> a -> bool)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = merge_sort_by_key (\xy -> xy.0) (leq) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}

entry radixSortRelation_short [n] [b]
  (block_size: i16)
  (xs: sortStruct_short [n] [b])
 : sortStruct_short [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i16.num_bits) (i16.get_bit)
entry radixSortRelation_int [n] [b]
  (block_size: i16)
  (xs: sortStruct_int [n] [b])
 : sortStruct_int [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i32.num_bits) (i32.get_bit)
entry radixSortRelation_long [n] [b]
  (block_size: i16)
  (xs: sortStruct_long [n] [b])
 : sortStruct_long [n] [b]
  = radixSortRelation_signed_integral (block_size) (xs) (i64.num_bits) (i64.get_bit)
entry radixSortRelation_float [n] [b]
  (block_size: i16)
  (xs: sortStruct_float [n] [b])
 : sortStruct_float [n] [b]
  = radixSortRelation_signed_float (block_size) (xs) (f32.num_bits) (f32.get_bit)
entry radixSortRelation_double [n] [b]
  (block_size: i16)
  (xs: sortStruct_double [n] [b])
 : sortStruct_double [n] [b]
  = radixSortRelation_signed_float (block_size) (xs) (f64.num_bits) (f64.get_bit)

entry mergeSortRelation_short [n] [b]
  (xs: sortStruct_short [n] [b])
: sortStruct_short [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_int [n] [b]
  (xs: sortStruct_int [n] [b])
: sortStruct_int [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_long [n] [b]
  (xs: sortStruct_long [n] [b])
: sortStruct_long [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_float [n] [b]
  (xs: sortStruct_float [n] [b])
: sortStruct_float [n] [b]
  = mergeSortRelation (xs) (<=)
entry mergeSortRelation_double [n] [b]
  (xs: sortStruct_double [n] [b])
: sortStruct_double [n] [b]
  = mergeSortRelation (xs) (<=)

-- GFUR SORTING

-- | Type used to preserve original index information when sorting (GFUR).
type sortInfo [len] 't = {is: [len](idx_t.t), xs: [len]t}
-- | Sorting information type (short)(GFUR).
type sortInfo_short [n] = sortInfo [n] i16
-- | Sorting information type (integer)(GFUR).
type sortInfo_int [n] = sortInfo [n] i32
-- | Sorting information type (long)(GFUR).
type sortInfo_long [n] = sortInfo [n] i64
-- | Sorting information type (float)(GFUR).
type sortInfo_float [n] = sortInfo [n] f32
-- | Sorting information type (double)(GFUR).
type sortInfo_double [n] = sortInfo [n] f64

local def radixSortColumn_signed_integral [n] 'a
  (incr: idx_t.t)
  (block_size: i16)
  (xs: [n]a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = my_radix_sort_int (i64.i16 block_size) 2 num_bits (\i ix -> get_bit i ix.1) ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}
local def radixSortColumn_signed_float [n] 'a 
  (incr: idx_t.t)
  (block_size: i16)
  (xs: [n]a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = my_radix_sort_float (i64.i16 block_size) 2 num_bits (\i ix -> get_bit i ix.1) ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}
local def mergeSortColumn [n] 'a
  (incr: idx_t.t)
  (xs: [n]a)
  (leq: a -> a -> bool)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = merge_sort_by_key (\ixs -> ixs.1) (leq) ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}

-- Order a payload column given the reordered indices.
local def orderByIndices [n] 't (block_size: idx_t.t) (dummy_elem: t) (is: [n](idx_t.t)) (ys: [n]t) : [n]t =
  partitioned_gather block_size dummy_elem ys is

-- | Sort a column of short type (GFUR).
entry radixSortColumn_short [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i16) : sortInfo_short [n] =
  radixSortColumn_signed_integral incr block_size xs (i16.num_bits) (i16.get_bit)
-- | Sort a column of integer type (GFUR).
entry radixSortColumn_int [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i32) : sortInfo_int [n] =
  radixSortColumn_signed_integral incr block_size xs (i32.num_bits) (i32.get_bit)
-- | Sort a column of long type (GFUR).
entry radixSortColumn_long [n] (incr: idx_t.t) (block_size: i16) (xs: [n]i64) : sortInfo_long [n] =
  radixSortColumn_signed_integral incr block_size xs (i64.num_bits) (i64.get_bit)
-- | Sort a column of float type (GFUR).
entry radixSortColumn_float [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f32) : sortInfo_float [n] =
  radixSortColumn_signed_float incr block_size xs (f32.num_bits) (f32.get_bit)
-- | Sort a column of double type (GFUR).
entry radixSortColumn_double [n] (incr: idx_t.t) (block_size: i16) (xs: [n]f64) : sortInfo_double [n] =
  radixSortColumn_signed_float incr block_size xs (f64.num_bits) (f64.get_bit)

-- | Sort a column of short type (GFUR).
entry mergeSortColumn_short [n] (incr: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of integer type (GFUR).
entry mergeSortColumn_int [n] (incr: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of long type (GFUR).
entry mergeSortColumn_long [n] (incr: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of float type (GFUR).
entry mergeSortColumn_float [n] (incr: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
  mergeSortColumn incr xs (<=)
-- | Sort a column of double type (GFUR).
entry mergeSortColumn_double [n] (incr: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
  mergeSortColumn incr xs (<=)

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) : [n]i16 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) : [n]i32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) : [n]i64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) : [n]f32 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double [n] (incr: idx_t.t) (block_size: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) : [n]f64 =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in orderByIndices block_size (0) offset_is ys


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

-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- Merge-Join Code
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################
-- ########################################################################################################################

-- | Type used to store the information of a join (between x&y) in a partition.
-- vs : the values of the x's
-- ix : the corresponding index of the value in x
-- iy : the first match in y
-- cm : number of matches found in y
-- NOTE - each tuple corresponds to an individual x column, containing info about all its matches (or no matches) in y.
-- Pure pairs are expressed with type joinPairs.
type joinTup [n] 't = {vs: [n]t, ix: [n]idx_t.t, iy: [n]idx_t.t, cm: [n]idx_t.t}

def uncooked_joinTup [nR] 't
  (tR: [nR]t)
  (offset_R: idx_t.t)
: joinTup [nR] t =
  {
    vs = copy tR,
    ix = idx_t.indicesWithIncrement (offset_R) (tR),
    iy = replicate nR (-1),
    cm = replicate nR 0
  }

def merge_joinTups [n1] [n2] 't
  (tup1 : joinTup [n1] t)
  (tup2 : joinTup [n2] t)
: joinTup [n1+n2] t =
  let newVs = tup1.vs ++ tup2.vs :> [n1+n2]t
  let newIx = tup1.ix ++ tup2.ix :> [n1+n2]idx_t.t
  let newIy = tup1.iy ++ tup2.iy :> [n1+n2]idx_t.t
  let newCm = tup1.cm ++ tup2.cm :> [n1+n2]idx_t.t
  in {vs = newVs, ix = newIx, iy = newIy, cm = newCm}

-- Find matches between tuples from a partition of R and a partition of S
-- extParallelism controls the external parallelism of a nested op (map x (reduce y))
-- Note: Futhark by Example claims the compiler uses loop fission for nested parallelism
-- so it might already be doing this...
-- TODO a hist-based implementation might be more efficient
def find_joinTuples [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (extParallelism: idx_t.t)
  (neq: t -> t -> bool)
  (gt: t -> t -> bool)
: joinTup [nR] t =
  let numIter = (nR+extParallelism-1) / extParallelism
  let pre_joinTup = uncooked_joinTup (tR) (offset_R)
  let joinLoop : {iter: idx_t.t, result: joinTup [nR] t}
    = loop p = {iter = 0, result = pre_joinTup}
    while p.iter < numIter do
      let start = p.iter*extParallelism
      let iter_size = idx_t.min (extParallelism) (nR - start)
      let iter_R = tR[start : start + iter_size] :> [iter_size]t
      in -- skip if R and S do not overlap
        let r_min = iter_R[0]
        let r_max = iter_R[iter_size-1]
        let s_min = if nS>0 then tS[0] else r_min
        let s_max = if nS>0 then tS[nS-1] else r_max
        in
          if (r_min `gt` s_max) then {iter= numIter, result= p.result} -- S < iter_R -> break
          else if (s_min `gt` r_max) then {iter= p.iter+1, result= p.result} -- S > iter_R -> continue
      else -- default case
      let ircs = zip3 (idx_t.indicesWithIncrement (offset_R+start) iter_R) (iter_R) (replicate iter_size 0)
      let iscs = zip3 (idx_t.indicesWithIncrement (offset_S) tS) tS (replicate nS 1)
      let ms = ircs -- TODO optimisation: if iscs below a certain size, use loop rather than reduce (?)
        |> map (\(i, x, c) ->
          let redTup = reduce_comm (\(i1, y1, c1) (i2, y2, c2) ->
              if ((y1 `neq` x) || i1 < 0) && ((y2 `neq` x) || i2 < 0) then (-1, x, 0)
              else if ((y1 `neq` x) || i1 < 0) then (i2, y2, c2)
              else if ((y2 `neq` x) || i2 < 0) then (i1, y1, c1)
              else (idx_t.min i1 i2, x, c1+c2)
            )
            (-1, x, 0)
            iscs
          in (redTup.0, redTup.2) -- keep only necessary info - iy & cm
        )
        |> unzip
      -- TODO figure out consumption tactics...
      let nextVs = p.result.vs
      let nextIx = p.result.ix
      let nextIy = (copy p.result.iy) with [start:start+iter_size] = ms.0
      let nextCm = (copy p.result.cm) with [start:start+iter_size] = ms.1
      in {
        iter = p.iter+1,
        result = {vs = nextVs, ix = nextIx, iy = nextIy, cm = nextCm}
      }
  in joinLoop.result

-- Pass as arguments the partitions derived from windowed_partitionFunc
-- finds for each partition of tR the first and last matching partitions that can match it in tS
-- returns the indices of those partitions, as they are in partitionPairs
-- (returns np if there's a potential of continuing into the next chunk of S)
def partitionMatchBounds [n] [np] 't
  (tR: [n]t)
  (tS: [n]t)
  (partitionPairs: [np](idx_t.t, idx_t.t))
  (extParallelism: idx_t.t)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
: [np](idx_t.t, idx_t.t) =
  partitionPairs
    |> zip (indices partitionPairs)
    |> map (\(j, (ri, si)) ->
      let r_min = tR[idx_t.min ri (n-1)]
      let ri_next = if j==np-1 then n else partitionPairs[j+1].0
      let r_max = tR[idx_t.max 0 (ri_next-1)]
      -- TODO turn the following sequential searches into binary ones? or selectable
      let firstMatch : {flag: bool, smatch: idx_t.t}
        = loop fm = {flag=true, smatch=j}
        while (fm.flag) do
          let sj = idx_t.min (n-1) partitionPairs[fm.smatch].1
          let s_min = tS[sj]
          let si_next = if fm.smatch>=np-1 then n else partitionPairs[fm.smatch+1].1
          let s_max = tS[idx_t.max sj (si_next-1)]
          let s_nextMin = tS[idx_t.min (np-1) si_next]
          let s_prevMax = tS[idx_t.max 0 (sj-1)]
          in
            if (r_min `gt` s_min)
            then
              if (r_min `leq` s_max)
              then {flag=false, smatch=fm.smatch}
              else {flag= !((r_min `leq` s_nextMin)||(fm.smatch+1>=np)), smatch=fm.smatch+1} -- break if at end OR if r_min <= s_nextMin
            else {flag= !((s_prevMax `leq` r_min)||(fm.smatch-1<=0)), smatch=idx_t.max 0 (fm.smatch-1)}
      let lastMatch : {flag:bool, smatch: idx_t.t}
        = loop lm = {flag=true, smatch=j}
        while (lm.flag && lm.smatch<np) do
          let sj = idx_t.min (n-1) partitionPairs[lm.smatch].1
          let s_min = tS[sj]
          let si_next = if lm.smatch>=np-1 then n else partitionPairs[lm.smatch+1].1
          let s_max = tS[idx_t.max sj (si_next-1)]
          let s_nextMin = tS[idx_t.min (np-1) si_next]
          let s_prevMax = tS[idx_t.max 0 (sj-1)]
          in
            if (s_max `gt` r_max)
            then
              if (s_min `leq` r_max)
              then {flag=false, smatch=lm.smatch}
              else {flag= !((s_prevMax `leq` r_max)||(lm.smatch-1<=0)), smatch= idx_t.max 0 (lm.smatch-1)} -- break if at 0 OR if s_prevMax <= r_max
            else {flag= !((r_max `leq` s_nextMin)), smatch=lm.smatch+1}
      in (firstMatch.smatch, lastMatch.smatch)
    )
    -- spent too much time taking care of corner cases for dense arrays...
    -- TODO tidy up & implement the extParallelism option
    -- TODO modify so can be -1 if R_min < S_min (or R_max < S_min)

def join_chunks [n] 't
  (tR: [n]t)
  (tS: [n]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (neq: t -> t -> bool)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
: joinTup [n] t =
  let parts = windowed_partitionFunc (partitionsPerWindow) (numberOfWindows) (tR) (tS) (leq) (gt)
  let np = length parts
  let pmbs = partitionMatchBounds (tR) (tS) (parts) (extParallelism) (leq) (gt)
  let loop_over : {iter: idx_t.t, tups: joinTup [n] t}
  = loop p = {iter=0, tups = uncooked_joinTup (tR) (offset_R)}
  while p.iter<np do
    let firstMatch = pmbs[p.iter].0
    let lastMatch = idx_t.min (np - 1) (pmbs[p.iter].1)
    in if (firstMatch >= np) then {iter = np, tups = p.tups} else -- break if reached the end
    if (lastMatch < 0) then {iter = p.iter+1, tups = p.tups} else -- continue if partition smaller than S_min - code unused yet
    let r_start = parts[p.iter].0
    let r_end = if p.iter + 1 == np then n else parts[p.iter+1].0
    let curR = tR[r_start:r_end] :> [r_end-r_start]t
    let inner_loop : {curPart: idx_t.t, curTups: joinTup [r_end-r_start] t}
    = loop dp = {curPart = firstMatch, curTups = uncooked_joinTup (curR) (offset_R + r_start)}
    while dp.curPart <= lastMatch do
      let s_start = parts[dp.curPart].1
      let s_end = if dp.curPart + 1 == np then n else parts[dp.curPart+1].1
      let curS = tS[s_start:s_end]
      let curJoin = find_joinTuples (curR) (curS) (offset_R+r_start) (offset_S+s_start) (extParallelism) (neq) (gt)
      let newVs = dp.curTups.vs
      let newIx = dp.curTups.ix
      let newIy = map2 (\alt neu -> if (alt<0) then neu else alt) (dp.curTups.iy) (curJoin.iy)
      let newCm = map2 (+) (dp.curTups.cm) (curJoin.cm)
      let nextTup = {vs=newVs, ix=newIx, iy=newIy, cm=newCm}
      in {curPart=dp.curPart+1, curTups=nextTup}
    let newIx = p.tups.ix
    let newVs = p.tups.vs
    let newIy_block = map2 (\alt neu -> if (alt<0) then neu else alt) (p.tups.iy[r_start:r_end]) (inner_loop.curTups.iy)
    let newCm_block = map2 (+) (p.tups.cm[r_start:r_end]) (inner_loop.curTups.cm)
    let newIy = (copy p.tups.iy) with [r_start:r_end] = newIy_block
    let newCm = (copy p.tups.cm) with [r_start:r_end] = newCm_block
    let newTups = {vs=newVs, ix=newIx, iy=newIy, cm=newCm}
    in {iter=p.iter+1, tups=newTups}
  in loop_over.tups

-- FOLLOWING FUNCTION
-- assume a single chunk of R, multiple chunks of S
-- nR serves as the max_window_size
def mergeJoin [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (neq: t -> t -> bool)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
: joinTup [nR] t =
  let numIter = (nR+nS-1)/nR
  let loop_over : {iter:idx_t.t, tups: joinTup [nR] t}
  = loop p = {iter=0, tups = uncooked_joinTup (tR) (offset_R)}
  while p.iter < numIter do
    let tS_start = p.iter*nR
    let tS_end = idx_t.min (nS) (tS_start + nR)
    let curS = tS[tS_start:tS_end]
    in if (curS[0] `gt` tR[nR-1]) then {iter=numIter, tups=p.tups} else -- if exceeded Rmax, break
    let curTup : joinTup [nR] t =
      if (tS_end - tS_start) < nR then -- last chunk of S - join without partitioning...
        -- TODO break tR into chunks equisized with last chunk of S
        -- adjust number of windows & partitions to new size
        -- might end up too complicated (?)
        find_joinTuples tR curS (offset_R) (offset_S+tS_start) (extParallelism) (neq) (gt)
      else -- default case
        let bS = curS :> [nR]t
        in join_chunks
          (tR) (bS)
          (offset_R) (offset_S+tS_start)
          (partitionsPerWindow) (numberOfWindows) (extParallelism)
          (neq) (leq) (gt)
    let newVs = p.tups.vs
    let newIx = p.tups.ix
    let newIy = map2 (\alt neu -> if (alt<0) then neu else alt) (p.tups.iy) (curTup.iy) -- TODO change to avoid rand acc (?)
    let newCm = map2 (+) (p.tups.cm) (curTup.cm)
    let nextTup = {vs=newVs, ix=newIx, iy=newIy, cm=newCm}
    let nextIter = if curS[(length curS)-1] `leq` tR[nR-1] then p.iter+1 else numIter -- break if exceeded Rmax
    in {iter=nextIter, tups = nextTup}
  in loop_over.tups

-- | The pairs obtained from joining x&y.
-- vs : the values of each pair
-- ix : the respective index in x
-- iy : the respective index in y
-- NOTE - unlike type joinTup, each tuple here corresponds to an individual match.
type~ joinPairs 't = {vs: []t, ix: []idx_t.t, iy: []idx_t.t}

def joinTups_to_joinPairs_InnerJoin [n] 't
  (psize: idx_t.t)
  (tups: joinTup [n] t)
  (dummy_elem: t)
 =
  let zipTups = zip4 tups.vs tups.ix tups.iy tups.cm
  let filteredTups = zipTups |> filter (\(v, ix, iy, cm) -> (cm>0))
  let n_filt = length filteredTups
  -- separate match counts & pair info
  let fcm = filteredTups |> map (\(v,ix,iy,cm)->cm)
  let fTups_minusCm = filteredTups |> map (\(v, ix, iy, cm) -> (v, ix, iy))
  -- obtain the starting indices of each match in the output array
  let tup_index = exscan (\cm1 cm2 -> cm1+cm2) 0 fcm
  -- obtain the total number of pairs
  let n_pairs =
    if ((length filteredTups)>0)
    then tup_index[(length tup_index)-1] + filteredTups[(length filteredTups)-1].3
    else 0
  -- initialise the array
  let pairsArray = partitioned_scatter
    (psize)
    (replicate n_pairs (dummy_elem, -1, -1))
    (tup_index)
    (fTups_minusCm)
  -- find pairs with multiplicity (in ys) to minimise the following loop
  let pairsWithMultiplicity = fcm |> zip tup_index |> filter (\(i, cm) -> cm>1)
  let n_mult = length pairsWithMultiplicity
  -- loop over output array for matches with multiplicity
  let loop_over : {iter: idx_t.t, buff: [](t, idx_t.t, idx_t.t)}
  = loop p = {iter=0, buff=pairsArray}
  while (p.iter<n_mult) && (n_pairs>n_filt) do -- second && skips the thing if no multiplicity in pairs
    let j = pairsWithMultiplicity[p.iter].0
    let nj = pairsWithMultiplicity[p.iter].1
    let newIy_block = (zip (iota nj) (replicate nj p.buff[j]))
      |> map (\(i, (v, ix, iy)) -> (v, ix, iy+i))
    in {
      iter = p.iter+1,
      buff = (copy p.buff) with [j:j+nj] = newIy_block
    }
  let unzPairs = loop_over.buff |> unzip3
  let pairs : joinPairs t = {vs=unzPairs.0, ix=unzPairs.1, iy=unzPairs.2}
  in pairs

def inner_SMJ [nR] [nS] 't
  (dummy_elem: t)
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
  (neq: t -> t -> bool)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
=
  let jTups = mergeJoin (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (neq) (leq) (gt)
  in joinTups_to_joinPairs_InnerJoin (scatter_psize) (jTups) (dummy_elem)

-- | Join pairs of type short.
type~ joinPairs_short = joinPairs i16
-- | Join pairs of type int.
type~ joinPairs_int = joinPairs i32
-- | Join pairs of type long.
type~ joinPairs_long = joinPairs i64
-- | Join pairs of type float.
type~ joinPairs_float = joinPairs f32
-- | Join pairs of type double.
type~ joinPairs_double = joinPairs f64

entry inner_SMJ_short [nR] [nS]
  (tR: [nR]i16)
  (tS: [nS]i16)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_short =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (scatter_psize) (!=) (<=) (>)

entry inner_SMJ_int [nR] [nS]
  (tR: [nR]i32)
  (tS: [nS]i32)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_int =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (scatter_psize) (!=) (<=) (>)

entry inner_SMJ_long [nR] [nS]
  (tR: [nR]i64)
  (tS: [nS]i64)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_long =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (scatter_psize) (!=) (<=) (>)

entry inner_SMJ_float [nR] [nS]
  (tR: [nR]f32)
  (tS: [nS]f32)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_float =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (scatter_psize) (!=) (<=) (>)

entry inner_SMJ_double [nR] [nS]
  (tR: [nR]f64)
  (tS: [nS]f64)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (extParallelism: idx_t.t)
  (scatter_psize: idx_t.t)
: joinPairs_double =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows: idx_t.t) (extParallelism) (scatter_psize) (!=) (<=) (>)

-- Payload gathering (GFTR)

-- | Function to gather the payload columns of a relation after the join.
def gather_payloads [ni] [n] 't
  (incr: idx_t.t)
  (psize: idx_t.t)
  (dummy_elem: t)
  (is: [ni]idx_t.t)
  (ys: [n]t)
=
  let offset_is = is |> map (\j -> j - incr)
  in partitioned_gather (psize) (dummy_elem) (ys) (offset_is)

entry gather_payloads_short (incr) (psize) (is) (ys: []i16)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_int (incr) (psize) (is) (ys: []i32)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_long (incr) (psize) (is) (ys: []i64)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_float (incr) (psize) (is) (ys: []f32)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_double (incr) (psize) (is) (ys: []f64)
  = gather_payloads incr psize (0) is ys

entry gather_payloads_GFTR (incr) (psize) (is) (pL_bytes: idx_t.t) (ys: [][pL_bytes]u8)
  = gather_payloads (psize) (dummy_elem) (ys) (offset_is)

  -- Payload gathering (GFUR)

  -- | Function to gather the payload columns of a relation after the join, over an array with previously gathered values.
def gather_payloads_GFUR [ni] [n] 't
  (incr: idx_t.t)
  (psize: idx_t.t)
  (dummy_array: [ni]t)
  (is: [ni]idx_t.t)
  (ys: [n]t)
=
  let offset_is = is |> map (\j -> j - incr)
  in partitioned_gather_over_array (psize) (dummy_array) (ys) (offset_is)

entry gather_payloads_short_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i16) (is: [ni]idx_t.t) (ys: [n]i16)
  = gather_payloads_GFUR incr psize preVals is ys

entry gather_payloads_int_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i32) (is: [ni]idx_t.t) (ys: [n]i32)
  = gather_payloads_GFUR incr psize preVals is ys

entry gather_payloads_long_GFUR [ni] [n] (incr) (psize) (preVals: [ni]i64) (is: [ni]idx_t.t) (ys: [n]i64)
  = gather_payloads_GFUR incr psize preVals is ys

entry gather_payloads_float_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f32) (is: [ni]idx_t.t) (ys: [n]f32)
  = gather_payloads_GFUR incr psize preVals is ys

entry gather_payloads_double_GFUR [ni] [n] (incr) (psize) (preVals: [ni]f64) (is: [ni]idx_t.t) (ys: [n]f64)
  = gather_payloads_GFUR incr psize preVals is ys

-- | Get the minimum index to start scanning from there.
entry min_idx = idx_t.minimum
-- | Get the maximum index to stop scanning there.
entry max_idx = idx_t.maximum -- TODO consider warning (...)