# Benchmarks with showcase specifications except default ones (and PORTO with 640K points).

data_loc=$1
num_runs=10
num_cells=10000

# 1 - tests for EPS --------------------------------------------------------------

	# NGSIM

		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 0.5 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 0.75 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.0 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.5 8 8192 $num_cells $num_runs

	# SPATIAL 2D

		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.002 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.004 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.006 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.010 8 8192 $num_cells $num_runs

	# IONO 2D

		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 0.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 0.75 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.0 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.25 4 8192 $num_cells $num_runs

	# PORTO

		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.002 8 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.004 8 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.006 8 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.010 8 1024 $num_cells $num_runs

	# SPATIAL 3D

		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.02 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.04 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.06 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.10 2 8192 $num_cells $num_runs


	# IONO 3D

		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 0.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 0.75 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.0 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.25 4 8192 $num_cells $num_runs


# 2 - tests for MIN_PTS --------------------------------------------------------------

	# NGSIM

		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 16 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 32 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 64S 8192 $num_cells $num_runs

	# SPATIAL 2D

		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 16 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 32 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 64 8192 $num_cells $num_runs

	# IONO 2D

		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 16 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 32 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 64 8192 $num_cells $num_runs

	# PORTO

		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 4 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 16 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 32 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 64 1024 $num_cells $num_runs

	# SPATIAL 3D

		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 1 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 16 8192 $num_cells $num_runs

	# IONO 3D

		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 16 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 32 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 64 8192 $num_cells $num_runs	

# 2 - tests for |D| --------------------------------------------------------------

	# NGSIM

		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 50000 1.25 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 100000 1.25 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 200000 1.25 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 800000 1.25 8 8192 $num_cells $num_runs


	# SPATIAL 2D

		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 25000 0.008 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 50000 0.008 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 100000 0.008 8 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 200000 0.008 8 8192 $num_cells $num_runs

	# IONO 2D

		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 50000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 100000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 200000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 2 IONO ${data_loc}/2D_iono.txt 800000 1.5 4 8192 $num_cells $num_runs

	# PORTO

		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 40000 0.008 8 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 80000 0.008 8 1024 $num_cells $num_runs
		bash bench_dclust.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 320000 0.008 8 1024 $num_cells $num_runs

	# SPATIAL 3D

		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 25000 0.08 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 50000 0.08 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 100000 0.08 2 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 200000 0.08 2 8192 $num_cells $num_runs


	# IONO 3D
		
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 100000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 200000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 800000 1.5 4 8192 $num_cells $num_runs
		bash bench_dclust.sh 3 IONO ${data_loc}/3D_iono.txt 1600000 1.5 4 8192 $num_cells $num_runs