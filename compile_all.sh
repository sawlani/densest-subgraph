g++ -O3 recursive_peeling_simple.cpp -o rps
cd exactDSP-cpp
gcc -O3 -Wall -DNO_MAIN hi_pr.c -c -o hi_pr.o
g++ -O3 densest.cpp hi_pr.o -o ../exact


