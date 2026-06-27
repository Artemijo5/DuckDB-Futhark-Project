import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/sorts/merge_sort"

-- Implementatin for Cartesian Grid

-- | Abstract module type to implement spatial index structures.
-- Partitions are represented by min/max points.
module type spatial_index = {
	type t
	type vector 'a

	-- | Create the index using index_spec.
	-- Returns a tuple containing:
	-- 1. the dataset sorted by index-based partitions.
	-- 2. partition positions (eg in a grid)
	-- 3. starting index of each partition in the sorted dataset
	-- 4. numerical id of each partition (eg on grid index, encodes cell's position)
	-- 5. transformed row indices
	val index_dataset [dim] [n] : [dim]i64 -> [n](vector t)
		-> ([n](vector t), [](vector i64), []i64, []i64, [n]i64)
}

-- Regular grid subdivisions.
module grid_index (V : vector) (N : real)
: spatial_index with t = N.t with vector 'a = V.vector a = {
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

	local def floor = N.floor

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

	-- idxSpec : [V.length]i64, represents #subdivisions per dimension
	def index_dataset idxSpec xs =
		--let np = idxSpec |> reduce (*) 1
		let idx_vec = V.from_array (idxSpec |> sized V.length)
		let dimPrefix = idxSpec |> exscan (*) 1
			|> sized V.length |> V.from_array
		let (mins, ranges) = xs |> get_mins_ranges
		let cell_widths = V.map2 (over)
			ranges
			(V.map (from_i64) idx_vec)
		let vecs = xs
			|> map (\x -> V.map2 (over) (V.map2 (minus) x mins) cell_widths)
			|> map (V.map (floor >-> to_i64))
		let pids = xs |> map (get_partition_id mins ranges idx_vec dimPrefix)
		-- Use vecs to sort cells
		-- originally was using pids
		-- but those might result in overflow for high dimensionality
		-- (still useful to calculate for low dimensionality datasets)
		let (is',vecs')
			= loop (is1,vecs1)
			= (indices xs,vecs)
			for j<V.length do
				zip is1 vecs1
				|> bucket_sort 2 (1+idxSpec[j]) (vecs1 |> map (V.get j))
				|> (.1)
				|> unzip
		let pids' = is' |> map (\i -> pids[i])
		let xs' = is' |> map (\i -> xs[i])
		let pids_is = vecs'
			|> group_boundaries (\vec1 vec2 -> V.map2 (!=) vec1 vec2 |> V.reduce (||) false)
			|> zip (indices vecs')
			|> filter (.1) |> map (.0)
		let pids_ids = pids_is |> map (\i -> pids'[i])
		let as_vectors = pids_is |> map (\i -> vecs'[i])
		in (xs', as_vectors, pids_is, pids_ids, is')
}

