# CUDA-DClust+ paper benchmarks.

data_loc=$1
num_runs=5
wsize=8192

echo "Tests for EPS ..."
# 1 - tests for EPS --------------------------------------------------------------

	echo "--- --- NGSIM"
	# NGSIM

		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 0.5 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 0.75 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.0 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.5 8 $wsize $num_runs

	echo "--- --- SPATIAL 2D"
	# SPATIAL 2D

		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.002 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.004 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.006 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.010 8 $wsize $num_runs

	echo "--- --- IONO 2D"
	# IONO 2D

		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 0.5 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 0.75 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.0 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.25 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 4 $wsize $num_runs

	echo "--- --- PORTO"
	# PORTO

		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.002 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.004 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.006 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.010 8 $wsize $num_runs

	echo "--- --- SPATIAL 3D"
	# SPATIAL 3D

		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.02 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.04 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.06 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.10 2 $wsize $num_runs

	echo "--- --- IONO 3D"
	# IONO 3D

		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 0.5 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 0.75 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.0 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.25 4 $wsize $num_runs

echo "Tests for MIN_PTS ..."
# 2 - tests for MIN_PTS --------------------------------------------------------------

	echo "--- --- NGSIM"
	# NGSIM

		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 4 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 16 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 32 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 400000 1.25 64 $wsize $num_runs

	echo "--- --- SPATIAL 2D"
	# SPATIAL 2D

		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 4 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 16 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 32 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 400000 0.008 64 $wsize $num_runs

	echo "--- --- IONO 2D"
	# IONO 2D

		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 8 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 16 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 32 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 400000 1.5 64 $wsize $num_runs

	echo "--- --- PORTO"
	# PORTO

		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 4 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 16 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 32 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 160000 0.008 64 $wsize $num_runs

	echo "--- --- SPATIAL 3D"
	# SPATIAL 3D

		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 1 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 4 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 8 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 400000 0.08 16 $wsize $num_runs

	echo "--- --- IONO 3D"
	# IONO 3D

		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 8 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 16 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 32 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 400000 1.5 64 $wsize $num_runs	

echo "Tests for |D| ..."
# 2 - tests for |D| --------------------------------------------------------------

	echo "--- --- NGSIM"
	# NGSIM

		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 50000 1.25 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 100000 1.25 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 200000 1.25 8 $wsize $num_runs
		bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 800000 1.25 8 $wsize $num_runs

	echo "--- --- SPATIAL 2D"
	# SPATIAL 2D

		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 25000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 50000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 100000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 SPATIAL ${data_loc}/2D_spatial_network.txt 200000 0.008 8 $wsize $num_runs

	echo "--- --- IONO 2D"
	# IONO 2D

		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 50000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 100000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 200000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 2 IONO ${data_loc}/2D_iono.txt 800000 1.5 4 $wsize $num_runs

	echo "--- --- PORTO"
	# PORTO

		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 40000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 80000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 320000 0.008 8 $wsize $num_runs
		bash bench_densebox.sh 2 PORTO ${data_loc}/Porto_Taxi_Service_Trajectory.txt 640000 0.008 8 $wsize $num_runs

	echo "--- --- SPATIAL 3D"
	# SPATIAL 3D

		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 25000 0.08 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 50000 0.08 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 100000 0.08 2 $wsize $num_runs
		bash bench_densebox.sh 3 SPATIAL ${data_loc}/3D_spatial_network.txt 200000 0.08 2 $wsize $num_runs


	echo "--- --- IONO 3D"
	# IONO 3D
		
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 100000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 200000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 800000 1.5 4 $wsize $num_runs
		bash bench_densebox.sh 3 IONO ${data_loc}/3D_iono.txt 1600000 1.5 4 $wsize $num_runs

echo "Final Tests from original paper..."
# Final Runs

	echo "--- --- NGSIM"
	bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 1000000 1.25 8 $wsize $num_runs
	echo "--- --- IONO 2D"
	bash bench_densebox.sh 2 IONO  "${data_loc}/2D_iono.txt" 1860000 1.5 4 $wsize $num_runs
	echo "--- --- PORTO"
	bash bench_densebox.sh 2 PORTO "${data_loc}/Porto_Taxi_Service_Trajectory.txt" 10000000 0.008 8 $wsize $num_runs