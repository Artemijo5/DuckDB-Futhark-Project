import "ftbasics"

-- Local modules for sorting functions
local module shortSorter = intData i16
local module intSorter = intData i32
local module longSorter = intData i64
local module floatSorter = fltData f32
local module doubleSorter = fltData f64
-- Order a payload column given the reordered indices.
local def orderByIndices 't (is: []idx_t) (ys: []t) : []t =
  is |> gather ys

-- | Sort a column of short type.
entry sortColumn_short [n] (xs: [n]i16) : sortInfo [n] i16 =
  shortSorter.sort xs
-- | Sort a column of integer type.
entry sortColumn_int [n] (xs: [n]i32) : sortInfo [n] i32 =
  intSorter.sort xs
-- | Sort a column of long type.
entry sortColumn_long [n] (xs: [n]i64) : sortInfo [n] i64 =
  longSorter.sort xs
-- | Sort a column of float type.
entry sortColumn_float [n] (xs: [n]f32) : sortInfo [n] f32 =
  floatSorter.sort xs
-- | Sort a column of double type.
entry sortColumn_double [n] (xs: [n]f64) : sortInfo [n] f64 =
  doubleSorter.sort xs

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short = orderByIndices i16
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int = orderByIndices i32
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long = orderByIndices i64
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float = orderByIndices f32
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double = orderByIndices f64
