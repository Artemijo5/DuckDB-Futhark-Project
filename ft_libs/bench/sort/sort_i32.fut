import "../../ftbasics"
import "../../ftsort"

-- How fast can we sort a column + payloads?
--
-- ==
-- entry: do_mergeSort_i32 do_radixSort_i32
-- compiled input @data/dat_i32.in

entry do_mergeSort_i32 [n]
	(xs: [n]i32)
	(pL: [n][]u8)
	(_ : [1]i32)
=
	let sxs = mergeSort_GFTR (<=) xs pL
	in (sxs.ks, sxs.pL)

entry do_radixSort_i32 [n]
	(xs: [n]i32)
	(pL: [n][]u8)
	(bitstep: [1]i32)
=
	let sxs = radixSort_int_GFTR
		bitstep[0] i32.num_bits (i32.clz) (i32.get_bit)
		xs pL
	in (sxs.ks, sxs.pL)