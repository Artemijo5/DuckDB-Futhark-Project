import "ftbasics"
import "ftsort"

-- | Type used to store the results of a join (between x&y) in a partition.
-- vs : the values of the x's
-- ix : the corresponding index of the value in x
-- iy : the first match in y
-- cm : number of matches found in y
-- Note : due to partitioning, it's possible for a single x entry to appear multiple, non-overlapping tuples.
-- This is only the case if y does not represent a primary key column.
type joinTup [n] 't = {vs: [n]t, ix: [n]idx_t.t, iy: [n]idx_t.t, cm: [n]idx_t.t}

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
def find_joinTuples [nR] [nS] 't
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: idx_t.t)
  (offset_S: idx_t.t)
  (extParallelism: idx_t.t)
  (neq: t -> t -> bool)
: joinTup [nR] t =
  let numIter = (nR+extParallelism-1) / extParallelism
  let pre_joinTup = {
    vs = (copy tR),
    ix = idx_t.indicesWithIncrement (offset_R) (tR),
    iy = replicate nR offset_S,
    cm = replicate nR 0
  }
  let joinLoop : {iter: idx_t.t, result: joinTup [nR] t}
    = loop p = {iter = 0, result = pre_joinTup}
    while p.iter < numIter do
      let start = p.iter*extParallelism
      let iter_size = idx_t.min (extParallelism) (nR - start)
      let iter_R = tR[start : start + iter_size] :> [iter_size]t
      let ircs = zip3 (idx_t.indicesWithIncrement (offset_R+start) iter_R) (iter_R) (replicate iter_size 0)
      let iscs = zip3 (idx_t.indicesWithIncrement (offset_S) tS) tS (replicate nS 1)
      let ms = ircs
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


-- Returns the same as partitionFunc but allows control of max parallelism
def matchPartitions_perChunkOfR [n] 't
  (partitionsPerWindow: idx_t.t)
  (numberOfWindows: idx_t.t)
  (tR: [n]t)
  (tS: [n]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
 : [partitionsPerWindow*numberOfWindows](idx_t.t, idx_t.t) =
  let totalNo = partitionsPerWindow*numberOfWindows
  let loop_over : {iter: idx_t.t, result: [totalNo](idx_t.t, idx_t.t)}
    = loop p = {iter=0, result = (replicate totalNo (n, n))}
    while p.iter<numberOfWindows do
      let start = p.iter*partitionsPerWindow
      let end = start+partitionsPerWindow
      let nextParts = partitionFunc (totalNo) (start) (end) tR tS (leq) (gt) :> [end-start](idx_t.t, idx_t.t)
      in {iter = p.iter+1, result = p.result with [start:end] = nextParts}
  in loop_over.result :> [partitionsPerWindow*numberOfWindows](idx_t.t, idx_t.t)

-- Pass as arguments the partitions derived from matchPartitions_perChunkOfR
-- finds for each partition of tR the first and last matching partitions that can match it in tS
-- returns the indices of those partitions, as they are in partitionPairs
-- (returns np if there's a potential of continuing into the next chunk of S)
def partitionMatchBounds [n] [np] 't
  (tR: [n]t)
  (tS: [n]t)
  (partitionPairs: [np](idx_t.t, idx_t.t))
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
  (extParallelism: idx_t.t)
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
    -- TODO tidy up