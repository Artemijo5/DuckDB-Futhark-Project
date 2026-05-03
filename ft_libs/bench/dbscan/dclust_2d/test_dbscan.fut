import "../../../lib/github.com/athas/vector/vector"
import "../../../dbscan/ft_spindex"
import "../../../dbscan/ft_distance"
import "../../../dbscan/ft_dclust"

module vector_2 = cat_vector vector_1 vector_1
module dist2    = euclidean_d vector_2 f64
module dclust2  = ft_dclust vector_2 f64 dist2

-- Perform dbscan.
-- ==
-- entry: do_dbscan
-- compiled input @data/dat.in

entry do_dbscan [n]
	(pts : [2][n]f64)
	(subdiv : [1]i64)
	(eps : [1]f64)
	(minPts : [1]i64)
	(extPar : [1]i64)
	(seed_count : [1]i64)
: ([n]bool, [n]i64) =
	let vcs = pts |> transpose
		|> map (sized vector_2.length >-> vector_2.from_array)
	in vcs |> dclust2.do_dclust extPar[0] seed_count[0]
		(replicate vector_2.length subdiv[0]) eps[0] minPts[0]
