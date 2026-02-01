futhark_backend=cuda
num_runs=25
dsize=33554432

futhark bench datagen.fut --backend=$futhark_backend --runs=1
rm -f data/*.in
mv "data/datagen:mk_str_100-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/avg100.in
mv "data/datagen:mk_str_35-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/avg35.in
mv "data/datagen:mk_str_10-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/avg10.in
mv "data/datagen:mk_str_det100-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/det100.in
mv "data/datagen:mk_str_det35-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/det35.in
mv "data/datagen:mk_str_det10-[$dsize]f32_[$dsize]u8_[$dsize]f32_[$dsize]u8.out" data/det10.in

futhark bench test_HSMJ.fut --backend=$futhark_backend --runs=$num_runs
rm -f data/*