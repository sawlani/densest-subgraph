#!/bin/bash
./tools/mace22/mace Ce -l 3 -u 3 $1 $1-triangles
python tools/triangle-weights.py $1 $1-triangles $1-triangle-weighted
