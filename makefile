CC=gcc
CFLAGS=-I.
DEPS= sortstages.h myutil.h ftsort.h mylogger.h duckdb.h

C-ftsort: ftsort.fut
	futhark c ftsort.fut --library
	gcc ftsort.c -o libftsort.so -fPIC -shared

CUDA-ftsort:
	futhark cuda ftsort.fut --library
	gcc ftsort.c -o libftsort.so -fPIC -shared -lcuda -lcudart -lnvrtc

sorting_test: sorting_test.c libftsort.so mylogger.c libduckdb.so
	$(CC) sorting_test.c -o sorting_test.o libftsort.so mylogger.c libduckdb.so $(CFLAGS)

one_pass_sort: one_pass_sort.c libftsort.so myutil.c mylogger.c libduckdb.so
	$(CC) one_pass_sort.c -o one_pass_sort.o libftsort.so myutil.c mylogger.c libduckdb.so $(CFLAGS)

two_pass_sort: two_pass_sort.c sortstages.c libftsort.so myutil.c mylogger.c libduckdb.so
	$(CC) two_pass_sort.c -o two_pass_sort.o sortstages.c libftsort.so myutil.c mylogger.c libduckdb.so $(CFLAGS)
