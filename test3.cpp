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
double d[maxn];
int n, m;
double eps = 0.1;
string ss = "30";
string st1 ("tests/"+ss);
string st2 ("tests/"+ss+".a");

double abs(double x) {
	if (x>0)
		return x;
	else
		return -x;
}

double calDeg(int i) {
	double ans = 0;
	for (int j = 0; j < n; j++)
		if (g[i][j])
			ans += d[j];
	return ans;
}

int main() {
	freopen (st1.c_str(), "r", stdin);
	freopen ("test.out", "w", stdout);
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			g[i][j] = false;
	for (int i = 0; i < n; i++) {
		d[i] = 1;
		w[i] = 0;
	}
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		p -= 1;
		q -= 1;
		g[p][q] = true;
		g[q][p] = true;
	}
	for (int tt = 0; tt < int(1.0/eps)*n; tt++) {
		int k = -1;
		for (int i = 0; i < n; i++)
			if (d[i]>0 && ((k==-1 || calDeg(i)<calDeg(k))))
				k = i;
		d[k] -= eps;
		w[k] += calDeg(k)*eps;
		// cout << w[k] << endl;
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