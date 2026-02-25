import "../../../ftbasics"
import "../../../joins/ftSMJ"

-- Datagen, 4&8-byte integer keys.
-- Going by Wu et al's paper:
-- 2 relations, sized n-2n
-- 100% match rate (output size = 2*n)
-- Narrow join (no payloads).
-- 
-- ==
-- entry: smj0_100 smj0_50 smj0_25 smj0_125
-- input @data/datagen.in
-- auto output

def smj0_matchRate [n]
	(matchRate : f16)
	(prob : [n]f16)
	(pL : [n][]u8)
=
	let ks1 : [n]i32 =   (0..2..<(i32.i64 (2*n)))
		|> sized n
	let ks2 : [n]i32 = (0..2..<(i32.i64 (2*n)))
		|> sized n
		|> map2 (\p v ->
			if p<=matchRate then v else (v+1)
		) prob
	in (ks1, pL, ks2, pL)


entry smj0_100 [n] (prob : [n]f16) (pL : [n][]u8)
= smj0_matchRate 1.0 prob pL

entry smj0_50 [n] (prob : [n]f16) (pL : [n][]u8)
= smj0_matchRate 0.5 prob pL

entry smj0_25 [n] (prob : [n]f16) (pL : [n][]u8)
= smj0_matchRate 0.25 prob pL

entry smj0_125 [n] (prob : [n]f16) (pL : [n][]u8)
= smj0_matchRate 0.125 prob pL

entry smj0_625 [n] (prob : [n]f16) (pL : [n][]u8)
= smj0_matchRate 0.0625 prob pL