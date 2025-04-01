# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering

-----------------------------------------
To use:
- add libduckdb.so and libduckdb_static.a from duckdb's Linux C installation page to the project directory https://duckdb.org/docs/installation/?version=stable&environment=cplusplus&platform=linux&download_method=direct&architecture=x86_64
- add futhak sorts package to the project directory https://github.com/diku-dk/sorts/tree/master (you must have futhark installed first)
- make sure that the directory is on the load path, eg ```` export LD_LIBRARY_PATH=`pwd` ````
- compile ftsort.fut as a library using the desired backend, and then compile ftsort.c to libftsort.so (with -fPIC -shared)
- likewise compile mylogger.c to libmylogger.so (with -fPIC -shared)
- compile sorting_test.c to sorting_test.o using libduckdb.so, libmylogger.so, libftsort.so
- IF CUDA BACKEND the last part requires compilation with nvcc, linking with -lcuda -lcudart -lnvrtc https://futhark.readthedocs.io/en/latest/man/futhark-cuda.html
-----------------------------------------

The purpose of this project is to implement some GPU-accelerated database algorithms, using the tools DuckDB and The Futhark Programming Language. 

Most former literature on GPU-accelerated database algorithms assumes control of low-level parameters. Futhark is quite divorced from the GPU hardware, being a functional programming language that leaves hardware optimisations to the compiler; thus different techniques from prior work may need to be employed.

Currently planned algorithms to implement:
- 2-pass sorting
- Partitioning
- Parallel join algorithms, such as the ones described in https://arxiv.org/abs/2312.00720
- Parallel group-by algorithms described in the above paper
- Possibly: AMS Sketches
- Possibly: Density-based Clustering, such as DBSCAN or HDBSCAN

Futhark is used to create parallel C functions, which are combined with DuckDB's C API.
