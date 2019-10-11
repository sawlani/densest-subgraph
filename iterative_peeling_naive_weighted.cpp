// Saurabh Sawlani

// Find maximum subgraph density for weighted graphs
// Uses BBST to store intermediate degrees
// 6-10 x slower than using linked lists, but simpler to understand

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

////////////////////////////////////////////////////////////////////////////////////////
// Helper for fast input

inline char GET_CHAR(){
	const int maxn = 131072;
	static char buf[maxn],*p1=buf,*p2=buf;
	return p1==p2&&(p2=(p1=buf)+fread(buf,1,maxn,stdin),p1==p2)?EOF:*p1++;
}
inline int getInt() {
	int res(0);
	char c = GET_CHAR();
	while(c < '0') c = GET_CHAR();
	while(c >= '0') {
		res = res * 10 + (c - '0');
		c = GET_CHAR();
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////////////

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
  //cin >> n >> m;
  n = getInt(); m = getInt();
  int * init_deg = new int[n];
  memset(init_deg, 0, sizeof(int) * n);
  
  int * w = new int[n];
  memset(w, 0, sizeof(int) * n);
  
  deg.reserve(n);

  set<pair<int, int>> * nbrs = new set<pair<int, int>>[n];

  int sum_wts = 0;

  for (int i = 0; i < m; i++) {
    int p, q, wt;
    p = getInt(); q = getInt(), wt = getInt();
  	//cin >> p >> q >> wt;
  	p -= 1;
    q -= 1;
    nbrs[p].insert(make_pair(q,wt));
    nbrs[q].insert(make_pair(p,wt));
    init_deg[p]+=wt;
    init_deg[q]+=wt;
    sum_wts += wt;
  }

  double mm_density = 0;

  vector<bool> exists(n);
  vector<bool> iterans(n);
  vector<bool> ans(n);

  auto endio = chrono::steady_clock::now();
  int init_time = chrono::duration_cast<chrono::milliseconds>(endio - startio).count();
  
  cout << "Time for reading input and initialization: " << init_time << " ms" << endl;
  
  int sum_iter_times = 0;

  for (int tt = 0; tt < iters; tt++) {

  	auto startiter = chrono::steady_clock::now();

  	deg_sorted.clear();

    for (int i = 0; i < n; i++) {
      deg[i] = w[i] + init_deg[i]; //degree for this iteration is "vertex weight" + actual degree
      deg_sorted.insert(i);
    }
        
    double iter_max_density = (double) sum_wts / n;
    int cur_sum_wts = sum_wts, cur_n = n;

    fill(exists.begin(), exists.end(), true);
    iterans = exists;

    while (cur_n > 0) {
      cur_n--;
      int k = *(deg_sorted.begin()); //k = min degree vertex
      w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
      deg_sorted.erase(k); //delete k
      for (pair<int, int> j : nbrs[k]) { //decrement degrees of k's neighbors
        int nbr = j.first;
    	int nbrwt = j.second;
        if (exists[nbr]) {
          deg_sorted.erase(nbr);
          deg[nbr] -= nbrwt;
          cur_sum_wts -= nbrwt;
          deg_sorted.insert(nbr);
        }
        exists[k] = false;
      }
      if (iter_max_density < (double) cur_sum_wts / cur_n) {
        iter_max_density = (double) cur_sum_wts / cur_n;
        iterans = exists;
      }
    }
    
    if(iter_max_density > mm_density) {
      mm_density = iter_max_density;
      cout << "mm changing " << mm_density << endl;
      ans = iterans;
    }

    auto enditer = chrono::steady_clock::now();
    int elapsed = chrono::duration_cast<chrono::milliseconds>(enditer - startiter).count();
    sum_iter_times += elapsed;
    
    cout << "Max density AT iteration " << tt+1 <<": " << iter_max_density << endl;
    cout << "Max density until iteration " << tt+1 <<": " << mm_density << endl;
    cout << "Avg time per iteration: " << sum_iter_times/(tt+1) << " ms" << endl;
    cout << "Total time: " << sum_iter_times + init_time << " ms" << endl;
  }

  string output_file;
  ofstream outfile;

  if (argc >= 3)
  {
    output_file = argv[2];
  }
  else
  {
    output_file = "soln.tmp";
  }

  outfile.open(output_file.c_str());
  for (int i=0;i<n;i++)
  {
    if (ans[i]) outfile << i+1 << endl;
  }

  return 0;
}