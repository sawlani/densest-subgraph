#!/usr/bin/python

import sys

graph = open(sys.argv[1])
out_graph = open(sys.argv[2], 'w')


for line in graph:
    e = list(map(int, line.split()))
    if len(e) == 2:
    	out_graph.write(line)
    	continue
    out_graph.write(str(e[0]) + ' ' + str(e[1]) + ' ' + str(-e[2]) + '\n')
out_graph.close()
