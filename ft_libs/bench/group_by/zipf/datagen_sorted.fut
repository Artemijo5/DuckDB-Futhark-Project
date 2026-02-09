import "../../../lib/github.com/diku-dk/sorts/merge_sort"
import "../../../ftSynthetic"

-- Sort i32 keys.
-- ==
-- entry: datagen_i32
-- input @data/data_i32.in
-- auto output

entry datagen_i32 [n]
	(zipf_order : []i64)
	(probs : [n]f64)
	(maxV : [1]i64)
	(s : [1]f64)
	(col1 : [n]i32)
	(col2 : [n]i32)
=
	let perK = (n + maxV[0] - 1) / maxV[0]
	let keys_ = iota (maxV[0])
		|> map (\k -> replicate perK k)
		|> flatten
		|> (\arr -> arr[0:n])
	let keys : [n]i64 =
		let hot_keys = zipf_order
			|> sized (maxV[0])
			|> zip (iota maxV[0])
			|> merge_sort (\(_,z1) (_,z2) -> z1<=z2)
			|> map (.0)
		in zipf_skewed keys_ hot_keys maxV[0] probs s[0] (f64.ceil s[0])
			|> merge_sort (<=)
	in (keys, maxV, col1, col2)


-- Sort i64 keys.
-- ==
-- entry: datagen_i64
-- input @data/data_i64.in
-- auto output

entry datagen_i64 [n]
	(zipf_order : []i64)
	(probs : [n]f64)
	(maxV : [1]i64)
	(s : [1]f64)
	(col1 : [n]i64)
	(col2 : [n]i64)
=
	let perK = (n + maxV[0] - 1) / maxV[0]
	let keys_ = iota (maxV[0])
		|> map (\k -> replicate perK k)
		|> flatten
		|> (\arr -> arr[0:n])
	let keys : [n]i64 =
		let hot_keys = zipf_order
			|> sized (maxV[0])
			|> zip (iota maxV[0])
			|> merge_sort (\(_,z1) (_,z2) -> z1<=z2)
			|> map (.0)
		in zipf_skewed keys_ hot_keys maxV[0] probs s[0] (f64.ceil s[0])
			|> merge_sort (<=)
	in (keys, maxV, col1, col2)