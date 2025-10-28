import "ftbasics"

-- Abstract Grouped Aggregations

local def grouped_reduce [n] 't
	(group_no : idx_t.t)
	(group_by_idx : [n]idx_t.t)
	(f : t -> t -> t)
	(ne : t)
	(xs : [n]t)
: [group_no]t =
	hist f ne group_no group_by_idx xs

local def grouped_all [n] 't
	(group_no : idx_t.t)
	(group_by_idx : [n]idx_t.t)
	(f : t -> bool)
	(xs : [n]t)
: [group_no]bool =
	let fxs = map f xs
	in grouped_reduce group_no group_by_idx (&&) (true) fxs

local def grouped_any [n] 't
	(group_no : idx_t.t)
	(group_by_idx : [n]idx_t.t)
	(f : t -> bool)
	(xs : [n]t)
: [group_no]bool =
	let fxs = map f xs
	in grouped_reduce group_no group_by_idx (||) (false) fxs

-- Functions to Find Group Ranges

-- Good idea to filter this afterwards
def sortgroup_find_known_key_counts [n] [key_no] 't
	(k_ids : [key_no]t)
	(sorted_ks : [n]t)
	(eq : t -> t -> bool)
	(gt : t -> t -> bool)
: ([key_no]idx_t.t, [key_no]idx_t.t) = -- returns index & size of each group
	let init_step = idx_t.min 1 (n/2)
	in k_ids
		|> map (\kv ->
			if (n==0) then (-1,0) else
			let bsearch_first = 
				loop (heshi, step) = (0, init_step)
				while step>0 do
					let cv = sorted_ks[heshi]
					let pv = if heshi==0 then cv else sorted_ks[heshi-1]
					let nv = if heshi==n-1 then cv else sorted_ks[heshi+1]
					in
						if (kv `eq` cv) && (heshi==0 || (kv `gt` pv))
							then (heshi, 0)
						else if (kv `eq` cv)
							then (heshi-step, idx_t.max 1 (step/2))
						else if (kv `gt` cv) then
							if (heshi == n-1 || (nv `gt` kv))
							then (-1, 0)
							else (heshi+step, idx_t.max 1 (step/2))
						else
							if (heshi==0 || (kv `gt` pv))
							then (-1, 0)
							else (heshi-step, idx_t.max 1 (step/2))
			let fm = bsearch_first.0
			let init_step_last = idx_t.max 1 (n-fm)/2
			let bsearch_last = if fm<0 then (-1,0) else
				loop (heshi, step) = (fm, init_step_last)
				while step>0 do
					let cv = sorted_ks[heshi]
					let nv = if heshi==n-1 then cv else sorted_ks[heshi+1]
					in
						if (kv `eq` cv) && (heshi==n-1 || (nv `gt` kv)) then
							(heshi, 0)
						else if (kv `eq` cv) then
							(heshi+step, idx_t.max 1 (step/2))
						else if (kv `gt` cv) then
							(heshi+step, idx_t.max 1 (step/2))
						else
							(heshi-step, idx_t.max 1 (step/2))
			let lm = bsearch_last.0
			let cm = if fm<0 then 0 else lm-fm+1
			in (fm,cm)
		)
		|> unzip

def sortgroup_find_unknown_key_counts [n] 't
	(sorted_ks : [n]t)
	(eq : t -> t -> bool)
: ([]idx_t.t, []idx_t.t) = -- returns index & size of each group
	let bounds = (iota n)
		|> map (\j ->
			if j == 0 then 0 else
			let this_k = sorted_ks[j]
			let prev_k = sorted_ks[j-1]
			in
				if this_k `eq` prev_k
				then (-1)
				else j
		)
		|> filter (\j -> j>=0)
	let sizes = (indices bounds)
		|> map (\j ->
			if j == (length bounds)-1 then (n-bounds[j]) else
			let this_ind = bounds[j]
			let next_ind = bounds[j+1]
			in next_ind-this_ind
		)
	in (bounds, sizes)

-- TODO this could be done with map and binary search
def sortgroup_partitionByIndex [group_no]
	(group_idx: [group_no]idx_t.t)
	(group_sizes: [group_no]idx_t.t)
	(len: idx_t.t)
: [len]idx_t.t =
	loop buff = (replicate len (-1))
	for j in (iota group_no) do
		let zuowei = group_idx[j]
		let xiaoda = group_sizes[j]
		in buff with [zuowei:zuowei+xiaoda] = replicate xiaoda j
		

-- Aggregation Modules

