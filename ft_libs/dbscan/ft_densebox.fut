-- Implementation taking ideas from FDBSCAN_denseBox.

import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../merge_path"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"
import "ft_undir_graph"

-- Wrapper for expand_outer_reduce
-- handling the case of 1 point.
local def expand_outer_red [n] 't
	sz get op ne (xs : [n]t)
= if n>1
	then xs |> expand_outer_reduce sz get op ne
	else xs |> expand (sz >-> (i64.max 1))
		(\x ind -> if (sz x)==0 then ne else get x ind)
		|> reduce op ne
		|> replicate n

module ft_densebox
	(V : vector)
	(F : real)
	(D : distance with vector 'a = V.vector a with t = F.t)
= {
	type t = F.t
	type vector 'a = V.vector a

	module I = grid_index V F

	local def zero = F.i32 0i32

	local def over  = (F./)
	local def times = (F.*)
	local def minus = (F.-)
	local def plus  = (F.+)
	local def sqrt = (F.sqrt)

	local def leq = (F.<=)

	local def to_i64 = F.to_i64
	local def from_i64 = F.i64

	local def ceil = F.ceil

	local def min = F.min
	local def minimum = F.minimum
	local def maximum = F.maximum

	-- | Partition dataset with a regular grid using given subdivisions per dimension,
	-- also ensuring that in no dimension is the grid thinner than eps.
	def partition_dataset [n]
		(eps : t)
		(pts : [](vector t))
	=
		-- get min & max values per dimension
		let cell_width = eps `over` (V.length |> from_i64 |> sqrt)
		let perDim = iota (V.length) |> map (\i -> pts |> map (V.get i))
		let mins = perDim |> seqmap zero (minimum) |> V.from_array
		let maxs = perDim |> seqmap zero (maximum) |> V.from_array
		let ranges = V.map2 (minus) maxs mins
		let sdv = V.map (\r -> r `over` cell_width) ranges
		let sdv' = sdv
			|> V.map (ceil >-> to_i64)
			|> V.map2 (i64.max) (V.replicate 1i64)
			|> V.to_array
		-- Auxilliary point so that cell_width is correct
		-- is appended to the end of the dataset
		-- and is removed after partitioning
		let maxs' = V.map2 (\mi sub ->
			mi `plus` (sub `times` cell_width)
		) mins sdv
		let (pts', bounds, part_is, cell_ids, og_is) = I.index_dataset sdv' (pts ++ [maxs'])
		let np = length part_is
		in (sdv', pts'[0:n],
			bounds |> sized np, part_is |> sized np, cell_ids |> sized np,
			og_is[0:n]
		)

	def get_part_info [n] [np]
		(minPts : i64)
		(part_is : [np]i64)
		(_ : [n](vector t))
	: ([np]i64, [np]bool, [n]i64) =
		let part_sz = iota np
			|> map (\i -> if i<np-1 then part_is[i+1] else n)
			|> map2 (\cur_i next_i -> next_i-cur_i) part_is
		let isDense = part_sz |> map (>=minPts)
		let part_ids = scatter (replicate n (-1)) part_is (iota np)
			|> scan (i64.max) (-1)
		in (part_sz, isDense, part_ids)

	local def get_most_populated_dim [np]
		(cell_ids : [np]i64)
		(subdiv : [V.length]i64)
	: i64 =
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		let as_vectors = cell_ids |> map (\cur_pid ->
			prefix_v
				|> V.map (\pref -> cur_pid / pref)
				|> V.map2 (\sdv pid_suffix -> pid_suffix%sdv) subdiv_v
		)
		let distinct_per_dim = replicate V.length 0
			|> seqmap 0 (\dim ->
				let d_vals = as_vectors
					|> map (V.get dim)
				let d_min = i64.minimum d_vals
				let d_vals' = d_vals |> map (\d_v -> d_v-d_min)
				let d_max' = i64.maximum d_vals'
				let num_vals = scatter (replicate (d_max' + 1) false)
					d_vals' (replicate np true)
					|> countFor (id)
				in num_vals
			)
		in argmin (>) (==) 0 distinct_per_dim

	def get_box_neighbourhoods [np]
		(subdiv : [V.length]i64)
		(cell_ids : [np]i64)
		(part_sz : [np]i64)
	: ([](i64,i64),[np]i64,[np]i64) =
		let adj_range = V.length |> from_i64 |> sqrt
			|> ceil |> to_i64
		let most_popd_dim = get_most_populated_dim cell_ids subdiv
		let subdiv_v = subdiv |> V.from_array
		let prefix_v = subdiv |> exscan (*) 1 |> V.from_array
		let as_vectors = cell_ids |> map (\cur_pid ->
			prefix_v
				|> V.map (\pref -> cur_pid / pref)
				|> V.map2 (\sdv pid_suffix -> pid_suffix%sdv) subdiv_v
		)
		-- Sort by the most populated dim
		let xs = as_vectors |> map (V.get most_popd_dim)
		let num_buckets = 1 + (i64.maximum xs)	
		let (xs', is') = bucket_sort 2 num_buckets xs (iota np)
		-- 1st stage: SMJ xs' with xs'
		-- check for neighbourhood only in one dimension
		let x_matches = xs' |> bsearch_range
			(\x1 x2 -> i64.abs (x1-x2) <= adj_range)
			(\x1 x2 -> x2 - x1 <= adj_range)
			(\x1 x2 -> x1 - x2 > adj_range)
			(\x1 x2 -> x2 - x1 > adj_range)
			(replicate np 0)
			(replicate np np)
			xs'
			-- return to original ordering
			|> scatter (replicate np (-1,0)) is'
		let part_pairs = x_matches
			|> zip (iota np)
			|> expand (\(_,(_,cm)) -> cm)
				(\(i1,(fm,_)) ind -> (i1, fm + ind))
			|> map (\(i1,i2) -> (i1,is'[i2]))
			-- filter for true neighbourhood
			|> filter (\(i1,i2) ->
				i1==i2 ||
				let vec1 = as_vectors[i1]
				let vec2 = as_vectors[i2]
				in V.map2 (-) vec1 vec2
					|> V.map (\diff -> diff**2)
					|> V.reduce (+) 0
					|> (\meas -> meas <= adj_range**2)
			)
		let part_pairs_sz = hist_lean (+) 0 np
			(part_pairs |> map (.0))
			(part_pairs |> map (\_ -> 1i64))
		let part_pairs_is = part_pairs_sz
			|> exscan (+) 0
		in (part_pairs, part_pairs_is, part_pairs_sz)



}