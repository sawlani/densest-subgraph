#!/bin/bash
./tools/mace22/mace Ce -l 3 -u 3 $1 $1-triangles
python tools/triangle-weights.py $1 $1-triangles $1-triangle-weighted
./ipnw 10 soln.tmp < $1-triangle-weighted
python tools/count_triangles.py soln.tmp $1-triangles
rm -f $1-triangles $1-triangle-weighted
