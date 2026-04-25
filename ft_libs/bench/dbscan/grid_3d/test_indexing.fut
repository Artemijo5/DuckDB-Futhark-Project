import "../../../lib/github.com/athas/vector/vector"
import "../../../dbscan/ft_spindex"

module vector_3 = cat_vector (cat_vector vector_1 vector_1) vector_1
module kd3      = grid_index vector_3 f64

-- Perform partitioning using kd-index.
-- ==
-- entry: do_kd_index
-- compiled input @data/dat.in
-- auto output

entry do_kd_index [n]
	(pts    : [3][n]f64)
	(depth  : [3]i64)
	(eps    : [1]f64)
	(minPts : [1]i64)
	(extPar : [1]i64)
: ([3][n]f64, [3][]f64, [3][]f64, []i64, [1]f64, [1]i64, [1]i64) =
	let vcs = pts |> transpose
		|> map (sized vector_3.length >-> vector_3.from_array)
	let (vcs',minmax,is,_) = kd3.index_dataset depth vcs
	let pts' = vcs' |> map (vector_3.to_array >-> sized 3)
		|> transpose
	let (min_vcs, max_vcs) = unzip minmax
	let mins = min_vcs |> map (vector_3.to_array >-> sized 3)
		|> transpose
	let maxs = max_vcs |> map (vector_3.to_array >-> sized 3)
		|> transpose
	in (pts', mins, maxs, is, eps, minPts, extPar)