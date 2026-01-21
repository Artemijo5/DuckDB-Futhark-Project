import "ftbasics"
import "lib/github.com/diku-dk/segmented/segmented"

type~ core_cluster [dim] 'a = {len: i64, core_pts: [][dim]a, core_ids: []i64}

module dbscan_real (F : real) = {
	type t = F.t

	local def eq  = (F.==)
	local def gt  = (F.>)
	local def lt  = (F.<)
	local def leq = (F.<=)

	local def plus   = (F.+)
	local def minus  = (F.-)
	local def times  = (F.*)

	local def zero = F.i32 0

	local def lowest = F.lowest
	local def highest = F.highest

	local def sqrt = F.sqrt

	def dist [dim] (x1 : [dim]t) (x2 : [dim]t)
	: t =
		let dfs = map2 (minus) x1 x2
		in dfs |> map (\df -> df `times` df) |> foldl (plus) zero |> sqrt

	def get_num_neighbours [n] [dim]
		(dat : [n][dim]t)
		(eps : t)
		(extPar : i64)
	: [n]i64 =
		let num_iter = (extPar + n - 1)/extPar
		in loop nnBuff = (replicate n 0) for j in (iota num_iter) do
			let inf = j*extPar
			let sup = i64.min n (inf+extPar)
			let this_xs = dat[inf:sup]
			let this_neighCount = this_xs
				|> map (\this_x ->
					dat
						|> map (\cmp_x -> dist this_x cmp_x)
						|> countFor (\d -> d `leq` eps)
				) |> map (\c -> c-1)
			in nnBuff with [inf:sup] = this_neighCount

	def find_core_points [n]
		(count_neighbours : [n]i64)
		(minPts : i64)
	: [n]bool =
		count_neighbours |> map (\c -> c>=minPts)

	def isolate_core_points [n] [dim]
		(dat : [n][dim]t)
		(isCore : [n]bool)
	: [][dim]t =
		(zip isCore dat) |> filter (\(ic,_) -> ic) |> map (\(_,x) -> x)

	def old_find_cluster_ids [n] [dim]
		(core_dat : [n][dim]t)
		(eps : t)
		(extPar : i64)
		(gather_psize : i64)
	: [n]i64 =
		let inner_iter = (extPar + n - 1)/extPar
		-- Each core point starts with itself as cluster head
		-- In each iteration, it changes to the smallest cluster head in its neighbourhood
		-- Until convergence
		let (cluster_heads,_) =
		loop (chBuff,conv) = (iota n, false) while (!conv) do
			let next_chBuff = 
			loop chBuff_ = chBuff for j in (iota inner_iter) do
				let inf = j*extPar
				let sup = i64.min n (inf + extPar)
				let this_pts = core_dat[inf:sup]
				let this_chs = chBuff_[inf:sup]
				let this_min_ch = map2 (\ch pt ->
						let coreNeigh = core_dat
							|> map (\other_pt -> dist pt other_pt)
							|> map (\d -> d `leq` eps)
						let neighChs = map2 (\other_ch isN ->
							if isN then other_ch else ch
						) chBuff_ coreNeigh -- using chBuff_ here could bring faster convergence
						in i64.minimum neighChs
					) this_chs this_pts
				in (copy chBuff_) with [inf:sup] = this_min_ch
			let next_conv = (map2 (==) chBuff next_chBuff) |> all (id)
			in (next_chBuff, next_conv)
		-- List Ranking to obtain cluster ids
		let is_cluster_head = map2 (==) (iota n) cluster_heads
		let ch_ids = is_cluster_head
			|> map (\isCh -> if isCh then 1 else 0)
			|> exscan (+) 0
		in partitioned_gather (i64.num_bits) (gather_psize) (-1) ch_ids cluster_heads

