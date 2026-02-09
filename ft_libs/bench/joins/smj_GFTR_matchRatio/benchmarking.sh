futhark_backend=cuda
num_runs=25

num_elem=$1
payload_bytes=$2

mkdir -p data

if [ "$3" ] && [ "$3" -eq 0 ]; then
	futhark dataset -b -g \[$num_elem\]f16 -b -g \[$num_elem\]\[$payload_bytes\]u8 > data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:smj0_100-data_datagen.in.out' data/i32_100.in
	mv 'data/datagen:smj0_50-data_datagen.in.out' data/i32_50.in
	mv 'data/datagen:smj0_25-data_datagen.in.out' data/i32_25.in
	mv 'data/datagen:smj0_125-data_datagen.in.out' data/i32_125.in
#	mv 'data/datagen:smj0_625-data_datagen.in.out' data/i32_625.in
elif [ "$3" ] && [ "$3" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_matching.fut
	rm -f data/*.in
	mv data/test_matching:smj1_i32-data_i32_100.in.out data/i32_100.in
	mv data/test_matching:smj1_i32-data_i32_50.in.out data/i32_50.in
	mv data/test_matching:smj1_i32-data_i32_25.in.out data/i32_25.in
	mv data/test_matching:smj1_i32-data_i32_125.in.out data/i32_125.in
#	mv data/test_matching:smj1_i32-data_i32_625.in.out data/i32_625.in
elif [ "$3" ] && [ "$3" -eq 2 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_expansion.fut
	rm -f data/*.in
	mv data/test_expansion:smj2_i32-data_i32_100.in.out data/i32_100.in
	mv data/test_expansion:smj2_i32-data_i32_50.in.out data/i32_50.in
	mv data/test_expansion:smj2_i32-data_i32_25.in.out data/i32_25.in
	mv data/test_expansion:smj2_i32-data_i32_125.in.out data/i32_125.in
#	mv data/test_expansion:smj2_i32-data_i32_625.in.out data/i32_625.in
elif [ "$3" ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elem $payload_bytes 0
	sh benchmarking.sh $num_elem $payload_bytes 1
	sh benchmarking.sh $num_elem $payload_bytes 2
	sh benchmarking.sh $num_elem $payload_bytes 3
fi