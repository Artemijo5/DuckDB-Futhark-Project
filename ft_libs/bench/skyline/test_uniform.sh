#!/bin/bash

futhark_backend=c
runs_No=10

dim=$1
n=$2

no_comp=''
#if [ -f "skyline_$((dim))d.c" ]; then
#	no_comp='--no-compilation'
#fi

a_subdiv=$3
m_subdiv_min=$4
m_subdiv_max=$5
m_subdiv_step=$6
m_size_thresh=$7
g_subdiv=$8
window_size=$9

verbose=
#verbose='-v -v -v'

mkdir -p data

# Datagen
futhark dataset \
	--i64-bounds=$a_subdiv:$a_subdiv -b -g [1]i64 \
	--i64-bounds=$m_subdiv_min:$m_subdiv_min -b -g [1]i64 \
	--i64-bounds=$m_subdiv_max:$m_subdiv_max -b -g [1]i64 \
	--i64-bounds=$m_subdiv_step:$m_subdiv_step -b -g [1]i64 \
	--i64-bounds=$m_size_thresh:$m_size_thresh -b -g [1]i64 \
	--i64-bounds=$g_subdiv:$g_subdiv -b -g [1]i64 \
	--i64-bounds=$window_size:$window_size -b -g [1]i64 \
	-b -g \[$dim\]\[$n\]f64 \
	> data/dat.in

futhark bench --backend=$futhark_backend --runs=$runs_No skyline_"$dim"d.fut $no_comp $verbose
rm -f data/*

# TODO get output size
#rm -f data/*.in
#mv "data/skyline_$((dim))d:do_skyline-data_dat.in.out" data/dat.out