local module type aggrCol = {
	type t

	val contains [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> [n]t -> [group_no]bool

	val hasInClosedInterval [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> t -> [n]t -> [group_no]bool
	val hasInOpenInterval [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> t -> [n]t -> [group_no]bool

	val withinClosedInterval [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> t -> [n]t -> [group_no]bool
	val withinOpenInterval [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> t -> [n]t -> [group_no]bool

	val all_gt [n]
		: (group_no : idx_t.t) -> ([n]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_geq [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_lt [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_leq [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> t -> [n]t -> [group_no]bool

	val minimum [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> [n]t -> [group_no]t
	val maximum [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> [n]t -> [group_no]t
}
local module type aggrNumericCol = {
	include aggrCol

	val sum [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> [n]t -> [group_no]t
	val prod [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> [n]t -> [group_no]t
	val sq_sum [n]
		: (group_no : idx_t.t)  -> ([n]idx_t.t) -> [n]t -> [group_no]t

	val mean [n] [group_no]
		: ([n]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val var [n] [group_no]
		: ([n]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val cov [n] [group_no]
		: ([n]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [n]t -> [group_no]t
}

local module mk_aggrCol_from_numeric (N: numeric) : aggrNumericCol with t = N.t = {
	type t = N.t

	local def lowest  : t = N.lowest
	local def highest : t = N.highest

	local def eq  = (N.==)
	local def neq = (N.!=)
	local def geq = (N.>=)
	local def leq = (N.<=)
	local def gt  = (N.>)
	local def lt  = (N.<)

	local def plus  = (N.+)
	local def minus = (N.-)
	local def times = (N.*)
	local def over  = (N./)

	local def zero = N.i64 0
	local def one  = N.i64 1

	-- Entry Points

	def contains (group_no) (grouped_idxs) (v) (xs)
		= grouped_any group_no grouped_idxs (\x -> x `eq` v) xs
	
	def hasInClosedInterval (group_no) (grouped_idxs) (lo) (hi) (xs)
		= grouped_any group_no grouped_idxs (\x -> (x`geq`lo) && (x`leq`hi)) xs
	def hasInOpenInterval (group_no) (grouped_idxs) (lo) (hi) (xs)
		= grouped_any group_no grouped_idxs (\x -> (x`gt`lo) && (x`lt`hi)) xs

	def withinClosedInterval (group_no) (grouped_idxs) (lo) (hi) (xs)
		= grouped_all group_no grouped_idxs (\x -> (x`geq`lo) && (x`leq`hi)) xs
	def withinOpenInterval (group_no) (grouped_idxs) (lo) (hi) (xs)
		= grouped_all group_no grouped_idxs (\x -> (x`gt`lo) && (x`lt`hi)) xs

	def all_gt (group_no) (grouped_idxs) (v) (xs)
		= grouped_all group_no grouped_idxs (\x -> x `gt` v) xs
	def all_geq (group_no) (grouped_idxs) (v) (xs)
		= grouped_all group_no grouped_idxs (\x -> x `geq` v) xs
	def all_lt (group_no) (grouped_idxs) (v) (xs)
		= grouped_all group_no grouped_idxs (\x -> x `lt` v) xs
	def all_leq (group_no) (grouped_idxs) (v) (xs)
		= grouped_all group_no grouped_idxs (\x -> x `leq` v) xs

	def minimum (group_no) (grouped_idxs) (xs)
		= grouped_reduce group_no grouped_idxs (\x1 x2 -> if (x1`lt`x2) then x1 else x2) highest xs
	def maximum (group_no) (grouped_idxs) (xs)
		= grouped_reduce group_no grouped_idxs (\x1 x2 -> if (x1`gt`x2) then x1 else x2) lowest xs

	def sum (group_no) (grouped_idxs) (xs)
		= grouped_reduce group_no grouped_idxs (plus) zero xs
	def prod (group_no) (grouped_idxs) (xs)
		= grouped_reduce group_no grouped_idxs (times) one xs
	def sq_sum (group_no) (grouped_idxs) (xs)
		= grouped_reduce group_no grouped_idxs (plus) zero (xs |> map (\x -> x`times`x))

	def mean [group_no] (grouped_idxs) (group_sizes: [group_no]idx_t.t) (xs) =
		let grouped_sums = sum group_no grouped_idxs xs
		in map2 (\svm size -> if size==0 then zero else (svm `over` (N.i64 size))) grouped_sums group_sizes
	def var [group_no] (grouped_idxs) (group_sizes: [group_no]idx_t.t) (xs) =
		let sums = sum group_no grouped_idxs xs
		let sq_sums = sq_sum group_no grouped_idxs xs
		in map3
			(\sq mu sz_ -> 
				let sz = if sz_>0 then (N.i64 sz_) else one
				in (sq `minus` ((mu`times`mu)`over`sz))`over`sz
			)
			sq_sums sums group_sizes
	def cov [group_no] (grouped_idxs) (group_sizes: [group_no]idx_t.t) (xs) (ys) =
		let sums_x = sum group_no grouped_idxs xs
		let sums_y = sum group_no grouped_idxs ys
		let sums_xy = sum group_no grouped_idxs (map2 (times) xs ys)
		in map4
			(\xy mu_x mu_y sz_ -> 
				let sz = if sz_>0 then (N.i64 sz_) else one
				in (xy `minus` ((mu_x`times`mu_y)`over`sz))`over`sz
			)
			sums_xy sums_x sums_y group_sizes
}

module GroupedAggregator_short  = mk_aggrCol_from_numeric i16
module GroupedAggregator_int    = mk_aggrCol_from_numeric i32
module GroupedAggregator_long   = mk_aggrCol_from_numeric i64
module GroupedAggregator_float  = mk_aggrCol_from_numeric f32
module GroupedAggregator_double = mk_aggrCol_from_numeric f64

