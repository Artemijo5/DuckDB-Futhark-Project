# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering

-----------------------------------------
Preparation:
- add the files from duckdb's [Linux C installation page](https://duckdb.org/docs/installation/?version=stable&environment=cplusplus&platform=linux&download_method=direct&architecture=x86_64) to `clibs/`
- (Optional) have duckdb installed to create test tables and see results outside of the C API
- have futhark installed in order to compile .fut files
- install [futhark's sort libraries](https://github.com/diku-dk/sorts) in `ft_libs/`
-----------------------------------------
To use:
1. `source set_path` to set LD_LIBRARY_PATH
2. compile futhark libraries (`make CUDA-LIB`, or `make C-LIB` for sequential compilation)
3. build the desired benchmark (located in `benchmarks/src/`, see makefile for options)
4. run the desired benchmark from a terminal in the project directory
Also check the futhark benchmarks in ft_libs/bench (WIP)
-----------------------------------------
Current futhark libraries:
- `ftRelational` : sort, joins, group-by aggregation
- `ftSkyline` : Skyline Computation
- `ftDBSCAN` : DBSCAN (entire dataset is processed at once, quadratic space)
- `ftDBSCAN_plus` : DBSCAN (directory-level index, WIP)
-----------------------------------------
The purpose of this project is to implement some GPU-based relational algorithms using the futhark programming language, identifying techniques for functional GPU database programming and evaluating performance.

Algorithms currently implemented:
- Relational Joins (SMJ, Radix Hash Join - based on https://arxiv.org/abs/2312.00720 and references) (note: Hash Joins are temporarily not being compiled)
- Group-by Aggregation
- Skyline Queries
- DBSCAN

-----------------------------------------
Main tools used:
- [DuckDB](https://duckdb.org/) is an in-process OLAP database. In this project, it is invoked through its [C API](https://duckdb.org/docs/stable/clients/c/overview).
- [Futhark](https://futhark-lang.org/index.html) is a functional progamming language that can be compiled to sequential C code or parallel CUDA or OpenCL code. Futhark functions are invoked through a functional core from C code.
