import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- 
-- ==
-- entry: smj0_i32
-- random input { [33554432][8]u8 [67108864][8]u8 }
-- auto output
-- random input { [33554432][16]u8 [67108864][16]u8 }
-- auto output
-- random input { [67108864][8]u8 [134217728][8]u8 }
-- auto output
-- random input { [67108864][16]u8 [134217728][16]u8 }
-- auto output
-- random input { [134217728][8]u8 [268435456][8]u8 }
-- auto output
-- random input { [134217728][16]u8 [268435456][16]u8 }
-- auto output

entry smj0_i32 [n]
	(pL1 : [n][]u8)
	(pL1 : [2*n][]u8)
=
	let ks1 : [n]i32 =   (0..1..<(i32.i64 n))
		|> sized n
	let ks2 : [2*n]i32 = (0..1..<(i32.i64 n))
		|> sized n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> sized (2*n)
	in (ks1, pL1, ks2, pL2)

entry smj0_i64 [n]
	(pL1 : [n][]u8)
	(pL1 : [2*n][]u8)
=
	let ks1 : [n]i64 =   (0..1..<n)
		|> sized n
	let ks2 : [2*n]i64 = (0..1..<n)
		|> sized n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> sized (2*n)
	in (ks1, pL1, ks2, pL2)