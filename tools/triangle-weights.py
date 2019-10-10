#!/usr/bin/python

import sys

graph = open(sys.argv[1])
triangledata = open(sys.argv[2])
out_graph = open(sys.argv[3], 'w')

triangles = {}
for line in triangledata:
    t = list(map(int, line.split()))
    print(t)
    if len(t)>3:
        continue
    e1 = (min(t[0],t[1]), max(t[0],t[1]))
    e2 = (min(t[1],t[2]), max(t[1],t[2]))
    e3 = (min(t[0],t[2]), max(t[0],t[2]))
    for e in [e1, e2, e3]:
        if not e in triangles:
            triangles[e] = 1
        else:
            triangles[e] += 1
    print(triangles)

triangledata.close()

first_line_flag = True
for line in graph:
    if first_line_flag:
        first_line_flag = False
        out_graph.write(line)
        continue

    e = list(map(int, line.split()))
    e = (min(e[0],e[1]), max(e[0],e[1]))
    if e in triangles:
        t = triangles[e]
    else:
        t = 0
    out_graph.write(str(min(e[0],e[1])) + ' ' + str(max(e[0],e[1])) + ' ' + str(t) + '\n')

graph.close()
out_graph.close()
