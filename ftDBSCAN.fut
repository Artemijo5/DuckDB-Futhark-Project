import "ftbasics"

-- TODO
-- test, compile, fix, etc

type~ cluster_info [n] [dim] 'a = {core_points : [][dim]a, cluster_heads : [n]i64}

module dbscan_module (F : real) = {
	type t = F.t

	def eq  = (F.==)
	def neq = (F.!=)
	def gt  = (F.>)
	def lt  = (F.<)
	def geq = (F.>=)
	def leq = (F.<=)

	def min = F.min
	def max = F.max
	def minimum = F.minimum
	def maximum = F.maximum

	def plus   = (F.+)
	def minus  = (F.-)
	def times  = (F.*)
	def over   = (F./)

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

	
	def find_cluster_heads [n] [dim]
		(core_dat : [n][dim]t)
		(neighbourCounts : [n]i64)
		(eps : t)
		(minPts : i64)
		(extPar : i64)
		(gather_psize : i64)
	: [n]i64 =
		let inner_iter = (extPar + n - 1)/extPar
		-- Stage 1 - find neighbour with highest neighbour count
		let heaviestNeighbours = loop neighBuff = (iota n) for j in (iota extPar) do
			let inf = j*extPar
			let sup = i64.min n (inf+extPar)
			let this_pts = core_dat[inf:sup] :>[sup-inf][dim]t
			let this_first_ch = this_pts
				|> map (\pt ->
					let isNeighbour = core_dat
						|> map (\other_pt ->
							dist pt other_pt
						)
						|> map (\d -> d `leq` eps)
					let nearby_neighCounts = map2
						(\isNeighbour neighCount ->
							if isNeighbour
							then neighCount
							else 0
						)
					-- passing > and lowest instead of < and highest
					-- effectively makes this argmax
					in argmin (>) (==) (i64.lowest) nearby_neighCounts
				)
			in neighBuff with [inf:sup] = this_first_ch
		-- Stage 2 - iteratively look for cluster head
		-- cluster heads are going to be the points that have themselves assigned as nearest neighbours
		-- making them the point with most neighbours in that cluster
		-- or the point with the smallest index out of the ones that tie with it
		let (clusterHeads,_) = loop (chBuff,cont) = (heaviestNeighbours,true) while cont do
			let new_cluster_head = partitioned_gather_over_array ((i32.i64 dim)*i64.num_bits) gather_psize
				chBuff chBuff chBuff
			let still_cont = any (id) (map2 (!=) new_cluster_head chBuff)
			in (new_cluster_head, still_cont)
		in clusterHeads


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
					if (d1 `gt` eps) && (d2 `gt` eps) then (-1,0)
					else if (d1 `lt` d2) then (ch1,d1)
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