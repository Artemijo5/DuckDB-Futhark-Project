-- Module for storing payload columns.
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
	let new_dat = cols.dat
		|> zip (iota rc)
		|> map (\(i,d) -> 
			if (i>=at_row && i<at_row+n)
			then (copy d) with [col_at:col_at+col_bs] = dat[i] |> sized col_bs
			else d
		)
	in {
		dat = new_dat,
		colBytes = cols.colBytes,
		colPrefix = cols.colPrefix
	}