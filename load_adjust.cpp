#include <iostream>
#include <cstring>
#include <chrono>

using namespace std;

int main(int argc, char** argv) {

  	auto start = chrono::steady_clock::now();

	int n, m;
	double eps = atof(argv[1]);

	cin >> n >> m;
	
	double * r = new double[m];
    memset(r, 0, sizeof(double) * m);
  	
  	double * load = new double[n];
    memset(load, 0, sizeof(double) * n);
  	
  	int * e_left = new int[m];
  	memset(e_left, 0, sizeof(int) * m);
  	
  	int * e_right = new int[m];
  	memset(e_right, 0, sizeof(int) * m);
  	
	for (int i = 0; i < n+1; i++)
		load[i] = 0; // loads on vertices
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		e_left[i] = p; // left end point of edge i is p
		e_right[i] = q; // right end point of edge i is q
		
		if (load[p] < load[q])
			r[i] = 0; // how much of the edge is assigned to the right side (here it is assigned to the left because p's load is lower)
		else
			r[i] = 1; // how much of the edge is assigned to the right side (here it is assigned to the right because q's load is lower)
		
		load[q] += r[i];
		load[p] += (1 - r[i]);
	}

	//for (int i = 0; i < n+1; i++)
	//	cout << load[i] << endl;
	//cout << endl << endl;
	
	auto mid = chrono::steady_clock::now();
	cerr << "Time for reading input: " << chrono::duration_cast<chrono::milliseconds>(mid - start).count() << " ms" << endl;
  	
	bool flag = false;
		
	int ii = 0;
	while (!flag) {
		ii += 1;
		flag = true;
		for (int i = 0; i < m; i++) {
			int p = e_left[i], q = e_right[i];
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
	}

	auto end = chrono::steady_clock::now();
	cerr << "Time for finding DSP value: " << chrono::duration_cast<chrono::milliseconds>(end - mid).count() << " ms" << endl;

	//for (int i = 0; i < n+1; i++)
	//	cout << load[i] << endl;
	//cout << endl << endl;
	
	
	double ds = 0;
	for (int i = 0; i < n+1; i++)
		if (ds < load[i])
			ds = load[i];
	cout << ds << endl;
	cout << "iterations: " << ii << endl;
	return 0;
}