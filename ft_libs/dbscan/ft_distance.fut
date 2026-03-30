import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "ft_spindex"

module type distance = {
	type t
	type vector 'a

	val dist : vector t -> vector t -> t

	val check_neighbourhood : t -> vector t -> vector t -> bool

	-- find closest to a certain point, also within a distance eps
	-- if distance is set to negative, ignored
	val find_closest_within [n] : t -> [n](vector t) -> vector t -> i64
	
	val dist_from_partition : (vector t, vector t) -> vector t -> t

	-- Ignore partition with pid
	val get_adj_partitions [np] [n]
		: [np](vector t, vector t) -> t -> i64 -> [n](vector t) -> []i64

	-- How many partitions are close to a point?
	val get_num_adj_partitions [np]
		: [np](vector t, vector t) -> t -> i64 -> vector t -> i64

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
	local def lt = (F.<)
	local def eq = (F.==)

	local def plus = (F.+)
	local def minus = (F.-)

	local def times = (F.*)
	local def sqrt = (F.sqrt)

	local def min = F.min
	local def max = F.max
	local def highest = F.highest

	local def minimum = F.minimum

	local def dist_squared pt1 pt2 =
		pt1 |> V.map2 (minus) pt2
		|> V.map (\x -> x `times` x)
		|> V.reduce (plus) zero

	def dist pt1 pt2 = sqrt (dist_squared pt1 pt2)

	def check_neighbourhood eps pt1 pt2 =
		(dist_squared pt1 pt2) `leq` (eps `times` eps)

	def find_closest_within eps pts pt = pts
		|> map (\pt' ->
			let d = dist_squared pt pt'
			in if (eps `lt` zero) || (d `leq` (eps `times` eps))
				then d else highest
		) |> argmin (lt) (eq) highest

	local def dist_squared_from_partition (part : (vector t, vector t)) pt =
		iota V.length |> seqmap zero (\i ->
			min (max (V.get i pt) (V.get i part.0))
				(V.get i part.1)
		) |> V.from_array
		|> dist_squared pt

	def dist_from_partition (part : (vector t, vector t)) pt =
		pt |> dist_squared_from_partition part |> sqrt

	-- Confirmed cuda compilation works
	def get_adj_partitions partitions eps pid pts =
		indices partitions |> filter (\i ->
			i!=pid && leq
				(pts |> map (dist_squared_from_partition partitions[i])
					|> minimum)
				(eps `times` eps)
		)

	def get_num_adj_partitions partitions eps pid pt =
		indices partitions |> countFor (\i ->
			i!=pid && leq
				(dist_squared_from_partition partitions[i] pt)
				(eps `times` eps)
		)

	def is_marginal (part : (vector t, vector t)) eps pt =
		V.map2 (minus) pt part.0
		|> V.map2 (min) (V.map2 (minus) part.1 pt)
		|> V.reduce (min) highest
		|> (\d -> d `leq` eps)
}