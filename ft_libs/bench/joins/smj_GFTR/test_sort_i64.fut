import "../../../ftbasics"
import "../../../ftsort"

-- Sorting stage of SMJ (GFTR).
-- Sort the 2 key columns, also transforming their payloads.
--
-- ==
-- entry: do_radixSort_i64
-- compiled input @data/dat_i64.in
-- auto output

entry do_mergeSort_i64 [n1] [n2]
	(xs1 : [n1]i64)
	(xs2 : [n2]i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let sxs1 = mergeSort_GFTR (<=) xs1 pL1
	let sxs2 = mergeSort_GFTR (<=) xs2 pL2
	in (sxs1.ks, sxs2.ks, sxs1.pL, sxs2.pL)

entry do_radixSort_i64 [n1] [n2]
	(xs1 : [n1]i64)
	(xs2 : [n2]i64)
	(pL1 : [n1][]u8)
	(pL2 : [n2][]u8)
=
	let sxs1 = radixSort_int_GFTR
		2 i64.num_bits (i64.clz) (i64.get_bit)
		xs1 pL1
	let sxs2 = radixSort_int_GFTR
		2 i64.num_bits (i64.clz) (i64.get_bit)
		xs2 pL2
	in (sxs1.ks, sxs2.ks, sxs1.pL, sxs2.pL)