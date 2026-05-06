import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/segmented/segmented"
import "skyline_base"
import "skyline_subdiv"

-- | Cell-based grid for SkyCell algorithm.
--
-- Cell-partitioning is applied recursively in levels.
-- At each level, the space is subdivided in 2 across all dimensions.
-- All partitions are examined for partial & complete domination,
-- and points of completely dominated partitions are filtered out.
-- Until the maximum number of points per partition reaches a given threshold.
module skycell_grid
	(V : vector)
	(N : numeric)
= {
	type t = N.t
	type vector 'a = V.vector a
	type tup = vector t

	local def minus = (N.-)
	local def over  = (N./)

	local def from_i64 = N.i64
	local def to_i64   = N.to_i64

	-- | Calculate the subdivisions & subdivision prefixes of the current level.
	-- Returned as vectors.
	def get_subdiv_vectors (level : i64) =
		let subdiv_v = V.replicate (2**level)
		let prefix_v = V.iota |> V.map (\d -> 2**(level*d))
		in (subdiv_v, prefix_v)

	-- | Given a vector of the cell distances from the pivot in each dimension,
	-- calculate a numerical cell id (cid).
	def vector_to_cid
		(prefix_v : vector i64)
		(vec : vector i64)
	: i64 = V.map2 (*) vec prefix_v
		|> V.reduce (+) 0

	-- | Given a numerical cid, return vector of cell distances.
	def cid_to_vector
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(cid : i64)
	: vector i64 = prefix_v
		|> V.map (\pref -> cid / pref)
		|> V.map2 (\sdv cid_suffix -> cid_suffix%sdv) subdiv_v

	-- | Obtain the skyline pivot
	-- & the width of each per dimension.
	def get_cell_width
		(pivot : tup)
		(antipivot : tup)
		(subdiv_v : vector i64)
	: vector t = V.map2 (minus) antipivot pivot
		|> V.map2 (\sdv range -> range `over` (from_i64 sdv)) subdiv_v

	-- | Obtain ALL cells dominated by c,
	-- and whether they're completely dominated.
	def get_all_dominated_cells
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(cid : i64)
	=
		let vec = cid
			|> cid_to_vector subdiv_v prefix_v
		let num_dominated_per_dim = V.map2 (-) subdiv_v vec
		let dom_prefix_per_dim = num_dominated_per_dim
			|> V.to_array
			|> exscan (*) 1
			|> V.from_array
		let num_dominated_total = V.reduce (*) 1 num_dominated_per_dim
		let dominated_cell_vecs = iota num_dominated_total
			|> map (cid_to_vector num_dominated_per_dim dom_prefix_per_dim)
			|> map (V.map2 (+) vec)
		let is_totally_dominated = dominated_cell_vecs
			|> map (V.map2 (<) vec)
			|> map (V.reduce (&&) true)
		let dominated_cids = dominated_cell_vecs
			|> map (\vec2 ->
				vector_to_cid prefix_v vec2
			)
		in zip dominated_cids is_totally_dominated

	-- | Obtain the adjacent cells from cid that it partially dominates,
	-- and if it totally dominates each of them.
	-- Invalid cells are returned as (-1).
	-- NOTE: cid must be either partially dominated or have at least 1 point in the dataset
	--
	-- Returns an array of (i64,i64,bool), where
	-- .0 is cid passed,
	-- .1 is cid dominated,
	-- .2 is if it's totally dominated
	def get_adjacent_dominated_cells
		(subdiv1_v : vector i64) -- returned by get_subdiv_vectors 1
		(prefix1_v : vector i64) -- returned by get_subdiv_vectors 1
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(cid : i64)
	=
		let vec = cid
			|> cid_to_vector subdiv_v prefix_v
		let dominated_cell_vecs = iota (2**V.length)
			|> map (cid_to_vector subdiv1_v prefix1_v)
			|> map (V.map2 (+) vec)
		let is_totally_dominated = dominated_cell_vecs
			|> map (V.map2 (<) vec)
			|> map (V.reduce (&&) true)
		let dominated_cids = dominated_cell_vecs
			|> map (\vec2 ->
				-- invalid id: exceeds subdiv_v in any dimension
				let is_invalid = vec2
					|> V.map2 (<=) subdiv_v
					|> V.reduce (||) false
				in if is_invalid then (-1)
				else vector_to_cid prefix_v vec2
			)
		in zip3
			(replicate (2**(V.length)) cid)
			dominated_cids
			is_totally_dominated

	-- | Get the cid of a particular point.
	def get_cid
		(pivot : vector t)
		(cell_widths : vector t)
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(pt : vector t)
	: i64 = V.map2 (minus) pt pivot
		|> V.map2 (\width dist -> dist `over` width) cell_widths
		|> V.map (to_i64)
		|> V.map2 (\sdv v -> i64.max (sdv-1) v) subdiv_v
		|> vector_to_cid prefix_v

	-- | Get the count of points per cid.
	def get_count_by_cid [n]
		(level : i64)
		(cids : [n]i64)
	= hist (+) 0 (2**(level*V.length)) cids (replicate n 1i64)

	-- | Find the closest cell to the start of the axes, wrt Euclidean distance.
	-- (Out of cells with count>0)
	def get_closest_cid
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(cids : []i64)
	= cids
		|> map (cid_to_vector subdiv_v prefix_v)
		|> map (V.map (\x -> x*x))
		|> map (V.reduce (+) 0)
		|> argmin (<) (==) (i64.highest)
		|> (\i -> cids[i])

	-- | Find the closest cell to the start of the axes, *on* each axis.
	-- (Out of cells with count>0)
	def get_closest_cid_per_dim
		(level : i64)
		(prefix_v : vector i64)
		(count_per_cid : [2**(level*V.length)]i64)
	= iota V.length
		|> map (\d -> zip (replicate (2**(level)) d) (iota (2**(level))))
		|> map (map (\(d,i) -> V.iota |> V.map (\j -> if j==d then i else 0)))
		|> map (map (vector_to_cid prefix_v))
		|> map (map (\cid -> if count_per_cid[cid]>0 then cid else (-1)))
		|> map (reduce (\c1 c2 -> if c1>=0 && (c2<0 || c2>c1) then c1 else c2) (-1))

	-- | Do entire filtering of cids for the current level.
	--
	-- Returns:
	-- 1. filtered points (based on their cell's domination status)
	-- 2. cells id's of each returned point
	-- 3. the count of returned points in each cell
	-- 4. whether or not each cell is partially dominated
	def filter_cells
		(pivot : tup)
		(antipivot : tup)
		(level : i64)
		(dat : skyData tup)
	=
		let cells_No = 2**(level*V.length)
		let (subdiv1_v, prefix1_v) = get_subdiv_vectors 1
		let (subdiv_v, prefix_v) = get_subdiv_vectors level
		let cell_widths = get_cell_width pivot antipivot subdiv_v
		-- Get the cid of every point
		let cids = dat |> map ((.0) >-> get_cid pivot cell_widths subdiv_v prefix_v)
		-- Get the count by cid, omit those with count==0
		let cid_counts = cids |> get_count_by_cid level
		let cids_present = cid_counts |> zip (indices cid_counts)
			|> filter (\(_,count) -> count>0) |> map (.0)
		-- Find partially & completely dominated cells by the closest
		let closest_cell = get_closest_cid subdiv_v prefix_v cids_present
		let (is_dominated_0,is_totally_dominated_0) =
			let domd_by_closest = closest_cell
				|> get_all_dominated_cells subdiv_v prefix_v
			let is_domd = scatter
				(replicate cells_No false)
				(domd_by_closest |> map (.0))
				(domd_by_closest |> map (\_ -> true))
			let is_totally_domd = scatter
				(replicate cells_No false)
				(domd_by_closest |> map (.0))
				(domd_by_closest |> map (.1))
			in (is_domd, is_totally_domd)
		-- If closest_cell != 0
		-- find the closest cell on each axis as well
		-- and use those to dominate
		let closest_per_dim = if closest_cell!=0
			then get_closest_cid_per_dim level prefix_v cid_counts
			else (replicate V.length (-1))
		let (is_dominated_1, is_totally_dominated_1)
		= loop (is_domd1,is_totally_domd1)
		= (is_dominated_0, is_totally_dominated_0)
		for j<V.length do
			let cur = closest_per_dim[j]
			in if cur<0 then (is_domd1,is_totally_domd1) else
			let domd_by_closest = cur
				|> get_all_dominated_cells subdiv_v prefix_v
			let is_domd = scatter
				(replicate cells_No false)
				(domd_by_closest |> map (.0))
				(domd_by_closest |> map (\_ -> true))
			let is_totally_domd = scatter
				(replicate cells_No false)
				(domd_by_closest |> map (.0))
				(domd_by_closest |> map (.1))
			in (
				map2 (||) is_domd1 is_domd,
				map2 (||) is_totally_domd1 is_totally_domd
			)
		-- Expand all cids to the ones they dominate.
		-- Do this by progressing distance from 0
		-- until all has been explored.
		--
		-- Any cids that have been dominated so far will not dominate any new ones
		-- so they do not need to be examined
		let (is_dominated, is_totally_dominated,_)
		= loop (is_domd,is_totally_domd,cur_dist)
		= (is_dominated_1, is_totally_dominated_1,0)
		while cur_dist < 2**(level) do
			-- get cells of cur_dist
			-- only if they have any elements or are partially dominated
			let cur_cells = iota cells_No
				|> filter (\cid -> !is_dominated_1[cid] && (cid_counts[cid]>0 || is_domd[cid]))
				|> map (cid_to_vector subdiv_v prefix_v)
				|> filter ((V.reduce (i64.max) (-1)) >-> (\d -> d==cur_dist))
				|> map (vector_to_cid prefix_v)
			-- get their dominations
			let (cur_domd,cur_is_totally_domd) = cur_cells
				|> map (get_adjacent_dominated_cells subdiv1_v prefix1_v subdiv_v prefix_v)
				-- if a cell is totally dominated
				-- then the ones it partially dominates will also be totally dominated
				|> map (map (\(cid1,cid2,is_tot) ->
					if is_totally_domd[cid1] then (cid2,true) else (cid2,is_tot)
				)) |> flatten |> unzip
			-- use reduce_by_index to 'scatter'
			let is_domd' = reduce_by_index
				(copy is_domd) (||) false cur_domd (cur_domd |> map (\_ -> true))
			let is_totally_domd' = reduce_by_index
				(copy is_totally_domd) (||) false cur_domd cur_is_totally_domd
			-- if there were no cells at the current dist
			-- then locate the next one that has valid cells
			let next_dist = if (length cur_cells)>0 then cur_dist+1
				else iota cells_No
				|> filter (\cid -> !is_dominated_1[cid] && (cid_counts[cid]>0 || is_domd'[cid]))
				|> map (cid_to_vector subdiv_v prefix_v)
				|> map (V.reduce (i64.max) (-1))
				|> filter (\d -> d>cur_dist)
				|> i64.minimum
			in (is_domd', is_totally_domd', next_dist)
		-- filter out points that have totally dominated cids
		let (dat',cids') = zip dat cids
			|> filter (\(_,cid) -> !is_totally_dominated[cid])
			|> unzip
		let cid_counts' = zip (indices cid_counts) cid_counts
			|> map (\(cid,ccount) -> if is_totally_dominated[cid] then 0 else ccount)
		in (dat',cids',cid_counts',is_dominated)

	-- | Get the number of cells that partially dominate cid (including cid itself).
	-- Used as sz function for expand operator.
	def num_dominant_cells
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(cid : i64)
	= (cid |> cid_to_vector subdiv_v prefix_v |> V.map (+ 1) |> V.reduce (*) 1)
	--	-- TODO
	--	-- subtract those that would totally dominate it
	--	-- then I have to modify the get function as well
	--	- (cid |> cid_to_vector subdiv_v prefix_v |> V.reduce (*) 1)

	-- Get the k'th cell that partially dominates this cid.
	-- Used as get function for expand operator.
	def get_kth_dominant_cell
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(k : i64)
		(cid : i64)
	=
		-- TODO modify to only expand to dominant area's hull (ignore totally dominant cells):
		-- create dim hyper-rectangles adjacent to cid
		-- the d'th hyper-rectangle has 1 subdiv at d, subdiv[d]-1 at (d+1)%dim, otherwise same as dominant area
		-- (this way there is no overlap and cid itself is the only one excluded)
		-- to get the k'th cell, do a sequential traversal of these rectangles
		-- if k > sum of the rectangles' cells, then it gets cid itself
		let cidwise_subdiv = cid |> cid_to_vector subdiv_v prefix_v
			|> V.map (+ 1)
		let cidwise_prefix = cidwise_subdiv |> V.to_array
			|> exscan (*) 1 |> V.from_array
		in k |> cid_to_vector cidwise_subdiv cidwise_prefix
			|> vector_to_cid prefix_v

	-- | For a set of cids, expand them to cells that partially dominate them.
	-- Filter out non-present.
	--
	-- Returns:
	-- 1. dominance pairs (cid1,cid2), where cid2 dominates c1
	-- 2. dom_pairs_sz : for cid1, the number of present cells that dominate it
	-- 3. dom pairs is : for cid1, the first index in dominance pairs corresponding to it
	def get_dominant_cells
		(subdiv_v : vector i64)
		(prefix_v : vector i64)
		(count_per_cid : []i64) -- sized (1+max_cid)
		(cids : []i64) -- cids that are present & partially dominated
	= 
		let domd_pairs = cids
			|> expand (num_dominant_cells subdiv_v prefix_v)
				(\cid k -> (cid,get_kth_dominant_cell subdiv_v prefix_v k cid))
			|> filter (\(_,domd_by) -> count_per_cid[domd_by]>0)
		let domd_pairs_sz = hist (+) 0 (length count_per_cid)
			(domd_pairs |> map (.0))
			(domd_pairs |> map (\_ -> 1i64))
		let domd_pairs_is = domd_pairs_sz
			|> exscan (+) 0
		in (domd_pairs, domd_pairs_sz, domd_pairs_is)

}

-- | Skyline Implementation based on SkyCell state-of-the-art GPU-based Skyline algorithm.
module skycell
	(V : vector)
	(N : numeric)
	(S  : skyline_base with t = N.t with tup = V.vector N.t)
= {
	type t = N.t
	type vector 'a = V.vector a
	type tup = vector t

	module skyline = mk_skyline V N S
	module cells_proc = skycell_grid V N

	-- | Use skycell_grid module to recursively subdivide space into cells,
	-- and filter out cells that are dominated,
	-- until either the maximum number of points per cell falls below a given threshold,
	-- or a maximum number of iterations is reached.
	--
	-- Returns:
	-- 1. filtered points
	-- 2. cids of filtered points
	-- 3. count per cid
	-- 4. whether is cid is (partially) dominated
	-- 5. recursion level that was reached
	def skycell_preproc
		(max_level : i64)
		(threshold_count : i64)
		(dat : skyData tup)
	= loop (pts,cids,cid_counts,is_cid_domd,j)
		= (dat,dat |> map (\_ -> 0i64),[0i64],[true],0)
		while (i64.maximum cid_counts)>threshold_count && j<max_level do
			let (pivot,antipivot) = skyline.get_minmax pts
			let (pts',cids',cid_counts',is_cid_domd') = pts
				|> cells_proc.filter_cells pivot antipivot (j+1)
			in (pts',cids',cid_counts',is_cid_domd',j+1)

	-- | Sort remaining points by cid,
	-- using temporary dictionary encoding for more efficient radix-sorting.
	def order_by_cid
		(count_per_cid : []i64)
		(cids : []i64)
		(dat : skyData tup)
	: ([]i64, skyData tup) =
		let encode_cids_by = count_per_cid
			|> map (\ccount -> (ccount > 0))
			|> dict_encoding
		let encoded_cids = cids |> map (\cid -> encode_cids_by[cid])
		let num_buckets = 1 + (last encode_cids_by)
		in indices cids
			|> bucket_sort 2 num_buckets encoded_cids
			|> (.1)
			|> map (\i -> (cids[i],dat[i]))
			|> unzip

	-- | For each cid,
	-- get the numberof comparisons its points will make for the final stage of Skycell.
	def get_dominated_cell_info
		(count_per_cid : []i64)
		(cid_dom_pairs : [](i64,i64))
		(cid_dom_sz : []i64)
		(cid_dom_is : []i64)
	= indices count_per_cid
		|> expand_outer_reduce
			(\cid -> if count_per_cid[cid]==0 then 0 else cid_dom_sz[cid])
			(\cid ind -> count_per_cid[(cid_dom_pairs[cid_dom_is[cid]+ind]).1])
			(+) 0

	-- | SkyCell final pointwise filtering.
	def pointwise_filter [n]
		(count_per_cid : []i64)
		(cid_dom_pairs : [](i64,i64))
		(cid_dom_sz : []i64)
		(cid_dom_is : []i64)
		(cids : [n]i64)
		(dat  : skyData tup)
	=
		let (cids',dat') = order_by_cid count_per_cid cids dat
		let cid_is = count_per_cid
			|> exscan (+) 0
		let cid_cmp_No = get_dominated_cell_info
			count_per_cid
			cid_dom_pairs
			cid_dom_sz
			cid_dom_is
		-- Prefix size of comparisons across cell pairs
		let cid_dom_pair_prefix_sz = cid_dom_pairs
			|> unzip |> (\(cid1,cid2) -> (
				cid1 |> group_boundaries (!=),
				cid2 |> map (\i -> count_per_cid[i])			
			))
			|> (\(flags,count2) -> segmented_scan (+) 0 flags count2
				-- convert to segmented excan
				|> map2 (\original cumulative -> cumulative - original) count2
			)
		let (cmps_is,cmps_res) = zip cids' (indices cids')
			|> expand
				(\(cid,_) -> cid_cmp_No[cid])
				(\(cid,i) ind -> (cid,i,ind))
			-- binary search to find current cmp
			|> unzip3
			|> (\(cids,is,inds) ->
				-- NOTE
				-- this usage of binary search is quite powerful
				-- prob need to mention it somewhere
				let parts_against_is = bsearch_last (>=) (<)
					(cids |> map (\cid -> cid_dom_is[cid]))
					(cids |> map (\cid -> cid_dom_is[cid] + cid_dom_sz[cid]))
					cid_dom_pair_prefix_sz
					inds
				let parts_against = parts_against_is |> map (\i -> (cid_dom_pairs[i]).1)
				let inds' = map2
					(\ind i2 -> ind - cid_dom_pair_prefix_sz[i2])
					inds
					parts_against_is
				in (
					is,
					map2 (+) (parts_against |> map (\cid -> cid_is[cid])) inds'
				)
			)
			|> (\(is,inds) -> zip is inds)
			|> map (\(i,ind) ->
				let pt2 = dat'[i]
				let pt1 = dat'[ind]
				in (i, S.pt_dominated pt1.0 pt2.0)
			) |> unzip
		let is_pt_eliminated = hist (||) false n
			cmps_is cmps_res
		in indices dat'
			|> filter (\i -> !is_pt_eliminated[i])
			|> map (\i -> dat'[i])


}