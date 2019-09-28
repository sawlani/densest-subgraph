#include <iostream>

using namespace std;

const int maxn = 100;
const int maxm = 1000;

int e[maxm][2];
double r[maxm];
double load[maxn];
int n, m;
double eps = 0.01;

int main() {
	
	cin >> n >> m;
	
	for (int i = 0; i < n+1; i++)
		load[i] = 0; // loads on vertices
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		e[i][0] = p; // left end point of edge i is p
		e[i][1] = q; // right end point of edge i is q
		
		if (load[p] < load[q]-1)
			r[i] = 0; // how much of the edge is assigned to the right side (here it is assigned to the left because p's load is lower)
		else if (load[q] < load[p]-1)
			r[i] = 1; // how much of the edge is assigned to the right side (here it is assigned to the right because q's load is lower)
		else
			r[i] = load[p]/2-load[q]/2 + 0.5;

		load[q] += r[i];
		load[p] += 1 - r[i];
	}

	for (int i = 0; i < n+1; i++)
		cout << load[i] << endl;
	cout << endl << endl;
	
	int ii = 0;
	while (ii < 10000) {
		ii += 1;
		bool flag = true;
		for (int i = 0; i < m; i++) {
			int p = e[i][0], q = e[i][1];
			while (load[p]<load[q]-eps and r[i]>=eps) { // flip eps from right to left (load at p is smaller than that at q)
				flag = false;
				load[p] += eps;
				load[q] -= eps;
				r[i] -= eps;
			}
			while (load[q]<load[p]-eps and r[i]+eps<=1) { // flip eps from left to right (load at q is smaller than that at p)
				flag = false;
				load[q] += eps;
				load[p] -= eps;
				r[i] += eps;
			}
		}
		if (flag)
			break;
	}

	for (int i = 0; i < n+1; i++)
		cout << load[i] << endl;
	cout << endl << endl;
	
	
	double ds = 0;
	for (int i = 0; i < n+1; i++)
		if (ds < load[i])
			ds = load[i];
	cout << ds << endl;
	cout << "iterations: " << ii << endl;
	return 0;
}