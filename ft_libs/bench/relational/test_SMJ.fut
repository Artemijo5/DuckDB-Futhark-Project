import "../../ftbasics"
import "../../joins/ftSMJ"

-- TODO figure out how to pass pre-sorted input
-- Will have to be from pre-made input files, and also have zipf-skewed data as well
-- TODO in general want input to be bounded so can have useful output sizes for 2nd phase

-- Match-finding, 4-byte integer keys.
--
-- ==
-- entry: smj1_i32
-- random input { [1000]i32 [1000]i32 }
-- auto output
-- random input { [10000]i32 [10000]i32 }
-- auto output
-- random input { [100000]i32 [10000]i32 }
-- auto output
-- random input { [100000]i32 [100000]i32 }
-- auto output

entry smj1_i32 [n1] [n2]
	(ks1 : [n1]i32)
	(ks2 : [n2]i32)
=
	let pL1 : [n1][0]u8 = (replicate n1 [])
	let pL2 : [n2][0]u8 = (replicate n2 [])
	let xs1 : sortStruct_int [n1] [0] = {k = ks1, pL = pL1}
	let xs2 : sortStruct_int [n2] [0] = {k = ks2, pL = pL2}
	let sks1 = (mergeSortRelation xs1 (<=)).k
	let sks2 = (mergeSortRelation xs2 (<=)).k
	let res = mergeJoin i32.num_bits sks1 sks2 0 0 (i64.highest) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm)

-- Match-finding, 8-byte integer keys.
--
-- ==
-- entry: smj1_i64
-- random input { [1000]i64 [1000]i64 }
-- auto output
-- random input { [10000]i64 [10000]i64 }
-- auto output
-- random input { [100000]i64 [10000]i64 }
-- auto output
-- random input { [100000]i64 [100000]i64 }
-- auto output

entry smj1_i64 [n1] [n2]
	(ks1 : [n1]i64)
	(ks2 : [n2]i64)
=
	let pL1 : [n1][0]u8 = (replicate n1 [])
	let pL2 : [n2][0]u8 = (replicate n2 [])
	let xs1 : sortStruct_long [n1] [0] = {k = ks1, pL = pL1}
	let xs2 : sortStruct_long [n2] [0] = {k = ks2, pL = pL2}
	let sks1 = (mergeSortRelation xs1 (<=)).k
	let sks2 = (mergeSortRelation xs2 (<=)).k
	let res = mergeJoin i64.num_bits sks1 sks2 0 0 (i64.highest) (==) (>) (<)
	in (res.vs, res.ix, res.iy, res.cm)

-- Expansion, 4-byte integer keys.
--
-- ==
-- entry: smj2_i32
-- input @data/test_SMJ:smj1_i32-[1000]i32_[1000]i32.out
-- auto output
-- input  @data/test_SMJ:smj1_i32-[10000]i32_[10000]i32.out
-- auto output
-- input  @data/test_SMJ:smj1_i32-[100000]i32_[10000]i32.out
-- auto output
-- input  @data/test_SMJ:smj1_i32-[100000]i32_[100000]i32.out
-- auto output

entry smj2_i32 [n]
	(vs : [n]i32) (ix : [n]i64) (iy : [n]i64) (cm : [n]i64)
=
	let jTups : joinTup [n] i32 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let res = joinTups_to_joinPairs_InnerJoin jTups (-1)
	in (res.vs, res.ix, res.iy)

-- Expansion, 8-byte integer keys.
--
-- ==
-- entry: smj2_i64
-- input @data/test_SMJ:smj1_i64-[1000]i64_[1000]i64.out
-- auto output
-- input  @data/test_SMJ:smj1_i64-[10000]i64_[10000]i64.out
-- auto output
-- input  @data/test_SMJ:smj1_i64-[100000]i64_[10000]i64.out
-- auto output
-- input  @data/test_SMJ:smj1_i64-[100000]i64_[100000]i64.out
-- auto output

entry smj2_i64 [n]
	(vs : [n]i64) (ix : [n]i64) (iy : [n]i64) (cm : [n]i64)
=
	let jTups : joinTup [n] i64 = {vs=vs,ix=ix,iy=iy,cm=cm}
	let res = joinTups_to_joinPairs_InnerJoin jTups (-1)
	in (res.vs, res.ix, res.iy)