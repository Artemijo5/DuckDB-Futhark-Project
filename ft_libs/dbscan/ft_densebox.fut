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
		-- and stays there after partitioning
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

	def get_dense_box_neighbourhoods [np]
		(subdiv : [V.length]i64)
		(cell_ids : [np]i64)
		(part_sz : [np]i64)
		(is_box_dense : [np]i64)
	-- : ([](i64,i64,bool),[np]i64,[np]i64) =
		-- TODO calculate
		-- part_pairs (this_cell, neigh_cell, is_inner_neighbour)
		-- part_pairs_is
		-- part_pairs_sz
	--	let adj_range = V.length |> f64.sqrt
	--		|> f64.ceil |> i64.f64
	--	let ajd_range_inner = V.length |> f64.sqrt
	--		|> f64.floor |> i64.f64
	= 0



}