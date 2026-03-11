import "lib/github.com/athas/vector/vector"

-- Functions to initialise and process vectors from multi-columnar data.

module vector_2  = cat_vector vector_1 vector_1
module vector_3  = cat_vector vector_2 vector_1
module vector_4  = cat_vector vector_2 vector_2
module vector_5  = cat_vector vector_3 vector_2
module vector_6  = cat_vector vector_3 vector_3
module vector_7  = cat_vector vector_4 vector_3
module vector_8  = cat_vector vector_4 vector_4
module vector_9  = cat_vector vector_5 vector_4
module vector_10 = cat_vector vector_5 vector_5
module vector_11 = cat_vector vector_6 vector_5
module vector_12 = cat_vector vector_6 vector_6

module vec_cols (V : vector) = {
	type vector 'a = a

	-- Input / Output
	-- Intended usage with duckdb:
	-- 1. create an empty cols with init_cols to hold the entire dataset.
	-- 2. for each datachunk, use create_cols & add to cols from 1.
	-- 3. once entire dataset is read, use crop_cols if needed.
	-- TODO examine alternative using vzip

		def init_cols 't (dummy: t) (n : i64) =
			replicate n (V.replicate dummy)

		def create_cols [n] 't (dat : [V.length][n]t) =
			replicate n (V.iota)
			|> zip (iota n)
			|> map (\(i,v) -> V.map (\j -> dat[j][i]) v)

		def set_col [n] 't (atCol : i64) (dat : [n]t) (vecs : *[n](V.vector t)) =
			(iota n)
			|> map (\i -> V.set atCol dat[i] vecs[i])

		def write_cols [n] 't (at_row : i64) (dat : [n](V.vector t)) (vecs : *[n](V.vector t))
			= vecs with [at_row:at_row+n] = dat

		def get_col [n] 't (col : i64) (vecs : [n](V.vector t)) =
			vecs |> map (V.get col)

		def read_col [n] 't (offs : i64) (limt : i64) (col : i64) (vecs : [n](V.vector t)) =
			let inf = i64.min n offs
			let sup = i64.min n (offs+limt)
			in vecs[inf:sup] |> map (V.get col)

		def crop_cols [n] 't (offs : i64) (limt : i64) (vecs : *[n](V.vector t)) =
			let inf = i64.min n offs
			let sup = i64.min n (offs+limt)
			in vecs[inf:sup]

	-- Operations

		def mapAll [n] 't 'ot
			(f : t -> ot)
			(vecs : [n](V.vector t))
		: [n](V.vector ot) =
			vecs |> map (V.map f)

		def reduceAll [n] 't
			(f : t -> t -> t)
			(ne : t)
			(vecs : [n](V.vector t))
		: [n]t =
			vecs |> map (V.reduce f ne)
}

module vcs1  = vec_cols vector_1
module vcs2  = vec_cols vector_2
module vcs3  = vec_cols vector_3
module vcs4  = vec_cols vector_4
module vcs5  = vec_cols vector_5
module vcs6  = vec_cols vector_6
module vcs7  = vec_cols vector_7
module vcs8  = vec_cols vector_8
module vcs9  = vec_cols vector_9
module vcs10 = vec_cols vector_10
module vcs11 = vec_cols vector_11
module vcs12 = vec_cols vector_12