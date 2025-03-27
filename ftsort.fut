import "ftbasics"

module sorter (CD: columnData) = {
  def sortColumn (col: [](CD.t)) =
    CD.sort col
}

-- TODO maybe use this?
entry sortColumn [n] 't (xs: [n]t) : sortInfo[n] t =
  sorter.sortColumn xs -- TODO does this work?

-- | Function to sort a column of integer type.
entry sortColumn_int [n] (xs: [n]i32) : sortInfo [n] i32 =
  (intData i32).sort xs
-- | Function to sort a column of long type.
entry sortColumn_long [n] (xs: [n]i64) : sortInfo [n] i64 =
  (intData i64).sort xs
-- | Function to sort a column of float type.
entry sortColumn_float [n] (xs: [n]f32) : sortInfo [n] f32 =
  (fltData f32).sort xs
-- | Function to sort a column of double type.
entry sortColumn_double [n] (xs: [n]f64) : sortInfo [n] f64 =
  (fltData f64).sort xs

-- | Order a payload column given the reordered indices.
entry def orderByIndices 't (is: []idx_t) (ys: []t) : []t =
  is |> gather ys
