import "ftbasics"

-- | Function to sort a column of integer type.
entry sortColumn_int [n] (xs: [n]i32) : sortInfo [n] t =
  (intData i32).sort xs
-- | Function to sort a column of long type.
entry sortColumn_long [n] (xs: [n]i64) : sortInfo [n] t =
  (intData i64).sort xs
-- | Function to sort a column of float type.
entry sortColumn_float [n] (xs: [n]f32) : sortInfo [n] t =
  (fltData f32).sort xs
-- | Function to sort a column of double type.
entry sortColumn_double [n] (xs: [n]f64) : sortInfo [n] t =
  (fltData f64).sort xs

-- | Order a payload column given the reordered indices.
entry def orderByIndices 't (is: []i32) (ys: []t) : []t =
  is |> gather ys
