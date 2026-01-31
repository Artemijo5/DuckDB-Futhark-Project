if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=c datagen.fut
	rm -f data/*.in
	mv 'data/datagen:smj0_100-[128000]f16_[128000][16]u8_[128000][16]u8.out' data/i32_100.in
	mv 'data/datagen:smj0_50-[128000]f16_[128000][16]u8_[128000][16]u8.out' data/i32_50.in
	mv 'data/datagen:smj0_25-[128000]f16_[128000][16]u8_[128000][16]u8.out' data/i32_25.in
	mv 'data/datagen:smj0_125-[128000]f16_[128000][16]u8_[128000][16]u8.out' data/i32_125.in
	mv 'data/datagen:smj0_625-[128000]f16_[128000][16]u8_[128000][16]u8.out' data/i32_625.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=c --runs=10 test_matching.fut
	rm -f data/*.in
	mv data/test_matching:smj1_i32-data_i32_100.in.out data/i32_100.in
	mv data/test_matching:smj1_i32-data_i32_50.in.out data/i32_50.in
	mv data/test_matching:smj1_i32-data_i32_25.in.out data/i32_25.in
	mv data/test_matching:smj1_i32-data_i32_125.in.out data/i32_125.in
	mv data/test_matching:smj1_i32-data_i32_625.in.out data/i32_625.in
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=c --runs=10 test_expansion.fut
	rm -f data/*.in
	mv data/test_expansion:smj2_i32-data_i32_100.in.out data/i32_100.in
	mv data/test_expansion:smj2_i32-data_i32_50.in.out data/i32_50.in
	mv data/test_expansion:smj2_i32-data_i32_25.in.out data/i32_25.in
	mv data/test_expansion:smj2_i32-data_i32_125.in.out data/i32_125.in
	mv data/test_expansion:smj2_i32-data_i32_625.in.out data/i32_625.in
elif [ "$1" ] && [ "$1" -eq 3 ]; then
	futhark bench --backend=c --runs=50 test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
	sh benchmarking.sh 2
	sh benchmarking.sh 3
fi
