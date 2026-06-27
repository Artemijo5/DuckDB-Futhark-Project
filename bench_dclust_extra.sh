data_loc=$1
num_runs=5
num_cells=10000

echo "--- --- NGSIM"
bash bench_densebox.sh 2 NGSIM "${data_loc}/Next_Generation_Simulation_(NGSIM).txt" 1000000 1.25 8 8192 $num_cells $num_runs
echo "--- --- IONO 2D"
bash bench_densebox.sh 2 IONO  "${data_loc}/2D_iono.txt" 1860000 1.5 4 8192 $num_cells $num_runs
#echo "--- --- PORTO"
#bash bench_densebox.sh 2 PORTO "${data_loc}/Porto_Taxi_Service_Trajectory.txt" 10000000 0.008 8 $wsize $num_runs