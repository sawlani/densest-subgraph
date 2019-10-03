//
// Random sampling based pagerank
// followed by Lanczos type SVD
//

#include <bits/stdc++.h>
using namespace std; 

#define MP make_pair 
#define A first 
#define B second 

#define PB push_back 
#define FR(i, a, b) for(int i=(a); i<(b); i++) 
#define FOR(i, n) FR(i, 0, n) 

const int MAXV = 2000000;

int n, m;
vector<int> e[MAXV];

char line[123456], temp1[123], temp2[123];

int main(int argt, char **args) {
  while(gets(line)) {
    if(line[0] == 'p') {
      sscanf(line, "%s%s%d%d", temp1, temp2, &n, &m);
    } else if(line[0] =='a') {
      int u, v, weight;
      sscanf(line, "%s%d%d%d", temp1, &u, &v, &weight);
weight = 1;
      FOR(blah, weight) {
        e[u].PB(v);
        e[v].PB(u);
      }
    }
  }
  m = 0;
  FOR(u, n) m += e[u].size();
  m /= 2;

  printf("%d %d\n", n, m);
  FOR(u, n) {
    for(auto v:e[u]) if(v > u) {
      printf("%d %d\n", u + 1, v + 1);
    }
  }
  return 0;
}
