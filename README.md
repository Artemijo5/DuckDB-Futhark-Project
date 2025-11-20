# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering

-----------------------------------------
To use:
- add libduckdb.so and libduckdb_static.a from duckdb's [Linux C installation page](https://duckdb.org/docs/installation/?version=stable&environment=cplusplus&platform=linux&download_method=direct&architecture=x86_64) to the project directory
- (Optional) have duckdb installed to create test tables and see results outside of the C API
- have futhark installed in order to compile .fut files (otherwise have futhark files compiled in the last commit)
- make sure that the directory is on the load path, eg ```` export LD_LIBRARY_PATH=`pwd` ````
- see makefile for compilation options
- for CUDA backend, linking done with -lcuda -lcudart -lnvrtc https://futhark.readthedocs.io/en/latest/man/futhark-cuda.html
-----------------------------------------
Current makefile options:
- ```C-ftSMJ```: (requires futhark) compiles SMJ futhark library to sequential C code & .so files
- ```CUDA-ftSMJ```: (requires futhark AND cuda installed) compiles SMJ futhark library to parallel CUDA code & .so files
- ```CUDA1-ftSMJ```: (requires only futhark) compiles SMJ futhark library to CUDA code files, which can then be compiled to .so
- ```CUDA2-ftSMJ```: (requires only CUDA) compiles CUDA code files of SMJ to .so files
The remainder are testing applications that process data from duckdb through the futhark core.
-----------------------------------------
The purpose of this project is to implement some GPU-accelerated database algorithms, using the tools DuckDB and The Futhark Programming Language. 

Former literature on GPU-accelerated database algorithms tends to assume control of low-level parameters, such as utilisation of each level of GPU cache, etc.
Futhark is a relatively high-level, functional programming language, and leaves much of hardware optimisation to the compiler; thus different techniques from prior work may need to be employed. DuckDB also requires adopting appropriate workflows with the data outside the GPU.

Algorithms currently implemented (still to be optimised & benchmarked):
- Relational Table Sorting (one-pass or two-pass)
- Relational Joins (SMJ, Radix Hash Join - based on https://arxiv.org/abs/2312.00720 and references)
- Group-by Aggregation
Algorithms currently being implemented:
- Skyline Queries
- DBSCAN
- Extensions of Joins to String data
- (possibly) Sketch Algorithms


Other tasks planned:
- containerise applications with docker and nvidia-ctk
- make applications callable directly from DuckDB (if possible - alternatively maybe a simple CLI)

-----------------------------------------
Tools used:
- [DuckDB](https://duckdb.org/) is an in-process OLAP database. In this project, it is invoked through its [C API](https://duckdb.org/docs/stable/clients/c/overview).
- [Futhark](https://futhark-lang.org/index.html) is a functional progamming language that can be compiled to sequential C code or parallel CUDA or OpenCL code. Futhark functions are invoked through a functional core from C code.
