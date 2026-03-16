#!/bin/bash

futhark_backend=cuda
runs_No=25

verbose=
#verbose="-v -v -v"

# Number of elements
n=$1
# Payload bytes
b=$2

# $3 -> data type (defaults to i32)
type="i32"
if [[ "$3" == "i64" ]]; then
	type="i64"
fi

# $4 -> bitstep (for radixSort)
bitstep=2
if [ $4 ]; then
	bitstep=$4
fi

# $5 -> sorting algorithm (merge_sort or radix_sort, defaults to both)
sort_opt=
if [[ "$5" == "merge_sort" ]]; then
	sort_opt="--entry-point=do_mergeSort_$type"
elif [[ "$5" == "radix_sort" ]]; then
	sort_opt="--entry-point=do_radixSort_$type"
fi

# Datagen
mkdir -p data
futhark dataset \
	-b -g \[$n\]$type \
	-b -g \[$n\]\[$b\]u8 \
	--i32-bounds=$bitstep:$bitstep -b -g \[1\]i32 \
	> data/dat_$type.in

# Run test
futhark bench --backend=$futhark_backend --runs=$runs_No sort_$type.fut $sort_opt $verbose

# Cleanup
rm -f data/*