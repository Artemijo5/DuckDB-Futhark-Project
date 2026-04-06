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
	-- 1. Create each column individually using typedCol.
	-- 2. Create an "empty" vec_cols using init_cols.
	-- 3. Use set_col for each column.
	--
	-- Alternatively
	-- 1. Make an "empty" vec_cols using init_cols.
	-- 2. For each datachunk, write its columns in a contiguous array.
	-- 3. Use create_cols on that array.
	-- 4. Use write_cols to write the new datachunk in the vec_cols from 1.
	-- 5. Use crop_cols to crop to the final size.
	--
	-- The alternative approach is probably better if any_vector is used,
	-- as set_col would have to copy all the vector fields in that case,
	-- and the data would get transposed anyway.

		def init_cols 't (dummy: t) (n : i64) =
			replicate n (V.replicate dummy)

		def create_cols 't (dat : [][]t) =
			dat
			|> transpose
			|> map (sized V.length)
			|> map (V.from_array)

		def set_col [n] 't (atCol : i64) (dat : [n]t) (vecs : *[n](V.vector t)) =
			map2 (\x vec -> V.set atCol x vec) dat vecs

		def write_cols [n] 't (at_row : i64) (dat : [n](V.vector t)) (vecs : *[](V.vector t))
			= vecs with [at_row:at_row+n] = dat

		def get_col 't (col : i64) (vecs : [](V.vector t)) =
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
			(ne: t)
			(vecs : [n](V.vector t))
		: [n]t =
			vecs |> map (V.reduce f ne)

		def foldlAll [n] 't
			(f : t -> t -> t)
			(ne: t)
			(vecs : [n](V.vector t))
		: [n]t =
			loop accs = replicate n ne
			for d < V.length do
				map2 (\acc vec ->
					f acc (V.get d vec)
				)
				accs
				vecs

		def check_all [n] 't
			(f : t -> bool)
			(vecs : [n](V.vector t))
		: [n]bool =
			vecs |> mapAll f |> foldlAll (&&) true

		def check_any [n] 't
			(f : t -> bool)
			(vecs : [n](V.vector t))
		: [n]bool =
			vecs |> mapAll f |> foldlAll (||) false
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

-- | Module type for making array-based columns.
module type array_cols = {
	type t

	val init_cols : i64 -> i64 -> [][]t

	val write_col [d] [n] [w] : i64 -> i64 -> *[d][n]t -> [w]t -> [d][n]t

	val read_col [d] [n] : i64 -> i64 -> i64 -> [d][n]t -> []t

	val set_col [d] [n] : i64 -> *[d][n]t -> [n]t -> [d][n]t

	val get_col [d] [n] : i64 -> [d][n]t -> [n]t

	val crop_cols [d] [n] : i64 -> i64 -> *[d][n]t -> [d][]t
}

module array_cols_numeric (N : numeric) : array_cols with t = N.t = {
	type t = N.t

	local def zero = N.i32 0

	def init_cols n d = replicate d (replicate n zero)

	def write_col [d] [n] (dim:i64) offs (cols : *[d][n]t) col =
		cols with [dim,offs:offs+(length col)] = col

	def read_col (dim:i64) offs limt cols =
		let inf = i64.min (length cols) offs
		let sup = i64.min (inf+limt) (length cols)
		in cols[dim,inf:sup]

	def set_col [d] [n] (dim:i64) (cols : *[d][n]t) col = cols with [dim] = col

	def get_col (dim:i64) cols = cols[dim]

	def crop_cols [d] [n] offs limt (cols : *[d][n]t) =
		let inf = i64.min (length cols) offs
		let sup = i64.min (inf+limt) (length cols)
		in cols[:,inf:sup]
}