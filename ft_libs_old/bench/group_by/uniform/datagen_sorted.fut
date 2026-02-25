import "../../../lib/github.com/diku-dk/sorts/merge_sort"

-- Sort i32 keys.
-- ==
-- entry: datagen_i32
-- input @data/data_i32.in
-- auto output

entry datagen_i32 [n]
	(_ : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i32)
	(col2 : [n]i32)
=
	let perK = (n + maxV[0] - 1) / maxV[0]
	let keys = iota (maxV[0])
		|> map (\k -> replicate perK k)
		|> flatten
		|> (\arr -> arr[0:n])
	in (keys, maxV, col1, col2)


-- Sort i64 keys.
-- ==
-- entry: datagen_i64
-- input @data/data_i64.in
-- auto output

entry datagen_i64 [n]
	(_ : [n]i64)
	(maxV : [1]i64)
	(col1 : [n]i64)
	(col2 : [n]i64)
=
	let perK = (n + maxV[0] - 1) / maxV[0]
	let keys = iota (maxV[0])
		|> map (\k -> replicate perK k)
		|> flatten
		|> (\arr -> arr[0:n])
	in (keys, maxV, col1, col2)