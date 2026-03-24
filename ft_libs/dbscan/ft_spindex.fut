import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/sorts/merge_sort"
import "../vector_cols"

-- Implementation of basic spatial index structures.
-- 1. Uniform Grid Partitioning
-- 2. Implicit kd-Tree

-- | Abstract module type to implement spatial index structures.
module type mk_spatial_index = {
	type t
	type vector 'a

	-- | Create the index using index_spec.
	-- Returns a tuple containing:
	-- 1. the dataset sorted by index-based partitions.
	-- 2. partition boundaries
	-- 3. starting index of each partition in the sorted dataset
	val index_dataset [dim] [n] : [dim]i64 -> [n](vector t) -> ([n](vector t), [](vector t, vector t), []i64)

	-- | Obtain all partitions adjacent to a selected partition.
	-- Returns the indices of all adjacent partitions (not including itself).
	val get_adj_partitions [np] : [np](vector t, vector t) -> i64 -> []i64

	-- | Obtain all points of a selected partition.
	val fetch_partition [np] [n] : [np]i64 -> [n](vector t) -> i64 -> [](vector t)
}

module mk_grid_index (V : vector) (N : numeric)
: mk_spatial_index with t = N.t with vector 'a = V.vector a = {
	type t = N.t
	type vector 'a = V.vector a

	local def over  = (N./)
	local def times = (N.*)
	local def minus = (N.-)
	local def plus  = (N.+)

	local def leq = (N.<=)

	local def to_i64 = (N.to_i64)
	local def from_i64 = (N.i64)

	local def zero = from_i64 0i64

	local def minimum = N.minimum
	local def maximum = N.maximum

	local def get_mins_ranges (xs : [](vector t)) : (vector t, vector t) =
		let perDim = iota (V.length) |> map (\i -> xs |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		let ranges = V.map2 (minus) maxs mins
		in (mins, ranges)

	local def get_partition_id
		(mins : vector t)
		(ranges : vector t)
		(idx_vec : vector i64)
		(dimPrefix : vector i64)
		(x : vector t)
	: i64 = x
		|> V.map2 (\mi xi -> xi `minus` mi) mins
		|> V.map2 (\pD xi -> xi `times` (from_i64 pD)) idx_vec
		|> V.map2 (\rg xi -> xi `over` rg) ranges
		|> V.map (to_i64)
		|> V.map2 (i64.min) (idx_vec |> V.map (\pD -> pD - 1))
		|> V.map2 (*) dimPrefix
		|> V.reduce (+) 0

	local def get_partitionBoundaries
		(mins : vector t)
		(ranges : vector t)
		(idx_vec : vector i64)
		(dimPrefix : vector i64)
		(pid : i64)
	: (vector t, vector t) =
		let pid_byDim = dimPrefix
			|> V.map2 (\spec pref -> (pid / pref) % spec) idx_vec
			|> V.map (from_i64)
		let step_byDim = idx_vec
			|> V.map (from_i64)
			|> V.map2 (over) ranges
		let stepsTaken = V.map2 (times) pid_byDim step_byDim
		let part_min = V.map2 (plus) mins stepsTaken
		let part_max = V.map2 (plus) part_min step_byDim
		in (part_min, part_max)

	def index_dataset idxSpec xs =
		let np = idxSpec |> reduce (*) 1
		let n = length xs
		let idx_vec = V.from_array (idxSpec |> sized V.length)
		let dimPrefix = idxSpec |> exscan (*) 1
			|> sized V.length |> V.from_array
		let (mins, ranges) = xs |> get_mins_ranges
		let pids = xs |> map (get_partition_id mins ranges idx_vec dimPrefix)
		let (pids', xs') = xs |> bucket_sort 2 np pids
		let firstByPid = hist (i64.+) 0i64 np (pids' |> sized n) (replicate n 1i64)
			|> exscan (+) 0
		let partBounds = iota np
			|> map (get_partitionBoundaries mins ranges idx_vec dimPrefix)
		in (xs', partBounds, firstByPid)

	def get_adj_partitions partitions i =
		let (this_mins, this_maxs) = partitions[i]
		let touch = indices partitions
			|> map2 (\(cmins,cmaxs) j -> j!=i
				&& (this_maxs |> V.map2 (leq) cmins |> V.reduce (&&) true)
				&& (cmaxs |> V.map2 (leq) this_mins |> V.reduce (&&) true)
			) partitions
		in touch
			|> zip (indices partitions)
			|> filter (.1)
			|> map (.0)

	def fetch_partition partIs xs i =
		let inf = partIs[i]
		let sup = if i==(length partIs)-1 then (length xs) else partIs[i+1]
		in xs[inf:sup]
}