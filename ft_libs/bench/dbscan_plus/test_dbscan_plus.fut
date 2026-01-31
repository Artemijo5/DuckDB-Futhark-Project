import "../../ftDBSCAN_plus"
import "../../lib/github.com/diku-dk/sorts/merge_sort"

-- DBSCAN+ on 2d data.
--
-- ==
-- entry: test_dbscan_plus
-- input @data/sparse_f64
-- auto output
-- input @data/dense_f64
-- auto output

entry test_dbscan_plus (pts : [2][]f64) =
	let flushed = internal_DBSCAN_double
		25 25
		true true 0
		2.0 4
		(i64.highest) true
		pts[0] pts[1]
	in (flushed.xs, flushed.ys, flushed.chain_ids)