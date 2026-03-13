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

# Partition the 1st
futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_partitionFirst.fut
rm -f data/*.in
mv "data/test_partitionFirst:do_partition1-data_dat.in.out" data/dat.in
# Construct partitionInfo
futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_partInfo.fut
rm -f data/*.in
mv "data/test_partInfo:do_partInfo-data_dat.in.out" data/dat.in
# Construct Hash Table
futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_hashTbl.fut
rm -f data/*.in
mv "data/test_hashTbl:do_hashTbl-data_dat.in.out" data/dat.in
# Partition the 2nd
futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_partitionSecond.fut
rm -f data/*

