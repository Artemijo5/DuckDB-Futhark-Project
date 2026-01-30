# TODO
# 1 datagen, which also does the partitioning & the tables...
# 2 joins, with both ftHashJoin and ftHashJoin_old

if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=cuda --runs=10 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:rhj0_100-[134217728]f16_[134217728][16]u8_[134217728][16]u8.out' data/i32_100.in
	mv 'data/datagen:rhj0_50-[134217728]f16_[134217728][16]u8_[134217728][16]u8.out' data/i32_50.in
	mv 'data/datagen:rhj0_25-[134217728]f16_[134217728][16]u8_[134217728][16]u8.out' data/i32_25.in
	mv 'data/datagen:rhj0_125-[134217728]f16_[134217728][16]u8_[134217728][16]u8.out' data/i32_125.in
	mv 'data/datagen:rhj0_625-[134217728]f16_[134217728][16]u8_[134217728][16]u8.out' data/i32_625.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
#	futhark bench --backend=cuda --runs=10 test_join1.fut
	futhark bench --backend=cuda --runs=10 test_join2.fut
	rm -f data/*.in
	mv data/test_join2:rhj-data_i32_100.in.out data/i32_100.in
	mv data/test_join2:rhj-data_i32_50.in.out data/i32_50.in
	mv data/test_join2:rhj-data_i32_25.in.out data/i32_25.in
	mv data/test_join2:rhj-data_i32_125.in.out data/i32_125.in
	mv data/test_join2:rhj-data_i32_625.in.out data/i32_625.in
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=50 test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
	sh benchmarking.sh 2
fi
