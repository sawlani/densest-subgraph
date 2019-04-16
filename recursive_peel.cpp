#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>   
using namespace std;

const int maxn = 100;
const int maxm = 1000;

bool g[maxn][maxn], h[maxn][maxn];
int e[maxm][2];
double deg[maxn], w[maxn];
bool alive[maxn];
int n, m;
double eps = 0.000001;
string ss = "30";
string st1 ("tests/"+ss);
string st2 ("tests/"+ss+".a");

double abs(double x) {
	if (x>0)
		return x;
	else
		return -x;
}

int main() {
	freopen (st1.c_str(), "r", stdin);
	freopen ("test.out", "w", stdout);
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			g[i][j] = false;
	for (int i = 0; i < n; i++)
		w[i] = 0;
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		p -= 1;
		q -= 1;
		g[p][q] = true;
		g[q][p] = true;
	}
	for (int tt = 0; tt < int(1.0/eps); tt++) {
		for (int i = 0; i < n; i++) {
			alive[i] = true;
			deg[i] = 0;
		}
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++) {
				h[i][j] = g[i][j];
				if (h[i][j])
					deg[i] += eps;
			}
		for (int i = 0; i < n; i++) {
			int k = -1;
			for (int j = 0; j < n; j++)
				if (alive[j] && (k==-1 || deg[j]+w[j]<deg[k]+w[k]))
					k = j;
			w[k] += deg[k];
			for (int j = 0; j < n; j++) {
				if (h[j][k])
					deg[j] -= eps;
				h[k][j] = false;
				h[j][k] = false;
			}
			alive[k] = false;
		}
		// double maxeps = 0;
		// for (int i = 0; i < n; i++)
		// 	for (int j = 0; j < n; j++)
		// 		if (g[i][j] && abs(w[i]-w[j])>maxeps)
		// 			maxeps = abs(w[i]-w[j]);
		// cout << maxeps << endl;
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
	int x = 0;
	for (int i = 0; i < t; i++)
		for (int j = 0; j < t; j++)
			if (g[a[i]-1][a[j]-1])
				x += 1;
	cout << double(x)/t/2;
	return 0;
}