# DuckDB-Futhark-Project
Study of GPU Algorithms on DuckDB Using the Futhark Programming Language

by: Artemios Triantaphyllou

supervised by prof. Vasileios Samoladas

Technical University of Crete, Department of Electrical and Computer Engineering


The purpose of this project is to implement some GPU-accelerated database algorithms, using the tools DuckDB and The Futhark Programming Language. 

Most former literature on GPU-accelerated database algorithms assumes control of low-level parameters. Futhark is quite divorced from the GPU hardware, being a functional programming language that leaves hardware optimisations to the compiler; thus different techniques from prior work may need to be employed.

Examples of algorithms to be implemented are: state-of-the-art join algorithms, possibly group-by's, sketches, and density-based clustering. This list will be updated.

Futhark is used to create parallel C functions, which are combined with DuckDB's C API.
