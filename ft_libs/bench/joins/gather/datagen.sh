#!/bin/sh

ft_inSize=1000000
ft_seedVal=20260128
if [ "$1" ]
	then
		ft_inSize=$1
fi
if [ "$2" ]
	then
		ft_seedVal=$2
fi
futhark dataset -s $ft_seedVal -b -g [$ft_inSize][4]u8 \
--i64-bounds=0:$((ft_inSize + ft_inSize / 100)) -b -g [$ft_inSize]i64 >> data/gatherIn_i32.in
futhark dataset -s $ft_seedVal -b -g [$ft_inSize][8]u8 \
--i64-bounds=0:$((ft_inSize + ft_inSize / 100)) -b -g [$ft_inSize]i64 >> data/gatherIn_i64.in