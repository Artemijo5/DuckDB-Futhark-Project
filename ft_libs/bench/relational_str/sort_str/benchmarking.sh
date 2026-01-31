futhark_backend=cuda
num_runs=25

futhark bench datagen.fut --backend=$futhark_backend --runs=1
rm -f data/*.in
mv 'data/datagen:mk_str_100-[33554432]f32_[33554432]u8.out' data/avg100.in
mv 'data/datagen:mk_str_35-[33554432]f32_[33554432]u8.out' data/avg35.in
mv 'data/datagen:mk_str_10-[33554432]f32_[33554432]u8.out' data/avg10.in
mv 'data/datagen:mk_str_det100-[33554432]f32_[33554432]u8.out' data/det100.in
mv 'data/datagen:mk_str_det35-[33554432]f32_[33554432]u8.out' data/det35.in
mv 'data/datagen:mk_str_det10-[33554432]f32_[33554432]u8.out' data/det10.in

futhark bench test_sort.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*.out

futhark bench test_hashSort.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*