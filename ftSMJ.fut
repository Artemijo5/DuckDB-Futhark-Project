import "ftbasics"
import "ftsort"


-- TODO
-- make this utilise the bound options of partitionFunc rather than cropping the arrays
-- but also, crop the arrays as needed for size equalisation...
def partition_pipeline [na] [nb] 't
  (window_size: idx_t.t)
  (threads: idx_t.t)
  (as: [na]t)
  (bs: [nb]t)
  (leq: t -> t -> bool)
  (gt: t -> t -> bool)
: [](idx_t.t, idx_t.t) =
  let nWin = (na + nb + window_size - 1) / window_size
  let nPart = nWin * threads
  let loop_over: {from: idx_t.t, parts: [nPart](idx_t.t, idx_t.t)}
    = loop p = {from = 0, parts = (replicate nPart (0, 0))}
    while p.from < nPart
    do
      if (na <= p.parts[p.from].0 + (window_size/threads) || nb <= p.parts[p.from].1 + (window_size/threads)) then {from = nPart, parts = p.parts} else
      let lbs = p.parts[p.from]
      let actual_ws = idx_t.minimum [window_size, (na - lbs.0), (nb - lbs.1)]
      let actual_ts = idx_t.min (1) (threads * actual_ws / window_size)
      let ubs = (lbs.0 + actual_ws, lbs.1 + actual_ws)
      let at = as[lbs.0:ubs.0] :> [actual_ws]t
      let bt = bs[lbs.1:ubs.1] :> [actual_ws]t
      let theseParts = partitionFunc (2*actual_ts) 0 (actual_ts+1) at bt (leq) (gt)
       |> map (\(ia, ib) -> (ia + lbs.0, ib + lbs.1))
      let newParts = (copy p.parts) with [p.from:(p.from + actual_ts + 1)] = theseParts
      in {from = p.from + actual_ts, parts = newParts}
  in loop_over.parts