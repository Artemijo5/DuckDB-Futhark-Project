import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_distance"
import "ft_undir_graph"

-- DBSCAN implementation based on G-DBSCAN

module ft_dbscan
	(V : vector)
	(F : real)
	(D : distance with vector 'a = V.vector a with t = F.t)
= {
	type t = F.t
	type vector 'a = V.vector a

	-- Auxilliary Types & Values

		def leq = (F.<=)
		def lt  = (F.<)

		def lowest = F.lowest
		def highest = F.highest

		def zero = F.i32 0
		def minus_one = F.i32 (-1)

		def find_core_pts [n]
			(extPar : i64)
			(eps : t)
			(minPts : i64)
			(pts : [n](vector t))
		: [n]bool =
			let init_num_neigh = replicate n 1i64
			let num_iter = (n + extPar - 1) / extPar
			let (final_num_neigh,_) = loop (num_neigh,j) = (init_num_neigh,0)
			while j<num_iter  do
				let inf = j*extPar
				let sup = i64.min n (inf+extPar)
				-- if all points after are already core - terminate
				let term_now = (inf..<n)
					|> map (\i -> num_neigh[i])
					|> all (\nc -> nc >= minPts)
				in if term_now then (num_neigh,num_iter) else
				let (this_left, this_right) = (inf..<sup)
					|> map (\i -> zip (replicate n i) (iota n))
					|> flatten
					|> filter (\(i1,i2) -> i1<i2)
					|> filter (\(i1,i2) -> num_neigh[i1]<minPts || num_neigh[i2]<minPts)
					|> filter (\(i1,i2) -> D.check_neighbourhood eps pts[i1] pts[i2])
					|> unzip
				let next_num_neigh =
					hist_lean (+) 0 n this_left (this_left |> map (\_ -> 1i64))
						|> map2 (+) (hist_lean (+) 0 n this_right (this_right |> map (\_ -> 1i64)))
						|> map2 (+) num_neigh
				in (next_num_neigh, j+1)
			in final_num_neigh |> map (\numNeigh -> numNeigh >= minPts)

		def isolate_core_pts [n]
			(isCore : [n]bool)
			(pts : [n](vector t))
		: [](vector t) = zip isCore pts
			|> filter (.0)
			|> map (.1)

		def mk_clusters [nc]
			(extPar : i64)
			(eps : t)
			(core_pts : [nc](vector t))
		: []i64 =
			let init_cid = iota nc
			let num_iter = (nc+extPar-1)/extPar in
			let (final_cid,_) = loop (cid,j) = (init_cid,0)
			while j<num_iter do
				let inf = j*extPar
				let sup = i64.min nc (inf + extPar)
				let term_now = (inf..<nc)
					|> map (\i -> cid[i])
					|> (\cid_after -> (i64.minimum cid_after, i64.maximum cid_after))
					|> (\(cid1,cid2) -> cid1==cid2)
				in if term_now then (cid,num_iter) else
				let cur_edges = (inf..<sup)
					|> map (\i -> iota nc
						|> zip (replicate nc i)
					)
					|> flatten
					|> filter (\(i1,i2) -> i1<i2)
					|> filter (\(i1,i2) -> cid[i1] != cid[i2])
					|> filter (\(i1,i2) -> D.check_neighbourhood eps core_pts[i1] core_pts[i2])
					|> map (\(i1,i2) ->
						let cid1 = cid[i1]
						let cid2 = cid[i2]
						in (i64.min cid1 cid2, i64.max cid1 cid2)
					)
				let cur_matrix = get_connected_subgraph_ids nc cur_edges
				let next_cid =  cid |> map (\i -> cur_matrix[i])
				in (next_cid, j+1)
			in final_cid |> encode_subgraph_ids

		def assign_clusters [n] [nc]
			(extPar_ : i64)
			(eps : t)
			(is_core : [n]bool)
			(pts : [n](vector t))
			(core_pts : [nc](vector t))
			(core_cid : [nc]i64)
		: [n]i64 =
			--let extPar = n
			let extPar = extPar_
			let (core_is,noncore_is) = iota n |> partition (\i -> is_core[i])
			let init_cid = scatter (replicate n (-1)) (core_is |> sized nc) core_cid
			let nnc = length noncore_is
			let num_iter = (nnc + extPar - 1) / extPar
			let final_noncore_cid = loop noncore_cid = replicate nnc (-1)
			for j<num_iter do
				let inf = j*extPar
				let sup = i64.min nnc (inf + extPar)
				let this_cid = noncore_is[inf:sup]
					|> map (\i -> pts[i])
					|> map (D.find_closest_within eps core_pts)
					|> map (\i -> if i>=0 && i<nc then core_cid[i] else (-1))
				in noncore_cid with [inf:sup] = this_cid
			in scatter init_cid (noncore_is |> sized nnc) final_noncore_cid

	-- dbscan pipeline

		-- TODO interpreter doesn't like loop sized (...) why?
		-- 1 way to solve would be to keep both buffered & flushed chain_id, isCore in n-sized array (...)
		-- would require care to ensure only already read pts are going there
		-- ... might require to rewrite previous funcs to follow this logic as well...
		def internal_dbscan [n]
			(extPar : i64)
			(eps : t)
			(minPts : i64)
			(pts : [n](vector t))
		: ([n]bool, [n]i64) =
			let is_core = pts
				|> find_core_pts extPar eps minPts
			let core_pts = pts
				|> isolate_core_pts is_core
			let core_cid = core_pts
				|> mk_clusters extPar eps
			let cid = assign_clusters extPar eps is_core pts core_pts core_cid
			in (is_core, cid)

}