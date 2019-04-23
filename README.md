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
python3 exactDSP-python/exactDSP.py * *input_file*
```

The result appears in the **results_exact** folder.

### C++ code

Currently doesn't work

## Approximation algorithms
### C++ code

To compile the C++ code, use:
```
g++ -o dsp recursive_peeling.cpp
```

To run the code, use:
```
./dsp * *number_of_iterations input_file output_file solution_file* *
```

The last two arguments are optional.


