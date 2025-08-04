type~ joinPairs 't = {vs: []t, ix: []i64, iy: []i64}

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
	let uncooked_joinPair : joinPairs t =
    {
      vs = copy tR,
      ix = indices tR,
      iy = indices tR
    }
  	in uncooked_joinPair

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