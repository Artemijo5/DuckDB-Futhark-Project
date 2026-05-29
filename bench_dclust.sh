#!/bin/bash

dim=$1
dataset_name=$2
dataset_loc=$3
dataset_size=$4
eps=$5
minPts=$6
num_seeds=$7
cells=$8
runs=$9

filename="logs/dclust_${dim}d_${dataset_name}_${dataset_size}_eps_${eps}_minPts_${minPts}_cells_${cells}.log.txt"

LD_LIBRARY_PATH=$(pwd):$(pwd)/clibs:$(pwd)/ft_clibs:$(pwd)/c_tests:$(pwd)/c_tests/src:$(pwd)/c_tests/src/dbscan:$LD_LIBRARY_PATH

mkdir -p logs

c_tests/dclust_"$dim"d.o -i $dataset_loc -s $dataset_size -e $eps -m $minPts -c $num_seeds -d $cells -I $runs -L $filename
