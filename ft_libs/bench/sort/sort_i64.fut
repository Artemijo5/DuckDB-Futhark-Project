import "../../ftbasics"
import "../../ftsort"

-- How fast can we sort a column + payloads?
--
-- ==
-- entry: do_mergeSort_i64 do_radixSort_i64
-- compiled input @data/dat_i64.in

entry do_mergeSort_i64 [n]
	(xs: [n]i64)
	(pL: [n][]u8)
	(_ : [1]i32)
=
	let sxs = mergeSort_GFTR (<=) xs pL
	in (sxs.ks, sxs.pL)

entry do_radixSort_i64 [n]
	(xs: [n]i64)
	(pL: [n][]u8)
	(bitstep: [1]i32)
=
	let sxs = radixSort_int_GFTR
		bitstep[0] i64.num_bits (i64.clz) (i64.get_bit)
		xs pL
	in (sxs.ks, sxs.pL)