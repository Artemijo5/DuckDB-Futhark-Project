import "lib/github.com/diku-dk/sorts/radix_sort"

-- TODO when using commented code for radixSort, compiler issues
-- TODO report on github

-- | Type used to sort a key column (GFTR), simultaneously transforming the payloads (stored as a byte array).
type sortStruct [n] [b] 'a = {k: [n]a, pL: [n*b]u8}

type sortStruct_int [n] [b] = sortStruct [n] [b] i32

local def radixSortRelation [n] [b] 'a 
  (block_size: i16)
  (xs: sortStruct [n] [b] a)
  --  (keyed_sortFunc :
  --    i16 -> ((a,[b]u8) -> a) -> i32 -> (i32 -> a -> i32) -> [n](a,[b]u8) -> [n](a,[b]u8))
  (num_bits: i32)
  (get_bit: i32 -> a -> i32)
 : sortStruct [n] [b] a =
  let reshaped_pL : [n][b]u8 = xs.pL |> unflatten
  let xys : [n](a, [b]u8) = zip xs.k reshaped_pL --  let sorted_xys = keyed_sortFunc block_size (\xy -> xy.0) num_bits get_bit xys
  let sorted_xys = blocked_radix_sort_int_by_key block_size (\xy -> xy.0) num_bits get_bit xys
  let un_xys : ([n]a, [n][]u8) = unzip sorted_xys
  in {k = un_xys.0, pL = un_xys.1 |> flatten}

entry radixSortRelation_int [n]
  (block_size: i16)
  (payload_bytes: i64)
  (xs: sortStruct_int [n] [payload_bytes])
 : sortStruct_int [n] [payload_bytes]
  = radixSortRelation (block_size) (xs) (i32.num_bits) (i32.get_bit) --  = radixSortRelation (block_size) (xs) (blocked_radix_sort_int_by_key) (i32.num_bits) (i32.get_bit)