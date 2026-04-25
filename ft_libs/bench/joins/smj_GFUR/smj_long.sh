#!/bin/bash

futhark_backend=cuda
runs_No=25
sort_entry="do_mergeSort_i64"
#sort_entry="do_radixSort_i64"

n1=$1
n2=$n1
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
	> data/dat_i64.in

futhark bench --backend=$futhark_backend --runs=1 datagen_long.fut --entry-point="do_datagen_i64"
rm -f data/*.in
mv "data/datagen_long:do_datagen_i64-data_dat_i64.in.out" data/dat_i64.in

# SMJ
if $combine ; then
	# Do all SMJ stages in one script.
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_SMJ_GFUR_i64.fut --entry-point="do_SMJ_i64"
	rm -f data/*
else
	# Sorting Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_sort_i64.fut --entry-point=$sort_entry
	rm -f data/*.in
	mv "data/test_sort_i64:$sort_entry-data_dat_i64.in.out" data/dat_i64.in
	# Match-finding Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_matchFinding_i64.fut --entry-point="do_matchfinding_i64"
	rm -f data/*.in
	mv "data/test_matchFinding_i64:do_matchfinding_i64-data_dat_i64.in.out" data/dat_i64.in
	# Expansion Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_expansion_i64.fut --entry-point="do_expansion_i64"
	rm -f data/*.in
	mv "data/test_expansion_i64:do_expansion_i64-data_dat_i64.in.out" data/dat_i64.in
	# Materialization Stage
	futhark bench $verbose --backend=$futhark_backend --runs=$runs_No test_materialization_i64.fut --entry-point="do_materialization_i64"
	rm -f data/*
fi
