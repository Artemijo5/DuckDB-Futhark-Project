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
	type partition

	type index_spec

	-- | Create the index using index_spec.
	-- Returns a tuple containing:
	-- 1. the dataset sorted by index-based partitions.
	-- 2. partition boundaries
	-- 3. starting index of each partition in the sorted dataset
	val index_dataset [n] : index_spec -> [n]t -> ([n]t, []partition, []i64)

	-- | Obtain all partitions neihbouring with a selected partition.
	-- Returns the indices of all neighbouring partitions (not including itself).
	val get_neighbouring_partitions [np] : [np]partition -> i64 -> []i64
}

module mk_gridIndex (V : vector) (N : numeric) : mk_spatial_index = {
	type t = N.t
	type vector 'a = V.vector a
	type partition = [2](vector t)

	type index_spec = [V.length]i64

	local def over = (N./)
	local def times= (N.*)
	local def minus= (N.-)

	local def to_i64 = (N.to_i64)
	local def from_i64 = (N.i64)

	local def minimum = N.minimum
	local def maximum = N.maximum

	local def get_mins_ranges (xs : [](vector t)) : (vector t, vector t) =
		let perDim = V.iota |> V.map (\i -> xs |> map (V.get i))
		let mins = perDim |> V.map (minimum)
		let maxs = perDim |> V.map (maximum)
		let ranges = V.map2 (minus) maxs mins
		in (mins, ranges)

	def get_partition_id
		(mins : vector t)
		(ranges : vector t)
		(idx : index_spec)
		(x : vector t)
	: i64 =
		let idx_vec = V.from_array idx
		let dimPrefix = idx |> exscan (*) 1
			|> V.from_array
		in x
			|> V.map2 (\mi xi -> xi `minus` mi) mins
			|> V.map2 (\pD xi -> xi `times` (from_i64 pD)) idx_vec
			|> V.map2 (\rg xi -> xi `over` rg) ranges
			|> V.map (to_i64)
			|> V.map2 (i64.min) idx_vec
			|> V.map2 (*) dimPrefix
			|> V.reduce (+) 0

	-- TODO finish
	def index_dataset idxSpec xs =
		let np = idxSpec |> scan (*) 1
		let (mins, ranges) = xs |> get_mins_ranges
		let pids = xs |> map (get_partition_id mins ranges idxSpec)
		let (pids', xs') = xs |> bucket_sort 2 np pids
		let perDim = idxSpec |> scan (*) 1
			|> V.from_array
		let partitionBoundaries = iota np
			|> map (\i -> dimPrefix |> V.map (\j -> i%j))
			|> map (\is -> is |> V.map (from_i64) |> V.map2 (times) )
		let partIdx = pids' |> group_boundaries (!=) |> zip (indices pids')
			|> filter (.1) |> map (.0)

	def get_neighbouring_partitions partitions i = []
}