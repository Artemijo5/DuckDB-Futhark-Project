-- Routines for storing & handling payload columns.
-- Data is stored in row-major order, as a contiguous array of bytes (u8).

import "ftbasics"

-- | Multi-columnar schema (ie bytes per column).
type colInfo [col_count] = {
	total_bytes : i64,
	colBytes : [col_count]i64,
	colPrefix : [col_count]i64
}

-- | Multi-columnar data stored as a contiguous byte array.
type~ columns [row_count] = [row_count][]u8

-- | Function to create a columnar schema.
def mk_columns [cc]
	(bytes_per_col : [cc]i64)
: colInfo [cc] =
	{
		total_bytes = i64.sum bytes_per_col,
		colBytes = bytes_per_col,
		colPrefix = bytes_per_col |> exscan (+) 0
	}

-- | Function to allocate space for columnar data, using colInfo.
-- Initialized as all-zeroes.
def init_columns [cc]
	(row_count : i64)
	(cInfo : colInfo [cc])
: columns [row_count] =
	replicate row_count (replicate cInfo.total_bytes 0u8)

-- | Function to crop a contiguous section of a columns instance.
-- Section specified by offs (OFFSET) and limt (LIMIT), similar to an SQL query.
-- NOTE: consumes the original columns.
-- TODO will need to see how this works in the C API (...)
def crop_columns [rc]
	(offs : i64)
	(limt : i64)
	(cols : *columns [rc])
: columns [i64.max 0 (i64.min limt (rc-offs))] =
	let new_rc = i64.max 0 (i64.min limt (rc-offs))
	let inf = i64.min rc offs
	let sup = i64.min rc (offs+limt)
	in cols[inf:sup] |> sized new_rc

-- | Function to read the values of a particular column.
-- Specifically reads n_rows rows, starting at at_row row.
-- Section specified by offs (OFFSET) and limt (LIMIT), similar to an SQL query.
def read_column [rc] [cc]
	(offs : i64)
	(limt : i64)
	(at_col : i64)
	(cInfo: colInfo [cc])
	(cols : columns [rc])
: [i64.max 0 (i64.min limt (rc-offs))][]u8 =
	let new_rc = i64.max 0 (i64.min limt (rc-offs))
	let col_at = cInfo.colPrefix[at_col]
	let col_bs = cInfo.colBytes[at_col]
	let inf = i64.min rc offs
	let sup = i64.min rc (offs+limt)
	in cols[inf:sup , col_at:col_at+col_bs]
		|> sized new_rc

-- | Function to write the values of a particular column.
-- Specifically writes n_rows rows, starting at at_row row.
-- If byte size of passed row is incorrect, throws runtime error.
-- NOTE: consumes the original columns.
-- TODO will need to see how this works in the C API (...)
def write_column [rc] [cc] [n]
	(at_row : i64)
	(at_col : i64)
	(dat : [n][]u8)
	(cInfo: colInfo [cc])
	(cols : *columns [rc])
: columns [rc] =
	let col_at = cInfo.colPrefix[at_col]
	let col_bs = cInfo.colBytes[at_col]
	in cols with [at_row:at_row+n , col_at:col_at+col_bs] = dat

-- | Module type for managing a typed column (either numeric or tuple-based).
-- Used for the key column & columns handled individually.
module type typedCol = {
	type t

	val mk_keyCol : (n_rows : i64) -> [n_rows]t
	val update_keyCol [n] [n_upd] : i64 -> [n_upd]t -> *[n]t -> [n]t
	val crop_keyCol [n] : i64 -> i64 -> *[n]t -> []t
	val read_keyCol [n] : i64 -> i64 ->  [n]t -> []t
}
-- | Parametric module for managing a numeric column.
-- Used for the key column & columns handled individually.
module col_numeric (N : numeric) : typedCol with t = N.t = {
	type t = N.t
	local def ne = N.i32 0

	def mk_keyCol n_rows
	= replicate n_rows ne

	def update_keyCol [n] [n_upd]
		(at_row : i64)
		(new_dat: [n_upd]t)
		(keyCol : *[n]t)
	= keyCol with [at_row:at_row+n_upd] = new_dat

	def crop_keyCol [n]
		(offs : i64)
		(limt : i64)
		(keyCol : *[n]t)
	=
		let inf = i64.min n offs
		let sup = i64.min n (offs+limt)
		in keyCol[inf:sup]

	def read_keyCol [n]
		(offs : i64)
		(limt : i64)
		(keyCol : [n]t)
	=
		let inf = i64.min n offs
		let sup = i64.min n (offs+limt)
		in keyCol[inf:sup]
}