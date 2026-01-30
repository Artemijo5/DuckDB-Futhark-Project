#num_elems=8192
#dataBounds_2d_sparse=200
#dataBounds_2d_dense=130
#dataBounds_3d_sparse=60
#dataBounds_3d_dense=39

num_elems=65536
dataBounds_2d_sparse=566
dataBounds_2d_dense=368
dataBounds_3d_sparse=120
dataBounds_3d_dense=78

ft_backend=cuda
num_runs=25

mkdir -p data

echo "Tests for DBSCAN for 2d & 3d data."
echo " "

echo " "
echo "2d sparse data -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-"
echo " "
futhark dataset --f32-bounds=0:$dataBounds_2d_sparse -b -g [$num_elems][2]f32 >> data/pts_f32
futhark dataset --f64-bounds=0:$dataBounds_2d_sparse -b -g [$num_elems][2]f64 >> data/pts_f64
futhark bench test_dbscan.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*

echo " "
echo "3d sparse data -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-"
echo " "
futhark dataset --f32-bounds=0:$dataBounds_3d_sparse -b -g [$num_elems][3]f32 >> data/pts_f32
futhark dataset --f64-bounds=0:$dataBounds_3d_sparse -b -g [$num_elems][3]f64 >> data/pts_f64
futhark bench test_dbscan.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*

echo " "
echo "2d dense data -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-"
echo " "
futhark dataset --f32-bounds=0:$dataBounds_2d_dense -b -g [$num_elems][2]f32 >> data/pts_f32
futhark dataset --f64-bounds=0:$dataBounds_2d_dense -b -g [$num_elems][2]f64 >> data/pts_f64
futhark bench test_dbscan.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*

echo " "
echo "3d dense data -#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-"
echo " "
futhark dataset --f32-bounds=0:$dataBounds_3d_dense -b -g [$num_elems][3]f32 >> data/pts_f32
futhark dataset --f64-bounds=0:$dataBounds_3d_dense -b -g [$num_elems][3]f64 >> data/pts_f64
futhark bench test_dbscan.fut --backend=$ft_backend --runs=$num_runs
rm -f data/*

