import "lib/github.com/diku-dk/sorts/merge_sort"

-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
type sortStruct [n] [b] 'a = {k: [n]a, pL: [n*b]u8}

type sortStruct_int [n] [b] = sortStruct [n] [b] i32

local def mergeSortRelation [n] [b] 'a
  (xs: sortStruct [n] [b] a)
  (leq: a -> a -> bool)
 : sortStruct [n] [b] a =
  let reshaped_pL : [n][b]u8 = xs.pL |> unflatten
  let xys : [n](a, [b]u8) = zip xs.k reshaped_pL
  let sorted_xys = merge_sort_by_key (\xy -> xy.0) (leq) xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1 |> flatten}

entry mergeSortRelation_int [n]
  (payload_bytes: i64)
  (xs: sortStruct_int [n] [payload_bytes])
 : sortStruct_int [n] [payload_bytes]
  = mergeSortRelation (xs) (<=)