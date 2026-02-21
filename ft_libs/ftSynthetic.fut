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

-- Based on 'Quickly generating billion-record synthetic databases'.
-- Jim Gray, Prakash Sundaresan, Susanne Englert, Ken Baclawski, Peter J. Weinberger
-- ACM SIGMOD Record, Volume 23, Issue 2, Pages 243-252
entry zipf_skewed [n]
	(us : [n]f64) -- uniform random floats in 0-1
	(nv : i64)
	(s_ : f64)
: [n]i64 =
	-- for s==1, add small offset to avoid eta==0
	let s = if s_==1 then (1.0000001) else s_
	let fn = f64.i64 nv
	let zeta (k : i64) (theta : f64) : f64
	= k
		|> iota
		|> map (\i -> i+1)
		|> map (f64.i64)
		|> map (\i -> (1.0 / i)**theta)
		|> f64.sum
	let alpha = 1 / (1 - s)
	let zetan = zeta nv s
	let zetas = zeta (i64.f64 s) 2.0
	let eta = (1.0 - (2.0/fn)**(1.0-s)) / (1.0 - zetas/zetan)
	in us |> map (\u ->
		let uz = u*zetan in
		if uz<1 then 0 else
		if uz< 1 + 0.5**s then 1 else
		(fn * (eta*u - eta + 1)**alpha) |> i64.f64
	)

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