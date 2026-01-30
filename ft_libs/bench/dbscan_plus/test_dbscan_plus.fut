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

entry test_dbscan_plus [n] (pts_ : [2][n]f64) =
	let pts = zip pts_[0] pts_[1]
	let maxval = f64.max (f64.maximum pts_[0]) (f64.maximum pts_[1])
	let subdv : f64 = 16 -- grid subdivisions per dimension
	let part_no : i64 = (i64.f64 subdv)*(i64.f64 subdv)
	let step = maxval / subdv
	-- sort pts by grid id - approximation of an index
	let get_grid_id ((x,y) : (f64,f64)) : i64 =
		(x/step |> f64.floor) + ((y/step)*subdv |> f64.floor)
		|> i64.f64
	let pts_byGrid = merge_sort_by_key (get_grid_id) (<=) pts
	let inf_byPart = hist
		(\i1 i2 -> if i1<i2 then i1 else i2)
		n
		part_no
		(pts_byGrid |> map (\pt -> get_grid_id pt))
		(iota n)
	-- do dbscan
	let clHandler_ = mk_dbcHandler_double
			0 0 maxval maxval (i64.f64 subdv) (i64.f64 subdv)
			true true 0
			2.0 4 (i64.highest)
	let (ret_pts, ret_ids, cc, _, _)
	=
	loop (flushed_pts, flushed_ids, collisions, old_clHandler, next_part)
	= ([],[],{ncc=0,chain_id=[],replaceWith=[]},clHandler_,0)
	while next_part>=0 do
		let inf = inf_byPart[next_part]
		let sup = if next_part==part_no-1 then n else inf_byPart[next_part+1]
		let (new_xs, new_ys) = pts_byGrid[inf:sup] |> unzip
		let clHandler = add_next_partition_double old_clHandler next_part new_xs new_ys
		let (this_fpts_preDBSCAN, this_fcids_preDBSCAN) =
			let this_flushed = flush_dat_double clHandler false
			in (zip this_flushed.xs this_flushed.ys, this_flushed.chain_ids)
		let clHandler1 = do_DBSCAN_double clHandler (i64.highest) true
		let next_next_part = next_partition_to_read_double clHandler1
		let (this_fpts, this_fcids) =
			if next_next_part >= 0
			then (this_fpts_preDBSCAN, this_fcids_preDBSCAN)
			else
				let this_flushed = flush_dat_double clHandler1 true
				let (this_fpts_postDBSCAN, this_fcids_postDBSCAN)
					= (zip this_flushed.xs this_flushed.ys, this_flushed.chain_ids)
				in (this_fpts_preDBSCAN ++ this_fpts_postDBSCAN, this_fcids_preDBSCAN ++ this_fcids_postDBSCAN)
		let next_cc = if next_next_part >= 0 then collisions else make_collisions_compact_double clHandler1
		in (this_fpts, this_fcids, next_cc, clHandler1, next_next_part)
	-- rectification pass
	let final_cids = rectify_cluster_ids_double ret_ids cc (i64.highest)
	in (ret_pts |> map (.0), ret_pts |> map (.1), ret_ids)