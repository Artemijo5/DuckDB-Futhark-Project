# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering

# NOTICE

code for thesis implementations and evaluation

plan to separate into distinct Futhark extensions for
- basic routines
- SMJ
- Strings
- spatial indexing
- DBSCAN
- (possibly) Skyline computation
-----------------------------------------

- ft_libs: contains futhark libraries
- c_tests: contains C scripts calling Futhark kernels in conjunction with DuckDB loading
- c_libs: libraries used by scripts in c_tests

Currently gnore shell scripts in top directory (were used to run benchmarks on specific datasets).

-----------------------------------------
Preparation:
- add the files from duckdb's [Linux C installation page](https://duckdb.org/docs/installation/?version=stable&environment=cplusplus&platform=linux&download_method=direct&architecture=x86_64) to `clibs/`
- (Optional) have duckdb installed to create test tables and see results outside of the C API
- have futhark installed in order to compile .fut files
- install [futhark's sort libraries](https://github.com/diku-dk/sorts) in `ft_libs/`
-----------------------------------------
To use:
1. `source set_path` to set LD_LIBRARY_PATH
2. compile futhark libraries (see options in `makefile`)
3. to run a script in c_tests, compile with gcc and link manually with referenced libraries (TODO add makefile options)
Also check the futhark benchmarks in ft_libs/bench (WIP)
-----------------------------------------
Current implementations:
- Sort-Merge Join, Partitioned Hash Join based on https://arxiv.org/abs/2312.00720 and references
- String-processing functions
- DBSCAN implementations based on https://dl.acm.org/doi/10.1145/3605573.3605594, https://github.com/l3lackcurtains/fast-cuda-gpu-dbscan
-----------------------------------------
The purpose of this project is to implement some GPU-based relational algorithms using the futhark programming language, identifying techniques for functional GPU database programming and evaluating performance.

Currently, see benchmarks in ft_libs/bench
-----------------------------------------
- [DuckDB](https://duckdb.org/) is an in-process OLAP database. In this project, it is invoked through its [C API](https://duckdb.org/docs/stable/clients/c/overview).
- [Futhark](https://futhark-lang.org/index.html) is a functional progamming language that can be compiled to sequential C code or parallel CUDA or OpenCL code. Futhark functions are invoked through a functional core from C code.
