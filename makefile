CC=gcc
CFLAGS=-I.
DEPS=mylogger.c libduckdb.so db_util.c libftRelational.so
LIBFLAGS=-fPIC -shared
CUDAFLAGS=-lcuda -lcudart -lnvrtc

C-ftRelational: ftRelational.fut
	futhark c ftRelational.fut --library
	$(CC) ftRelational.c -o libftRelational.so $(LIBFLAGS)

CUDA-ftRelational: ftRelational.fut
	futhark cuda ftRelational.fut --library
	$(CC) ftRelational.c -o libftRelational.so $(LIBFLAGS) $(CUDAFLAGS)

CUDA1-ftRelational: ftRelational.fut
	futhark cuda ftRelational.fut --library

CUDA2-ftRelational: ftRelational.c
	$(CC) ftRelational.c -o libftRelational.so $(LIBFLAGS) $(CUDAFLAGS)

two_pass_sort: two_pass_sort.c sortstages.c sort_util.c $(DEPS)
	$(CC) two_pass_sort.c -o two_pass_sort.o \
		sortstages.c sort_util.c $(DEPS) $(CFLAGS)

sort_merge_join: sort_merge_join.c sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c $(DEPS)
	$(CC) sort_merge_join.c -o sort_merge_join.o \
		sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c $(DEPS) $(CFLAGS)

radix_hash_join: radix_hash_join.c join_util.c radixJoin_util.c RadixJoinStages.c $(DEPS)
	$(CC) radix_hash_join.c -o radix_hash_join.o \
		radixJoin_util.c join_util.c RadixJoinStages.c $(DEPS) $(CFLAGS)

group_by_aggregation: group_by_aggregation.c $(DEPS)
	$(CC) group_by_aggregation.c -o group_by_aggregation.o $(DEPS) $(CFLAGS)

C-ftSkyline: ftSkyline.fut
	futhark c ftSkyline.fut --library
	$(CC) ftSkyline.c -o libftSkyline.so $(LIBFLAGS)

CUDA-ftSkyline: ftSkyline.fut
	futhark cuda ftSkyline.fut --library
	$(CC) ftSkyline.c -o libftSkyline.so $(LIBFLAGS) $(CUDAFLAGS)

CUDA1-ftSkyline: ftSkyline.fut
	futhark cuda ftSkyline.fut --library

CUDA2-ftSkyline: ftSkyline.c
	$(CC) ftSkyline.c -o libftSkyline.so $(LIBFLAGS) $(CUDAFLAGS)

skyline_test: skyline_test.c libftSkyline.so
	$(CC) skyline_test.c -lm -o skyline_test.o libduckdb.so libftSkyline.so $(CFLAGS)

Skyline: skyline.c mylogger.c libduckdb.so db_util.c libftSkyline.so
	$(CC) skyline.c -lm -o skyline.o mylogger.c libduckdb.so db_util.c libftSkyline.so $(CFLAGS)