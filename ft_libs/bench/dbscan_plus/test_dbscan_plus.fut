import "../../ftDBSCAN_plus"
import "../../lib/github.com/diku-dk/sorts/merge_sort"

-- DBSCAN+ on 2d data.
--
-- ==
-- entry: test_dbscan_plus
-- input @data/sparse_f64
-- input @data/dense_f64

entry test_dbscan_plus [n] (pts : [2][n]f64) =
	--let eps : f64 = 2.0 * ( (f64.i64 n)/8192 |> f64.sqrt )
	let eps : f64 = 2.0
	let minPts : i64 = 4
	let flushed = internal_DBSCAN_double
		32 32
		true true 0
		eps 4
		(i64.highest) true
		pts[0] pts[1]
	in (flushed.xs, flushed.ys, flushed.chain_ids)