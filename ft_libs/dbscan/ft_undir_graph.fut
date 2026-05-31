-- BFS is performed through a series of successive reduce_by_index calls.
-- In each iteration, each point looks for the smallest index any of its neighbours have found,
-- until convergence.

import "../ftbasics"

-- | Assign the same id to points belonging to the same connected subgraph.
-- The undirected graph is represented as an array of unique index pairs (i1,i2), i1<=i2.
-- A subraph's id is the smallest index of its elements.
--
-- The algorithm used is a parallel Breadth-First Traversal.
--
-- Parameters:
-- k : number of nodes (== max node id + 1)
-- pairs : undirected graph
-- Returns:
-- a k-sized array with the subgraph id of each node
def get_connected_subgraph_ids [n]
	(k : i64)
	(pairs : [n](i64,i64))
: [k]i64 =
	let (mins,maxs) = unzip pairs
	let (_,g_ids) =
	-- In each iteration, node k asks itself & its neihbours
	-- for the minimum-indexed node they currently 'see',
	-- and send that node to their current cluster pivots,
	-- until convergence.
	--
	-- Worst case O(k) span, O(k^2) work
	-- (assuming reduce_by_inddex has O(1) span, O(k) work)
	-- if the entire graph is a line with only 2 edges per node excluding the extremes.
	--
	-- In general, span = O(length of the largest path to the min node in any connected subgraph).
	loop (old_mins, new_mins) = (replicate k (-1), iota k)
	while any (id) (map2 (!=) old_mins new_mins) do
		let mins_from_mins = mins |> map (\i -> new_mins[i])
		let mins_from_maxs = maxs |> map (\i -> new_mins[i])
		let pivots_from_maxs = hist_lean (i64.min) i64.highest k
			mins_from_mins mins_from_maxs
		let pivots_from_mins = hist_lean (i64.min) i64.highest k
			mins_from_maxs mins_from_mins
		let pivots_combined = map3 (\v1 v2 v3 -> i64.min v1 (i64.min v2 v3))
			new_mins pivots_from_mins pivots_from_maxs
		-- 'Shortcut' inner loop to eliminate monotone paths
		-- will result in less histogram calls overall...
		let (_,pivots_final)
		 = loop (old_inner, new_inner) = (iota k, pivots_combined)
		while any (id) (map2 (!=) old_inner new_inner) do
			(new_inner, new_inner |> map (\i -> new_inner[i]))
		in (new_mins, pivots_final)
	in g_ids

-- | Apply dictionary encoding to subgraph id's.
def encode_subgraph_ids [n] (sg_ids : [n]i64) : [n]i64 =
	let num_ids = 1 + (i64.maximum sg_ids)
	let present_ids = scatter (replicate num_ids false) sg_ids (replicate n true)
		|> dict_encoding
	in sg_ids |> map (\i -> if i<0 then i else present_ids[i])
