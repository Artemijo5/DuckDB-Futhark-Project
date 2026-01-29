# TODO
# 1 datagen, which also does the partitioning & the tables...
# 2 joins, with both ftHashJoin and ftHashJoin_old

if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=cuda --runs=25 datagen.fut
	mv 'data/datagen:rhj0-#0_("33554432i64").out' data/data1.in
	mv 'data/datagen:rhj0-#1_("67108864i64").out' data/data2.in
	mv 'data/datagen:rhj0-#2_("134217728i64").out' data/data3.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
#	futhark bench --backend=cuda --runs=10 test_join1.fut
	futhark bench --backend=cuda --runs=25 test_join2.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
fi
