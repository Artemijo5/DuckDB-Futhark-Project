# TODO
# 1 datagen, which also does the partitioning & the tables...
# 2 joins, with both ftHashJoin and ftHashJoin_old

if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=cuda --runs=10 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:rhj0-[33554432][8]u8_[67108864][8]u8.out' data/data1_1.in
	mv 'data/datagen:rhj0-[33554432][16]u8_[67108864][16]u8.out' data/data1_2.in
	mv 'data/datagen:rhj0-[67108864][8]u8_[134217728][8]u8.out' data/data2_1.in
	mv 'data/datagen:rhj0-[67108864][16]u8_[134217728][16]u8.out' data/data2_2.in
	mv 'data/datagen:rhj0-[134217728][8]u8_[268435456][8]u8.out' data/data3_1.in
	mv 'data/datagen:rhj0-[134217728][16]u8_[268435456][16]u8.out' data/data3_2.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
#	futhark bench --backend=cuda --runs=10 test_join1.fut
	futhark bench --backend=cuda --runs=10 test_join2.fut
	rm -f data/*.in
	mv 'data/test_join2:rhj-data_data1_1.in.out' data/data1_1.in
	mv 'data/test_join2:rhj-data_data1_2.in.out' data/data1_2.in
	mv 'data/test_join2:rhj-data_data2_1.in.out' data/data2_1.in
	mv 'data/test_join2:rhj-data_data2_2.in.out' data/data2_2.in
	mv 'data/test_join2:rhj-data_data3_1.in.out' data/data3_1.in
	mv 'data/test_join2:rhj-data_data3_2.in.out' data/data3_2.in
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=50 test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
	sh benchmarking.sh 2
fi
