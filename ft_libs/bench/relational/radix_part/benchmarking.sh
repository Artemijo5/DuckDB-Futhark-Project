if [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=cuda test_part.fut
	mv 'data/test_part:part2-[33554432][4]u8_[33554432][0]u8.out' data/partitioned_1.in
	mv 'data/test_part:part2-[67108864][4]u8_[67108864][0]u8.out' data/partitioned_2.in
	mv 'data/test_part:part2-[134217728][4]u8_[134217728][0]u8.out' data/partitioned_3.in
#	mv 'data/test_part:part2-[268435456][4]u8_[268435456][0]u8.out' data/partitioned_4.in
	rm -f data/*.out
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=50 test_partitionInfo.fut
	rm -f data/partitioned*.in
	mv data/test_partitionInfo:partInfo-data_partitioned_1.in.out data/partitioned_1.in
	mv data/test_partitionInfo:partInfo-data_partitioned_2.in.out data/partitioned_2.in
	mv data/test_partitionInfo:partInfo-data_partitioned_3.in.out data/partitioned_3.in
#	mv data/test_partitionInfo:partInfo-data_partitioned_4.in.out data/partitioned_4.in
elif [ "$1" ] && [ "$1" -eq 3 ]; then
	futhark bench --backend=cuda --runs=50 test_rhTbl.fut
	rm -f data/*
else
	sh benchmarking.sh 1
	sh benchmarking.sh 2
	sh benchmarking.sh 3
fi