import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- 
-- ==
-- entry: smj0_i32 smj0_i64
-- compiled input { 33554432i64 }
-- auto output
-- compiled input { 67108864i64 }
-- auto output
-- compiled input { 134217728i64 }
-- auto output

entry smj0_i32
	(n : i64)
=
	let ks1 : [n]i32 =   (0..1..<(i32.i64 n))
		|> sized n
	let ks2 : [2*n]i32 = (0..1..<(i32.i64 n))
		|> sized n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> sized (2*n)
	let pL1 : [n][0]u8 = (replicate n [])
	let pL2 : [2*n][0]u8 = (replicate (2*n) [])
	in (ks1, pL1, ks2, pL2)

entry smj0_i64
	(n : i64)
=
	let ks1 : [n]i64 =   (0..1..<n)
		|> sized n
	let ks2 : [2*n]i64 = (0..1..<n)
		|> sized n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> sized (2*n)
	let pL1 : [n][0]u8 = (replicate n [])
	let pL2 : [2*n][0]u8 = (replicate (2*n) [])
	in (ks1, pL1, ks2, pL2)