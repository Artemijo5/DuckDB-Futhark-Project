import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"

-- Functions to expand joinPairs to Outer Join results.

-- | Given the output of an Inner Join,
-- convert it to a Left-Outer join.
-- Outer result rows are appended at the end.
def inner_to_left_outer [n] 't
	(tR : [n]t)
	(res : joinPairs t)
: joinPairs t =
	let absent_ix = scatter (replicate n true)
		res.ix (res.ix |> map (\_ -> false))
	let (out_vs,out_ix,out_iy) = iota n
		|> filter (\i -> absent_ix[i])
		|> map (\i -> (tR[i],i,-1))
		|> unzip3
	in {vs = res.vs ++ out_vs, ix = res.ix ++ out_ix, iy = res.iy ++ out_iy}

-- | Given the output of an Inner Join,
-- convert it to a Right-Outer join.
-- Outer result rows are appended at the end.
def inner_to_right_outer [n] 't
	(tS : [n]t)
	(res : joinPairs t)
: joinPairs t =
	let right_outer1 = inner_to_left_outer tS
		{vs=res.vs, ix=res.iy, iy=res.ix}
	in {vs=right_outer1.vs, ix=right_outer1.iy, iy=right_outer1.ix}

-- | Given the output of an Inner Join,
-- convert it to a Full-Outer join.
-- Outer result rows are appended at the end.
def inner_to_full_outer [nR] [nS] 't
	(tR : [nR]t)
	(tS : [nS]t)
	(res : joinPairs t)
: joinPairs t = res
	|> inner_to_left_outer tR
	|> inner_to_right_outer tS
