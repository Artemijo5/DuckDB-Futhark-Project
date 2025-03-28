import "ftbasics"


-- Local modules for sorting functions
local module intSorter = intData i32
local module longSorter = intData i64
local module floatSorter = fltData f32
local module doubleSorter = fltData f64

-- | Function to sort a column of integer type.
entry sortColumn_int [n] (xs: [n]i32) : sortInfo [n] i32 =
  intSorter.sort xs
-- | Function to sort a column of long type.
entry sortColumn_long [n] (xs: [n]i64) : sortInfo [n] i64 =
  longSorter.sort xs
-- | Function to sort a column of float type.
entry sortColumn_float [n] (xs: [n]f32) : sortInfo [n] f32 =
  floatSorter.sort xs
-- | Function to sort a column of double type.
entry sortColumn_double [n] (xs: [n]f64) : sortInfo [n] f64 =
  doubleSorter.sort xs


-- | Order a payload column given the reordered indices.
entry def orderByIndices 't (is: []idx_t) (ys: []t) : []t =
  is |> gather ys
