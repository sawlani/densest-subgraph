gcc -O2 -Wall -DNO_MAIN hi_pr.c -c -o hi_pr.o
g++ -O2 densest.cpp hi_pr.o -o densest
