import "../../../ftbasics"
import "../../../joins/ftHashJoin"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- 
-- ==
-- entry: smj0_i32 smj0_i64
-- compiled input { 100i64 }
-- auto output

entry rhj0_8
	(n : i64)
=
	let ks1_ : [n][4]u8 =   iota n
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
	let ks2_ : [2*n]i64 = iota n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
		|> sized (2*n)
	let pL1_ : [n][0]u8 = (replicate n [])
	let pL2_ : [2*n][0]u8 = (replicate (2*n) [])
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 8 ks1_ pL1_ 0 4 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 8 ks2_ pL2_ 0 4 2
	in (ks1, pL1, ks2, pL2)

entry rhj0_12
	(n : i64)
=
	let ks1_ : [n][4]u8 =   iota n
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
	let ks2_ : [2*n]i64 = iota n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
		|> sized (2*n)
	let pL1_ : [n][0]u8 = (replicate n [])
	let pL2_ : [2*n][0]u8 = (replicate (2*n) [])
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 12 ks1_ pL1_ 0 3 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 12 ks2_ pL2_ 0 3 2
	in (ks1, pL1, ks2, pL2)

entry rhj0_16
	(n : i64)
=
	let ks1_ : [n][4]u8 =   iota n
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
	let ks2_ : [2*n]i64 = iota n
		|> map (\i -> (replicate 2 i))
		|> flatten
		|> map (\i -> [i/256/256/256%256,i/256/256%256,i/256%256,i%256])
		|> map (map u8.i64)
		|> sized (2*n)
	let pL1_ : [n][0]u8 = (replicate n [])
	let pL2_ : [2*n][0]u8 = (replicate (2*n) [])
	let (ks1,pL1) = partition_and_deepen (i16.highest) (i64.highest) 16 ks1_ pL1_ 0 2 2
	let (ks2,pL2) = partition_and_deepen (i16.highest) (i64.highest) 16 ks2_ pL2_ 0 2 2
	in (ks1, pL1, ks2, pL2)