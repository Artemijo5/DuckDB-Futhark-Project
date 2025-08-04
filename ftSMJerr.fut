type joinPairs_int = {vs: [2]i32, ix: [2]i64, iy: [2]i64}

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
	let uncooked_joinPair : joinPairs_int =
    {
      vs = [4, 5],
      ix = [0, 1],
      iy = [0, 1]
    }
  	in uncooked_joinPair