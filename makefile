CC=gcc
CFLAGS=-I.
DEPS=mylogger.c libduckdb.so
LIBFLAGS=-fPIC -shared
CUDAFLAGS=-lcuda -lcudart -lnvrtc

C-ftSMJ: ftSMJ.fut
	futhark c ftSMJ.fut --library
	gcc ftSMJ.c -o libftSMJ.so $(LIBFLAGS)

CUDA-ftSMJ: ftSMJ.fut
	futhark cuda ftSMJ.fut --library
	gcc ftSMJ.c -o libftSMJ.so $(LIBFLAGS) $(CUDAFLAGS)

CUDA1-ftSMJ: ftSMJ.fut
	futhark cuda ftSMJ.fut --library

CUDA2-ftSMJ: ftSMJ.fut
	gcc ftSMJ.c -o libftSMJ.so $(LIBFLAGS) $(CUDAFLAGS)

two_pass_sort: two_pass_sort.c sortstages.c libftSMJ.so myutil.c $(DEPS)
	$(CC) two_pass_sort.c -o two_pass_sort.o sortstages.c libftSMJ.so myutil.c $(DEPS) $(CFLAGS)

sort_merge_join_GFTR: sort_merge_join_GFTR.c myutil.c smjutil.c sortstages.c SMJstages.c libftSMJ.so $(DEPS)
	$(CC) sort_merge_join_GFTR.c -o sort_merge_join_GFTR.o myutil.c smjutil.c sortstages.c SMJstages.c libftSMJ.so $(DEPS) $(CFLAGS)

sort_merge_join_GFUR: sort_merge_join_GFUR.c myutil.c smjutil.c sortstages.c SMJstages.c libftSMJ.so $(DEPS)
	$(CC) sort_merge_join_GFUR.c -o sort_merge_join_GFUR.o myutil.c smjutil.c sortstages.c SMJstages.c libftSMJ.so $(DEPS) $(CFLAGS)

joinTest: joinTest.c libftSMJ.so
	$(CC) joinTest.c -o joinTest.o libftSMJ.so $(CFLAGS)
