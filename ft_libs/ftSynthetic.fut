def linear_correlated [dim] [n]
	(f    : f32)
	(mag  : f32)
	(anticorr : [dim-1]bool) -- is correlated with first dim
	(rss  : [dim+1][n]f32)
: [dim][n]f32 =
	let xss = iota dim |> map (\d ->
		let rs = rss[d+1]
		let s = if (d>0 && anticorr[d-1]) then (-1.0) else (1.0)
		in rs
			|> map (\r -> r * (f32.sqrt (1 - f*f)))
			|> map2 (\r0 r -> s*f*r0 + r) rss[0]
	)
	let min_xss = xss |> map (f32.minimum)
	let xss_ = map2 (\xs mx -> map (\x -> x-mx) xs) xss min_xss
	let max_xss_ = xss_ |> map (f32.maximum)
	in map2 (\xs mx -> map (\x -> x / mx * mag) xs) xss_ max_xss_

def funnels [dim] [n]
	(f   : f32)
	(mag : f32)
	(rss : [2*dim][n]f32)
: [dim][n]f32 =
	let ms_ = rss[0:dim]
		|> map (map (\r -> r*r))
		|> transpose
	let mss = (iota dim)
		|> map (\d ->
			ms_ |> map (\m -> (copy m) with [d] = 0.0)
				|> map (reduce (+) 0.0)
				|> map (f32.sqrt)
				|> map2 (\r m -> m / (r+0.001)) rss[d]
		)
	let sgnss = mss |> map (map (\m -> if m>1.0 then 1.0 else -1.0))
	let xss = (iota dim) |> map (\d->
		let rs  = rss[d]
		let rs_ = rss[dim+d]
		let ms = mss[d]
		let sgns = sgnss[d]
		in map2 (\s r_ -> 1 + f*s*r_) sgns rs_
			|> map2 (*) rs
			|> map2 (\m r -> r / (f32.sqrt m + 0.001)) ms
			|> map (f32.abs)
			|> map (f32.sqrt)
	)
	let min_xss = xss |> map (f32.minimum)
	let xss_ = map2 (\xs mx -> map (\x -> x-mx) xs) xss min_xss
	let max_xss_ = xss_ |> map (f32.maximum)
	in map2 (\xs mx -> map (\x -> x / mx * mag) xs) xss_ max_xss_

def hyperbolic [dim] [n]
	(f : f32)
	(mag_inner : f32)
	(mag : f32)
	(rss_ : [dim][n]f32) -- random values from 0 to 1
	(s : f32)
 : [n][dim]f32 =
	let rss = rss_
		|> map (map (\r -> mag_inner
			|> (f32./) 1.0
			|> (f32.-) mag_inner
			|> (f32.*) r
			|> (f32.+) (1/mag_inner)
		))
	let mss = (iota dim)
		|> map (\d ->
			rss
			|> map (map (\r -> 1.0 / r))
			|> transpose
			|> map (\rs -> (copy rs) with [d] = 0.0)
			|> map (f32.sum)
			|> map (\r -> r/(f32.i64 (dim-1)))
			|> map (\r -> f*f |> (f32.-) 1.0 |> f32.sqrt |> (f32.*) r)
			|> map (\r -> r**s)
		)
	let xss = rss
		|> map (map (\r -> r*f))
		|> map2 (map2 (+)) mss
	let min_xss = xss |> map (f32.minimum)
	let xss_ = map2 (\xs mx -> map (\x -> x-mx) xs) xss min_xss
	let max_xss_ = xss_ |> map (f32.maximum)
	in map2 (\xs mx -> map (\x -> x / mx * mag) xs) xss_ max_xss_ |> transpose

def zipf_skewed [n] [n_hot]
	(xs : [n]i64)
	(hot_keys : [n_hot]i64)
	(num_keys : i64)
	(r_prob : [n]f32)
	(s : f32)
	(q : f32)
: [n]i64 =
	let h_n = iota (i64.max num_keys (n_hot+1))
		|> map (\n -> 1/((q+(f32.i64 (n+1)))**s))
		|> f32.sum
	let prob_k = (iota n_hot) |> map (\i -> 1.0/((((f32.i64 i)+q)**s)*h_n))
	in map2 (\x p ->
		let (newV,_,_) =
			loop (y,acc,j) = (x,0,0) while j < n_hot do
				if p<acc+prob_k[j]
				then (hot_keys[j],0,n_hot)
				else (y,acc+prob_k[j],j+1)
		in newV
	) xs r_prob