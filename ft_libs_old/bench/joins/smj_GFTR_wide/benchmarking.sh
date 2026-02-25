futhark_backend=cuda
num_runs=25
type="i32"

num_elem=$1
payload_bytes=$2


mkdir -p data

if [ "$3" ] && [ "$3" -eq 0 ]; then
	futhark dataset -b -g \[$((2*num_elem))\]\[$payload_bytes\]u8 > data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 datagen.fut
	rm -f data/*.in
	mv data/datagen:smj0_$type-data_datagen.in.out data/smjdat.in
elif [ "$3" ] && [ "$3" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_matching.fut
	rm -f data/*.in
	mv data/test_matching:smj1_$type-data_smjdat.in.out data/smjdat.in
elif [ "$3" ] && [ "$3" -eq 2 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_expansion.fut
	rm -f data/*.in
	mv data/test_expansion:smj2_$type-data_smjdat.in.out data/smjdat.in
elif [ "$3" ] && [ "$3" -eq 3 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elem $payload_bytes 0
	sh benchmarking.sh $num_elem $payload_bytes 1
	sh benchmarking.sh $num_elem $payload_bytes 2
	sh benchmarking.sh $num_elem $payload_bytes 3
fi