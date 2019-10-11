#!/usr/bin/python

import sys

verticesdata = open(sys.argv[1])
triangledata = open(sys.argv[2])

vertices = set()
for line in verticesdata:
    t = int(line)
    vertices.add(t)

verticesdata.close()

triangles = 0
for line in triangledata:
    t = list(map(int, line.split()))
    if t[0] in vertices and t[1] in vertices and t[2] in vertices:
        triangles += 1
triangledata.close()
if(len(vertices)==0):
	td=0
else:
	td = float(triangles)/len(vertices)
print(td)