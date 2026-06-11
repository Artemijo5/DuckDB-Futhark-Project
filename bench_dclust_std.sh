# Benchmarks with default specifications.
# As well as PORTO dataset with 640K points.

data_loc=$1
num_runs=10
num_cells=2000

bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 8 8192 $num_cells $num_runs
bash bench_dclust_segm.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 8 8192 $num_cells $num_runs

bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 8 8192 $num_cells $num_runs
bash bench_dclust_segm.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 8 8192 $num_cells $num_runs

bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 4 8192 $num_cells $num_runs
bash bench_dclust_segm.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 4 8192 $num_cells $num_runs

bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 8 1024 $num_cells $num_runs
bash bench_dclust_segm.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 8 1024 $num_cells $num_runs

bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 640000 0.008 8 1024 $num_cells $num_runs
bash bench_dclust_segm.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 640000 0.008 8 1024 $num_cells $num_runs

bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 2 8192 $num_cells $num_runs
bash bench_dclust_segm.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 2 8192 $num_cells $num_runs

bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 4 8192 $num_cells $num_runs
bash bench_dclust_segm.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 4 8192 $num_cells $num_runs