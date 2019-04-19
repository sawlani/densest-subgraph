#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>   
#include <queue> 
#include <set>
#include <unordered_set>
using namespace std;

//const int maxn = 100;
//const int maxm = 1000;


int n, m;
vector<double> deg, w;
//double eps = 0.01;
//string ss = "30";

struct classcomp {
  bool operator() (const int& lhs, const int& rhs) const
  {return deg[lhs]<deg[rhs] || (deg[lhs]==deg[rhs] && lhs<rhs);}
};

set<int,classcomp> ordered_deg; //BBST storing degrees

int main(int argc, char** argv) {

	double eps = atof(argv[1]);
	string input_file = argv[2];
	
	string output_file;

	if (argc >= 4)
	{
		output_file = argv[3];
	}
	else
	{
		output_file = "results.txt";
	}
	//string st1 ("tests/"+ss);
	//string st2 ("tests/"+ss+".a");

	freopen (input_file.c_str(), "r", stdin); //input file
	freopen (output_file.c_str(), "w", stdout); //output file
	
	cin >> n >> m;
	unordered_set<int> g[n];
	int e[m][2];
	
	w.reserve(n);
	deg.reserve(n);

	for (int i = 0; i < n; i++) 
		w[i] = 0; //initial vertex weights=0, i.e., no self loops at the start
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
			deg[i] = w[i]; //degree for this iteration is "vertex weight" + actual degree
			g[i].clear();			
		} 
		for (int i = 0; i < m; i++) {
			int p = e[i][0], q = e[i][1];
			deg[p] += eps; //edges in each iteration are scaled by eps: so no need to divide by T=1/eps at the end.
			deg[q] += eps;
			g[p].insert(q);
			g[q].insert(p);
		}
		for (int i = 0; i < n; i++)
			ordered_deg.insert(i);
		for (int i = 0; i < n; i++) {
			int k = *(ordered_deg.begin()); //k = min degree vertex
			w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
			ordered_deg.erase(k); //delete k
			for (int j : g[k]) { //decrement degrees of k's neighbors
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
			ds = w[i]; // Find max load
	cout << "Approximate density: " << ds << endl;

	////////////////////////
	// If the solution is known, compute actual value of maximum density from solution file:
	////////////////////////
	if(argc == 5) {
		string solution_file = argv[4];

		freopen (solution_file.c_str(), "r", stdin);
		int t;
		cin >> t;
		int a[n];
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
					x += 1; // Sum degrees in solution
		cout << "Exact density: " << double(x)/t/2; // Rho* = average degree/2
	}
	return 0;
}