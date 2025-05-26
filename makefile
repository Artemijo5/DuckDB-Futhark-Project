CC=gcc
CFLAGS=-I.
DEPS= sortstages.h myutil.h ftsort.h mylogger.h duckdb.h

C-ftsort: ftsort.fut
	futhark c ftsort.fut --library

C-sorting_test: sorting_test.c ftsort.c mylogger.c libduckdb.so
	$(CC) sorting_test.c -o sorting_test.o ftsort.c mylogger.c libduckdb.so $(CFLAGS)

C-one_pass_sort: one_pass_sort.c ftsort.c myutil.c mylogger.c libduckdb.so
	$(CC) one_pass_sort.c -o one_pass_sort.o ftsort.c myutil.c mylogger.c libduckdb.so $(CFLAGS)

C-two_pass_sort: two_pass_sort.c sortstages.c ftsort.c myutil.c mylogger.c libduckdb.so
	$(CC) two_pass_sort.c -o two_pass_sort.o sortstages.c ftsort.c myutil.c mylogger.c libduckdb.so $(CFLAGS)