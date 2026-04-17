#!/bin/bash

futhark_backend=cuda
runs_No=25

n1=$1
n2=$n1
b=$2
pL_b=$3

radix_bits=$4

maxval=$5

combine=false
verbose=
#verbose='-v -v -v'

mkdir -p data

# Datagen
futhark dataset \
	-b -g \[$n1\]i64 \
	-b -g \[$n2\]i64 \
	-b -g \[$n1\]\[$pL_b\]u8 \
	-b -g \[$n2\]\[$pL_b\]u8 \
	--i64-bounds=$b:$b -b -g \[1\]i64 \
	--i32-bounds=$radix_bits:$radix_bits -b -g \[1\]i32 \
	--i64-bounds=$maxval:$maxval -b -g \[1\]i64 \
	> data/dat.in

futhark bench --backend=$futhark_backend --runs=1 datagen_many.fut
rm -f data/*.in
mv "data/datagen_many:do_datagen-data_dat.in.out" data/dat.in

# Transformation Phase
futhark bench --backend=$futhark_backend --runs=$runs_No test_transformation.fut
rm -f data/*.in
mv "data/test_transformation:do_transformation-data_dat.in.out" data/dat.in
# Join Phase - Matchfinding & Expansion
futhark bench --backend=$futhark_backend --runs=$runs_No test_matchfinding.fut
rm -f data/*.in
mv "data/test_matchfinding:do_matchfinding-data_dat.in.out" data/dat.in
# Materialization
futhark bench --backend=$futhark_backend --runs=$runs_No test_materialization.fut
rm -f data/*


