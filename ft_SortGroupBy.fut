import "ftbasics"

-- Abstract Grouped Aggregations

def grouped_reduce [n] [group_no] 't
	(group_idx : [group_no]idx_t.t)
	(group_sizes : [group_no]idx_t.t)
	(f : t -> t -> t)
	(ne : t)
	(xs : [n]t)
: [group_no]t =
	let max_size = idx_t.maximum group_sizes
	let regularised_xs = (iota group_no)
		|> map (\gi ->
			let this_inf = group_idx[gi]
			let this_sup = this_inf + group_sizes[gi]
			let this_pad = max_size - group_sizes[gi]
			in (xs[this_inf:this_sup] ++ (replicate this_pad ne)) :> [max_size]t
		)
	in map (\gxs -> reduce f ne gxs) regularised_xs

def grouped_all [n] [group_no] 't
	(group_idx : [group_no]idx_t.t)
	(group_sizes : [group_no]idx_t.t)
	(f : t -> bool)
	(true_el : t)
	(xs : [n]t)
: [group_no]bool =
	let max_size = idx_t.maximum group_sizes
	let regularised_xs = (iota group_no)
		|> map (\gi ->
			let this_inf = group_idx[gi]
			let this_sup = this_inf + group_sizes[gi]
			let this_pad = max_size - group_sizes[gi]
			in (xs[this_inf:this_sup] ++ (replicate this_pad true_el)) :> [max_size]t
		)
	in map (all f) regularised_xs

def grouped_any [n] [group_no] 't
	(group_idx : [group_no]idx_t.t)
	(group_sizes : [group_no]idx_t.t)
	(f : t -> bool)
	(false_el : t)
	(xs : [n]t)
: [group_no]bool =
	let max_size = idx_t.maximum group_sizes
	let regularised_xs = (iota group_no)
		|> map (\gi ->
			let this_inf = group_idx[gi]
			let this_sup = this_inf + group_sizes[gi]
			let this_pad = max_size - group_sizes[gi]
			in (xs[this_inf:this_sup] ++ (replicate this_pad false_el)) :> [max_size]t
		)
	in map (any f) regularised_xs

-- Functions to Find Group Ranges

def find_known_keys [n] [key_no] 't
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

def find_unknown_keys [n] 't
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

-- Aggregation Modules
-- TODO "cannot combine type" ... maybe better left as bare functions?

-- can also be used with strings (?)
module type aggrCol = {
	type t

	val contains [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> [n]t -> [group_no]bool

	val withinClosedInterval [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> t -> [n]t -> [group_no]bool
	val withinOpenInterval [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> t -> [n]t -> [group_no]bool

	val all_gt [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_geq [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_lt [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> [n]t -> [group_no]bool
	val all_leq [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> t -> [n]t -> [group_no]bool

	val minimum [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]bool
	val maximum [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]bool
}
module type aggrNumericCol = {
	include aggrCol

	val sum [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val prod [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val sq_sum [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t

	val mean [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val var [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [group_no]t
	val cov [n] [group_no]
		: ([group_no]idx_t.t)  -> ([group_no]idx_t.t) -> [n]t -> [n]t -> [group_no]t
}

module mk_aggrCol_from_numeric (N: numeric) : aggrNumericCol with t = N.t = {
	type t = N.t

	def lowest  : t = N.lowest
	def highest : t = N.highest

	def (!=) = (N.!=)
	def (>=) = (N.>=)
	def (<=) = (N.<=)
	def (<) = (N.<)
	def (>) = (N.>)

	def (+) = (N.+)
	def (-) = (N.-)
	def (*) = (N.*)
	def (/) = (N./)

	def zero = N.i64 0
	def one = N.i64 1
	def two = N.i64 2

	def midpoint (a:t) (b:t) : t = (a + b) / two

	-- Entry Points

	def contains (group_idx) (group_sizes) (v) (xs)
		= !(grouped_all group_idx group_sizes (\x -> x != v) v xs)
	def withinClosedInterval (group_idx) (group_sizes) (lo) (hi) (xs)
		= grouped_all group_idx group_sizes (\x -> x>=lo && x <=hi) lo xs
	def withinOpenInterval (group_idx) (group_sizes) (lo) (hi) (xs)
		= grouped_all group_idx group_sizes (\x -> x>lo && x <hi) (midpoint lo hi) xs

	def all_gt (group_idx) (group_sizes) (v) (xs)
		= grouped_all group_idx group_sizes (\x -> x > v) v xs
	def all_geq (group_idx) (group_sizes) (v) (xs)
		= grouped_all group_idx group_sizes (\x -> x >= v) v xs
	def all_lt (group_idx) (group_sizes) (v) (xs)
		= grouped_all group_idx group_sizes (\x -> x < v) v xs
	def all_leq (group_idx) (group_sizes) (v) (xs)
		= grouped_all group_idx group_sizes (\x -> x <= v) v xs

	def minimum (group_idx) (group_sizes) (xs)
		= grouped_reduce group_idx group_sizes (\x1 x2 -> if x1<x2 then x1 else x2) highest xs
	def maximum (group_idx) (group_sizes) (xs)
		= grouped_reduce group_idx group_sizes (\x1 x2 -> if x1>x2 then x1 else x2) lowest xs

	def sum (group_idx) (group_sizes) (xs)
		= grouped_reduce group_idx group_sizes (+) zero xs
	def prod (group_idx) (group_sizes) (xs)
		= grouped_reduce group_idx group_sizes (*) one xs
	def sq_sum (group_idx) (group_sizes) (xs)
		= grouped_reduce group_idx group_sizes (+) zero (xs |> map (\x -> x*x))

	def mean (group_idx) (group_sizes) (xs) =
		let grouped_sums = sum group_idx group_sizes xs
		in map2 (\svm size -> if size==0 then 0 else svm / (N.i64 size)) grouped_sums group_sizes
	def var (group_idx) (group_sizes) (xs) =
		let mean = sum group_idx group_sizes xs
		let sq_mean = map2
			(\svm size -> if size==0 then 0 else svm / (N.i64 size))
			(sq_sum group_idx group_sizes xs)
			group_sizes
		in map2 (\xsq mu -> xsq - (mu*mu)) sq_mean mean
	def cov (group_idx) (group_sizes) (xs) (ys) =
		let mean_x = sum group_idx group_sizes xs
		let mean_y = sum group_idx group_sizes ys
		let mean_xy = map2
			(\svm size -> if size==0 then 0 else svm / (N.i64 size))
			(sq_sum group_idx group_sizes (map2 (*) xs ys))
			group_sizes
		in map3 (\xy mu_x mu_y -> xy - mu_x*mu_y) mean_xy mean_x mean_y
}
