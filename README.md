# hierarchical-matrices-lib

This is an incomplete C++ library for efficiently computing approximations of the most common matrix calculations via hierarchical matrices.

Note that input matrices can only be square!

To use this library in your code, download the repository and use #include "hierarchical_matrices_lib.cpp"
while making sure that the corresponding file is reachable from the code where you've included it.
You will at least need the Lapacke library and at most the development tools listed below to compile your code now.

The only file that should be changed is "user_settings", in which you will be able to switch between the datatypes  
float, double and complex floats. If you are only using float, there is no need to change the settings  
since it is the default datatype.

Please see the included documentation file for further instructions on how to use this library.  
Any mentioning of pages refer to the book "Hierarchical Matrices", see below.  

Concrete examples can be found in the "example.cc". If you're using the tools as listed below,
changing the paths in the Makefile should make you able to compile the code via typing "make" into a terminal/console.

## Available functions:

* Constructor
* Matrix-matrix addition
* Matrix-vector multiplication

## Used sources/tools for development:

* "Hierarchical Matrices" by Mario Bebendorf from 2008  
(see https://link.springer.com/book/10.1007/978-3-540-77147-0)  
Specifically the first two chapters about "Low-Rank Matrices and Matrix Partitioning" and "Hierarchical Matrices"
* OS: Windows
* C++ version: C17
* Compiler: MinGW 32
* Libraries: LAPACKE 3.10 (Instructions: https://icl.utk.edu/lapack-for-windows/lapack/#lapacke)  
--> Needs C, C++ and Fortran compiler (e.g. gcc, g++ and gfortran included in MinGW)
* Documentation: Doxygen
