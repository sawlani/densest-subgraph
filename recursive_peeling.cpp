#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>   
#include <queue> 
#include <set>
#include <unordered_set>
using namespace std;

const int maxn = 100;
const int maxm = 1000;

unordered_set<int> g[maxn];
int e[maxm][2];
double deg[maxn], w[maxn];
int n, m;
double eps = 0.01;
string ss = "30";
string st1 ("tests/"+ss);
string st2 ("tests/"+ss+".a");

struct classcomp {
  bool operator() (const int& lhs, const int& rhs) const
  {return deg[lhs]<deg[rhs] || (deg[lhs]==deg[rhs] && lhs<rhs);}
};

set<int,classcomp> ordered_deg;

int main() {
	freopen (st1.c_str(), "r", stdin);
	freopen ("test.out", "w", stdout);
	cin >> n >> m;
	for (int i = 0; i < n; i++) 
		w[i] = 0;
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		p -= 1;
		q -= 1;
		e[i][0] = p;
		e[i][1] = q;
	}
	for (int tt = 0; tt < int(1.0/eps); tt++) {
		ordered_deg.clear();
		for (int i = 0; i < n; i++) {
			deg[i] = w[i];
			g[i].clear();			
		} 
		for (int i = 0; i < m; i++) {
			int p = e[i][0], q = e[i][1];
			deg[p] += eps;
			deg[q] += eps;
			g[p].insert(q);
			g[q].insert(p);
		}
		for (int i = 0; i < n; i++)
			ordered_deg.insert(i);
		for (int i = 0; i < n; i++) {
			int k = *(ordered_deg.begin());
			w[k] = deg[k];
			ordered_deg.erase(k);
			for (int j : g[k]) {
				ordered_deg.erase(j);
				deg[j] -= eps;
				ordered_deg.insert(j);
				g[j].erase(k);	
			}
		}
	}
	double ds = 0;
	for (int i = 0; i < n; i++)
		if (ds < w[i])
			ds = w[i];
	cout << ds << endl;
	freopen (st2.c_str(), "r", stdin);
	int t;
	cin >> t;
	int a[maxn];
	for (int i = 0; i < t; i++) 
		cin >> a[i];
	for (int i = 0; i < n; i++) 
		g[i].clear();			
	for (int i = 0; i < m; i++) {
		int p = e[i][0], q = e[i][1];
		g[p].insert(q);
		g[q].insert(p);
	}
	int x = 0;
	for (int i = 0; i < t; i++)
		for (int j = 0; j < t; j++)
			if (g[a[i]-1].count(a[j]-1)>=1)
				x += 1;
	cout << double(x)/t/2;
	return 0;
}