#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>   
#include <queue> 
#include <set>
#include <unordered_set>
#include <algorithm>
#include <numeric>
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

	int iters = atoi(argv[1]);
	double eps = 1.0/iters;
	string input_file = argv[2];
	
	string output_file;
	ofstream outfile;

	if (argc >= 4)
	{
		output_file = argv[3];
	}
	else
	{
		output_file = "results/results_recursive_peeling.txt";
	}
	//string st1 ("tests/"+ss);
	//string st2 ("tests/"+ss+".a");

	freopen (input_file.c_str(), "r", stdin); //input file
	//freopen (output_file.c_str(), "a", stdout); //output file
	outfile.open(output_file.c_str(), ios_base::app);
	cin >> n >> m;
	unordered_set<int> g[n];
	unordered_set<int> adj[n];
	int e[m][2];

	w.reserve(n);
	deg.reserve(n);

	//cout << n << m << endl;
	for (int i = 0; i < n; i++) 
		w[i] = 0; //initial vertex weights=0, i.e., no self loops at the start
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		p -= 1;
		q -= 1;
		e[i][0] = p;
		e[i][1] = q;
		adj[p].insert(q);
		adj[q].insert(p);
	}

	auto begin = clock();
	for (int tt = 0; tt < iters; tt++) {
		//cout << tt << endl;
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

	vector<int> V(n);
	int x=0;
	iota(V.begin(),V.end(),x++); //Initializing
	sort(V.begin(),V.end(), [&](int i,int j){return w[i]>w[j];} );

	/*for (int i = 0; i< n;i++)
	{
		outfile << w[i] << endl;
		//outfile << V[i] << endl;
	}
	outfile << endl;
	for (int i = 0; i< n;i++)
	{
		//outfile << w[i] << endl;
		outfile << V[i] << endl;
	}*/

	vector<bool> insol(n, false);
	vector<bool> dsg(n, false);
	double maxdens=0;
	double curdens;
	int curedges=0;
	//insol[V[0]] = true;
	for (int i = 0; i < n; i++)
	{
		for (int j : adj[V[i]])
		{
			//outfile << j+1 << "is a neighbor of" << V[i]+1 << endl;
			if (insol[j])
			{
				curedges++;
			}
		}
		insol[V[i]] = true;

		curdens = double(curedges)/(i+1);
		//outfile << endl << curdens << endl;
		if (curdens > maxdens)
		{
			maxdens = curdens;
			dsg = insol;
		}
	}

	auto end = clock();
	
	cout << "Filename: " << input_file << endl;
	cout << "Iterations: " << iters << endl;
	cout << "Max load: " << w[V[0]] << endl;
	cout << "Time in seconds: " << float(end-begin) /  CLOCKS_PER_SEC << endl;  
	cout << "Approximate maximum density: " << maxdens << endl;
	//outfile << "Approximate densest subgraph:" << endl;

	/*for (int i = 0; i< n;i++)
	{
		if (dsg[i])
		{
			outfile << i << endl;
		}
	}*/
	//outfile << endl << endl;
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
		outfile << "Exact density: " << double(x)/t/2; // Rho* = average degree/2
	}
	return 0;
}