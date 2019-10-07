// Saurabh Sawlani

// Find maximum subgraph density
// Uses BBST to store intermediate degrees
// 6-10 x slower than recursive_peeling_yu, but simpler to understand

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include<cassert>
#include <vector>   
#include <queue> 
#include<list>
#include <set>
#include<cstring>
#include<ctime>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;

vector<int> deg;

struct classcomp {
  bool operator() (const int& lhs, const int& rhs) const
  {return deg[lhs]<deg[rhs] || (deg[lhs]==deg[rhs] && lhs<rhs);}
};

set<int,classcomp> deg_sorted; //BBST storing degrees

//////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

  cout << "Finding maximum subgraph density (naive version using BBST)..." << endl;
  
  auto startio = chrono::steady_clock::now();

  int iters = atoi(argv[1]);
  int m, n;
  cin >> n >> m;
  int * init_deg = new int[n];
  memset(init_deg, 0, sizeof(int) * n);
  
  int * w = new int[n];
  memset(w, 0, sizeof(int) * n);
  
  deg.reserve(n);

  unordered_set<int> * nbrs = new unordered_set<int>[n];

  for (int i = 0; i < m; i++) {
    int p, q;
    cin >> p >> q;
  	p -= 1;
    q -= 1;
    nbrs[p].insert(q);
    nbrs[q].insert(p);
    init_deg[p]++;
    init_deg[q]++;
  }

  auto endio = chrono::steady_clock::now();
  int init_time = chrono::duration_cast<chrono::milliseconds>(endio - startio).count();
  
  cout << "Time for reading input and initialization: " << init_time << " ms" << endl;
  
  double mm_density = 0;

  int sum_iter_times = 0;
  vector<bool> exists(n);

  for (int tt = 0; tt < iters; tt++) {

  	auto startiter = chrono::steady_clock::now();

  	deg_sorted.clear();

    for (int i = 0; i < n; i++) {
      deg[i] = w[i] + init_deg[i]; //degree for this iteration is "vertex weight" + actual degree
      deg_sorted.insert(i);
    }
        
    double iter_max_density = (double)m / n;
    int cur_m = m, cur_n = n;

    fill(exists.begin(), exists.end(), true);

    while (cur_n > 0) {
      cur_n--;
      int k = *(deg_sorted.begin()); //k = min degree vertex
      w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
      deg_sorted.erase(k); //delete k
      for (int j : nbrs[k]) { //decrement degrees of k's neighbors
        if (exists[j]) {
          deg_sorted.erase(j);
          deg[j] -= 1;
          cur_m --;
          if (deg[j] == 0) {
            cur_n--;
            exists[j] = false;
          }
          else {
            deg_sorted.insert(j);
          }
        }
        exists[k] = false;
      }
      if (iter_max_density < (double) cur_m / cur_n) {
        iter_max_density = (double) cur_m / cur_n;
      }
    }
    
    if(iter_max_density > mm_density) {
      mm_density = iter_max_density;
    }

    auto enditer = chrono::steady_clock::now();
    int elapsed = chrono::duration_cast<chrono::milliseconds>(enditer - startiter).count();
    sum_iter_times += elapsed;
    
    cout << "Max density AT iteration " << tt+1 <<": " << iter_max_density << endl;
    cout << "Max density until iteration " << tt+1 <<": " << mm_density << endl;
    cout << "Avg time per iteration: " << sum_iter_times/(tt+1) << " ms" << endl;
    cout << "Total time: " << sum_iter_times + init_time << " ms" << endl;
  }

  return 0;
}