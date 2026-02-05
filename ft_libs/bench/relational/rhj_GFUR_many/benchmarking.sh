futhark_backend=cuda
num_runs=25

num_elems=$1
payload_bytes=$2
num_keys=$3


if [ "$4" ] && [ "$4" -eq 0 ]; then
	futhark dataset \
	-b -g \[$num_elems\]\[$payload_bytes\]u8 \
	-b -g \[$num_elems\]\[$payload_bytes\]u8 \
	-b -g \[$num_elems\]i64 \
	-b -g \[$num_elems\]i64 \
	--i64-bounds=$num_keys:$num_keys -b -g \[1\]i64 \
	> data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:rhj0-data_datagen.in.out' data/rhj_data.in
elif [ "$4" ] && [ "$4" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_part.fut
	rm -f data/*.in
	mv 'data/test_part:rpart-data_rhj_data.in.out' data/rhj_data.in
elif [ "$4" ] && [ "$4" -eq 2 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_join2.fut
	rm -f data/*.in
	mv 'data/test_join2:rhj-data_rhj_data.in.out' data/rhj_data.in
elif [ "$4" ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elems $payload_bytes $num_keys 0
	sh benchmarking.sh $num_elems $payload_bytes $num_keys 1
	sh benchmarking.sh $num_elems $payload_bytes $num_keys 2
	sh benchmarking.sh $num_elems $payload_bytes $num_keys 3
fi
