import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_spindex"

module type distance = {
	type t
	type vector 'a

	val dist_squared : vector t -> vector t -> t
	val dist : vector t -> vector t -> t
	
	val dist_fromPartition : (vector t, vector t) -> vector t -> t

	-- Ignore partition with pid
	val get_adj_partitions [np] [n]
		: [np](vector t, vector t) -> t -> i64 -> [n](vector t) -> []i64

	val is_marginal : (vector t, vector t) -> t -> vector t -> bool
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
	local def highest = F.highest

	local def minimum = F.minimum

	def dist_squared pt1 pt2 =
		let pt1' = pt1 |> V.map (\x -> x`times`x)
		let pt2' = pt2 |> V.map (\x -> x`times`x)
		in pt1' |> V.map2 (minus) pt2' |> V.reduce (plus) zero

	def dist pt1 pt2 = sqrt (dist_squared pt1 pt2)

	local def dist_squared_fromPartition (part : (vector t, vector t)) pt =
		iota V.length |> seqmap zero (\i ->
			min (max (V.get i pt) (V.get i part.0))
				(V.get i part.1)
		) |> V.from_array
		|> dist_squared pt

	def dist_fromPartition (part : (vector t, vector t)) pt =
		pt |> dist_squared_fromPartition part |> sqrt

	-- TODO see how this goes with cuda compilation
	-- might have to use seqmap before filter (...)
	def get_adj_partitions partitions eps pid pts =
		indices partitions |> filter (\i ->
			i!=pid && leq
				(eps `times` eps)
				(pts |> map (dist_squared_fromPartition partitions[i])
					|> minimum)
		)

	def is_marginal (part : (vector t, vector t)) eps pt =
		V.map2 (minus) pt part.0
		|> V.map2 (min) (V.map2 (minus) part.1 pt)
		|> V.reduce (min) highest
		|> leq eps
}
