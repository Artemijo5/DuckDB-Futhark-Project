futhark_backend=c
num_runs=10

num_strs=$1
min_len=$2
max_len=$3

futhark dataset --i64-bounds=$min_len:$max_len -b -g \[$num_strs\]i64 -b -g \[$((num_strs*max_len))\]u8 > data/strDat.in

futhark bench datagen.fut --backend=$futhark_backend --runs=1
rm -f data/*.in
mv 'data/datagen:mk_str_nLen-data_strDat.in.out' data/strDat_nl.in
mv 'data/datagen:mk_str_yLen-data_strDat.in.out' data/strDat_yl.in

futhark bench test_HSMJ.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*