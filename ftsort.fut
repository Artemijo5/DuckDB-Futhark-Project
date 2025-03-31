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
entry sortColumn_short [n] (incr: idx_t.t) (xs: [n]i16) : sortInfo_short [n] =
  xs |> shortSorter.sort incr
-- | Sort a column of integer type.
entry sortColumn_int [n] (incr: idx_t.t) (xs: [n]i32) : sortInfo_int [n] =
  xs |> intSorter.sort incr
-- | Sort a column of long type.
entry sortColumn_long [n] (incr: idx_t.t) (xs: [n]i64) : sortInfo_long [n] =
  xs |> longSorter.sort incr
-- | Sort a column of float type.
entry sortColumn_float [n] (incr: idx_t.t) (xs: [n]f32) : sortInfo_float [n] =
  xs |> floatSorter.sort incr
-- | Sort a column of double type.
entry sortColumn_double [n] (incr: idx_t.t) (xs: [n]f64) : sortInfo_double [n] =
  xs |> doubleSorter.sort incr

-- | Order a payload column of type short, given the reordered indices.
entry orderByIndices_short [n] (incr: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i16) =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type int, given the reordered indices.
entry orderByIndices_int [n] (incr: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i32) =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type long, given the reordered indices.
entry orderByIndices_long [n] (incr: idx_t.t) (is: [n](idx_t.t)) (ys: [n]i64) =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type float, given the reordered indices.
entry orderByIndices_float [n] (incr: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f32) =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
-- | Order a payload column of type double, given the reordered indices.
entry orderByIndices_double [n] (incr: idx_t.t) (is: [n](idx_t.t)) (ys: [n]f64) =
  let offset_is : [n](idx_t.t) = is |> map (\j -> j - incr)
  in ys |> orderByIndices offset_is
