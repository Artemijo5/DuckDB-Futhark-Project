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
			(eps : t)
			(minPts : i64)
			(pts : [n](vector t))
		: [n]bool =	pts |> map (\pt -> 
				pts |> countFor (D.check_neighbourhood eps pt)
			)
			|> map (\numNeigh -> numNeigh >= minPts)

		def isolate_core_pts [n]
			(isCore : [n]bool)
			(pts : [n](vector t))
		: [](vector t) = zip isCore pts
			|> filter (.0)
			|> map (.1)

		def mk_neigh_graph [nc]
			(extPar : i64)
			(eps : t)
			(core_pts : [nc](vector t))
		: [](i64,i64) =
			let num_iter = (nc+extPar-1)/extPar in
			loop output_graph : [](i64,i64) = []
			for j<num_iter do
				let inf = j*extPar
				let sup = i64.min nc (inf + extPar)
				let cur_edges = (inf..<sup)
					|> map (\i -> iota nc
						|> zip (replicate nc i)
					)
					|> flatten
					|> filter (\(i1,i2) -> i1!=i2)
					|> filter (\(i1,i2) -> D.check_neighbourhood eps core_pts[i1] core_pts[i2])
				in output_graph ++ cur_edges

		def mk_clusters [nc]
			(_ : [nc](vector t)) -- core_pts
			(neigh_graph : [](i64,i64))
		: [nc]i64 = get_connected_subgraph_ids nc neigh_graph
			|> encode_subgraph_ids

		def assign_clusters [n] [nc]
			(extPar : i64)
			(eps : t)
			(is_core : [n]bool)
			(pts : [n](vector t))
			(core_pts : [nc](vector t))
			(core_cid : [nc]i64)
		: [n]i64 =
			let (core_is,noncore_is) = iota n |> partition (\i -> is_core[i])
			let init_cid = scatter (replicate n (-1)) (core_is |> sized nc) core_cid
			let nnc = length noncore_is
			let num_iter = (nnc+extPar-1)/extPar in
			loop output_cid = init_cid
			for j<num_iter do
				let inf = j*extPar
				let sup = i64.min nnc (inf+extPar)
				let cur_cid = noncore_is[inf:sup]
					|> map (\i ->
						-- find closest core pt and use its cid
						D.find_closest_within eps core_pts pts[i]
							|> (\i -> if i>=0 && i<nc then core_cid[i] else (-1))
					)
				in scatter output_cid noncore_is[inf:sup] cur_cid

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
			let is_core = find_core_pts eps minPts pts
			let core_pts = isolate_core_pts is_core pts
			let core_cid = core_pts
				|> mk_neigh_graph extPar eps
				|> mk_clusters core_pts
			let cid = assign_clusters extPar eps is_core pts core_pts core_cid
			in (is_core, cid)

}