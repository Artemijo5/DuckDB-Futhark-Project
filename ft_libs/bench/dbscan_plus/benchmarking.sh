num_elems=1048576
dataBounds_2d_sparse=2264
dataBounds_2d_dense=1472

#num_elems=65536
#dataBounds_2d_sparse=566
#dataBounds_2d_dense=368

#num_elems=32767
#dataBounds_2d_sparse=400
#dataBounds_2d_dense=260

#num_elems=8192
#dataBounds_2d_sparse=200
#dataBounds_2d_dense=150

#num_elems=2048
#dataBounds_2d_sparse=100
#dataBounds_2d_dense=75

ft_backend=c
num_runs=1

mkdir -p data

echo "Tests for DBSCAN+ for 2d data."
echo " "

futhark dataset --f64-bounds=0:$dataBounds_2d_sparse -b -g [2][$num_elems]f64 > data/sparse_f64
futhark dataset --f64-bounds=0:$dataBounds_2d_dense -b -g [2][$num_elems]f64 > data/dense_f64
futhark bench test_dbscan_plus.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*
