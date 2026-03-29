import "../ftbasics"
import "../lib/github.com/athas/vector/vector"
import "../lib/github.com/diku-dk/sorts/merge_sort"
import "../lib/github.com/diku-dk/segmented/segmented"
import "ft_spindex"
import "ft_distance"

type~ flushed_t [dim] 't = {
	n : i64,
	pts : [dim][]t,
	isCore : []bool,
	chain_id : []i64
}

module ft_dbscan
	(V : vector)
	(F : real)
	(I : spatial_index with vector 'a = V.vector a with t = F.t)
	(D : distance with vector 'a = V.vector a with t = F.t)
= {
	type t = F.t
	type vector 'a = V.vector a

	type~ dbc_partition = {
		minmax : (vector t, vector t),
		pts : [](vector t),
		isCore : []bool,
		isMargin : []bool,
		isTightMargin : []bool,
		chain_id : []i64
	}

	type~ dbc_state [part_no] = {
		cur_part_id : i64,
		num_visited : i64,
		is_part_visited : [part_no]bool,
		relevant_parts : []i64,
		offs : i64,
		next_offs : i64,
		cid_offs : i64
	}

	type~ dbc_buffer = {
		pts : [](vector t),
		isCore : []bool,
		chain_id : []i64
	}

	type~ flushed_vectors = flushed_t [1] (vector t)
	type~ flushed_data = flushed_t [V.length] t

	def init_dbc_buffer : dbc_buffer = {pts=[],isCore=[],chain_id=[]}

	def init_dbc_state (part_no : i64) : dbc_state [part_no] = {
		cur_part_id = -1,
		num_visited = 0,
		is_part_visited = replicate part_no false,
		relevant_parts = [],
		offs = 0,
		next_offs = 0,
		cid_offs = 0
	}

	def next_partition [part_no] (state : dbc_state [part_no]) : i64 =
		if length state.relevant_parts > 0
		then state.relevant_parts |> head
		else state.is_part_visited
			|> argmin (\iv1 iv2 -> iv2 && !iv1) (==) (true)

	def read_next_partition [part_no]
		(parts_minmax : [part_no](vector t, vector t))
		(parts_is : [part_no]i64)
		(pts : [](vector t))
		(state : dbc_state [part_no])
	: (dbc_state [part_no], dbc_partition) = 
		let next_pid = next_partition state
		let next_pts = I.fetch_partition parts_is pts next_pid
		let n = length next_pts
		let next_part : dbc_partition = {
			minmax = parts_minmax[next_pid],
			pts = next_pts,
			isCore = replicate n false, -- TODO
			isMargin = replicate n false, -- TODO
			isTightMargin = replicate n false, -- TODO
			chain_id = replicate n (-1)
		}
		let next_state = state -- TODO
		in (next_state, next_part)
		-- TODO update state relevant parts, get neighbours of next_part, ...
		-- also mark isMargin, isTightMargin, use those to find neihbouring partitions...

}