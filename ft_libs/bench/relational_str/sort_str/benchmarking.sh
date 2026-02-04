futhark_backend=cuda
num_runs=25

num_strs=$1
min_len=$2
max_len=$3

mkdir -p data

futhark dataset --i64-bounds=$min_len:$max_len -b -g \[$num_strs\]i64 -b -g \[$((num_strs*max_len))\]u8 > data/strDat.in

futhark bench datagen.fut --backend=$futhark_backend --runs=1
rm -f data/*.in
mv 'data/datagen:mk_strs-data_strDat.in.out' data/strDat.in

futhark bench test_sort.fut --backend=$futhark_backend --runs=$num_runs
futhark bench test_hashSort.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*