#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include<cassert>
#include <vector>   
#include <queue> 
#include<list>
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
	//double eps = 1.0/iters;
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
	//int LIM = 2 * m;
	list<pair<int, vector<int> > > ordered_deg;
	vector<list<pair<int, vector<int> > >::iterator> itr(n);
	vector<pair<int, int> > e;
	vector<int> deg(n), w(n), pos(n);
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
	vector<pair<int, int> > deg_sorted(n);
	vector<int> m_ans;
	double mm_density = 0;
	for (int tt = 0; tt < iters; tt++) {
		//cout << tt << endl;
		for (int i = 0; i < n; i++) {
			deg[i] = w[i]; //degree for this iteration is "vertex weight" + actual degree
			g[i] = adj[i];
			//cout << w[i] << endl;
		}
		for (int i = 0; i < m; i++) {
			deg[e[i].first]++;
			deg[e[i].second]++;
		}
		for(int i(0); i < n; i++) {
			deg_sorted[i] = make_pair(deg[i], i);
		}
		sort(deg_sorted.begin(), deg_sorted.end());
		for(int i = 0; i < n; i++) {
			int v = deg_sorted[i].second;
			if(ordered_deg.empty() || deg_sorted[i].first != ordered_deg.back().first) {
				ordered_deg.push_back(make_pair(deg_sorted[i].first, vector<int>()));
			}
			ordered_deg.back().second.push_back(v);
			itr[v] = ordered_deg.end();
			itr[v]--;
			pos[v] = itr[v]->second.size() - 1;
		}
		double max_density = (double)m / n;
		int cur_m = m, cur_n = n;
		vector<int> ans;
		int max_size = 0;
		while(ordered_deg.empty() == false) {
			//cout << "??" << endl;
			auto i = ordered_deg.begin();
			int k = i->second[0];//k = min degree vertex
			//cout << "k = " << k << ' ' << i->first << '!' << deg[k] << endl;
			int swp = i->second.back();
			i->second[0] = i->second.back();
			pos[swp] = 0;
			i->second.pop_back(); //delete k
			if(i->second.empty()) {
				ordered_deg.erase(i);
			}
			w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
			pos[k] = -1;
			cur_n -= 1;
			ans.push_back(k);
			for (int j : g[k]) { //decrement degrees of k's neighbors
				if(pos[j] == -1) continue;
				cur_m -= 1;
				//cout << k << "->" << j << ' ' << deg[j] << ' ' << pos[j] << endl;
				//cout << "?" << ordered_deg[deg[j]].size()<< endl;
				auto i = itr[j];
				int swp = i->second.back();
				i->second[pos[j]] = i->second.back();
				pos[swp] = pos[j];
				assert(i->second.size());
				i->second.pop_back();
				if(i->second.empty()) {
					ordered_deg.erase(i++);
				}
				deg[j]--;
				bool flag = false;
				if(i == ordered_deg.begin()) {
					flag = true;
				}else {
					auto k = i;
					k--;
					if(k->first != deg[j]) {
						flag = true;
					}
				}
				if(flag) {
					itr[j] = ordered_deg.insert(i, make_pair(deg[j], vector<int>(1, j)));
					pos[j] = 0;
				}else {
					i--;
					itr[j] = i;
					i->second.push_back(j);
					pos[j] = i->second.size() - 1;
				}
				//cout << itr[j]->first << ' ' << deg[j] << ' ' << itr[j]->second[pos[j]] << ' ' << j << endl;
			}
			if(max_density < (double)cur_m / cur_n) {
				max_size = ans.size();
			}
			max_density = max(max_density, (double)cur_m / cur_n);
		}
		reverse(ans.begin(), ans.end());
		ans.resize(n - max_size);
		if(max_density > mm_density) {
			m_ans = ans;
			mm_density = max_density;
		}
		printf("Max density = %.12f (iteration %d)\n", max_density, tt);
	}

	vector<bool> insol(n, false);
	for(int i : m_ans) insol[i] = true;
	double maxdens=0;
	int curedges=0;
	for (int i = 0; i < n; i++)
	{
		if(insol[i] == false) continue;
		for (int j : adj[i])
		{
			//outfile << j+1 << "is a neighbor of" << V[i]+1 << endl;
			if (insol[j])
			{
				curedges++;
			}
		}
	}
	maxdens = curedges / 2. / m_ans.size();

	//outfile << "Time: " << clock() << endl;
	cerr << "Time: " << clock() << endl;
	//outfile << "Filename: " << input_file << endl;
	//outfile << "Iterations: " << iters << endl;
	//outfile << "Max load: " << w[V[0]] * eps << endl;
	cerr << "Approximate maximum density: " << maxdens << endl;
	cerr << "Approximate densest subgraph:" << endl;
	outfile << m_ans.size() << endl;
	for(int i : m_ans) 
	{
		outfile << i << endl;
	}
	////////////////////////
	// If the solution is known, compute actual value of maximum density from solution file:
	////////////////////////
	if(argc == 5) {
		string solution_file = argv[4];

		freopen (solution_file.c_str(), "r", stdin);
		int t;
		cin >> t;
		int a[n];
		int x = 0;
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
