import "lib/github.com/diku-dk/sorts/radix_sort"

def gather 'a (xs: []a) (is: []i32) =
  map (\i -> xs[i]) is

def key_index_pairs 'a (xk: []a) : [](a, i32) =
  zip xk (indices xk)

def search_for_matches 'a (val: a) (arr: [](a, i32)) (from: i32) (to: i32) : (i32, i32) =
  -- Search for matches to val in array arr[from:to] (start inclusive, end exclusive)
  -- array arr has been zipped with its original indices and sorted
  -- returns a tuple (first, last) of the range of matches (start inclusive, end exclusive)
  -- if no match found: return (-1, -1)
  -- idea: scan & scatter 
  first : i32 = -1
  last : i32 = -1
  barr : [](a, i32) = arr[from:to]
    |> map (\x -> x.0)
    |> zip (from..<to)
    |> filter (\x -> x.0 == val)
  -- TODO could it be done more efficiently with map-reduce? prob not since results of filter are what is wanted...
  -- TODO find some way to do this efficiently -- for each partition rather than for each val? if possible...
  -- of course, consider doing array programming magic to do it in a 'proper' way as in the papers...
  -- might be hard to figure out...
  if (length barr) > 0 then first = barr[0].1 else first = -1
  if (length barr) > 0 then last = barr[-1].1 + 1 else last = -1
  (first, last)

def partitionIndices 'a (xs: [](a, i32)) (p: a -> i32) (noPartitions: i32) : []i32 =
  -- obtain partition indices of array xs
  -- xs is an array that has been sorted, and zipped with its indices
  -- that is, the indices where each partition starts
  xpi: [](i32, i32) = xs
    |> map (\x -> p x.0)
    |> zip (indices xs)
  -- id's = argmin each partition id from xpi
  -- TODO remove duplicates from xpi, keeping the smallest index instance each time
  -- then save indices in order, return as [noPartitions] array

  -- TODO for now I'll be working on simpler single-table stuff, once I'm done there I'll return to joins...

-- TODO later on, implement sort-merge algorithms with MergePath algorithm
-- left for later...
-- Merging the keys relies on the MergePath algorithm
-- https://liuyehcf.github.io/resources/paper/Merge-Path-Parallel-Merging-Made-Simple.pdf
--def merge_matrix 'a (xk: [](a, i32)) (yk: [](a, i32)) =
  -- xk serve as the vertical "indices" of the matrix
  -- yk as the horizontal ones
  -- produce a xk.size x yk.size matrix of all 0's
--  xk |> map (replicate (length yk) 0)

--def radix_partition 'a (xp: []a) (numbuckets: i32) (bucketsize: i32) =
  -- aaaaand I'm stuck...
  -- gonna need some more reading to actually proceed (...)
  -- TODO do that
