import "../../ftSynthetic"

-- Generate hyperbolically anti-correlated data for skyline.
-- ==
-- entry: do_datagen
-- input @data/dat.in
-- auto output

entry do_datagen [d]
	(mag : [1]f64)
	(uss : [d][]f64)
= uss |> hyperbolic_f64
	0.7 2.0 1.618 mag[0]