	def mk_adjacency_list [n] [dim]
		(core_dat : [n][dim]t)
		(eps : t)
		(extPar : i64)
	: ([n]i64, []i64) = -- returns compact neighbour list + starting indices of each pt
		let inner_iter = (extPar + n - 1)/extPar
		let (neighcounts, graph) : ([n]i64, []i64) =
			loop (iter_nc, iter_graph) = (replicate n 0, [])
			for j in iota inner_iter do
				let inf = j*extPar
				let sup = i64.min n (inf + extPar)
				let this_pts = core_dat[inf:sup]
				let neigh_matrices = this_pts
					|> zip (inf..<sup)
					|> map (\(i1,pt1) ->
						core_dat |> zip (indices core_dat) |> map (\(i2,pt2) ->
							if i1 == i2 then (-1,-1,false) else
							let d = dist pt1 pt2
							in (i1, i2, d `leq` eps)
						)
					)
				let (compact_reps, compact_neigh) = neigh_matrices
					|> flatten
					|> filter (.2)
					|> map (\(i1,i2,_) -> (i1-inf,i2))
					|> unzip
				let nc = length compact_reps
				let this_neighCount = hist (+) 0 (sup-inf) (compact_reps :> [nc]i64) (replicate nc 1)
				in (iter_nc with [inf:sup] = this_neighCount, iter_graph ++ compact_neigh)
		in (exscan (+) 0 neighcounts, graph)

	def new_find_cluster_ids [n] [dim]
		(core_dat : [n][dim]t)
		(eps : t)
		(extPar : i64)
		(gather_psize : i64)
	: [n]i64 =
		-- Construct compact adjacency list
		let (dat_is, dat_neighs) = mk_adjacency_list core_dat eps extPar
		let ndn = length dat_neighs
		-- Use adjacency list to get smallest reachable pt
		let cluster_heads =
			-- Iterate over neighs:
			-- assign each neigh to be replaced by its smallest (first) neighbour
			-- until convergence
			let (_,itered_neighlist) : ([ndn]i64, [ndn]i64) =
				loop (old_list,new_list) = (replicate ndn (-1), dat_neighs :> [ndn]i64)
				while any (id) (map2 (\alt neu -> alt != neu) old_list new_list) do
					let newer_list_ = partitioned_gather
						(i64.num_bits) (gather_psize) (-1) new_list
						(new_list |> map (\i -> dat_is[i]))
					let newer_list = map2 (\alt neu->
						if alt>neu then neu else alt)
					new_list newer_list_
					in (new_list, newer_list)
			-- obtain ids via segmented reduction
			let segment_flags = scatter (replicate ndn false) dat_is (replicate n true)
			in segmented_reduce (\n1 n2 -> if n1<n2 then n1 else n2) (i64.highest)
				segment_flags (itered_neighlist :> [ndn]i64)
		-- List Ranking to obtain cluster ids
		let is_cluster_head = map2 (==) (iota n) (cluster_heads :> [n]i64)
		let ch_ids = is_cluster_head
			|> map (\isCh -> if isCh then 1 else 0)
			|> exscan (+) 0
		in partitioned_gather (i64.num_bits) (gather_psize) (-1) ch_ids (cluster_heads :> [n]i64)



	def match_to_cluster_head [n] [cn] [dim]
		(dat : [n][dim]t)
		(core_dat : [cn][dim]t)
		(cluster_head : [cn]i64)
		(eps : t)
		(extPar : i64)
	: [n]i64 =
		let num_iter = (extPar + n - 1)/extPar
		in loop buff = (replicate n (-1)) for j in (iota num_iter) do
			let inf = j*extPar
			let sup = i64.min n (inf+extPar)
			let this_pts = dat[inf:sup]
			let cur_upd = this_pts |> map (\this_x ->
				core_dat
				|> map (\cd -> dist this_x cd)
				|> zip cluster_head
				|> reduce_comm (\(ch1,d1) (ch2,d2) ->
					if (d1 `gt` eps) && (d2 `gt` eps) then (-1,highest)
					else if (d1 `lt` d2) then (ch1,d1)
					else (ch2,d2) -- non-deterministic if d1==d2
				) (-1,highest)
			) |> map (.0)
			in buff with [inf:sup] = cur_upd

	def do_DBSCAN [n] [dim]
		(dat : [n][dim]t)
		(eps : t)
		(minPts : i64)
		(pMem : i64)
		(gather_psize : i64)
		(construct_list : bool)
	: [n]i64 =
		let extPar1 = i64.max 1 (pMem/n)
		let neighCounts = get_num_neighbours dat eps extPar1
		let isCore = find_core_points neighCounts minPts
		let corePts = isolate_core_points dat isCore
		in
			if (length corePts)==0
			then (replicate n (-1))
			else
		let extPar2 = i64.max 1 (pMem/(i64.max 1 (length corePts)))
		let core_cids = 
			if construct_list
			then new_find_cluster_ids corePts eps extPar2 gather_psize
			else old_find_cluster_ids corePts eps extPar2 gather_psize
		in match_to_cluster_head dat corePts core_cids eps extPar2

