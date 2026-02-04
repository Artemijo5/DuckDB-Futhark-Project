futhark_backend=cuda
num_runs=25

num_elems=$1
payload_bytes=$2

if [ "$3" ] && [ "$3" -eq 0 ]; then
	futhark dataset \
	-b -g \[$num_elems\]f16 \
	-b -g \[$num_elems\]\[$payload_bytes\]u8 \
	-b -g \[$num_elems\]i64 \
	-b -g \[$num_elems\]i64 \
	> data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 --runs=10 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:rhj0_100-data_datagen.in.out' data/i32_100.in
	mv 'data/datagen:rhj0_50-data_datagen.in.out' data/i32_50.in
	mv 'data/datagen:rhj0_25-data_datagen.in.out' data/i32_25.in
	mv 'data/datagen:rhj0_125-data_datagen.in.out' data/i32_125.in
#	mv 'data/datagen:rhj0_625-data_datagen.in.out' data/i32_625.in
elif [ "$3" ] && [ "$3" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_join2.fut
	rm -f data/*.in
	mv data/test_join2:rhj-data_i32_100.in.out data/i32_100.in
	mv data/test_join2:rhj-data_i32_50.in.out data/i32_50.in
	mv data/test_join2:rhj-data_i32_25.in.out data/i32_25.in
	mv data/test_join2:rhj-data_i32_125.in.out data/i32_125.in
#	mv data/test_join2:rhj-data_i32_625.in.out data/i32_625.in
elif [ "$3" ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elems $payload_bytes 0
	sh benchmarking.sh $num_elems $payload_bytes 1
	sh benchmarking.sh $num_elems $payload_bytes 2
fi
