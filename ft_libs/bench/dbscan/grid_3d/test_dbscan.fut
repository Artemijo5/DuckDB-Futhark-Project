import "../../../lib/github.com/athas/vector/vector"
import "../../../dbscan/ft_spindex"
import "../../../dbscan/ft_distance"
import "../../../dbscan/dbscan"

module vector_3 = cat_vector (cat_vector vector_1 vector_1) vector_1
module kd3      = grid_index vector_3 f64
module dist3    = euclidean_d vector_3 f64
module dbscan3  = ft_dbscan vector_3 f64 kd3 dist3

-- Perform dbscan.
-- ==
-- entry: do_dbscan
-- compiled input @data/dat.in

entry do_dbscan [n] [part_no]
	(pts  : [3][n]f64)
	(mins : [3][]f64)
	(maxs : [3][]f64)
	(p_is : [part_no]i64)
	(eps    : [1]f64)
	(minPts : [1]i64)
	(extPar : [1]i64)
: ([n]bool, [n]i64) =
	let vcs = pts |> transpose
		|> map (sized vector_3.length >-> vector_3.from_array)
	let minmax = zip
		(mins
			|> transpose
			|> map (sized vector_3.length >-> vector_3.from_array)
			|> sized part_no)
		(maxs
			|> transpose
			|> map (sized vector_3.length >-> vector_3.from_array)
			|> sized part_no)
	let res = dbscan3.internal_dbscan extPar[0] eps[0] minPts[0]
		minmax p_is vcs
	in (res.is_core, res.chain_id)