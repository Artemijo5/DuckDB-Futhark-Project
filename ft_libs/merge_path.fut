import "ftbasics"

-- Merge-Path Algorithm.
-- See:
-- Merge Path - Parallel Merging Made Simple
-- Saher Odeh, Oded Green, Oz Shmueli, Yitzhak Birk
-- Electrical Engineering Departent
-- Technion
-- Haifa, Israel
-- 2012 IEEE
-- 26th International Parallel and Distributed Processing Symposium Workshops & PhD Forum

-- | Get the i'th diagonal in the Merge-Path matrix,
-- represented by its minimum & maximum indices in arrays A,B.
--
-- The first diagonal is ((0,0),(0,0)).
-- We obtain the i'th array by adding i to the max values,
-- and truncating according to the Merge-Path matrix's shape.
def get_diagonal
    (size_A : i64)
    (size_B : i64)
    (i : i64)
: ((i64,i64),(i64,i64)) =
    let maxA = i64.min (size_A-1) i
    let maxB = i64.min (size_B-1) i
    let minA = i64.min (size_A-1) (i64.max 0 (i - size_B + 1))
    let minB = i64.min (size_B-1)( i64.max 0 (i - size_A + 1))
    in ((minA,minB),(maxA,maxB))

-- | Move the point across the diagonal.
-- First point is (minA, maxB).
-- Moving the point by i adds i to its A coordinate,
-- and subtracts i from its B coordinate.
def move_across_diag
    (diag : ((i64,i64),(i64,i64)))
    (pt : (i64,i64))
    (step : i64)
: (i64,i64) =
    let ((minA,minB),(maxA,maxB)) = diag
    let a = i64.min maxA (i64.max minA (pt.0 + step))
    let b = i64.min maxB (i64.max minB (pt.1 - step))
    in (a,b)

-- | Find the point where the diagonal meets the Merge Path,
-- which is the first index where A >= B.
-- This is done via a binary search.
--
-- Implementation ensures A is always on the left side of comparisons.
def search_diagonal [na] [nb] 't
    (geq : t -> t -> bool)
    (lt  : t -> t -> bool)
    (as : [na]t)
    (bs : [nb]t)
    (diag : ((i64,i64),(i64,i64)))
=
    let ((minA,minB),(maxA,maxB)) = diag
    let diag_len = maxA-minA+1
    -- numIter = 1 + ceil(log2(diag_len))
    let sub_from = if (diag_len)&(diag_len-1)==0 then 64i32 else 65i32
    let numIter = i64.i32 (sub_from - (i64.clz diag_len))
    let (foundAt,_) = loop (cur_pt,j) = ((maxA,minB),1)
    while j <= numIter do
        let step = ((diag_len + (2 ** j) - 1) / (2 ** j))
        -- First match where:
        -- i1 : current point in diagonal
        -- i0 : previous point in diagonal
        -- 1. a[i1] >= b[i1]
        -- 2. a[i0] <  b[i0] || i1 is the first point
        -- It is possible that a[i1] < b[i1] across entire diagonal
        -- In that case return (minA, maxB)
        let cur_geq = as[cur_pt.0] `geq` bs[cur_pt.1]
        let prv_pt  = move_across_diag diag cur_pt (-1)
        let prv_lt  = (cur_pt.0 == minA && cur_pt.1 == maxB)
            || (as[prv_pt.0] `lt` bs[prv_pt.1])
        in
            if (cur_geq && prv_lt)
                then (if cur_pt.1!=0 then (cur_pt.0,cur_pt.1+1) else cur_pt, numIter+1)
            else if prv_lt
                then (move_across_diag diag cur_pt step, j+1)
            else (move_across_diag diag cur_pt (i64.neg step), j+1)
    in foundAt

-- | For as, bs sorted,
-- returns boundaries of co-partitions using Merge-Path.
-- These split each of as, bs into ts segments.
def find_merge_path [na] [nb] 't
    (geq : t -> t -> bool)
    (lt  : t -> t -> bool)
    (as : [na]t)
    (bs : [nb]t)
    (ts : i64)
: [ts](i64,i64) =
    let diags = iota ts
        |> map (\i -> i*(na+nb-1)/ts)
        |> map (get_diagonal na nb)
    let segments = diags
        |> map (search_diagonal geq lt as bs)
    in segments

-- TODO
-- figure out exactly how to use results in SMJ (...)

-- probably
-- do a small binary search among partitions to find first & last match in B's per A's
-- and use those to set init_is in final binary search?
-- to set init_is, either use scatter & scan, or binary search, or expand
-- binary search might be faster, assuming merge_path is small
-- finally, for all elements in R, perform bsearch_range in the respective range (...)