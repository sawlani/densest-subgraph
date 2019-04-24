# densest-subgraph

## File input type
Every program reads an edge list in the following format:

```
m n
u1 v1
u2 v2
.
.
.
um vm
```

## Exact algorithm
### Python code

To run the exact max-flow based algorithm,
use
```
python3 exactDSP-python/exactDSP.py input_file
```

The result gets appended to the **results/results_exact** file.

### C++ code

To compile and run the code, use
```
cd exactDSP-cpp
chmod +x compile.sh
./densest < input_file
```
The results are on the command line.

## Approximation algorithms
### Recursive peeling: C++ code

To compile the C++ code, use:
```
g++ -o dsp recursive_peeling.cpp
```

To run the code, use:
```
./dsp number_of_iterations input_file output_file solution_file
```
The last two arguments are optional.

### MWU: C++ code

To compile the C++ code, use:
```
cd approxDSP-MWU
g++ -o mwu dsp_mwu.cpp
```

To run the code, use:
```
./mwu number_of_iterations accuracy input_file output_file solution_file
```
The last argument are optional.

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