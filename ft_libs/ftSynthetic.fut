-- | Function to generate zipf-distributed integers from 0 to (N-1).
-- Can be used with other keys by using the generated values as indices.
--
-- Based on 'Quickly generating billion-record synthetic databases'.
-- Jim Gray, Prakash Sundaresan, Susanne Englert, Ken Baclawski, Peter J. Weinberger
-- ACM SIGMOD Record, Volume 23, Issue 2, Pages 243-252
--
-- Liberties taken from literature:
-- 1. this can be used for skewness > 1
--   (seems to fail at some point when skewness > 2.5)
--   Inteded to be used for up to 2 for experiments
-- 2. non-continuous for skewness == 1
--   -> use value 1.0000001 instead
entry zipf_skewed [n]
	(s_ : f64)
	(nv : i64)
	(us : [n]f64) -- uniform random floats in [0,1]
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

entry zipf_skewed_i32 s nv us : []i32 = zipf_skewed s nv us
	|> map (i32.i64)

-- | Parametric module for generating different types of synthetic data.
-- Specifically:
-- 1. Linearly (anti-)correlated multi-dimensional data.
-- 2. Anti-correlated (hyperbolic) multi-dimensional data.
--
-- Data is processed in the form of multidimensional arrays ([dim][n]t).
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

	-- | Generate linearly correlated points.
	-- Need to supply (d+1) uniformly random vectors for d dimensions.
	-- Can use anticorr flags to make individual dimensions linearly anti-correlated to the first.
	def linear_correlated [dim] [n]
		(f   : t)
		(mag : t)
		(anticorr : [dim-1]bool) -- is correlated with first dim
		(rs0 : [n]t)      -- random values from 0 to 1
		(rss : [dim][n]t) -- random values from 0 to 1
	: [dim][n]t =
		let xss = iota dim |> map (\d ->
			let rs = rss[d]
			let s = if (d>0 && anticorr[d-1]) then (neg one) else (one)
			in rs
				|> map (\r -> f |> times f |> minus one |> sqrt |> times r)
				|> map2 (\r0 r -> s |> times f |> times r0 |> plus r) rs0
		)
		let min_xss = xss |> map (minimum)
		let xss_ = map2 (\xs mx -> map (\x -> x `minus` mx) xs) xss min_xss
		let max_xss_ = xss_ |> map (maximum)
		in map2 (\xs mx -> map (\x -> mx |> over x |> times mag) xs) xss_ max_xss_

	-- | Generate hyperbolic or 'crescent' anti-correlated data in multiple dimensions.
	def hyperbolic [dim] [n]
		(f : t)
		(s : t)
		(mag_inner : t)
		(mag : t)
		(rss_ : [dim][n]t) -- random values from 0 to 1
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

-- | mk_synthetic_real module for f16 (half)
module mk_synthetic_f16 = mk_synthetic_real f16
-- | mk_synthetic_real module for f32 (float)
module mk_synthetic_f32 = mk_synthetic_real f32
-- | mk_synthetic_real module for f64 (double)
module mk_synthetic_f64 = mk_synthetic_real f64

-- | Generate linearly correlated f16 (half) data.
entry linear_correlated_f16 = mk_synthetic_f16.linear_correlated
-- | Generate linearly correlated f32 (float) data.
entry linear_correlated_f32 = mk_synthetic_f32.linear_correlated
-- | Generate linearly correlated f64 (double) data.
entry linear_correlated_f64 = mk_synthetic_f64.linear_correlated

-- | Generate hyperbolic anti-correlated f16 (half) data.
entry hyperbolic_f16 = mk_synthetic_f16.hyperbolic
-- | Generate hyperbolic anti-correlated f32 (float) data.
entry hyperbolic_f32 = mk_synthetic_f32.hyperbolic
-- | Generate hyperbolic anti-correlated f64 (double) data.
entry hyperbolic_f64 = mk_synthetic_f64.hyperbolic