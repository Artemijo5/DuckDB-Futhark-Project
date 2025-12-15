# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering

(Pending to organise better)

-----------------------------------------
Preparation:
- add the files from duckdb's [Linux C installation page](https://duckdb.org/docs/installation/?version=stable&environment=cplusplus&platform=linux&download_method=direct&architecture=x86_64) to clibs/
- (Optional) have duckdb installed to create test tables and see results outside of the C API
- have futhark installed in order to compile .fut files
- install [futhark's sort libraries](https://github.com/diku-dk/sorts) in /ft_libs
-----------------------------------------
To use:
1. export LD_LIBRARY_PATH using the string in ./set_path
2. compile futhark libraries (`make CUDA-LIB`, or `make C-LIB` for sequential compilation)
3. build the desired benchmark (located in /benchmarks/src, see makefile for options)
4. run the desired benchmark from a terminal in the project directory
-----------------------------------------
Current futhark libraries:
- ftRelational : used by sort, joins, group-by aggregation
- ftSkyline : used by Skyline (TODO revise)
- ftDBSCAN : used by DBSCAN
-----------------------------------------
The purpose of this project is to implement some GPU-based relational algorithms using the futhark programming language, identifying techniques for functional GPU database programming and evaluating performance.

Algorithms currently implemented:
- Sorting
- Relational Joins (SMJ, Radix Hash Join - based on https://arxiv.org/abs/2312.00720 and references)
- Group-by Aggregation
- Skyline Queries
- DBSCAN (currently only for datasets that fit fully in the GPU)

Algorithms to be implemented:
- Extensions of Joins to String data
- (possibly) Sketch Algorithms

-----------------------------------------
Main tools used:
- [DuckDB](https://duckdb.org/) is an in-process OLAP database. In this project, it is invoked through its [C API](https://duckdb.org/docs/stable/clients/c/overview).
- [Futhark](https://futhark-lang.org/index.html) is a functional progamming language that can be compiled to sequential C code or parallel CUDA or OpenCL code. Futhark functions are invoked through a functional core from C code.
