import "lib/github.com/diku-dk/sorts/radix_sort"

def gather 'a (xs: []a) (is: []i32) =
  map (\i -> xs[i]) is

def isolate_keys 'a (xs: [][]a) =
  zip xs[0][:] (indices xs[0][:])
