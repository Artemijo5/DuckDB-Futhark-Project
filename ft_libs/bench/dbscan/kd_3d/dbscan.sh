#!/bin/bash

backend='cuda'
num_runs=25

# Number of points
n=$1
# kd-index depth
d=$2
# max value per dimension
v=$3
# eps
eps=$4
# minPts
minPts=$5
# extPar
extPar=$6

mkdir -p data

# Create test dataset
futhark dataset \
	--f64-bounds="0:$v"            -b -g \[3\]\[$n\]f64 \
	--i64-bounds="$d:$d"           -b -g \[1\]i64 \
	--f64-bounds="$eps:$eps"       -b -g \[1\]f64 \
	--i64-bounds="$minPts:$minPts" -b -g \[1\]i64 \
	--i64-bounds="$extPar:$extPar" -b -g \[1\]i64 \
> data/dat.in

# Index dataset
futhark bench test_indexing.fut --backend=$backend --runs=$num_runs
rm -f data/*.in
mv 'data/test_indexing:do_kd_index-data_dat.in.out' data/dat.in

# Do DBSCAN
futhark bench test_dbscan.fut --backend=$backend --runs=$num_runs
rm -f data/*