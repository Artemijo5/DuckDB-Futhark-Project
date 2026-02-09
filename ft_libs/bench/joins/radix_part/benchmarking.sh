futhark_backend=cuda
num_runs=25

num_elem=$1
key_bytes=$2
payload_bytes=$3

if [ "$4" ] && [ "$4" -eq 1 ]; then
	mkdir -p data
	futhark dataset -b -g \[$num_elem\]\[$key_bytes\]u8 -b -g \[$num_elem\]\[$payload_bytes\]u8 > data/part_data.in
	futhark bench --backend=$futhark_backend --runs=$num_runs test_part.fut
	rm -f data/*.in
	mv 'data/test_part:part2-data_part_data.in.out' data/part_data.in
elif [ "$4" ] && [ "$4" -eq 2 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_partitionInfo.fut
	rm -f data/*.in
	mv data/test_partitionInfo:partInfo-data_part_data.in.out data/part_data.in
elif [ "$4" ] && [ "$4" -eq 3 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_rhTbl.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elem $key_bytes $payload_bytes 1
	sh benchmarking.sh $num_elem $key_bytes $payload_bytes 2
	sh benchmarking.sh $num_elem $key_bytes $payload_bytes 3
fi