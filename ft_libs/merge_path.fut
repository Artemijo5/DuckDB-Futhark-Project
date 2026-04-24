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

-- | For as, bs sorted,
-- for each partition of as, find the region of bs
-- it needs to be compared with for SMJ.
-- Returns a tuple of minimum & maximum (inclusive-exclusive) indices in bs,
-- per partition of as.
def merge_path_find_matching_partitions [na] [nb] [ts] 't
    (geq: t -> t -> bool)
    (leq: t -> t -> bool)
    (gt : t -> t -> bool)
    (lt : t -> t -> bool)
    (as : [na]t)
    (bs : [nb]t)
    (merge_path : [ts](i64,i64))
: [ts](i64,i64) =
    let (a_bounds, b_bounds) = unzip merge_path
    -- Filter 0-sized bs partitions
    let b_bounds' = indices b_bounds
        |> filter (\i ->
            (i==(ts-1) || b_bounds[i]!=b_bounds[i+1])
            && b_bounds[i]<nb
        )
        |> map (\i -> b_bounds[i])
    let ts' = length b_bounds'
    -- For each partition, get minimum & maximum values
    let a_ranges = indices a_bounds |> map (\i ->
        let inf = a_bounds[i]
        let sup = if i==(ts-1) then (na-1)
            else a_bounds[i+1]-1
        in (inf,sup)
    )
    let b_ranges = indices b_bounds' |> map (\i ->
        let inf = b_bounds'[i]
        let sup = if i==(ts'-1) then (nb-1)
            else b_bounds'[i+1]-1
        in (inf,sup)
    )
    -- Perform binary search
    let match_ranges = a_ranges |> bsearch_range
        (\(ia,iA) (ib,iB) -> iA>=ia && ia<na &&
            ((as[iA] `geq` bs[ib]) && (as[ia] `leq` bs[iB]))) -- this is true for the first match
        (\(ia,iA) (ib,_) -> ia==na ||
            (as[iA] `geq` bs[ib])) -- this is true for the last match
        (\(ia,_) (_,iB) -> ia==na ||
            (as[ia] `gt` bs[iB])) -- while looking for first match - compare with previous partition
        (\(ia,iA) (ib,_) -> ia<na &&
            (as[iA] `lt`  bs[ib])) -- while looking for last match - compare with next partition
        (replicate ts 0)
        b_ranges
    -- map to index ranges
    in match_ranges |> map (\(first_m, count) -> (first_m, first_m+count))
        |> map (\(first_m,last_m) -> if first_m<0 then (-1,-1) else
            (b_bounds'[first_m], if last_m==ts' then nb else b_bounds'[last_m])
        )

-- | Perform bsearch_range implementation using Merge-Path co-partitioning.
--
-- For as, bs sorted,
-- for each value in as,
-- find the index of the first match and the count of matches in bs.
def bsearch_range_merge_path [na] [nb] 't
    (eq : t -> t -> bool)
    (geq: t -> t -> bool)
    (leq: t -> t -> bool)
    (gt : t -> t -> bool)
    (lt : t -> t -> bool)
    (merge_path_threads : i64)
    (as : [na]t)
    (bs : [nb]t)
: [na](i64,i64) =
    -- Calculate the Merge-Path.
    let merge_path = find_merge_path
        (geq) (lt) as bs merge_path_threads
    -- Get search regions in bs per partition in as
    let part_match = merge_path_find_matching_partitions
        (geq) (leq) (gt) (lt) as bs merge_path
    -- Find the partition id of each element in as
    -- ie the last partition where part_index <= element_index
    let a_bounds = merge_path |> map (.0)
    let (min_is,max_is) = indices as |> bsearch_last
        (>=) (<) (replicate na 0) a_bounds
        |> map (\i -> part_match[i])
        |> unzip
    in as |> bsearch_range_bounded
        (eq) (geq) (gt) (lt) min_is max_is bs