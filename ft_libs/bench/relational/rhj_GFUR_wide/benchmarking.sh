futhark_backend=cuda
num_runs=25

num_elems=$1
payload_bytes=$2


if [ "$3" ] && [ "$3" -eq 0 ]; then
	futhark dataset \
	-b -g \[$num_elems\]i64 \
	-b -g \[$((2*num_elems))\]i64 \
	-b -g \[$num_elems\]\[$payload_bytes\]u8 \
	-b -g \[$((2*num_elems))\]\[$payload_bytes\]u8 \
	> data/datagen.in
	futhark bench --backend=$futhark_backend --runs=1 datagen.fut
	rm -f data/*.in
	mv 'data/datagen:rhj0-data_datagen.in.out' data/rhj_data.in
elif [ "$3" ] && [ "$3" -eq 1 ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_join2.fut
	rm -f data/*.in
	mv 'data/test_join2:rhj-data_rhj_data.in.out' data/rhj_data.in
elif [ "$3" ]; then
	futhark bench --backend=$futhark_backend --runs=$num_runs test_materialization.fut
	rm -f data/*
else
	sh benchmarking.sh $num_elems $payload_bytes 0
	sh benchmarking.sh $num_elems $payload_bytes 1
	sh benchmarking.sh $num_elems $payload_bytes 2
fi
