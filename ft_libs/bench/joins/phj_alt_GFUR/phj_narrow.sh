#!/bin/bash

futhark_backend=cuda
runs_No=25

n1=$1
n2=$((2*n1))
b=$2

radix_bits=$3

combine=false
verbose=
#verbose='-v -v -v'

mkdir -p data

# Datagen
futhark dataset \
	-b -g \[$n1\]i64 \
	-b -g \[$n2\]i64 \
	-b -g \[$n1\]\[0\]u8 \
	-b -g \[$n2\]\[0\]u8 \
	--i64-bounds=$b:$b -b -g \[1\]i64 \
	--i32-bounds=$radix_bits:$radix_bits -b -g \[1\]i32 \
	> data/dat.in

futhark bench --backend=$futhark_backend --runs=1 datagen_wide.fut
rm -f data/*.in
mv "data/datagen_wide:do_datagen-data_dat.in.out" data/dat.in

# Transformation Phase
futhark bench --backend=$futhark_backend --runs=$runs_No test_transformation_narrow.fut
rm -f data/*.in
mv "data/test_transformation_narrow:do_transformation-data_dat.in.out" data/dat.in
# Join Phase - Matchfinding & Expansion
futhark bench --backend=$futhark_backend --runs=$runs_No test_matchfinding.fut
rm -f data/*


