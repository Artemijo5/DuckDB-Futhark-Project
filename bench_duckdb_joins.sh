#!/bin/bash

type=$1 # INTEGER or BIGINT
R_size=$2
S_size=$3
num_pL=$4
num_iter=$5
skew=$6 # optional argument

LD_LIBRARY_PATH=$(pwd):$(pwd)/clibs:$(pwd)/ft_clibs:$(pwd)/c_tests:$(pwd)/c_tests/src:$(pwd)/c_tests/src/dbscan:$LD_LIBRARY_PATH

log1="logs/duckdb_wide_${R_size}_${S_size}_${num_pL}_${type}.log.txt"
log2="logs/duckdb_wide_${R_size}_${skew}_${num_pL}_${type}.log.txt"
mkdir -p logs

echo "Creating tables for wide join..."
c_tests/mk_tbls_wide.o -R $R_size -S $S_size -r $R_size -s $R_size -k $type -p $type -P $num_pL
echo "Running test for wide join..."
c_tests/duckdb_sort_join.o -I $num_iter -L $log1
echo "Ran test for wide join."

if [ "$6" ]; then
	echo "Creating foreign key relation for zipf join..."
	c_tests/mk_tbls_zipf.o -S $R_size -s $R_size -z $skew -k $type -p $type -P $num_pL
	echo "Running test for zipf join..."
	c_tests/duckdb_sort_join.o -I $num_iter -L $log2
	echo "Ran test for zipf join."
fi