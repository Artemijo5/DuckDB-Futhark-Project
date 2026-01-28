if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=cuda datagen.fut
	mv 'data/datagen:smj0_i32-#0_("33554432i64").out' data/i32_0.in
	mv 'data/datagen:smj0_i32-#1_("67108864i64").out' data/i32_1.in
	mv 'data/datagen:smj0_i32-#2_("134217728i64").out' data/i32_2.in
	mv 'data/datagen:smj0_i64-#0_("33554432i64").out' data/i64_0.in
	mv 'data/datagen:smj0_i64-#1_("67108864i64").out' data/i64_1.in
	mv 'data/datagen:smj0_i64-#2_("134217728i64").out' data/i64_2.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=cuda --runs=50 test_matching.fut
	rm -f data/i*.in
	mv data/test_matching:smj1_i32-data_i32_0.in.out data/i32_0.in
	mv data/test_matching:smj1_i32-data_i32_1.in.out data/i32_1.in
	mv data/test_matching:smj1_i32-data_i32_2.in.out data/i32_2.in
	mv data/test_matching:smj1_i64-data_i64_0.in.out data/i64_0.in
	mv data/test_matching:smj1_i64-data_i64_1.in.out data/i64_1.in
	mv data/test_matching:smj1_i64-data_i64_2.in.out data/i64_2.in
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=50 test_expansion.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
	sh benchmarking.sh 2
fi
