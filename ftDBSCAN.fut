import "ftbasics"
import "lib/github.com/diku-dk/sorts/merge_sort"

-- TODO
-- test, compile, fix, etc

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
		(pMem : i64)
		(gather_psize : i64)
	: [n]i64 =
		let extPar1 = i64.max 1 (n/pMem)
		let neighCounts = get_num_neighbours dat eps extPar1
		let isCore = find_core_points neighCounts minPts
		let (corePts, coreNeighCounts) = unzip (isolate_core_points dat neighCounts isCore)
		let extPar2 = i64.max 1 ((length corePts)/pMem)
		let core_chs = find_cluster_heads corePts coreNeighCounts eps minPts extPar2 gather_psize
		in match_to_cluster_head dat core core_chs eps extPar2
		-- Note : since core_points is discared, cluster head index stops referring directly to that point
		-- but it still serves as a distinct cluster index

	-- TODO version with payloads?
	-- TODO must keep filtered cluster_ids as well
--	def filter_out_noise [n] [dim]
--		(dat : [n][dim]t)
--		(cluster_ids : [n]i64)
--	: [n]i64 =
--		dat |> zip cluster_ids |> filter (\(_,cid) -> cid>=0) |> map (.0)

	-- TODO version with payloads?
	-- TODO must keep sorted cluster ids as well
--	def sort_by_cluster_id [n] [dim]
--		(dat : [n][dim]t)
--		(cluster_ids)

}

module dbscan_float = dbscan_module f32
module dbscan_float = dbscan_module f64