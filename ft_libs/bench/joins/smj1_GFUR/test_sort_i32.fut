import "../../../ftbasics"
import "../../../ftsort"

-- Sorting stage of SMJ (GFUR).
-- Sort the 2 key columns, also transforming their payloads.
--
-- ==
-- entry: do_radixSort_i32
-- compiled input @data/dat_i32.in
-- auto output

entry do_mergeSort_i32 [n1] [n2]
	(xs1 : [n1]i32)
	(xs2 : [n2]i32)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let sxs1 = mergeSort_GFUR (<=) xs1
	in (sxs1.ks, sxs1.is, xs2, pL1, pL2)

entry do_radixSort_i32 [n1] [n2]
	(xs1 : [n1]i32)
	(xs2 : [n2]i32)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let sxs1 = radixSort_int_GFUR
		2 i32.num_bits (i32.clz) (i32.get_bit)
		xs1
	in (sxs1.ks, sxs1.is, xs2, pL1, pL2)