#!/bin/bash
g++ -O3 iterative_peeling.cpp -o ip
g++ -O3 iterative_peeling_naive_weighted.cpp -o ipnw
cd exactDSP-cpp
gcc -O3 -Wall -DNO_MAIN hi_pr.c -c -o hi_pr.o
g++ -O3 densest.cpp hi_pr.o -o ../exact
g++ -O3 densest_weighted.cpp hi_pr.o -o ../exactweighted
cd ..