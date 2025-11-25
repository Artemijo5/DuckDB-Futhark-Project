import "ftbasics"
import "lib/github.com/diku-dk/sorts/merge_sort"

-- TODO
-- test, compile, fix, etc

type~ core_cluster [dim] 'a = {core_pts: [][dim]a, core_ids: []i64}

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
	: [][dim]t =
		(zip isCore dat) |> filter (\(ic,_) -> ic) |> map (\(_,x) -> x)

	def find_cluster_ids [n] [dim]
		(core_dat : [n][dim]t)
		(eps : t)
		(minPts : i64)
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
			let next_conv = (map2 (!=) chBuff next_chBuff) |> any (id)
			in (next_chBuff, next_conv)
		-- List Ranking to obtain cluster ids
		let is_cluster_head = map2 (==) (iota n) cluster_heads
		let ch_ids = is_cluster_head
			|> map (\isCh -> if isCh then 1 else 0)
			|> exscan (+) 0
		in partitioned_gather (i64.num_bits) (gather_psize) (-1) ch_ids cluster_heads

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
	: [n]i64 =
		let extPar1 = i64.max 1 (pMem/n)
		let neighCounts = get_num_neighbours dat eps extPar1
		let isCore = find_core_points neighCounts minPts
		let corePts = isolate_core_points dat neighCounts isCore
		let extPar2 = i64.max 1 (pMem/(length corePts))
		let core_cids = find_cluster_ids corePts eps minPts extPar2 gather_psize
		in match_to_cluster_head dat corePts core_cids eps extPar2

	def do_DBSCAN_star [n] [dim]
		(dat : [n][dim]t)
		(eps : t)
		(minPts : i64)
		(pMem : i64)
		(gather_psize : i64)
	: core_cluster [dim] t =
		let extPar1 = i64.max 1 (n/pMem)
		let neighCounts = get_num_neighbours dat eps extPar1
		let isCore = find_core_points neighCounts minPts
		let corePts = isolate_core_points dat neighCounts isCore
		let extPar2 = i64.max 1 ((length corePts)/pMem)
		let core_cids = find_cluster_ids corePts eps minPts extPar2 gather_psize
		in {core_pts = corePts, core_ids = core_cids}
}

type~ core_cluster_float [dim] = core_cluster [dim] f32
type~ core_cluster_double [dim] = core_cluster [dim] f64

module dbscan_float = dbscan_module f32
module dbscan_double = dbscan_module f64