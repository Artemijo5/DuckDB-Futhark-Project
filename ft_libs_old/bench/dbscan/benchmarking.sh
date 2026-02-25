#num_elems=2048
#dataBounds_2d_sparse=100
#dataBounds_2d_dense=75
#dataBounds_3d_sparse=30
#dataBounds_3d_dense=24

num_elems=4096
dataBounds_2d_sparse=141.42
dataBounds_2d_dense=106
dataBounds_3d_sparse=37.797
dataBounds_3d_dense=30.2376

#num_elems=8192
#dataBounds_2d_sparse=200
#dataBounds_2d_dense=130
#dataBounds_3d_sparse=47.6
#dataBounds_3d_dense=39

#num_elems=32768
#dataBounds_2d_sparse=400
#dataBounds_2d_dense=260
#dataBounds_3d_sparse=75.6
#dataBounds_3d_dense=61.9086

#num_elems=65536
#dataBounds_2d_sparse=566
#dataBounds_2d_dense=368
#dataBounds_3d_sparse=95.242
#dataBounds_3d_dense=78

ft_backend=cuda
num_runs=25

mkdir -p data

echo "Tests for DBSCAN for 2d & 3d data."
echo " "

futhark dataset --f32-bounds=0:$dataBounds_2d_sparse -b -g [$num_elems][2]f32 >> data/sparse2d_f32.in
futhark dataset --f32-bounds=0:$dataBounds_2d_dense -b -g [$num_elems][2]f32 >> data/dense2d_f32.in
futhark dataset --f32-bounds=0:$dataBounds_3d_sparse -b -g [$num_elems][3]f32 >> data/sparse3d_f32.in
futhark dataset --f32-bounds=0:$dataBounds_3d_dense -b -g [$num_elems][3]f32 >> data/dense3d_f32.in

futhark dataset --f64-bounds=0:$dataBounds_2d_sparse -b -g [$num_elems][2]f64 >> data/sparse2d_f64.in
futhark dataset --f64-bounds=0:$dataBounds_2d_dense -b -g [$num_elems][2]f64 >> data/dense2d_f64.in
futhark dataset --f64-bounds=0:$dataBounds_3d_sparse -b -g [$num_elems][3]f64 >> data/sparse3d_f64.in
futhark dataset --f64-bounds=0:$dataBounds_3d_dense -b -g [$num_elems][3]f64 >> data/dense3d_f64.in

futhark bench test1.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*.in

mv 'data/test1:dbscan1_f32-data_sparse2d_f32.in.out' data/sparse2d_f32.in
mv 'data/test1:dbscan1_f32-data_dense2d_f32.in.out' data/dense2d_f32.in
mv 'data/test1:dbscan1_f32-data_sparse3d_f32.in.out' data/sparse3d_f32.in
mv 'data/test1:dbscan1_f32-data_dense3d_f32.in.out' data/dense3d_f32.in

mv 'data/test1:dbscan1_f64-data_sparse2d_f64.in.out' data/sparse2d_f64.in
mv 'data/test1:dbscan1_f64-data_dense2d_f64.in.out' data/dense2d_f64.in
mv 'data/test1:dbscan1_f64-data_sparse3d_f64.in.out' data/sparse3d_f64.in
mv 'data/test1:dbscan1_f64-data_dense3d_f64.in.out' data/dense3d_f64.in

futhark bench test2.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*.in

mv 'data/test2:dbscan2_f32-data_sparse2d_f32.in.out' data/sparse2d_f32.in
mv 'data/test2:dbscan2_f32-data_dense2d_f32.in.out' data/dense2d_f32.in
mv 'data/test2:dbscan2_f32-data_sparse3d_f32.in.out' data/sparse3d_f32.in
mv 'data/test2:dbscan2_f32-data_dense3d_f32.in.out' data/dense3d_f32.in

mv 'data/test2:dbscan2_f64-data_sparse2d_f64.in.out' data/sparse2d_f64.in
mv 'data/test2:dbscan2_f64-data_dense2d_f64.in.out' data/dense2d_f64.in
mv 'data/test2:dbscan2_f64-data_sparse3d_f64.in.out' data/sparse3d_f64.in
mv 'data/test2:dbscan2_f64-data_dense3d_f64.in.out' data/dense3d_f64.in

futhark bench test3.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*