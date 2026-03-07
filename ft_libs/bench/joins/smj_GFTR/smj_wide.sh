#! /bin/bash

futhark_backend=cuda
runs_No=25
sort_entry="do_mergeSort_i32"

n1=$1
n2=$((2*n1))
b=$2
#If any third argument is given, do all SMJ in one script.
combine=$3

mkdir -p data

# Datagen
futhark dataset \
	-b -g \[$n1\]i64 \
	-b -g \[$n2\]i64 \
	-b -g \[$n1\]\[$b\]u8 \
	-b -g \[$n2\]\[$b\]u8 \
	> data/dat_i32.in

futhark bench --backend=$futhark_backend --runs=$runs_No datagen_wide.fut --entry-point="do_datagen_i32"
rm -f data/*.in
mv "data/datagen_wide:do_datagen_i32-data_dat_i32.in.out" data/dat_i32.in

# SMJ
if [ $combine ]; then
	# Do all SMJ stages in one script.
	futhark bench --backend=$futhark_backend --runs=$runs_No test_SMJ_GFTR.fut --entry-point="do_SMJ_i32"
	rm -f data/*
else
	# Sorting Stage
	futhark bench --backend=$futhark_backend --runs=$runs_No test_sort.fut --entry-point=$sort_entry
	rm -f data/*.in
	mv "data/test_sort:$sort_entry-data_dat_i32.in.out" data/dat_i32.in
	# Match-finding Stage
	futhark bench --backend=$futhark_backend --runs=$runs_No test_matchFinding.fut --entry-point="do_matchfinding_i32"
	rm -f data/*.in
	mv "data/test_matchFinding:do_matchfinding_i32-data_dat_i32.in.out" data/dat_i32.in
	# Expansion Stage
	futhark bench --backend=$futhark_backend --runs=$runs_No test_expansion.fut --entry-point="do_expansion_i32"
	rm -f data/*.in
	mv "data/test_expansion:do_expansion_i32-data_dat_i32.in.out" data/dat_i32.in
	# Materialization Stage
	futhark bench --backend=$futhark_backend --runs=$runs_No test_materialization.fut --entry-point="do_materialization_i32"
	rm -f data/*
fi
