import "../../ftSynthetic"

-- Generate linearly (anti-)correlated data for skyline.
-- ==
-- entry: do_datagen
-- input @data/dat.in
-- auto output

entry do_datagen [d]
	(do_anticorr : [1]bool)
	(f : [1]f64)
	(mag : [1]f64)
	(us0 : []f64)
	(uss : [d][]f64)
= uss |> linear_correlated_f64
	f[0] mag[0] (replicate (d-1) do_anticorr[0]) us0