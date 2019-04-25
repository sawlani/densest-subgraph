#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include<cassert>
#include <vector>   
#include <queue> 
#include <set>
#include<ctime>
#include <unordered_set>
#include <algorithm>
#include <numeric>
using namespace std;



int n, m;
//double eps = 0.01;
//string ss = "30";
	


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
	vector<int> * adj = new vector<int>[n];
	vector<int> * g = new vector<int>[n];
	vector<int> * ordered_deg = new vector<int> [m + 1];
	vector<pair<int, int> > e;
	vector<int> deg, w, pos;
	w.resize(n);
	deg.resize(n);
	pos.resize(n);
	for (int i = 0; i < n; i++) 
		w[i] = 0; //initial vertex weights=0, i.e., no self loops at the start
	for (int i = 0; i < m; i++) {
		int p, q;
		scanf("%d%d", &p, &q);
		p -= 1;
		q -= 1;
		e.push_back(make_pair(p, q));
		adj[p].push_back(q);
		adj[q].push_back(p);
	}
	for (int tt = 0; tt < iters; tt++) {
		cout << tt << endl;
		for (int i = 0; i < n; i++) {
			deg[i] = w[i]; //degree for this iteration is "vertex weight" + actual degree
			g[i] = adj[i];
		} 
		for (int i = 0; i < m; i++) {
			deg[e[i].first]++;
			deg[e[i].second]++;
			assert(ordered_deg[i].empty());
		}
		for(int i = 0; i < n; i++) {
			ordered_deg[deg[i]].push_back(i);
			pos[i] = (int)ordered_deg[deg[i]].size() - 1;
		}
		int min_deg = 0;
		//cout <<deg[1971235] << ' ' << pos[1971235] << '!' << ordered_deg[deg[1971235]][pos[1971235]] << endl;
		for(int _ = 0; _ < n; _++) {
			// cout << _ << endl;
			//cout << tt << ' ' << min_deg << endl;
			for( ; min_deg <= m && ordered_deg[min_deg].empty(); ) {
				min_deg++;
			}
			if(min_deg > m) break;
			//)cout << "min" << min_deg << ' ' << ordered_deg[min_deg].size() << endl;
			int k = ordered_deg[min_deg][0];//k = min degree vertex
			//cout << "k" << k << endl;
			// cout << "!!!!" << endl;
			int swp = ordered_deg[min_deg].back();
			ordered_deg[min_deg][0] = ordered_deg[min_deg].back();
			pos[swp] = 0;
			ordered_deg[min_deg].pop_back(); //delete k
			//cout << ordered_deg[min_deg].size() << endl;
			w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
			pos[k] = -1;
			for (int j : g[k]) { //decrement degrees of k's neighbors
				if(pos[j] == -1) continue;
				//cout << k << "->" << j << ' ' << deg[j] << ' ' << pos[j] << endl;
				// cout << ordered_deg[deg[j]].size()<< endl;
				int swp = ordered_deg[deg[j]].back();
				ordered_deg[deg[j]][pos[j]] = ordered_deg[deg[j]].back();
				pos[swp] = pos[j];
				assert(ordered_deg[deg[j]].size());
				ordered_deg[deg[j]].pop_back();
				deg[j]--;
				ordered_deg[deg[j]].push_back(j);
				pos[j] = (int)ordered_deg[deg[j]].size() - 1;
			}
			min_deg--;
			if(min_deg < 0) min_deg = 0;
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

	outfile << "Time: " << clock() << endl;
	cerr << "Time: " << clock() << endl;
	outfile << "Filename: " << input_file << endl;
	outfile << "Iterations: " << iters << endl;
	outfile << "Max load: " << w[V[0]] * eps << endl;
	outfile << "Approximate maximum density: " << maxdens << endl;
	//outfile << "Approximate densest subgraph:" << endl;

	/*for (int i = 0; i< n;i++)
	{
		if (dsg[i])
		{
			outfile << i << endl;
		}
	}*/
	outfile << endl << endl;
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
			int p = e[i].first, q = e[i].second;
			g[p].push_back(q);
			g[q].push_back(p);
		}
		vector<int> flag(n);
		for(int i = 0; i < t; i++) {
			flag[a[i] - 1] = true;
		}
		for(int i = 0; i < t; i++) {
			for(int j : g[a[i] - 1]) {
				x += flag[j - 1];// Sum degrees in solution
			}
		}
		outfile << "Exact density: " << double(x)/t/2; // Rho* = average degree/2
	}
	return 0;
}
