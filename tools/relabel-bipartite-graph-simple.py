#!/usr/bin/python

import sys

graph = open(sys.argv[1])
out_graph = open(sys.argv[2], 'w')
leftV = int(sys.argv[3])

for line in graph:
    if (line[0] == '#') or (line[0] == '%'):
        continue

    e = map(int, line.split())
    e[1] += leftV
    out_graph.write(str(e[0]) + ' ' + str(e[1]) + '\n')