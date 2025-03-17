import "lib/github.com/diku-dk/sorts/radix_sort"

def gather 'a (xs: []a) (is: []i32) =
  map (\i -> xs[i]) is

def key_index_pairs 'a (xk: []a) =
  zip xk (indices xk)

-- TODO later on, implement sort-merge algorithms with MergePath algorithm
-- left for later...
-- Merging the keys relies on the MergePath algorithm
-- https://liuyehcf.github.io/resources/paper/Merge-Path-Parallel-Merging-Made-Simple.pdf
--def merge_matrix 'a (xk: [](a, i32)) (yk: [](a, i32)) =
  -- xk serve as the vertical "indices" of the matrix
  -- yk as the horizontal ones
  -- produce a xk.size x yk.size matrix of all 0's
--  xk |> map (yk |> map (0)) -- TODO does this work???

--def radix_partition 'a (xp: []a) (numbuckets: i32) (bucketsize: i32) =
  -- aaaaand I'm stuck...
  -- gonna need some more reading to actually proceed (...)
  -- TODO do that