	def do_DBSCAN_star [n] [dim]
		(dat : [n][dim]t)
		(eps : t)
		(minPts : i64)
		(pMem : i64)
		(gather_psize : i64)
		(construct_list : bool)
	: core_cluster [dim] t =
		let extPar1 = i64.max 1 (pMem/n)
		let neighCounts = get_num_neighbours dat eps extPar1
		let isCore = find_core_points neighCounts minPts
		let corePts = isolate_core_points dat isCore
		in
			if (length corePts)==0
			then {len = 0, core_pts = corePts :> [0][dim]t, core_ids = [] :> [0]i64}
			else
		let extPar2 = i64.max 1 (pMem/(i64.max 1 (length corePts)))
		let core_cids = 
			if construct_list
			then new_find_cluster_ids corePts eps extPar2 gather_psize
			else old_find_cluster_ids corePts eps extPar2 gather_psize
		in {len = length corePts, core_pts = corePts, core_ids = core_cids}
}

type~ core_cluster_float [dim] = core_cluster [dim] f32
type~ core_cluster_double [dim] = core_cluster [dim] f64

module dbscan_float = dbscan_real f32
module dbscan_double = dbscan_real f64

-- Tests

	def test_dat : [][2]f32 = [
		[2.00, 0.05],
		[2.00, 2.00],
		[2.20, 2.25],
		[3.20, 2.20],
		[1.75, 0.75],
		[9.00, 1.00],
		[0.20, 0.20],
		[1.50, 1.00],
		[0.50, 6.55],
		[0.50, 7.50],
		[9.50, 1.10],
		[7.15, 1.80],
		[8.50, 1.00],
		[1.75, 8.00],
		[0.20, 0.50],
		[0.50, 1.50],
		[1.00, 8.00],
		[0.50, 5.75],
		[1.50, 8.00],
		[9.00, 9.00],
		[7.15, 1.00],
		[9.00, 1.50],
		[9.00, 2.25],
		[2.80, 7.90],
		[0.50, 8.50],
		[1.00, 1.00],
		[1.10, 0.75],
		[2.00, 7.50],
	]

	def test_find_neighbour_counts =
		let dat : [][2]f32 = test_dat
		in zip dat (dbscan_float.get_num_neighbours dat 1.0 (length dat))

	def test_list =
		let dat : [][2]f32 = test_dat
		let neighCounts = dbscan_float.get_num_neighbours dat 1.0 (length dat)
		let isCore = dbscan_float.find_core_points neighCounts 3
		let corePts = dbscan_float.isolate_core_points dat isCore
		in dbscan_float.mk_adjacency_list corePts 1.0 1024

	def test_DBSCAN (pMem : i64) (construct_list : bool) =
		let dat : [][2]f32 = test_dat
		in zip dat (dbscan_float.do_DBSCAN dat 1.0 3 pMem 1024 construct_list)

	def test_DBSCAN_star (pMem : i64) (construct_list : bool) =
		let dat : [][2]f32 = test_dat
		let dat_star = dbscan_float.do_DBSCAN_star dat 1.0 3 pMem 1024 construct_list
		in zip dat_star.core_pts dat_star.core_ids


-- Entry Points

	entry ftDBSCAN_float = dbscan_float.do_DBSCAN
	entry ftDBSCAN_double = dbscan_double.do_DBSCAN

	entry ftDBSCAN_star_float [dim] dat eps minPts pMem gather_psize construct_list
	: core_cluster_float [dim] =
		dbscan_float.do_DBSCAN_star dat eps minPts pMem gather_psize construct_list

	entry ftDBSCAN_star_double [dim] dat eps minPts pMem gather_psize construct_list
	: core_cluster_double [dim] =
		dbscan_double.do_DBSCAN_star dat eps minPts pMem gather_psize construct_list

-- TODO ensure compact method yields no errors

