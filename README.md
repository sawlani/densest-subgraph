# densest-subgraph

## File input type
Every program reads an edge list in the following format:

```
n m
u1 v1
u2 v2
.
.
.
um vm
```

## Exact algorithm

### C++ code

To compile and run the code, use
```
cd exactDSP-cpp
gcc -O4 -Wall -DNO_MAIN hi_pr.c -c -o hi_pr.o
g++ -O3 densest.cpp hi_pr.o -o ../exact
cd ..
./exact <multiplier> < input_file
```

## Approximation algorithms
### Recursive peeling: C++ code

To compile the C++ code, use:
```
g++ -O3 recursive_peeling_simple.cpp -o rps
./rps <number_of_iterations> < input_file
```

### MWU: C++ code

To compile the C++ code, use:
```
cd approxDSP-MWU
g++ -o mwu dsp_mwu.cpp
```

To run the code, use:
```
./mwu number_of_iterations accuracy input_file output_file
```
The last argument is optional.

The results are on the command line.

### MWU-version2
To use the alternate MWU formulation,
replace
```
g++ -o mwu dsp_mwu.cpp
```
with
```
g++ -o mwu dsp_mwu2.cpp
```
