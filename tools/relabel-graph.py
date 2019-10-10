#!/usr/bin/python

import sys

graph = open(sys.argv[1])
out_graph = open(sys.argv[2], 'w')


vid = 1
vid_map = {}
m = 0
for line in graph:
    if (line[0] == '#') or (line[0] == '%'):
        continue

    e = list(map(int, line.split()))
    if not e[0] in vid_map:
        vid_map[e[0]] = vid
        vid += 1
    if not e[1] in vid_map:
        vid_map[e[1]] = vid
        vid += 1

n = vid-1
edges = [set() for i in range(n+1)]

graph.seek(0)
for line in graph:
    if (line[0] == '#') or (line[0] == '%'):
        continue

    e = list(map(int, line.split()))
    u = vid_map[e[0]]
    v = vid_map[e[1]]
    if (max(u,v) not in edges[min(u,v)]) and (u != v): # don't add edges more than once and delete self loops
    	edges[min(u, v)].add(max(u, v))
    	m+=1
graph.close()

out_graph.write(str(n) + ' ' + str(m) + '\n')
for l in enumerate(edges):
    for j in l[1]:
    	out_graph.write(str(l[0]) + ' ' + str(j) + '\n')
out_graph.close()
