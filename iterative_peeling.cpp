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

int n, m;

struct Edge {
  int y, next;
};

struct Node {
  int deg, next, prev, idx;
  inline void clear() {
    deg = next = prev = 0;
    idx = -1;
  }
};

Node * lists;

__inline void linklists (int x, int y) {
  if(y == 0) return;
  lists[x].next = y;
  lists[y].prev = x;
};

int * nxt, * prv, *itr;

__inline void linknodes (int x, int y) {
  if(y == -1) return;
  nxt[x] = y;
  prv[y] = x;
};

__inline void eraselist(int x) {
  lists[lists[x].prev].next = lists[x].next;
  if(lists[x].next != 0) lists[lists[x].next].prev = lists[x].prev;
};

__inline void erasenode (int x) {
  if(prv[x] == -1) {
    lists[itr[x]].idx = nxt[x];
  }
  if(prv[x] != -1) nxt[prv[x]] = nxt[x];
  if(nxt[x] != -1) prv[nxt[x]] = prv[x];

};

int l = 0;
Edge * edges;
int * idx;

__inline void build(int x, int y) {
    edges[++l].next = idx[x];
    edges[l].y = y;
    idx[x] = l;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char** argv) {

  auto startio = chrono::steady_clock::now();

  int iters = atoi(argv[1]);
  
  n = getInt(); m = getInt();
  edges = new Edge[m * 2 + 10];
  idx = new int[n];
  memset(idx, 0, sizeof(int) * n);
  int * init_deg = new int[n];
  memset(init_deg, 0, sizeof(int) * n);
  l = 0;
  
  lists = new Node[n + 2 * m + 10];
  int n_list = 0;
  itr = new int[n];
  int * deg = new int[n], * w = new int[n], * pos = new int [n];
  memset(deg, 0, sizeof(int) * n);
  memset(w, 0, sizeof(int) * n); //initial vertex weights=0, i.e., no self loops at the start 
  memset(pos, 0, sizeof(int) * n);
  prv = new int[n]; nxt = new int[n];
  for (int i = 0; i < m; i++) {
    int p, q;
    p = getInt(); q = getInt();
  	p -= 1;
    q -= 1;
    build(p, q);
    build(q, p);
    init_deg[p]++;
    init_deg[q]++;
  }
  pair<int, int> * deg_sorted = new pair<int, int>[n];
  vector<int> m_ans;
  double mm_density = 0;

  auto endio = chrono::steady_clock::now();
  int sum_iter_times = 0;
  int init_time = chrono::duration_cast<chrono::milliseconds>(endio - startio).count();
  
  cout << "Time for reading input and initialization: " << init_time << " ms" << endl;
  
  for (int tt = 0; tt < iters; tt++) {

  	auto startiter = chrono::steady_clock::now();

    for (int i = 0; i < n; i++) {
      nxt[i] = prv[i] = -1;
      pos[i] = 0;
      
      deg[i] = w[i] + init_deg[i]; //degree for this iteration is "vertex weight" + actual degree
      deg_sorted[i] = make_pair(deg[i], i);
    }
    sort(deg_sorted, deg_sorted + n);
    n_list = 0;

    for(int i = 0; i < n; i++) {
      int v = deg_sorted[i].second;
      if(n_list == 0 || lists[n_list].deg != deg_sorted[i].first) {
        ++n_list;
        lists[n_list].clear();
        linklists(n_list - 1, n_list);
        lists[n_list].deg = deg_sorted[i].first;
      }
      
      linknodes(v, lists[n_list].idx);
      lists[n_list].idx = v;
      itr[v] = n_list;
    }
    
    double max_density = (double)m / n;
    int cur_m = m, cur_n = n;
    vector<int> ans;
    int max_size = 0;
    while(lists[0].next) {
      
      int i = lists[0].next;
      int k = lists[i].idx;
      
      if(nxt[k] == -1) {
        eraselist(i);
      }else {
        erasenode(k);
      }
      pos[k] = -1;
      w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
      cur_n -= 1;
      ans.push_back(k);
      for (int p = idx[k]; p; p = edges[p].next) { //decrement degrees of k's neighbors
        int j = edges[p].y;
        if(pos[j] == -1) continue;
        cur_m -= 1;
        
        int i = itr[j];
        erasenode(j);
        int i1 = lists[i].prev;
        
        if(lists[i].idx == -1) eraselist(i);
        deg[j]--;       
        prv[j] = nxt[j] = -1;
        if(i1 == 0 || lists[i1].deg != deg[j]) {
          ++n_list;
          lists[n_list].clear();
          itr[j] = n_list;
          int i2 = lists[i1].next;
          lists[n_list].deg = deg[j];
          lists[n_list].idx = j;
          linklists(i1, n_list);
          if(i2) linklists(n_list, i2);
        }
        else {
          linknodes(j, lists[i1].idx);
          lists[i1].idx = j;
          itr[j] = i1;
        }
      }
      if(cur_n == 0) continue;
      if(max_density < (double)cur_m / cur_n) {
        max_size = ans.size();
      }
      max_density = max(max_density, (double)cur_m / cur_n);
    }
    reverse(ans.begin(), ans.end());
    ans.resize(n - max_size);
    if(max_density > mm_density) {
      m_ans = ans;
      mm_density = max_density;
    }

    auto enditer = chrono::steady_clock::now();
    int elapsed = chrono::duration_cast<chrono::milliseconds>(enditer - startiter).count();
    sum_iter_times += elapsed;

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
  for (int i : m_ans)
  {
    outfile << i+1 << endl;
  }

  return 0;
}