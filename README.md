# densest-subgraph

## File input type
For algorithms for unweighted graphs, the input graph file is required to be of the format:

```
n m
u1 v1
u2 v2
.
.
.
um vm
```

For algorithms for weighted graphs, the input graph file is required to be of the format:

```
n m
u1 v1 w1
u2 v2 w2
.
.
.
um vm wm
```


To compile all four versions of the code, i.e.,

1. exact max-flow based algorithm for unweighted graphs
2. exact max-flow based algorithm for weighted graphs
3. approximate iterative peeling for unweighted graphs
4. approximate iterative peeling for weighted graphs

simply run
```
sudo chmod +x compile_all.sh
./compile_all.sh
```

To run the code, the syntax goes as follows:

1. exact max-flow based algorithm for unweighted graphs

```
./exact multiplier < input_file
```

2. exact max-flow based algorithm for weighted graphs

```
./exactweighted multiplier < input_file
```

3. approximate iterative peeling for unweighted graphs

```
./ip no_of_iterations < input_file
```

4. approximate iterative peeling for weighted graphs

```
./ipnw no_of_iterations < input_file
```
