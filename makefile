CC=gcc
CFLAGS=-I.
DEPS= sortstages.h myutil.h ftsort.h mylogger.h duckdb.h

C-ftsort: ftsort.fut
	futhark c ftsort.fut --library
	gcc ftsort.c -o libftsort.so -fPIC -shared

CUDA-ftsort: ftsort.fut
	futhark cuda ftsort.fut --library
	gcc ftsort.c -o libftsort.so -fPIC -shared -lcuda -lcudart -lnvrtc

C-ftSMJ: ftSMJ.fut
	futhark c ftSMJ.fut --library
	gcc ftSMJ.c -o libftSMJ.so -fPIC -shared

CUDA-ftSMJ: ftSMJ.fut
	futhark cuda ftSMJ.fut --library
	gcc ftSMJ.c -o libftSMJ.so -fPIC -shared -lcuda -lcudart -lnvrtc

sorting_test: sorting_test.c libftsort.so mylogger.c libduckdb.so
	$(CC) sorting_test.c -o sorting_test.o libftsort.so mylogger.c libduckdb.so $(CFLAGS)

one_pass_sort: one_pass_sort.c libftsort.so myutil.c mylogger.c libduckdb.so
	$(CC) one_pass_sort.c -o one_pass_sort.o libftsort.so myutil.c mylogger.c libduckdb.so $(CFLAGS)

two_pass_sort: two_pass_sort.c sortstages.c libftsort.so myutil.c mylogger.c libduckdb.so
	$(CC) two_pass_sort.c -o two_pass_sort.o sortstages.c libftsort.so myutil.c mylogger.c libduckdb.so $(CFLAGS)

sort_merge_join_GFTR: sort_merge_join_GFTR.c smjutil.c sortstages.c libftsort.so libftSMJ.so myutil.c mylogger.c libduckdb.so
	$(CC) sort_merge_join_GFTR.c -o sort_merge_join_GFTR.o smjutil.c sortstages.c libftsort.so libftSMJ.so myutil.c mylogger.c libduckdb.so $(CFLAGS)

sort_merge_join_GFUR: sort_merge_join_GFUR.c smjutil.c sortstages.c libftsort.so libftSMJ.so myutil.c mylogger.c libduckdb.so
	$(CC) sort_merge_join_GFUR.c -o sort_merge_join_GFUR.o smjutil.c sortstages.c libftsort.so libftSMJ.so myutil.c mylogger.c libduckdb.so $(CFLAGS)
