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

-- TODO why does this return nans ? probably remake...
def funnels [dim] [n]
	(f   : f32)
	(mag : f32)
	(rss : [2*dim][n]f32)
: [dim][n]f32 =
	let mss = rss[0:dim]
		|> map (map (\r -> r*r))
		|> transpose
	let xss = (iota dim) |> map (\d->
		let rs  = rss[d]
		let rs_ = rss[dim+d]
		let ms = mss
			|> map (\ms -> (copy ms) with [d] = 0.0)
			|> map (reduce (+) 0.0)
			|> map (f32.sqrt)
			|> map2 (\r m -> m / (r+0.01)) rs
		let sgns = ms
			|> map (\m -> if m>1.0 then 1.0 else -1.0)
		in map2 (\s r_ -> 1 + f*s*r_) sgns rs_
			|> map2 (*) rs
			|> map2 (\m r -> r / (f32.sqrt m)) ms
			|> map (f32.sqrt)
	)
	let min_xss = xss |> map (f32.minimum)
	let xss_ = map2 (\xs mx -> map (\x -> x-mx) xs) xss min_xss
	let max_xss_ = xss_ |> map (f32.maximum)
	in map2 (\xs mx -> map (\x -> x / mx * mag) xs) xss_ max_xss_