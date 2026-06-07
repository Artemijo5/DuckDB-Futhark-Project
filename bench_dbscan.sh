#!/bin/bash

dim=$1
dataset_name=$2
dataset_loc=$3
dataset_size=$4
eps=$5
minPts=$6
extpar=$7
runs=$8

filename="logs/gdbscan_${dim}d_${dataset_name}_${dataset_size}_eps_${eps}_minPts_${minPts}_extPar_${extpar}.log.txt"

LD_LIBRARY_PATH=$(pwd):$(pwd)/clibs:$(pwd)/ft_clibs:$(pwd)/c_tests:$(pwd)/c_tests/src:$(pwd)/c_tests/src/dbscan:$LD_LIBRARY_PATH

mkdir -p logs

c_tests/dbscan_"$dim"d.o -i $dataset_loc -s $dataset_size -e $eps -m $minPts -x $extpar -I $runs -L $filename
