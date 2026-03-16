#!/bin/bash

futhark_backend=cuda
runs_No=25
sort_entry="do_radixSort_i32"

n1=$1
n2=$((2*n1))
b=$2

combine=false
verbose=
#verbose='-v -v -v'

mkdir -p data

# Datagen
futhark dataset \
	-b -g \[$n1\]i64 \
	-b -g \[$n2\]i64 \
	-b -g \[$n1\]\[$b\]u8 \
	-b -g \[$n2\]\[$b\]u8 \
	> data/dat_i32.in

futhark bench --backend=$futhark_backend --runs=1 datagen_wide.fut --entry-point="do_datagen_i32"
rm -f data/*.in
mv "data/datagen_wide:do_datagen_i32-data_dat_i32.in.out" data/dat_i32.in

# SMJ
if $combine ; then
	# Do all SMJ stages in one script.
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_SMJ_GFUR_i32.fut --entry-point="do_SMJ_i32"
	rm -f data/*
else
	# Sorting Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_sort_i32.fut --entry-point=$sort_entry
	rm -f data/*.in
	mv "data/test_sort_i32:$sort_entry-data_dat_i32.in.out" data/dat_i32.in
	# Match-finding Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_matchFinding_i32.fut --entry-point="do_matchfinding_i32"
	rm -f data/*.in
	mv "data/test_matchFinding_i32:do_matchfinding_i32-data_dat_i32.in.out" data/dat_i32.in
	# Expansion Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_expansion_i32.fut --entry-point="do_expansion_i32"
	rm -f data/*.in
	mv "data/test_expansion_i32:do_expansion_i32-data_dat_i32.in.out" data/dat_i32.in
	# Materialization Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_materialization_i32.fut --entry-point="do_materialization_i32"
	rm -f data/*
fi
