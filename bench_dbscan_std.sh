# Benchmarks with default specifications.
# As well as PORTO dataset with 640K points.

data_loc=$1
extPar=$2
extPar_PORTO=$3
num_runs=10
num_cells=2000

bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 8 ${extPar} $num_runs

bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 8 ${extPar} $num_runs

bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 4 ${extPar} $num_runs

bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 8 ${extPar_PORTO} $num_runs

bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 640000 0.008 8 ${extPar_PORTO} $num_runs

bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 2 ${extPar} $num_runs

bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 4 ${extPar} $num_runs