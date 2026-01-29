if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=cuda datagen.fut
	rm -f data/*.in
	mv 'data/datagen:smj0_i32-[33554432][8]u8_[67108864][8]u8.out' data/i32_1_1.in
	mv 'data/datagen:smj0_i32-[33554432][16]u8_[67108864][16]u8.out' data/i32_1_2.in
	mv 'data/datagen:smj0_i32-[67108864][8]u8_[134217728][8]u8.out' data/i32_2_1.in
	mv 'data/datagen:smj0_i32-[67108864][16]u8_[134217728][16]u8.out' data/i32_2_2.in
	mv 'data/datagen:smj0_i32-[134217728][8]u8_[268435456][8]u8.out' data/i32_3_1.in
	mv 'data/datagen:smj0_i32-[134217728][16]u8_[268435456][16]u8.out' data/i32_3_2.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=cuda --runs=25 test_matching.fut
	rm -f data/*.in
	mv data/test_matching:smj1_i32-data_i32_1_1.in.out data/i32_1_1.in
	mv data/test_matching:smj1_i32-data_i32_1_2.in.out data/i32_1_2.in
	mv data/test_matching:smj1_i32-data_i32_2_1.in.out data/i32_2_1.in
	mv data/test_matching:smj1_i32-data_i32_2_2.in.out data/i32_2_2.in
	mv data/test_matching:smj1_i32-data_i32_3_1.in.out data/i32_3_1.in
	mv data/test_matching:smj1_i32-data_i32_3_2.in.out data/i32_3_2.in
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=10 test_expansion.fut
	rm -f data/*.in
	mv data/test_expansion:smj2_i32-data_i32_1_1.in.out data/i32_1_1.in
	mv data/test_expansion:smj2_i32-data_i32_1_2.in.out data/i32_1_2.in
	mv data/test_expansion:smj2_i32-data_i32_2_1.in.out data/i32_2_1.in
	mv data/test_expansion:smj2_i32-data_i32_2_2.in.out data/i32_2_2.in
	mv data/test_expansion:smj2_i32-data_i32_3_1.in.out data/i32_3_1.in
	mv data/test_expansion:smj2_i32-data_i32_3_2.in.out data/i32_3_2.in
elif [ "$1" ] && [ "$1" -eq 3 ]; then
	futhark bench --backend=cuda --runs=50 test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
	sh benchmarking.sh 2
	sh benchmarking.sh 3
fi
