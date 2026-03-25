import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_spindex"

module type distance = {
	type t
	type vector 'a

	val dist_squared : vector t -> vector t -> t
	val dist : vector t -> vector t -> t
	
	val dist_fromPartition : (vector t, vector t) -> vector t -> t

	val get_adj_partitions [np] : [np](vector t, vector t) -> t -> i64 -> []i64
}

module euclidean_d
	(V : vector)
	(F : real)
: distance with t=F.t with vector 'a = V.vector a = {
	type t = F.t
	type vector 'a = V.vector a

	local def zero = F.i32 0

	local def leq = (F.<=)

	local def plus = (F.+)
	local def minus = (F.-)

	local def times = (F.*)
	local def sqrt = (F.sqrt)

	local def min = F.min
	local def max = F.max

	def dist_squared pt1 pt2 =
		let pt1' = pt1 |> V.map (\x -> x`times`x)
		let pt2' = pt2 |> V.map (\x -> x`times`x)
		in pt1' |> V.map2 (minus) pt2' |> V.reduce (plus) zero

	def dist pt1 pt2 = sqrt (dist_squared pt1 pt2)

	def dist_fromPartition (part : (vector t, vector t)) pt =
		let min' = iota V.length |> seqmap zero (\i ->
			if ((V.get i pt) `leq` (V.get i part.1))
			then max (V.get i pt) (V.get i part.0)
			else (V.get i part.0)
		) |> V.from_array
		let max' = iota V.length |> seqmap zero (\i ->
			if ((V.get i part.0) `leq` (V.get i pt))
			then min (V.get i pt) (V.get i part.1)
			else (V.get i part.1)
		) |> V.from_array
		in min (dist pt min') (dist pt max')

	-- TODO this returns false positives at the corners
	-- since it finds intersects with expanded rectangle
	-- rather than actual euclidean distance
	def get_adj_partitions partitions eps pid =
		let (this_mins, this_maxs) = partitions[pid]
			|> (\(tm,tM) -> (
				tm |> V.map (\mi -> mi `minus` eps),
				tM |> V.map (\ma -> ma `plus` eps)
			))
		let touch = indices partitions
			|> map2 (\(cmins,cmaxs) i -> i!=pid
				&& (this_maxs |> V.map2 (leq) cmins |> V.reduce (&&) true)
				&& (cmaxs |> V.map2 (leq) this_mins |> V.reduce (&&) true)
			) partitions
		in touch
			|> zip (indices partitions)
			|> filter (.1)
			|> map (.0)
}
