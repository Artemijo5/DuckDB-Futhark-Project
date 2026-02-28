-- Routines for storing & handling payload columns.
-- Data is stored in row-major order, as a contiguous array of bytes (u8).

import "ftbasics"

-- | Type for storing multi-columnar data as a contiguous byte array.
type columns [row_count] [col_count] [byte_count] = {
	dat : [row_count][byte_count]u8,
	colBytes : [col_count]i64,
	colPrefix : [col_count]i64
}

-- | Function to allocate space for a columns instance.
def mk_columns [cc]
	(n_rows : i64)
	(bytes_per_col : [cc]i64)
: columns [n_rows] [cc] [i64.sum bytes_per_col] =
	let bc = i64.sum bytes_per_col
	in {
		dat = replicate n_rows (replicate bc 0),
		colBytes = bytes_per_col,
		colPrefix = bytes_per_col |> exscan (+) 0
	}

-- | Function to crop a contiguous section of a columns instance.
-- Section specified by offs (OFFSET) and limt (LIMIT), similar to an SQL query.
def crop_columns [rc] [cc] [bc]
	(offs : i64)
	(limt : i64)
	(cols : columns [rc] [cc] [bc])
: columns [i64.max 0 (i64.min limt (rc-offs))] [cc] [bc] =
	let new_rc = i64.max 0 (i64.min limt (rc-offs))
	let inf = i64.min rc offs
	let sup = i64.min rc (offs+limt)
	in {
		dat = cols.dat[inf:sup] |> sized new_rc,
		colBytes = cols.colBytes,
		colPrefix = cols.colPrefix
	}

-- | Function to read the values of a particular column.
-- Specifically reads n_rows rows, starting at at_row row.
def read_column [rc] [cc] [bc]
	(n_rows : i64)
	(at_row : i64)
	(at_col : i64)
	(cols : columns [rc] [cc] [bc])
: [][]u8 =
	let col_at = cols.colPrefix[at_col]
	let col_bs = cols.colBytes[at_col]
	in cols.dat[at_row:at_row+n_rows , col_at:col_at+col_bs]

-- | Function to write the values of a particular column.
-- Specifically writes n_rows rows, starting at at_row row.
-- If byte size of passed row is incorrect, throws runtime error.
def write_column [rc] [cc] [bc] [n]
	(at_row : i64)
	(at_col : i64)
	(dat : [n][]u8)
	(cols : columns [rc] [cc] [bc])
: columns [rc] [cc] [bc] =
	let col_at = cols.colPrefix[at_col]
	let col_bs = cols.colBytes[at_col]
	let new_dat = (copy cols.dat)
		with [at_row:at_row+n , col_at:col_at+col_bs]
		= (dat :> [n][col_bs]u8)
	in {
		dat = new_dat,
		colBytes = cols.colBytes,
		colPrefix = cols.colPrefix
	}

-- | Module type for managing a typed column (either numeric or tuple-based).
-- Used for the key column & columns handled individually.
module type keyCol = {
	type t

	val mk_keyCol : (n_rows : i64) -> [n_rows]t
	val update_keyCol [n] [n_upd] : i64 -> [n]t -> [n_upd]t -> [n]t
	val crop_keyCol [n] : i64 -> i64 -> [n]t -> []t
}
-- | Module for managing a numeric column.
-- Used for the key column & columns handled individually.
module keyCol_numeric (N : numeric) : keyCol with t = N.t = {
	type t = N.t
	local def ne = N.i32 0

	def mk_keyCol n_rows
	= replicate n_rows ne

	def update_keyCol [n] [n_upd]
		(at_row : i64)
		(new_dat: [n_upd]t)
		(keyCol : [n]t)
	= (copy keyCol) with [at_row:at_row+n_upd] = new_dat

	def crop_keyCol [n]
		(offs : i64)
		(limt : i64)
		(keyCol : [n]t)
	=
		let inf = i64.min n offs
		let sup = i64.min n (offs+limt)
		in keyCol[inf:sup]
}