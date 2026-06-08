CC=gcc
CFLAGS=-I . -std=c99

LD_LIBRARY_PATH=$(pwd):$(pwd)/clibs:$(pwd)/ft_clibs:$(pwd)/c_tests:$(pwd)/c_tests/src:$(pwd)/c_tests/src/dbscan:$LD_LIBRARY_PATH

DEPS=clibs/mylogger.c clibs/libduckdb.so

LIBFLAGS=-fPIC -shared
CUDAFLAGS=-lcuda -lcudart -lnvrtc

DBSCAN:
	mkdir -p ft_clibs
	futhark cuda ft_libs/dbscan/dbscan_entry.fut -o ft_clibs/dbscan_entry --library
	$(CC) ft_clibs/dbscan_entry.c -o ft_clibs/libdbscan.so $(CFLAGS) $(CUDAFLAGS) $(LIBFLAGS)
	$(CC) c_tests/src/dbscan/futhark_dbscan_2d.c -o c_tests/dbscan_2d.o $(CFLAGS) $(DEPS) ft_clibs/libdbscan.so -lm
	$(CC) c_tests/src/dbscan/futhark_dbscan_3d.c -o c_tests/dbscan_3d.o $(CFLAGS) $(DEPS) ft_clibs/libdbscan.so -lm

C-DBSCAN:
	mkdir -p ft_clibs
	futhark c ft_libs/dbscan/dbscan_entry.fut -o ft_clibs/dbscan_entry --library
	$(CC) c_tests/src/dbscan/futhark_dbscan_2d.c -o c_tests/dbscan_2d.o $(CFLAGS) $(DEPS) ft_clibs/dbscan_entry.c -lm
	$(CC) c_tests/src/dbscan/futhark_dbscan_3d.c -o c_tests/dbscan_3d.o $(CFLAGS) $(DEPS) ft_clibs/dbscan_entry.c -lm

DClust:
	mkdir -p ft_clibs
	futhark cuda ft_libs/dbscan/dclust_entry.fut -o ft_clibs/dclust_entry --library
	$(CC) ft_clibs/dclust_entry.c -o ft_clibs/libdclust.so $(CFLAGS) $(CUDAFLAGS) $(LIBFLAGS)
	$(CC) c_tests/src/dbscan/futhark_dclust_2d.c -o c_tests/dclust_2d.o $(CFLAGS) $(DEPS) ft_clibs/libdclust.so -lm
	$(CC) c_tests/src/dbscan/futhark_dclust_3d.c -o c_tests/dclust_3d.o $(CFLAGS) $(DEPS) ft_clibs/libdclust.so -lm

DClust-segm:
	mkdir -p ft_clibs
	futhark cuda ft_libs/dbscan/dclust_entry_segments.fut -o ft_clibs/dclust_entry_segm --library
	$(CC) ft_clibs/dclust_entry_segm.c -o ft_clibs/libdclust_segm.so $(CFLAGS) $(CUDAFLAGS) $(LIBFLAGS)
	$(CC) c_tests/src/dbscan/futhark_dclust_segm_2d.c -o c_tests/dclust_segm_2d.o $(CFLAGS) $(DEPS) ft_clibs/libdclust_segm.so -lm
	$(CC) c_tests/src/dbscan/futhark_dclust_segm_3d.c -o c_tests/dclust_segm_3d.o $(CFLAGS) $(DEPS) ft_clibs/libdclust.so -lm

C-DClust:
	mkdir -p ft_clibs
	futhark c ft_libs/dbscan/dclust_entry.fut -o ft_clibs/dclust_entry --library
	$(CC) c_tests/src/dbscan/futhark_dclust_2d.c -o c_tests/dclust_2d.o $(CFLAGS) $(DEPS) ft_clibs/dclust_entry.c -lm
	$(CC) c_tests/src/dbscan/futhark_dclust_3d.c -o c_tests/dclust_3d.o $(CFLAGS) $(DEPS) ft_clibs/dclust_entry.c -lm

C-DClust-segm:
	mkdir -p ft_clibs
	futhark c ft_libs/dbscan/dclust_entry_segments.fut -o ft_clibs/dclust_entry_segm --library
	$(CC) c_tests/src/dbscan/futhark_dclust_segm_2d.c -o c_tests/dclust_segm_2d.o $(CFLAGS) $(DEPS) ft_clibs/dclust_entry_segm.c -lm
	$(CC) c_tests/src/dbscan/futhark_dclust_segm_3d.c -o c_tests/dclust_segm_3d.o $(CFLAGS) $(DEPS) ft_clibs/dclust_entry_segm.c -lm

DuckDB-Synthetic:
	mkdir -p ft_clibs
	futhark cuda ft_libs/ftSynthetic.fut -o ft_clibs/ftSynthetic --library
	$(CC) ft_clibs/ftSynthetic.c -o ft_clibs/libsynthetic.so $(CFLAGS) $(CUDAFLAGS) $(LIBFLAGS)
	$(CC) c_tests/src/mk_tbls_zipf.c -o c_tests/mk_tbls_zipf.o $(CFLAGS) $(DEPS) ft_clibs/libsynthetic.so -lm

C-DuckDB-Synthetic:
	mkdir -p ft_clibs
	futhark c ft_libs/ftSynthetic.fut -o ft_clibs/ftSynthetic --library
	$(CC) c_tests/src/mk_tbls_zipf.c -o c_tests/mk_tbls_zipf.o $(CFLAGS) $(DEPS) ft_clibs/ftSynthetic.c -lm

Joins:
	mkdir -p ft_clibs
	futhark cuda ft_libs/join_entry.fut -o ft_clibs/join_entry --library
	$(CC) ft_clibs/join_entry.c -o ft_clibs/libjoin.so $(CFLAGS) $(CUDAFLAGS) $(LIBFLAGS)

C-Joins:
	mkdir -p ft_clibs
	futhark c ft_libs/join_entry.fut -o ft_clibs/join_entry --library