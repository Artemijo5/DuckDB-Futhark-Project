import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

type sortTup [len] 't = {is: [len]i32, xs: [len]t}

-- sorts xs by given function
def smart_sorter 'k 't (p : t -> k) (xs : []t) : []t =
  -- TODO placeholder -- use appropriate sorting function by matching to the type t
  xs
  -- case i32: blocked_radix_sort_int_by_key 256 (\ix -> p ix.1) i32.num_bits i32.get_bit ixs

-- sorts column on a function & couples it with the original indices
def sortColumnOn [] 'k 't (p : t -> k) (xs : []t) : sortTup t =
  let sorted_xs = xs
    |> zip (i32.i64 (indices xs))
    |> smart_sorter (\ix -> p ix.1)
  let tup : ([]i32, []t) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

-- sorts a column & couples it with the original indices
entry def sortColumn 't [] (xs : []t) : sortTup t =
  xs |> sortColumnOn (\x -> x)

-- placeholder functions specifically for i32:
def sortIntColOn (p : i32 -> i32) (xs : []i32) : sortTup i32 =
  let sorted_xs = xs
    |> zip (i32.i64 (indices xs))
    |> (blocked_radix_sort_int_by_key 256 (\ix -> p ix.1) i32.num_bits i32.get_bit)
  let tup : ([]i32, []i32) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

entry def sortIntCol (xs : []i32) : sortTup i32 = xs |> sortIntColOn (\x -> x)

-- sort column on given indices (via a gather operation)
entry def orderByIndices 't (is: []i32) (ys: []t) : []t =
  is |> gather ys
