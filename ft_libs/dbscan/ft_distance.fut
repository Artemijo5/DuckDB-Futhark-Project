import "../ftbasics"
import "../lib/github.com/athas/vector/vector"

-- Module for metric distance.
-- Provides functions for point distances,
-- as well as distances between min/max rectangles,
-- to facilitate the min/max partitioning of ft_spindex.

module type distance = {
	type t
	type vector 'a

	-- | Obtain the distance between 2 points.
	val dist : vector t -> vector t -> t

	-- | Check if 2 points meet a neighbourhood criterion.
	val check_neighbourhood : t -> vector t -> vector t -> bool

	-- | Find closest to a certain point, also within a distance eps
	-- if distance is set to negative, ignored
	val find_closest_within [n] : t -> [n](vector t) -> vector t -> i64
	
	-- | Find a point's distance from a min/max rectangular partition.
	val dist_from_partition : (vector t, vector t) -> vector t -> t

	-- | Find minimum distance between 2 min/max rectangles.
	val get_rectangle_dist : (vector t, vector t) -> (vector t, vector t) -> t

	-- | Get all min/max rectangles neighbouring a specific one.
	-- Afterwards can call get_adj_partitions_withPts
	-- to filter those that also have a point neighbouring them.
	val get_adj_partitions [np]
		: [np](vector t, vector t) -> t -> i64 -> []i64

	-- | Get all min/max partitions neighbouring a set of points.
	-- Good to first filter partitions with get_adj_partitions,
	-- and isolate marginal points.
	val get_adj_partitions_withPts [np] [n]
		: [np](vector t, vector t) -> t -> i64 -> [n](vector t) -> []i64

	-- | How many partitions are close to a point?
	val get_num_adj_partitions [np]
		: [np](vector t, vector t) -> t -> i64 -> vector t -> i64

	-- | Is a point within a given eps distance of a min/max partition's margin (from within)?
	val is_marginal : (vector t, vector t) -> t -> vector t -> bool

	-- | For a given set of points, generate neighbour pairs.
	-- The point with the smaller index is first, with the largest is second.
	-- This can be used as an implicit undirected neighbourhood graph,
	-- using the same logic as for the undir_graph module.
	--
	-- i64 parameter divides nested parallelism into sequential blocks,
	-- to avoid manifesting a (n^2)-sized array that could explode memory.
	val get_neighbour_pairs [n] : i64 -> t -> [n](vector t) -> [](i64,i64)
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
	local def neq = (F.!=)

	local def plus = (F.+)
	local def minus = (F.-)
	local def times = (F.*)

	local def abs = (F.abs)
	local def sgn = (F.sgn)
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

	def get_rectangle_dist (part1 : (vector t, vector t)) (part2 : (vector t, vector t))
	: t =
		let p1 = V.map2 (\mi ma -> (mi,ma)) part1.0 part1.1
		let p2 = V.map2 (\mi ma -> (mi,ma)) part2.0 part2.1
		-- get all pairwise min/max dists to see if E overlap
		-- if E overlap in a dim, then that dim's distance is 0
		-- otherwise, it's the absolutely minimum of the 2 distances
		in V.map2 (\(mi1,ma1) (mi2,ma2) ->
			(mi1 `minus` mi2, mi1 `minus` ma2, ma1 `minus` mi2, ma1 `minus` ma2)
		) p1 p2
			|> V.map (\(d_mm,d_mM,d_Mm,d_MM) ->
					let overlap_m = neq (sgn d_mm) (sgn d_mM)
					let overlap_M = neq (sgn d_Mm) (sgn d_MM)
					let d_m = if overlap_m then zero
						else min (abs d_mm) (abs d_mM)
					let d_M = if overlap_M then zero
						else min (abs d_Mm) (abs d_MM)
					in min d_m d_M
				)
			|> V.map (\diff -> diff `times` diff)
			|> V.reduce (plus) zero
			|> sqrt

	def get_adj_partitions partitions eps pid =
		indices partitions |> filter (\i ->
			if i==pid then false else
			let p = partitions[pid]
			let this_p = partitions[i]
			let d = p |> get_rectangle_dist this_p
			in (d `leq` eps)		
		)

	-- confirmed cuda compilation works
	def get_adj_partitions_withPts partitions eps pid pts =
		indices partitions |> filter (\i ->
			i!=pid && (pts
				|> map (dist_squared_from_partition partitions[i])
				|> minimum
				|> (\d -> d `leq` (eps `times` eps)))
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

	-- pts1 & pts2 are meant to be isolated subsets of the same set
	-- specifically, pts1 is a window taken from the set
	-- pts2 are all pts 'after' (with indices greater than) that window
	-- OR pts1 == pts2
	-- return pts1 index left, pts2 index right
	-- left index <= right index (self-neighbourood is also returned)
	local def do_get_neigh_pairs [n1] [n2]
		(eps : t)
		(pts1  : [n1](vector t))
		(offs1 : i64)
		(pts2  : [n2](vector t))
		(offs2 : i64)
	: [](i64,i64) =
		iota n1 |> map (\i1 ->
			iota n2 |> map (\i2 ->
				let are_neigh = check_neighbourhood eps pts1[i1] pts2[i2]
				in if are_neigh
					then (offs1+i1,offs2+i2)
					else (-1,-1)
			)
		) |> flatten |> filter (\(i1,i2) -> i1>=0 && i1<=i2)

	def get_neighbour_pairs extPar eps pts =
		let num_iter = ((length pts) + extPar - 1) / extPar
		in loop pairs : [](i64,i64) = []
		for j < num_iter do
			let inf = j*extPar
			let sup = i64.min (length pts) (inf + extPar)
			let pts1 = pts[inf:sup]
			let pts2 = pts[sup:(length pts)]
			let pairs1 = do_get_neigh_pairs eps pts1 inf pts1 inf
			let pairs2 = do_get_neigh_pairs eps pts1 inf pts2 sup
			in pairs ++ pairs1 ++ pairs2

}