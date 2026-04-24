import "../../../lib/github.com/athas/vector/vector"
import "../../../dbscan/ft_spindex"

module vector_2 = cat_vector vector_1 vector_1
module kd2 = kd_index vector_2 f64

-- Perform partitioning using kd-index.
-- ==
-- entry: do_kd_index
-- compiled input @data/dat.in
-- auto output

entry do_kd_index [n]
	(pts    : [2][n]f64)
	(depth  : [1]i64)
	(eps    : [1]f64)
	(minPts : [1]i64)
	(extPar : [1]i64)
: ([2][n]f64, [2][]f64, [2][]f64, []i64, [1]f64, [1]i64, [1]i64) =
	let vcs = pts |> transpose
		|> map (sized vector_2.length >-> vector_2.from_array)
	let (vcs',minmax,is) = kd2.index_dataset depth vcs
	let pts' = vcs' |> map (vector_2.to_array >-> sized 2)
		|> transpose
	let (min_vcs, max_vcs) = unzip minmax
	let mins = min_vcs |> map (vector_2.to_array >-> sized 2)
		|> transpose
	let maxs = max_vcs |> map (vector_2.to_array >-> sized 2)
		|> transpose
	in (pts', mins, maxs, is, eps, minPts, extPar)