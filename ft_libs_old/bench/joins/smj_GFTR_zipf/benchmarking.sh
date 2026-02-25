futhark_backend=cuda
num_runs=25
type="i32"

num_elem=$1
payload_bytes=$2
skewness=$3

mkdir -p data

if [ "$4" ] && [ "$4" -eq 0 ]; then
	futhark dataset \
	-b -g \[$num_elem\]\[$payload_bytes\]u8 \
	-b -g \[$num_elem\]i64 \
	-b -g \[$num_elem\]f64 \
	--f64-bounds=$skewness:$skewness -b -g \[1\]f64 \
	 > data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 datagen.fut
	rm -f data/*.in
	mv data/datagen:smj0_$type-data_datagen.in.out data/smjdat.in
elif [ "$4" ] && [ "$4" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_matching.fut
	rm -f data/*.in
	mv data/test_matching:smj1_$type-data_smjdat.in.out data/smjdat.in
elif [ "$4" ] && [ "$4" -eq 2 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_expansion.fut
	rm -f data/*.in
	mv data/test_expansion:smj2_$type-data_smjdat.in.out data/smjdat.in
elif [ "$4" ] && [ "$4" -eq 3 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elem $payload_bytes $skewness 0
	sh benchmarking.sh $num_elem $payload_bytes $skewness 1
	sh benchmarking.sh $num_elem $payload_bytes $skewness 2
	sh benchmarking.sh $num_elem $payload_bytes $skewness 3
fi