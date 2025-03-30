import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

-- Local modules for sorting functions
local module shortSorter = intData i16
local module intSorter = intData i32
local module longSorter = intData i64
local module floatSorter = fltData f32
local module doubleSorter = fltData f64
-- Order a payload column given the reordered indices.
local def orderByIndices 't (is: [](idx_t.t)) (ys: []t) : []t =
  is |> gather ys

-- | Sort a column of short type.
entry sortColumn_short [n] (xs: [n]i16) : sortInfo_short [n] =
  shortSorter.sort xs
-- | Sort a column of integer type.
entry sortColumn_int [n] (xs: [n]i32) : sortInfo_int [n] =
  intSorter.sort xs
-- | Sort a column of long type.
entry sortColumn_long [n] (xs: [n]i64) : sortInfo_long [n] =
  longSorter.sort xs
-- | Sort a column of float type.
entry sortColumn_float [n] (xs: [n]f32) : sortInfo_float [n] =
  floatSorter.sort xs
-- | Sort a column of double type.
entry sortColumn_double [n] (xs: [n]f64) : sortInfo_double [n] =
  doubleSorter.sort xs

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short (is: [](idx_t.t)) (ys: []i16) = ys |> orderByIndices is
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int (is: [](idx_t.t)) (ys: []i32) = ys |> orderByIndices is
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long (is: [](idx_t.t)) (ys: []i64) = ys |> orderByIndices is
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float (is: [](idx_t.t)) (ys: []f32) = ys |> orderByIndices is
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double (is: [](idx_t.t)) (ys: []f64) = ys |> orderByIndices is
