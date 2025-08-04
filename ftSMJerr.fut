import "ftbasics"

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

type~ joinPairs 't = {vs: []t, ix: []i64, iy: []i64}

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

def inner_SMJ 't [nR] [nS]
  (dummy_elem: t)
  (tR: [nR]t)
  (tS: [nS]t)
  (offset_R: i64)
  (offset_S: i64)
  (partitionsPerWindow: i64)
  (numberOfWindows: i64)
  (extParallelism: i64)
  (scatter_psize: i64)
  (neq: t -> t -> bool)
  (leq: t -> t -> bool)
  (gt : t -> t -> bool)
: joinPairs t =
	let jT = uncooked_joinTup tR 0
	in joinTups_to_joinPairs_InnerJoin scatter_psize jT dummy_elem

type~ joinPairs_int = joinPairs i32

entry inner_SMJ_int [nR] [nS]
  (tR: [nR]i32)
  (tS: [nS]i32)
  (offset_R: i64)
  (offset_S: i64)
  (partitionsPerWindow: i64)
  (numberOfWindows: i64)
  (extParallelism: i64)
  (scatter_psize: i64)
: joinPairs_int =
  inner_SMJ
    (0) (tR) (tS) (offset_R) (offset_S) (partitionsPerWindow) (numberOfWindows) (extParallelism) (scatter_psize) (!=) (<=) (>)