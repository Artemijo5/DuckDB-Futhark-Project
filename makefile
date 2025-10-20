CC=gcc
CFLAGS=-I.
DEPS=mylogger.c libduckdb.so db_util.c
LIBFLAGS=-fPIC -shared
CUDAFLAGS=-lcuda -lcudart -lnvrtc

C-ftRelational: ftRelational.fut
	futhark c ftRelational.fut --library
	gcc ftRelational.c -o libftRelational.so $(LIBFLAGS)

CUDA-ftRelational: ftRelational.fut
	futhark cuda ftRelational.fut --library
	gcc ftRelational.c -o libftRelational.so $(LIBFLAGS) $(CUDAFLAGS)

CUDA1-ftRelational: ftRelational.fut
	futhark cuda ftRelational.fut --library

CUDA2-ftRelational: ftRelational.fut
	gcc ftRelational.c -o libftRelational.so $(LIBFLAGS) $(CUDAFLAGS)

two_pass_sort: two_pass_sort.c sortstages.c libftRelational.so sort_util.c $(DEPS)
	$(CC) two_pass_sort.c -o two_pass_sort.o \
		sortstages.c libftRelational.so sort_util.c $(DEPS) $(CFLAGS)

sort_merge_join_GFTR: sort_merge_join_GFTR.c sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c libftRelational.so $(DEPS)
	$(CC) sort_merge_join_GFTR.c -o sort_merge_join_GFTR.o \
		sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c libftRelational.so $(DEPS) $(CFLAGS)

sort_merge_join_GFUR: sort_merge_join_GFUR.c sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c libftRelational.so $(DEPS)
	$(CC) sort_merge_join_GFUR.c -o sort_merge_join_GFUR.o \
		sort_util.c join_util.c smjutil.c sortstages.c SMJstages.c libftRelational.so $(DEPS) $(CFLAGS)

radix_hash_join: radix_hash_join.c join_util.c radixJoin_util.c RadixJoinStages.c libftRelational.so $(DEPS)
	$(CC) radix_hash_join.c -o radix_hash_join.o \
		radixJoin_util.c join_util.c RadixJoinStages.c libftRelational.so $(DEPS) $(CFLAGS)
