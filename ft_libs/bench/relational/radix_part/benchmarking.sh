if [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=cuda test_part.fut
	mv 'data/test_part:part2_8-[10000000][4]u8_[10000000][0]u8.out' data/partitioned_8.in
	mv 'data/test_part:part2_12-[10000000][4]u8_[10000000][0]u8.out' data/partitioned_12.in
	mv 'data/test_part:part2_16-[10000000][4]u8_[10000000][0]u8.out' data/partitioned_16.in
	rm -f data/*.out
elif [ "$1" ] && [ "$1" -eq 2 ]; then
	futhark bench --backend=cuda --runs=50 test_partitionInfo.fut
	rm -f data/partitioned*.in
	mv data/test_partitionInfo:partInfo_8-data_partitioned_8.in.out data/partitioned_8.in
	mv data/test_partitionInfo:partInfo_12-data_partitioned_12.in.out data/partitioned_12.in
	mv data/test_partitionInfo:partInfo_16-data_partitioned_16.in.out data/partitioned_16.in
elif [ "$1" ] && [ "$1" -eq 3 ]; then
	futhark bench --backend=cuda --runs=50 test_rhTbl.fut
	rm -f data/*
else
	sh benchmarking.sh 1
	sh benchmarking.sh 2
	sh benchmarking.sh 3
fi