import "../../ftbasics"
import "../../joins/ftSMJ"

-- Sorting

-- Merge Sort

-- Merge-sort with 4-byte integer keys.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- ==
-- entry: merge_sort_i32
-- random input { [1000]i32 [1000][4]u8 }
-- auto output
-- random input { [1000]i32 [1000][8]u8 }
-- auto output
-- random input { [10000]i32 [10000][4]u8 }
-- auto output
-- random input { [10000]i32 [10000][8]u8 }
-- auto output
-- random input { [100000]i32 [100000][4]u8 }
-- auto output
-- random input { [100000]i32 [100000][8]u8 }
-- auto output

entry merge_sort_i32 [n] [b] (ks : [n]i32) (pL : [n][b]u8) =
	let xs : sortStruct_int [n] [b] = {k = ks, pL = pL}
	let res = mergeSortRelation xs (<=)
	in (res.k, res.pL)

-- Merge-sort with 8-byte integer keys.
-- Payloads vary between 0, 4 bytes & 8 bytes.
--
-- ==
-- entry: merge_sort_i64
-- random input { [1000]i64 [1000][0]u8 }
-- auto output
-- random input { [1000]i64 [1000][4]u8 }
-- auto output
-- random input { [1000]i64 [1000][8]u8 }
-- auto output
-- random input { [10000]i64 [10000][0]u8 }
-- auto output
-- random input { [10000]i64 [10000][4]u8 }
-- auto output
-- random input { [10000]i64 [10000][8]u8 }
-- auto output
-- random input { [100000]i64 [100000][0]u8 }
-- auto output
-- random input { [100000]i64 [100000][4]u8 }
-- auto output
-- random input { [100000]i64 [100000][8]u8 }
-- auto output

entry merge_sort_i64 [n] [b] (ks : [n]i64) (pL : [n][b]u8) =
	let xs : sortStruct_long [n] [b] = {k = ks, pL = pL}
	let res = mergeSortRelation xs (<=)
	in (res.k, res.pL)

-- Radix Sort
-- seemingly won't compile (!!!)

-- Radix-sort with 4-byte integer keys.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- 
-- entry: radix_sort_i32
-- random input { [1000]i32 [1000][0]u8 }
-- auto output
-- random input { [1000]i32 [1000][4]u8 }
-- auto output
-- random input { [1000]i32 [1000][8]u8 }
-- auto output
-- random input { [10000]i32 [10000][0]u8 }
-- auto output
-- random input { [10000]i32 [10000][4]u8 }
-- auto output
-- random input { [10000]i32 [10000][8]u8 }
-- auto output
-- random input { [100000]i32 [100000][0]u8 }
-- auto output
-- random input { [100000]i32 [100000][4]u8 }
-- auto output
-- random input { [100000]i32 [100000][8]u8 }
-- auto output

entry radix_sort_i32 [n] [b] (ks : [n]i32) (pL : [n][b]u8) =
	let xs : sortStruct_int [n] [b] = {k = ks, pL = pL}
	let res = radixSortRelation_signed_integral 128000 xs i32.num_bits i32.get_bit
	in (res.k, res.pL)

-- Radix-sort with 8-byte integer keys.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- 
-- entry: radix_sort_i64
-- random input { [1000]i64 [1000][4]u8 }
-- auto output
-- random input { [1000]i64 [1000][8]u8 }
-- auto output
-- random input { [10000]i64 [10000][4]u8 }
-- auto output
-- random input { [10000]i64 [10000][8]u8 }
-- auto output
-- random input { [100000]i64 [100000][4]u8 }
-- auto output
-- random input { [100000]i64 [100000][8]u8 }
-- auto output

entry radix_sort_i64 [n] [b] (ks : [n]i64) (pL : [n][b]u8) =
	let xs : sortStruct_long [n] [b] = {k = ks, pL = pL}
	let res = radixSortRelation_signed_integral 128000 xs i64.num_bits i64.get_bit
	in (res.k, res.pL)

