import "../lib/github.com/diku-dk/segmented/segmented"
import "../ftbasics"
import "strUtil"

def inner_to_left_outer_str
	(tR : strInfo)
	(res : joinPairs_str)
: joinPairs_str =
	let n = length tR.idxs
	let present_ix = scatter (replicate n false)
		res.ix (res.ix |> map (\_ -> true))
	let (out_ix,out_iy) = iota n
		|> filter (\i -> !present_ix[i])
		|> map (\i -> (i,-1))
		|> unzip
	let out_vs = str_gather tR out_ix
	in {
		strs = appendStrs false out_vs.idxs out_vs.contents (copy res.strs),
		ix = res.ix ++ out_ix,
		iy = res.iy ++ out_iy
	}

def inner_to_right_outer_str
	(tS : strInfo)
	(res : joinPairs_str)
: joinPairs_str =
	let right_outer1 = inner_to_left_outer_str tS
		{strs=res.strs, ix=res.iy, iy=res.ix}
	in {strs=right_outer1.strs, ix=right_outer1.iy, iy=right_outer1.ix}

def inner_to_full_outer_str
	(tR : strInfo)
	(tS : strInfo)
	(res : joinPairs_str)
: joinPairs_str = res
	|> inner_to_left_outer_str tR
	|> inner_to_right_outer_str tS