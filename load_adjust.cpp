#include <iostream>

using namespace std;

const int maxn = 100;
const int maxm = 1000;

bool g[maxn][maxn];
int e[maxm][2];
double r[maxm];
double l[maxn];
int n, m;
double eps = 0.03;

int main() {
	freopen ("test.in", "r", stdin);
	freopen ("test.out", "w", stdout);
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			g[i][j] = false;
	for (int i = 0; i < n; i++)
		l[i] = 0;
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		g[p][q] = true;
		g[q][p] = true;
		e[i][0] = p;
		e[i][1] = q;
		r[i] = 1;
		l[q] += 1;
	}
	int ii = 0;
	while (ii < 10000) {
		ii += 1;
		bool flag = true;
		for (int i = 0; i < m; i++) {
			int p = e[i][0], q = e[i][1];
			if (l[p]<l[q]+eps and r[i]>=eps) {
				flag = false;
				l[p] += eps;
				l[q] -= eps;
				r[i] -= eps;
			}
			if (l[q]<l[p]+eps and r[i]+eps<=1) {
				flag = false;
				l[q] += eps;
				l[p] -= eps;
				r[i] += eps;
			}
		}
		if (flag)
			break;
	}
	double ds = 0;
	for (int i = 0; i < n; i++)
		if (ds < l[i])
			ds = l[i];
	cout << ds;
	return 0;
}