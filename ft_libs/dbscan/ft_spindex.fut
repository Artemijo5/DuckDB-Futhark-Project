import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/sorts/merge_sort"
import "../lib/github.com/diku-dk/segmented/segmented"

-- Implementation of basic (min/max) spatial index structures.
-- 1. Uniform Grid Partitioning
-- 2. Array Index based on kd-Tree
-- The indices subdivide space into non-overlapping min/max rectangles.
-- Partitions are defined by min & max values for each dimension.

-- | Abstract module type to implement spatial index structures.
-- Partitions are represented by min/max points.
module type spatial_index = {
	type t
	type vector 'a

	-- | Create the index using index_spec.
	-- Returns a tuple containing:
	-- 1. the dataset sorted by index-based partitions.
	-- 2. partition boundaries
	-- 3. starting index of each partition in the sorted dataset
	-- 4. transformed row indices
	val index_dataset [dim] [n] : [dim]i64 -> [n](vector t)
		-> ([n](vector t), [](vector t, vector t), []i64, [n]i64)

	-- | Obtain all partitions adjacent to a selected partition.
	-- t parameter serves as max eps-distance for adjacency (can be zero for true adjacency).
	-- i64 parameter is the index of the selected partition in partitions.
	-- Returns the indices of all adjacent partitions (not including itself).
	val get_adj_partitions [np] : [np](vector t, vector t) -> t -> i64 -> []i64

	-- | Obtain the indices of all points of a selected partition.
	val fetch_partition [np] [n] : [np]i64 -> [n](vector t) -> i64 -> []i64
}

-- Regular grid subdivisions.
module grid_index (V : vector) (N : numeric)
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
		let np = idxSpec |> reduce (*) 1
		let n = length xs
		let idx_vec = V.from_array (idxSpec |> sized V.length)
		let dimPrefix = idxSpec |> exscan (*) 1
			|> sized V.length |> V.from_array
		let (mins, ranges) = xs |> get_mins_ranges
		let pids = xs |> map (get_partition_id mins ranges idx_vec dimPrefix)
		let (pids', xs', is') = xs |> indices |> zip xs
			|> bucket_sort 2 np pids
			|> (\(ps, xis) -> let (xis1,xis2) = unzip xis in (ps,xis1,xis2))
		let firstByPid = hist (i64.+) 0i64 np (pids' |> sized n) (replicate n 1i64)
			|> exscan (+) 0
		let partBounds = iota np
			|> map (get_partitionBoundaries mins ranges idx_vec dimPrefix)
		in (xs', partBounds, firstByPid, is')

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

	def fetch_partition partIs xs i =
		let inf = partIs[i]
		let sup = if i==(length partIs)-1 then (length xs) else partIs[i+1]
		in (inf..<sup)
}

-- min/max kd-tree -based index.
-- (essentially holds only the last level of the kd-tree as an array)
module kd_index (V : vector) (N : numeric)
: spatial_index with t = N.t with vector 'a = V.vector a = {
	type t = N.t
	type vector 'a = V.vector a

	local def minus = (N.-)
	local def plus  = (N.+)

	local def leq = (N.<=)
	local def lt  = (N.<)

	local def from_i64 = (N.i64)

	local def zero = from_i64 0i64
	local def lowest = N.lowest
	local def highest = N.highest

	local def min = N.min
	local def max = N.max
	local def minimum = N.minimum
	local def maximum = N.maximum

	local def get_mins_maxs (xs : [](vector t)) : (vector t, vector t) =
		let perDim = iota (V.length) |> map (\i -> xs |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		in (mins, maxs)

	-- idxSpec : [1]i64, represents levels of kd tree
	def index_dataset [n] idxSpec (xs : [n](vector t)) =
		-- kd_depth = min idxSpec (ceil log2 n)
		let kd_depth = n
			|> i64.clz
			|> (i32.-) (if n&(n-1)==0 then 64 else 65)
			|> i64.i32
			|> i64.min (head idxSpec)
		-- sort by median of the next dimension each level
		let (min_pt, max_pt) = xs |> get_mins_maxs
		let (fxs, fis, _, fmins, fmaxs, fsizes) =
			loop (pts, is, pids, part_mins, part_maxs, part_sizes)
			: ([n](vector t), [n]i64, [n]i64, [](vector t), [](vector t), []i64)
			= (xs, iota n, replicate n 0, [min_pt], [max_pt], [n])
			for j < kd_depth do
				let curDim = j % V.length
				let (old_pids, pts', is') = zip3 pids pts is
					|> merge_sort_by_key
						(\(pid,x,_) -> (pid, V.get curDim x))
						(\(pid1,x1) (pid2,x2) -> pid1<pid2 ||
							(pid1==pid2 && (x1 `leq` x2))
						)
					|> unzip3
				let part_is = part_sizes |> exscan (+) 0
				let median_vals = part_is
					|> map2 (\pSize pInd ->
						pInd + pSize/2
					) part_sizes
					|> map (\i -> V.get curDim (pts'[i64.min i (n-1)]))
				let new_pids = pts'
					|> zip old_pids
					|> map (\(pid,x) -> if ((V.get curDim x) `lt` median_vals[pid]) then 2*pid else 2*pid+1)
				let new_minVals = hist (min) highest (2*(length part_sizes)) new_pids (pts' |> map (V.get curDim))
				let new_part_mins = indices part_mins |> expand (\_ -> 2)
					(\i ind -> V.set curDim new_minVals[2*i + ind] part_mins[i])
				let new_maxVals = hist (max) lowest  (2*(length part_sizes)) new_pids (pts' |> map (V.get curDim))
				let new_part_maxs = indices part_maxs |> expand (\_ -> 2)
					(\i ind -> V.set curDim new_maxVals[2*i + ind] part_maxs[i])
				let new_part_sizes = hist (+) 0 (2*(length part_sizes))
					new_pids
					(replicate n 1)
			in (pts', is', new_pids, new_part_mins, new_part_maxs, new_part_sizes)
		-- filter out empty partitions
		let np = length fmins
		let (fmins', fmaxs', fsizes') = zip3
			(fmins |> sized np)
			(fmaxs |> sized np)
			(fsizes |> sized np)
			|> filter (\(_,_,sz) -> sz>0)
			|> unzip3
		let np' = length fmins'
		let fparts = zip (fmins' |> sized np') (fmaxs' |> sized np')
		in (fxs, fparts, fsizes' |> exscan (+) 0, fis)

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

	def fetch_partition partIs xs i =
		let inf = partIs[i]
		let sup = if i==(length partIs)-1 then (length xs) else partIs[i+1]
		in (inf..<sup)
}

-- Don't subdivide space.
module non_index (V : vector) (N : numeric)
: spatial_index with t = N.t with vector 'a = V.vector a = {
	type t = N.t
	type vector 'a = V.vector a

	local def minus = (N.-)
	local def plus  = (N.+)

	local def leq = (N.<=)
	local def lt  = (N.<)

	local def from_i64 = (N.i64)

	local def zero = from_i64 0i64

	local def minimum = N.minimum
	local def maximum = N.maximum

	local def get_mins_maxs (xs : [](vector t)) : (vector t, vector t) =
		let perDim = iota (V.length) |> map (\i -> xs |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		in (mins, maxs)

	-- idxSpec : [1]i64, represents levels of kd tree
	def index_dataset _ xs =
		(xs, [get_mins_maxs xs], [0i64], indices xs)

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

	def fetch_partition _ xs _ =
		indices xs
}