import "lib/github.com/diku-dk/sorts/radix_sort"
import "ftbasics"

type sortTup [len] 't = {is: [len]i32, xs: [len]t}

-- sorts a column & couples it with the original indices
entry def sortColumn [] (xs: []i32) : sortTup i32 =
  let ixs : [](i32, i32) = xs |> zip (indices xs)
  let sorted_ixs : [](i32, i32) = blocked_radix_sort_int_by_key 256 (\ix -> ix.1) i32.num_bits i32.get_bit ixs
  let tup : ([]i32, []i32) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

-- sorts column on a function & couples it with the original indices
def sortColumnOn [] 't (p: a -> i32) (xs: []t) : sortTup t =
  let ixs : [](i32, t) = xs |> zip (indices xs)
  let sorted_ixs : [](i32, t) = blocked_radix_sort_int_by_key 256 (\ix -> p ix.1) i32.num_bits i32.get_bit ixs
  let tup : ([]i32, []t) = unzip sorted_ixs
  in {is = tup.0, xs = tup.1}

-- sort column on given indices (via a gather operation)
entry def orderByIndices 'a (is: []i32) (ys: []a) : []a =
  is |> gather ys
