import "lib/github.com/diku-dk/sorts/merge_sort"
import "ftbasics"

-- Sorting Functions

-- 1. Radix Sort
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

-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
type sortStruct [n] [b] 'a = {k: [n]a, pL: [n][b]u8}

type sortStruct_short [n] [b] = sortStruct [n] [b] i16
type sortStruct_int [n] [b] = sortStruct [n] [b] i32
type sortStruct_long [n] [b] = sortStruct [n] [b] i64
type sortStruct_float [n] [b] = sortStruct [n] [b] f32
type sortStruct_double [n] [b] = sortStruct [n] [b] f64

def radixSortRelation_signed_integral [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = my_radix_sort_int (i64.i16 block_size) 2 num_bits (\i xy -> get_bit i xy.0) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
def radixSortRelation_signed_float [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = my_radix_sort_float (i64.i16 block_size) 2 num_bits (\i xy -> get_bit i xy.0) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}
def mergeSortRelation [n] [b] 'a
  (xs: sortStruct [n] [b] a)
  (leq: a -> a -> bool)
 : sortStruct [n] [b] a =
  let xys : [n](a, [b]u8) = zip xs.k xs.pL
  let sorted_xys = merge_sort_by_key (\xy -> xy.0) (leq) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1}

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

def radixSortColumn_signed_integral [n] 'a
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
def radixSortColumn_signed_float [n] 'a 
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
def mergeSortColumn [n] 'a
  (incr: idx_t.t)
  (xs: [n]a)
  (leq: a -> a -> bool)
 : sortInfo [n] a =
  let ixs = xs |> zip (idx_t.indicesWithIncrement incr xs)
  let sorted_ixs = merge_sort_by_key (\ixs -> ixs.1) (leq) ixs
  let un_ixs = unzip sorted_ixs
  in {is = un_ixs.0, xs = un_ixs.1}

-- Order a payload column given the reordered indices.
def orderByIndices [n] 't (block_size: idx_t.t) (dummy_elem: t) (is: [n](idx_t.t)) (ys: [n]t) : [n]t =
  partitioned_gather block_size dummy_elem ys is

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

def overlay_joinTups [n1] [n2] 't
  (tup1 : joinTup [n1] t)
  (tup2 : joinTup [n2] t)
  (offset : idx_t.t)
: joinTup [n1] t =
  let newVs = tup1.vs
  let newIx = tup1.ix
  let newIy = (copy tup1.iy) with [offset:offset+n2] = tup2.iy
  let newCm = (copy tup1.cm) with [offset:offset+n2] = tup2.cm
  in {vs = newVs, ix = newIx, iy = newIy, cm = newCm}

