module mk_synthetic_real (F : real) = {
	type t = F.t

	local def gt = (F.>)
	local def lt = (F.<)
	local def geq = (F.>=)
	local def leq = (F.<=)
	local def eq = (F.==)

	local def neg = (F.neg)

	local def plus = (F.+)
	local def minus = (F.-)
	local def times = (F.*)
	local def over = (F./)
	local def pow = (F.**)

	local def from_i64 = (F.i64)
	local def to_i64 = (F.to_i64)

	local def zero = F.i32 0
	local def one = F.i32 1
	local def eps = F.f32 0.001
	local def lowest = F.lowest
	local def highest = F.highest

	local def sqrt = (F.sqrt)
	local def abs = (F.abs)
	local def sum = (F.sum)

	local def min = (F.min)
	local def max = (F.max)
	local def minimum = (F.minimum)
	local def maximum = (F.maximum)

	def linear_correlated [dim] [n]
		(f    : t)
		(mag  : t)
		(anticorr : [dim]bool) -- is correlated with first dim
		(rss  : [dim+2][n]t) -- random values from 0 to 1
	: [dim+1][n]t =
		let xss = iota (dim+1) |> map (\d ->
			let rs = rss[d+1]
			let s = if (d>0 && anticorr[d-1]) then (neg one) else (one)
			in rs
				|> map (\r -> f |> times f |> minus one |> sqrt |> times r)
				|> map2 (\r0 r -> s |> times f |> times r0 |> plus r) rss[0]
		)
		let min_xss = xss |> map (minimum)
		let xss_ = map2 (\xs mx -> map (\x -> x `minus` mx) xs) xss min_xss
		let max_xss_ = xss_ |> map (maximum)
		in map2 (\xs mx -> map (\x -> mx |> over x |> times mag) xs) xss_ max_xss_

	def funnels [dim] [n]
		(_ : [dim][0]t)
		(f   : t)
		(mag : t)
		(rss : [2*dim][n]t)
	: [dim][n]t =
		let ms_ = rss[0:dim]
			|> map (map (\r -> r `times` r))
			|> transpose
		let mss = (iota dim)
			|> map (\d ->
				ms_ |> map (\m -> (copy m) with [d] = zero)
					|> map (sum)
					|> map (sqrt)
					|> map2 (\r m -> m `over` (r `plus` eps)) rss[d]
			)
		let sgnss = mss |> map (map (\m -> if (m `gt` one) then one else (neg one)))
		let xss = (iota dim) |> map (\d->
			let rs  = rss[d]
			let rs_ = rss[dim+d]
			let ms = mss[d]
			let sgns = sgnss[d]
			in map2 (\s r_ -> f |> times s |> times r_ |> plus one) sgns rs_
				|> map2 (times) rs
				|> map2 (\m r -> r `over` (sqrt (m `plus` eps))) ms
				|> map (abs)
				|> map (sqrt)
		)
		let min_xss = xss |> map (minimum)
		let xss_ = map2 (\xs mx -> map (\x -> x `minus` mx) xs) xss min_xss
		let max_xss_ = xss_ |> map (maximum)
		in map2 (\xs mx -> map (\x -> mx |> over x |> times mag) xs) xss_ max_xss_

	def hyperbolic [dim] [n]
		(f : t)
		(mag_inner : t)
		(mag : t)
		(rss_ : [dim][n]t) -- random values from 0 to 1
		(s : t)
	 : [dim][n]t =
		let rss = rss_
			|> map (map (\r -> mag_inner
				|> over one
				|> minus mag_inner
				|> times r
				|> plus (one `over` mag_inner)
			))
		let mss = (iota dim)
			|> map (\d ->
				rss
				|> map (map (\r -> one `over` r))
				|> transpose
				|> map (\rs -> (copy rs) with [d] = zero)
				|> map (sum)
				|> map (\r -> r `over` (from_i64 (dim-1)))
				|> map (\r -> f |> times f |> minus one |> sqrt |> times r)
				|> map (\r -> r `pow` s)
			)
		let xss = rss
			|> map (map (\r -> r `times` f))
			|> map2 (map2 (plus)) mss
		let min_xss = xss |> map (minimum)
		let xss_ = map2 (\xs mx -> map (\x -> x `minus` mx) xs) xss min_xss
		let max_xss_ = xss_ |> map (maximum)
		in map2 (\xs mx -> map (\x -> mx |> over x |> times mag) xs) xss_ max_xss_

}

def zipf_skewed [n] [n_hot] 't
	(xs : [n]t)
	(hot_keys : [n_hot]t)
	(num_keys : i64)
	(r_prob : [n]f64)
	(s : f64)
	(q : f64)
: [n]t =
	let h_n = iota (i64.max num_keys (n_hot+1))
		|> map (\n -> 1/((q+(f64.i64 (n+1)))**s))
		|> f64.sum
	let prob_k = (iota n_hot)
		|> map (\i -> 1.0/((((f64.i64 i)+q)**s)*h_n))
		|> scan (+) 0.0
	in map2 (\x p ->
		let (newV,_) =
			loop (y,j) = (x,0) while j < n_hot do
				if p<prob_k[j]
				then (hot_keys[j],n_hot)
				else (y,j+1)
		in newV
	) xs r_prob


module mk_synthetic_half   = mk_synthetic_real f16
module mk_synthetic_float  = mk_synthetic_real f32
module mk_synthetic_double = mk_synthetic_real f64

entry linear_correlated_half   = mk_synthetic_half.linear_correlated
entry linear_correlated_float  = mk_synthetic_float.linear_correlated
entry linear_correlated_double = mk_synthetic_double.linear_correlated

entry funnels_half   = mk_synthetic_half.funnels ([])
entry funnels_float  = mk_synthetic_float.funnels ([])
entry funnels_double = mk_synthetic_double.funnels ([])

entry hyperbolic_half   = mk_synthetic_half.hyperbolic
entry hyperbolic_float  = mk_synthetic_float.hyperbolic
entry hyperbolic_double = mk_synthetic_double.hyperbolic

entry zipf_skewed_short (xs : []i16) hk nk rp s q = zipf_skewed xs hk nk rp s q
entry zipf_skewed_int (xs : []i32) hk nk rp s q = zipf_skewed xs hk nk rp s q
entry zipf_skewed_long (xs : []i64) hk nk rp s q = zipf_skewed xs hk nk rp s q
entry zipf_skewed_half (xs : []f16) hk nk rp s q = zipf_skewed xs hk nk rp s q
entry zipf_skewed_float (xs : []f32) hk nk rp s q = zipf_skewed xs hk nk rp s q
entry zipf_skewed_double (xs : []f64) hk nk rp s q = zipf_skewed xs hk nk rp s q