#!/bin/bash

futhark_backend=cuda
runs_No=25

verbose=
#verbose="-v -v -v"

# Number of elements
n=$1
# Payload bytes
b=$2
# Maxval
maxval=$3

# $4 -> data type (defaults to i32)
type="i32"
otherType="i64"
if [[ "$4" == "i64" ]]; then
	type="i64"
	otherType="i32"
fi

# $5 -> bitstep (for radixSort)
bitstep=2
if [ $5 ]; then
	bitstep=$5
fi

# $6 -> sorting algorithm (merge_sort or radix_sort, defaults to both)
sort_opt=
if [[ "$6" == "merge_sort" ]]; then
	sort_opt="--entry-point=do_mergeSort_$type"
elif [[ "$6" == "radix_sort" ]]; then
	sort_opt="--entry-point=do_radixSort_$type"
fi

# Datagen
mkdir -p data
futhark dataset \
	-b -g \[$n\]i64 \
	-b -g \[$n\]\[$b\]u8 \
	--i32-bounds=$bitstep:$bitstep -b -g \[1\]i32 \
	--i64-bounds=$maxval:$maxval -b -g \[1\]i64 \
	> data/dat_$type.in

# Dummy input for other type datagen to avoid datagen compilation error.
futhark dataset -b -g [0]i64 -b -g [0][0]u8 -b -g [1]i32 -b -g [1]i64 \
> data/dat_$otherType.in

futhark bench --backend=$futhark_backend --runs=1 datagen_iota.fut --entry-point=shuffle_$type
rm -f data/*.in
mv "data/datagen_iota:shuffle_$type-data_dat_$type.in.out" data/dat_$type.in
rm -f data/*.out

# Run test
futhark bench --backend=$futhark_backend --runs=$runs_No sort_$type.fut $sort_opt $verbose

# Cleanup
rm -f data/*