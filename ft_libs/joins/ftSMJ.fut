import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"

-- Merge-Join routines for Sort-Merge Join.
-- See:
-- 'Efficiently Processing Joins and Grouped Aggregations on GPUs'
-- Bowen Wu, Dimitrios Koutsoukos, Gustavo Alonso
-- ACM Manag. Data, Vol. 3, No. 1 (SIGMOD), Article 39
-- & References

-- | SMJ match-finding phase.
-- Essentially bulk range binary searches between 2 sorted key columns.
def smj_matchFinding [nR] [nS] 't
	(eq : t -> t -> bool)
	(geq: t -> t -> bool)
	(gt : t -> t -> bool)
	(lt : t -> t -> bool)
	(tR : [nR]t)
	(tS : [nS]t)
: joinTup [nR] t =
	let (iS, count_m) = tR
		|> bsearch_range eq geq gt lt (replicate nR 0) tS
		|> unzip
	in {vs = tR, ix = iota nR, iy = iS, cm = count_m}

-- | SMJ expansion phase (for Inner Join).
def smj_expand [nR] 't (matches : joinTup [nR] t) : joinPairs t =
	let (exp_vs, exp_ix, exp_iy) = zip4 matches.vs matches.ix matches.iy matches.cm
		|> expand (.3) (\(v,ix,iy,_) ind -> (v,ix,iy+ind))
		|> unzip3
	in {vs = exp_vs, ix = exp_ix, iy = exp_iy}

-- | SMJ expansion phase (for Left Outer Join).
def smj_leftOuter_expand [nR] 't (matches : joinTup [nR] t) : joinPairs t =
	let matches' = {
		vs = matches.vs,
		ix = matches.ix,
		iy = matches.iy,
		cm = matches.cm |> map (\c -> i64.max c 1)
	}
	in smj_expand matches'

-- | SMJ full Merge-Join routine (for Inner Join).
def do_InnerSMJ [nR] [nS] 't
	(eq : t -> t -> bool)
	(geq: t -> t -> bool)
	(gt : t -> t -> bool)
	(lt : t -> t -> bool)
	(tR : [nR]t)
	(tS : [nS]t)
: joinPairs t =
	smj_matchFinding eq geq gt lt tR tS
	|> smj_expand