-- Find matches between tuples from a partition of R and a partition of S
-- NOTE : rv is at the left side of all comparisons
def find_joinTuples [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (eq: t -> t -> bool)
  (gt: t -> t -> bool)
  (lt: t -> t -> bool)
: joinTup [nR] t =
  let init_step = idx_t.max 1 (nS/2)
  let tR_matches = tR
    |> map (\rv ->
      -- 2 binary searches for first and last matches in tS
      let bsearch_first =
        loop (first_match, step) = (0, init_step)
        while step>0 do
          let sv = tS[first_match]
          let pv = if first_match==0 then sv else tS[first_match-1]
          let nv = if first_match==nS-1 then sv else tS[first_match+1]
          in
            if ((rv `eq` sv) && (first_match==0 || (rv `gt` pv))) -- found first match
            then (first_match, 0)
            else if (rv `eq` sv) -- found value range but not its beginning
            then (first_match - step, idx_t.max 1 (step/2))
            else if (rv `lt` sv) then
              if (first_match==0 || (rv `gt` pv))
              then (-1, 0)
              else (first_match-step, idx_t.max 1 (step/2))
            else -- if (rv `gt` sv) then
              if (first_match==nS-1 || (rv `lt` nv))
              then (-1, 0)
              else (first_match+step, idx_t.max 1 (step/2))
      let frv = bsearch_first.0
      let init_step_last = idx_t.max 1 ((nS-frv)/2)
      let bsearch_last = if frv == -1 then (-1, 0) else
        loop (last_match, step) = (nS-1, init_step_last)
        while step>0 do
          let sv = tS[last_match]
          let nv = if last_match==nS-1 then sv else tS[last_match+1]
          in
            if ((rv `eq` sv) && (last_match==nS-1 || (rv `lt` nv))) -- found last match
              then (last_match, 0)
            else if (rv `eq` sv) -- found value range but not its end
              then (last_match+step, idx_t.max 1 (step/2))
            else if (rv `lt` sv)
              then (last_match-step, idx_t.max 1 (step/2))
            else -- if (rv `gt` sv) then
              (last_match+step, idx_t.max 1 (step/2))
      let lrv = bsearch_last.0
      let cm = if (frv<0) then 0 else (lrv-frv+1)
      in (frv, cm)
    )
  in {
    vs = tR,
    ix = idx_t.indicesWithIncrement (offset_R) (tR),
    iy = tR_matches |> map (.0) |> map (\j -> if (j<0) then j else j + offset_S),
    cm = tR_matches |> map (.1)
  }

-- NOTE : R is at the left side of all comparisons
def mergeJoin [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (eq: t -> t -> bool)
  (gt : t -> t -> bool)
  (lt : t -> t -> bool)
: joinTup [nR] t =
  let numIter_R = (nR + partitionSize - 1)/partitionSize
  let numIter_S = (nS + partitionSize - 1)/partitionSize
  let loop_over_R : (idx_t.t, joinTup [nR] t, idx_t.t)
  = loop (iter, jtup, first_relevant_in_S) = (0, uncooked_joinTup tR offset_R, 0)
  while iter < numIter_R do
    let tR_start = iter * partitionSize
    let tR_end = idx_t.min (nR) (tR_start + partitionSize)
    let cur_R = tR[tR_start:tR_end]
    let cur_R_size = tR_end-tR_start
    let loop_over_S : (idx_t.t, joinTup [tR_end-tR_start] t , bool, idx_t.t)
    = loop (s_iter, s_jtup, isRelevant, minRelevant)
      = (
          first_relevant_in_S,
          uncooked_joinTup cur_R (offset_R+tR_start),
          true,
          first_relevant_in_S
        )
    while (s_iter < numIter_S && isRelevant) do
      let tS_start = s_iter * partitionSize
      let tS_end = idx_t.min (nS) (tS_start + partitionSize)
      let cur_S = tS[tS_start:tS_end]
      let cur_S_size = tS_end-tS_start
      let thisRelYet = !(cur_R[0] `gt` cur_S[cur_S_size-1])
      let thisStillRel = !(cur_R[cur_R_size-1] `lt` cur_S[0])
      in
        if !thisRelYet then (s_iter+1, s_jtup, true, s_iter+1) else -- stil not relevant - skip to next
        if !thisStillRel then (s_iter, s_jtup, thisStillRel, minRelevant) else -- already not relevant - break
        let nextRel = !(cur_R[cur_R_size-1] `lt` cur_S[cur_S_size-1]) -- will next S be relevant? if not, break afterwards
        let new_s_jtup = find_joinTuples cur_R cur_S (offset_R+tR_start) (offset_S+tS_start) (eq) (gt) (lt)
        let new_s_iy = map2 (\alt neu -> if (alt<0) then neu else alt) (s_jtup.iy) (new_s_jtup.iy)
        let new_s_cm = map2 (+) (s_jtup.cm) (new_s_jtup.cm)
        let next_s_jtup = {vs = s_jtup.vs, ix = s_jtup.ix, iy = new_s_iy, cm = new_s_cm}
        in (s_iter+1, next_s_jtup, nextRel, minRelevant)
    let new_jtup = overlay_joinTups jtup loop_over_S.1 tR_start
    in (iter+1, new_jtup, loop_over_S.3)
  in loop_over_R.1

-- | The pairs obtained from joining x&y.
-- vs : the values of each pair
-- ix : the respective index in x
-- iy : the respective index in y
-- NOTE - unlike type joinTup, each tuple here corresponds to an individual match.
type~ joinPairs 't = {vs: []t, ix: []idx_t.t, iy: []idx_t.t}

def joinTups_to_joinPairs_InnerJoin [n] 't
--  (psize: idx_t.t)
  (tups: joinTup [n] t)
  (dummy_elem: t)
 =
  let filtTups_idx = (iota n) |> map (\i -> if tups.cm[i]>0 then i else (-1))
  -- separate match counts & pair info
  let fcm = gather 0 tups.cm filtTups_idx
  let fTups_minusCm = gather (dummy_elem, -1, -1) (zip3 tups.vs tups.ix tups.iy) filtTups_idx
  -- obtain the starting indices of each match in the output array
  let tup_index = map2
    (\c z -> if c>0 then z else (-1))
    fcm
    (exscan (\cm1 cm2 -> cm1+cm2) 0 fcm)
  -- obtain the total number of pairs
  let n_pairs = idx_t.sum fcm
  -- initialise the array
  let pairsArray = scatter -- partitioned_scatter
    --(psize)
    (replicate n_pairs (dummy_elem, -1, -1))
    (tup_index)
    (fTups_minusCm)
  let pairsWithMultiplicity = fcm
    |> zip tup_index
    |> filter (\(_,cm) -> cm>1)
  -- TODO test
  let max_mult =
    if (length pairsWithMultiplicity > 0)
    then pairsWithMultiplicity
      |> map (\(_,cm) -> cm)
      |> idx_t.maximum
    else 1
  let loop_over : [](t, idx_t.t, idx_t.t)
  = loop buff = pairsArray
  for iter in (1..<max_mult) do
    let this_scatter_idxs = (indices tup_index)
      |> map (\i ->
        if fcm[i]>iter
        then tup_index[i]+iter
        else (-1)
      )
    in scatter (copy buff) this_scatter_idxs (fTups_minusCm |> map (\(v,ri,si) -> (v,ri,si+iter)))
  let unzPairs = loop_over |> unzip3
  let pairs : joinPairs t = {vs=unzPairs.0, ix=unzPairs.1, iy=unzPairs.2}
  in pairs

-- NOTE : R is at the left side of all comparisons
def inner_SMJ [nR] [nS] 't
  (dummy_elem: t)
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (partitionSize: idx_t.t)
  (scatter_psize: idx_t.t)
  (eq: t -> t -> bool)
  (gt : t -> t -> bool)
  (lt : t -> t -> bool)
=
  let jTups = mergeJoin (tR) (tS) (offset_R) (offset_S) (partitionSize) (eq) (gt) (lt)
  in joinTups_to_joinPairs_InnerJoin (jTups) (dummy_elem)

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