import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- TODO why won't radix sort compile (!!!)

-- Merge Sort

-- Merge-sort with 4-byte integer keys.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- ==
-- entry: merge_sort_i32
-- random input { [33554432]i32 [33554432][0]u8 }
-- auto output
-- random input { [33554432]i32 [33554432][4]u8 }
-- auto output
-- random input { [33554432]i32 [33554432][8]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][0]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][4]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][8]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][0]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][4]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][8]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][0]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][4]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][8]u8 }
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
-- random input { [33554432]i64 [33554432][0]u8 }
-- auto output
-- random input { [33554432]i64 [33554432][4]u8 }
-- auto output
-- random input { [33554432]i64 [33554432][8]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][0]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][4]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][8]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][0]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][4]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][8]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][0]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][4]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][8]u8 }
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
-- random input { [33554432]i32 [33554432][0]u8 }
-- auto output
-- random input { [33554432]i32 [33554432][4]u8 }
-- auto output
-- random input { [33554432]i32 [33554432][8]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][0]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][4]u8 }
-- auto output
-- random input { [67108864]i32 [67108864][8]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][0]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][4]u8 }
-- auto output
-- random input { [134217728]i32 [134217728][8]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][0]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][4]u8 }
-- auto output
-- random input { [268435456]i32 [268435456][8]u8 }
-- auto output

entry radix_sort_i32 [n] [b] (ks : [n]i32) (pL : [n][b]u8) =
	let xs = zip ks pL
	let res = my_radix_sort_int 128000 2 i32.num_bits (\i (v,_) -> i32.get_bit i v) xs
	in unzip res

-- Radix-sort with 8-byte integer keys.
-- Payloads vary between 4 bytes & 8 bytes.
--
-- 
-- entry: radix_sort_i64
-- random input { [33554432]i64 [33554432][0]u8 }
-- auto output
-- random input { [33554432]i64 [33554432][4]u8 }
-- auto output
-- random input { [33554432]i64 [33554432][8]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][0]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][4]u8 }
-- auto output
-- random input { [67108864]i64 [67108864][8]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][0]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][4]u8 }
-- auto output
-- random input { [134217728]i64 [134217728][8]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][0]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][4]u8 }
-- auto output
-- random input { [268435456]i64 [268435456][8]u8 }
-- auto output

entry radix_sort_i64 [n] [b] (ks : [n]i64) (pL : [n][b]u8) =
	let xs = zip ks pL
	let res = my_radix_sort_int 128000 2 i64.num_bits (\i (v,_) -> i64.get_bit i v) xs
	in unzip res

