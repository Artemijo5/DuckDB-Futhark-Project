CC=gcc
CFLAGS=-I . -std=c99

DEPS=clibs/mylogger.c clibs/libduckdb.so clibs/db_util.c
REL_DEPS=ft_clibs/libftRelational.so
SKYLINE_DEPS=ft_clibs/libftSkyline.so
DBSCAN_DEPS=ft_clibs/libftDBSCAN.so

LIBFLAGS=-fPIC -shared
CUDAFLAGS=-lcuda -lcudart -lnvrtc

SORT_DEPS=$(DEPS) $(REL_DEPS) $(wildcard algo_utils/sort/*.c)
JOIN_DEPS=$(SORT_DEPS) algo_utils/join/join_util.c
SMJ_DEPS=$(JOIN_DEPS) $(wildcard algo_utils/join/smj/*.c)
RHJ_DEPS=$(JOIN_DEPS) $(wildcard algo_utils/join/rhj/*.c)
AGGR_DEPS=$(SMJ_DEPS)

CUDA-LIBS:
	make CUDA-ftRelational && make CUDA-ftDBSCAN && make CUDA-ftSkyline

C-LIBS:
	make C-ftRelational && make C-ftDBSCAN && make C-ftSkyline

C-ftRelational: ft_libs/ftRelational.fut
	futhark c ft_libs/ftRelational.fut --library -o ft_clibs/ftRelational
	$(CC) ft_clibs/ftRelational.c -o $(REL_DEPS) $(LIBFLAGS) $(CFLAGS)

CUDA1-ftRelational: ftRelational.fut
	futhark cuda ft_libs/ftRelational.fut --library -o ft_clibs/ftRelational

CUDA2-ftRelational: ftRelational.c
	$(CC) ft_clibs/ftRelational.c -o $(REL_DEPS) \
		$(LIBFLAGS) $(CFLAGS) $(CUDAFLAGS)

CUDA-ftRelational: ftRelational.fut
	make CUDA1-ftRelational
	make CUDA2-ftRelational

two_pass_sort: benchmarks/src/two_pass_sort.c $(SORT_DEPS)
	$(CC) benchmarks/src/two_pass_sort.c -o benchmarks/two_pass_sort.o \
		$(SORT_DEPS) $(CFLAGS)

sort_merge_join: benchmarks/src/sort_merge_join.c $(SMJ_DEPS)
	$(CC) benchmarks/src/sort_merge_join.c -o benchmarks/sort_merge_join.o \
		$(SMJ_DEPS) $(CFLAGS)

radix_hash_join: benchmarks/src/radix_hash_join.c $(RHJ_DEPS)
	$(CC) benchmarks/src/radix_hash_join.c -o benchmarks/radix_hash_join.o \
		$(RHJ_DEPS) $(CFLAGS)

group_by_aggregation: benchmarks/src/group_by_aggregation.c $(AGGR_DEPS)
	$(CC) benchmarks/src/group_by_aggregation.c -o benchmarks/group_by_aggregation.o \
		$(AGGR_DEPS) $(CFLAGS)

C-ftSkyline: ft_libs/ftSkyline.fut
	futhark c ft_libs/ftSkyline.fut --library -o ft_clibs/ftSkyline
	$(CC) ft_clibs/ftSkyline.c -o $(SKYLINE_DEPS) $(LIBFLAGS) $(CFLAGS)

CUDA1-ftSkyline: ftSkyline.fut
	futhark cuda ft_libs/ftSkyline.fut --library -o ft_clibs/ftSkyline

CUDA2-ftSkyline: ftSkyline.c
	$(CC) ft_clibs/ftSkyline.c -o $(SKYLINE_DEPS) \
		$(LIBFLAGS) $(CFLAGS) $(CUDAFLAGS)

CUDA-ftSkyline: ftSkyline.fut
	make CUDA1-ftSkyline
	make CUDA2-ftSkyline

Skyline: benchmarks/src/skyline.c $(DEPS) $(SKYLINE_DEPS)
	$(CC) benchmarks/src/skyline.c -lm -o benchmarks/skyline.o \
		$(DEPS) $(SKYLINE_DEPS) $(CFLAGS)

C-ftDBSCAN: ft_libs/ftDBSCAN.fut
	futhark c ft_libs/ftDBSCAN.fut --library -o ft_clibs/ftDBSCAN
	$(CC) ft_clibs/ftDBSCAN.c -o $(DBSCAN_DEPS) $(LIBFLAGS)

CUDA-ftDBSCAN: ftDBSCAN.fut
	futhark cuda ftDBSCAN.fut --library
	$(CC) ft_clibs/ftDBSCAN.c -o $(DBSCAN_DEPS) $(LIBFLAGS) $(CUDAFLAGS)

CUDA1-ftDBSCAN: ftDBSCAN.fut
	futhark cuda ft_libs/ftDBSCAN.fut --library -o ft_clibs/ftDBSCAN

CUDA2-ftDBSCAN: ftDBSCAN.c
	$(CC) ft_clibs/ftDBSCAN.c -o $(DBSCAN_DEPS) $(LIBFLAGS) $(CUDAFLAGS)

DBSCAN: benchmarks/src/dbscan.c $(DEPS) $(DBSCAN_DEPS)
	$(CC) benchmarks/src/dbscan.c -lm -o benchmarks/dbscan.o \
		$(DEPS) $(DBSCAN_DEPS) $(CFLAGS)