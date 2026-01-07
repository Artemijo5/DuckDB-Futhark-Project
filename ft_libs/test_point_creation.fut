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
		let xs_ = map2 (\s r_ -> 1 + f*s*r_) sgns rs_
			|> map2 (*) rs
			|> map2 (\m r -> r / (f32.sqrt m + 0.001)) ms
		let min_xs_ = xs_ |> f32.minimum
		in xs_
			|> map (\x -> x - min_xs_)
			|> map (f32.sqrt)
	)
	let max_xss = xss |> map (f32.maximum)
	in map2 (\xs mx -> map (\x -> x / mx * mag) xs) xss max_xss