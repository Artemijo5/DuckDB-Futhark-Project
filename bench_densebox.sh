#!/bin/bash

dim=$1
dataset_name=$2
dataset_loc=$3
dataset_size=$4
eps=$5
minPts=$6
wsize=$7
runs=$8

filename="logs/densebox_${dim}d_${dataset_name}_${dataset_size}_eps_${eps}_minPts_${minPts}.log.txt"

LD_LIBRARY_PATH=$(pwd):$(pwd)/clibs:$(pwd)/ft_clibs:$(pwd)/c_tests:$(pwd)/c_tests/src:$(pwd)/c_tests/src/dbscan:$LD_LIBRARY_PATH

mkdir -p logs

c_tests/densebox.o -d $dim -i $dataset_loc -s $dataset_size -e $eps -m $minPts -w $wsize -I $runs -L $filename