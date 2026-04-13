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
def get_connected_subgraph_ids_unencoded
	(k : i64)
	(pairs : [](i64,i64))
: [k]i64 =
	let (mins,maxs) = unzip pairs
	let (_,g_ids) =
	-- In each iteration, node k asks itself & its neihbours
	-- for the minimum-indexed node they currently 'see',
	-- until convergence.
	-- Worst case O(N), if the entire graph is a line with connections in monotonous order.
	loop (old_mins, new_mins) = (replicate k (-1), iota k)
	while (any (id) (map2 (!=) old_mins new_mins)) do
		let mins_from_mins = mins |> map (\i -> new_mins[i])
		let mins_from_maxs = maxs |> map (\i -> new_mins[i])
		let pivots_from_maxs = hist (i64.min) (i64.highest) k mins mins_from_maxs
		let pivots_from_mins = hist (i64.min) (i64.highest) k maxs mins_from_mins
		let pivots = map2 (i64.min) pivots_from_mins pivots_from_maxs
		in (new_mins, pivots)
	in g_ids

import "../ftbasics"

-- | Assign the same id to points belonging to the same connected subgraph.
-- The undirected graph is represented as an array of unique index pairs (i1,i2), i1<=i2.
-- Id's returned are dictionary-encoded.
--
-- The algorithm used is a parallel Breadth-First Traversal.
--
-- Parameters:
-- k : number of nodes (== max node id + 1)
-- pairs : undirected graph
-- Returns:
-- a k-sized array with the subgraph id of each node
def get_connected_subgraph_ids
	(k : i64)
	(pairs : [](i64,i64))
: [k]i64 =
	let ids = get_connected_subgraph_ids_unencoded k pairs
	-- The pts where i == id are the pivots.
	let is_pivot = ids |> map2 (\i g_id -> i==g_id) (iota k)
	let group_encoding = is_pivot |> dict_encoding
	in ids |> map (\i -> group_encoding[i])
