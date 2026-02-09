mkdir -p data

futhark_backend=cuda
num_runs=25

num_elems=$1
key_vals=$2
zipf_skewness=$3

futhark dataset \
	-b -g \[$key_vals\]i64 \
	-b -g \[$num_elems\]f64 \
	--i64-bounds=$key_vals:$key_vals -b -g \[1\]i64 \
	--f64-bounds=$zipf_skewness:$zipf_skewness -b -g \[1\]f64 \
	-b -g \[$num_elems\]i32 \
	-b -g \[$num_elems\]i32 \
	> data/data_i32.in
futhark dataset \
	-b -g \[$key_vals\]i64 \
	-b -g \[$num_elems\]f64 \
	--i64-bounds=$key_vals:$key_vals -b -g \[1\]i64 \
	--f64-bounds=$zipf_skewness:$zipf_skewness -b -g \[1\]f64 \
	-b -g \[$num_elems\]i64 \
	-b -g \[$num_elems\]i64 \
	> data/data_i64.in

# if any 3rd argument is given: do sorted tests
# else do unsorted test
if [ "$4" ]; then
	futhark bench datagen_sorted.fut --backend=$futhark_backend --runs=1
	rm -f data/*.in
	mv 'data/datagen_sorted:datagen_i32-data_data_i32.in.out' data/data_i32.in
	mv 'data/datagen_sorted:datagen_i64-data_data_i64.in.out' data/data_i64.in
	futhark bench test_histogram.fut --backend=$futhark_backend --runs=$num_runs
	futhark bench test_segmented.fut --backend=$futhark_backend --runs=$num_runs
	rm -f data/*
else
	futhark bench datagen_unsorted.fut --backend=$futhark_backend --runs=1
	rm -f data/*.in
	mv 'data/datagen_unsorted:datagen_i32-data_data_i32.in.out' data/data_i32.in
	mv 'data/datagen_unsorted:datagen_i64-data_data_i64.in.out' data/data_i64.in
	futhark bench test_histogram.fut --backend=$futhark_backend --runs=$num_runs
	rm -f data/*
fi
