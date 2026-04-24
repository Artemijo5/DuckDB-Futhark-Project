import "../../../lib/github.com/athas/vector/vector"
import "../../../dbscan/ft_spindex"
import "../../../dbscan/ft_distance"
import "../../../dbscan/dbscan"

module vector_2 = cat_vector vector_1 vector_1
module kd2 = grid_index vector_2 f64
module dist2    = euclidean_d vector_2 f64
module dbscan2  = ft_dbscan vector_2 f64 kd2 dist2

-- Perform dbscan.
-- ==
-- entry: do_dbscan
-- compiled input @data/dat.in
-- auto output

entry do_dbscan [n] [part_no]
	(pts  : [2][n]f64)
	(mins : [2][]f64)
	(maxs : [2][]f64)
	(p_is : [part_no]i64)
	(eps    : [1]f64)
	(minPts : [1]i64)
	(extPar : [1]i64)
: ([n]bool, [n]i64) =
	let vcs = pts |> transpose
		|> map (sized vector_2.length >-> vector_2.from_array)
	let minmax = zip
		(mins
			|> transpose
			|> map (sized vector_2.length >-> vector_2.from_array)
			|> sized part_no)
		(maxs
			|> transpose
			|> map (sized vector_2.length >-> vector_2.from_array)
			|> sized part_no)
	let res = dbscan2.internal_dbscan extPar[0] eps[0] minPts[0]
		minmax p_is vcs
	in (res.is_core, res.chain_id)