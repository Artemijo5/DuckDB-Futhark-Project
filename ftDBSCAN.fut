import "ftbasics"

-- TODO
-- test, compile, fix, etc

type~ cluster_info [n] [dim] 'a = {core_points : [][dim]a, cluster_heads : [n]i64}

module dbscan_module (F : real) = {
	type t = F.t

	def (==) = (F.==)
	def (!=) = (F.!=)
	def (>)  = (F.>)
	def (<)  = (F.<)
	def (>=) = (F.>=)
	def (<=) = (F.<=)

	def min = F.min
	def max = F.max
	def minimum = F.minimum
	def maximum = F.maximum

	def (+)  = (F.+)
	def (-)  = (F.-)
	def (*)  = (F.*)
	def (/)  = (F./)
	def (**) = (F.**)

	def zero = F.i32 0
	def one = F.i32 1
	def two = F.i32 2

	def lowest = F.lowest
	def highest = F.highest

	def sum = F.sum
	def prod = F.product
	def sqrt = F.sqrt

	def dist [dim] (x1 : [dim]t) (x2 : [dim]t)
	: t =
		let sx1 = x1 |> map (\x -> x*x)
		let sx2 = x2 |> map (\x -> x*x)
		let sdf = map2 (-) sx1 sx2
		in sdf |> foldl (+) zero |> sqrt

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
						|> countFor (\d -> d<=eps)
				)
			in nnBuff with [inf:sup] = this_neighCount

	def find_core_points [n]
		(count_neighbours : [n]i64)
		(minPts : i64)
	: [n]bool =
		count_neighbours |> map (\c -> c>=minPts)

	def isolate_core_points [n] [dim]
		(dat : [n][dim]t)
		(neighbourCounts : [n]i64)
		(isCore : [n]bool)
	: []([dim]t,i64) =
		(zip3 isCore dat neighbourCounts) |> filter (\(ic,_,_) -> ic) |> map (\(_,x,c) -> (x,c))

	-- this will be a pain to test & compile & fix...
	-- if doesn't do cuda compilation, will have to simplify somehow...
	-- maybe make sequential first and work from there...
	def find_cluster_heads [n] [dim]
		(core_dat : [n][dim]t)
		(neighbourCounts : [n]i64)
		(eps : t)
		(minPts : i64)
		(extPar : i64)
	: [n]i64 =
		let inner_iter = (extPar + n - 1)/extPar
		let (assigned_head,_,_) =
		loop (curHead, j, estim_len) = (iota n, 0, n)
		while j<estim_len do
			let assignHead =
			loop md = copy curHead
			for k in (iota inner_iter) do
				let inf = k*extPar
				let sup = i64.min n (inf+extPar)
				let this_pts = curHead[inf:sup]
				let this_num_neighbours = this_pts |> map (\hi -> neighbourCounts[hi]) -- TODO partitioned gather?
				let min_dist_pts =
					-- find minimum distance core point other than the current one
					this_pts |> map (\this_hi ->
						let this_x = dat[this_hi]
						let dists = curHead
							|> map (\h -> core_dat[h])
							|> map (\cand -> dist x cand)
						let closest = reduce_comm (\(d1, ind1) (d2, ind2) ->
								if ind1==this_hi then (d2, ind2)
								else if ind2==this_hi || (d1<d2) then (d1,ind1)
								else if d2<d1 then (d2,ind2)
								else (d1, i64.min ind1 ind2) -- equal distances, handled here for determinism
							)
							(highest, this_hi)
							(zip dists curHead)
						in
							if closest.0 <= eps
							then closest.1
							else this_hi
					)
				let min_dist_neighs = min_dist_pts |> map (\ni -> neighbourCounts[ni]) -- TODO partitioned gather?
				let this_update_heads map4 (\hi hc ni nc ->
						if hi==ni || hc>nc || (hc==nc && hi<ni)
						then hi
						else ni
					)
					this_pts
					this_num_neighbours
					min_dist_pts
					min_dist_neighs
				in md with [inf:sup] = this_update_heads
			let cur_finalIter_estim = curHead
				|> zip assignHead
				|> countFor (\alt neu -> alt != neu)
			in (assignHead, j+1, cur_finalIter_estim)
		in assigned_head

	def match_to_cluster_head [n] [cn] [dim]
		(dat : [n][dim]t)
		(core_dat : [cn][dim]t)
		(cluster_head : [cn]i64)
		(eps : [n]t)
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
					if d1>eps && d2>eps then (-1,0)
					else if d1<d2 then (ch1,d1)
					else (ch2,d2)
				) (-1,highest)
			) |> map (.0)
			in buff with [inf:sup] = cur_upd

	def do_DBSCAN [n] [dim]
		(dat : [n][dim]t)
		(eps : t)
		(minPts : i64)
		(extPar : i64)
	= 0 -- TODO
	-- : cluster_info [n] [dim] t =







}

module dbscan_float = dbscan_module f32
module dbscan_float = dbscan_module f64