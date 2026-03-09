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

	def init_cols 't (dummy: t) (n : i64) =
		replicate n (V.replicate dummy)

	def set_col [n] 't (atCol : i64) (dat : [n]t) (vecs : *[n](V.vector t)) =
		(iota n)
		|> map (\i -> V.set atCol dat[i] vecs[i])

	def get_col [n] 't (col : i64) (vecs : [n](V.vector t)) =
		vecs |> map (V.get col)

	def create_cols [n] 't (dat : [V.length][n]t) =
		replicate n (V.iota)
		|> zip (iota n)
		|> map (\(i,v) -> V.map (\j -> dat[j][i]) v)

	-- TODO
	-- figure out map, reduction, hist, and other operations I'll need to do
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