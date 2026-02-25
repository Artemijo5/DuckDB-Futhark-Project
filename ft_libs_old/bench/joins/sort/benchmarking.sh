mkdir -p data

futhark_backend=cuda
num_runs=25

num_elems=$1
payload_bytes=$2

futhark dataset -b -g \[$num_elems\]i32 -b -g \[$num_elems\]\[$payload_bytes\]u8 > data/dat_i32.in
futhark dataset -b -g \[$num_elems\]i64 -b -g \[$num_elems\]\[$payload_bytes\]u8 > data/dat_i64.in
futhark bench test_sort.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*