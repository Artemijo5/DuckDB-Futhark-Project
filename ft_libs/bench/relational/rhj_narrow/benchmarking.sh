# TODO
# 1 datagen, which also does the partitioning & the tables...
# 2 joins, with both ftHashJoin and ftHashJoin_old

if [ "$1" ] && [ "$1" -eq 0 ]; then
	futhark bench --backend=c datagen.fut
	mv 'data/datagen:rhj0_8-#0_("33554432i64").out' data/data8.in
	mv 'data/datagen:rhj0_12-#0_("33554432i64").out' data/data12.in
	mv 'data/datagen:rhj0_16-#0_("33554432i64").out' data/data16.in
elif [ "$1" ] && [ "$1" -eq 1 ]; then
	futhark bench --backend=c --runs=10 test_join1.fut
	futhark bench --backend=c --runs=10 test_join2.fut
	rm -f data/*
else
	sh benchmarking.sh 0
	sh benchmarking.sh 1
fi
