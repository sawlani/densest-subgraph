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
#include<cstring>
#include<ctime>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <iomanip>
using namespace std;



int n, m;
//double eps = 0.01;
//string ss = "30";
	
struct Edge {
	int y, next;
};

struct Node {
	int deg, next, prev, idx;
	inline void clear() {
		deg = next = prev = 0;
		idx = -1;
	}
};
inline char GET_CHAR(){
	const int maxn = 131072;
	static char buf[maxn],*p1=buf,*p2=buf;
	return p1==p2&&(p2=(p1=buf)+fread(buf,1,maxn,stdin),p1==p2)?EOF:*p1++;
}
inline int getInt() {
	int res(0);
	char c = GET_CHAR();
	while(c < '0') c = GET_CHAR();
	while(c >= '0') {
		res = res * 10 + (c - '0');
		c = GET_CHAR();
	}
	return res;
}
int l = 0;
Edge * edges;
int * idx;
__inline void build(int x, int y) {
		edges[++l].next = idx[x];
		edges[l].y = y;
		idx[x] = l;
};
int * id1;
int l1 = 0;
Edge * e1;
__inline void bd1(int x, int y) {
	e1[++l1].next = id1[x];
	e1[l1].y = y;
	id1[x] = l1;
}
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
	auto begin = clock();
	n = getInt(); m = getInt();
	edges = new Edge[m * 2 + 10];
	e1 = new Edge[n + m + 10];
	idx = new int[n];
	id1 = new int[n];
	memset(id1, 0, sizeof(int) * n);
	memset(idx, 0, sizeof(int) * n);
	int * init_deg = new int[n];
	memset(init_deg, 0, sizeof(int) * n);
	l = 0;
	//int LIM = 2 * m;
	int n_list = 0;
	int * deg = new int[n], * w = new int[n], * pos = new int [n];
	memset(deg, 0, sizeof(int) * n);
	memset(w, 0, sizeof(int) * n);//initial vertex weights=0, i.e., no self loops at the start 
	memset(pos, 0, sizeof(int) * n);
	//cout << clock() << endl;
	//outfile << clock() << endl;
	for (int i = 0; i < m; i++) {
		int p, q;
		p = getInt();
		q = getInt();
		assert(1 <= p && p <= n && 1 <= q && q <= n);
		p -= 1;
		q -= 1;
		build(p, q);
		build(q, p);
		init_deg[p]++;
		init_deg[q]++;
	}
	//cout << clock() << endl;
	outfile << "Test dataset: " << input_file << endl;
	outfile << "I/O finishes: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
	cerr << "I/O finishes: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
	vector<int> m_ans;
	double mm_density = 0;
	double gr_density = 0;
	for (int tt = 0; tt < iters; tt++) {
		//cout << tt << endl;
		//cout << clock() << endl;
		int mx = 0;

		memset(id1, 0, sizeof(int) * n);
		l1 = 0;
		for (int i = 0; i < n; i++) {
			pos[i] = 0;
			//cout << w[i] << ' ' << init_deg[i] << endl;
			deg[i] = w[i] + init_deg[i]; //degree for this iteration is "vertex weight" + actual degree
			mx = max(mx, deg[i]);
			bd1(deg[i], i);
		}
		double max_density = (double)m / n;
		int cur_m = m, cur_n = n;
		vector<int> ans;
		int max_size = 0;
		for(int i(0); i <= mx; i++) {
			//printf("%d\n", i);
			int bak = id1[i]; id1[i] = 0;
			int mn = i + 1;
			for(int u = bak; u; u = e1[u].next) {
				int v = e1[u].y;
				if(pos[v] == -1) continue;
				//if(i == 1) cout << v << endl;
				ans.push_back(v);
				w[v] = deg[v];
				pos[v] = -1;
				cur_n -= 1;
				for(int p(idx[v]); p; p = edges[p].next) {
					int j = edges[p].y;
					if(pos[j] == -1) continue;
					cur_m -= 1;
					deg[j]--;
					mn = min(mn, deg[j]);
					bd1(deg[j], j);
				}
			}
			//printf("%d\n", i);
			if(cur_n == 0) continue;
			if(max_density < (double)cur_m / cur_n) {
				max_size = ans.size();
			}
			max_density = max(max_density, (double)cur_m / cur_n);
			i = min(i, mn - 1);
		}
		reverse(ans.begin(), ans.end());
		ans.resize(n - max_size);
		if(max_density > mm_density) {
			m_ans = ans;
			mm_density = max_density;
		}
		if (tt==0) gr_density = mm_density;
		outfile <<  tt << ", " << std::setprecision(12) << mm_density << endl;
		//		printf("Max density = %.12f (iteration %d)\n", mm_density, tt);
		outfile << "Time: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
		cerr <<  tt << ", " << std::setprecision(12) << mm_density << endl;
		//		printf("Max density = %.12f (iteration %d)\n", mm_density, tt);
		cerr << "Time: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
	
	}
	cerr << "Improvement over greedy: " << float((mm_density - gr_density)*100/gr_density) << "%" << endl;
		outfile << "Improvement over greedy: " << float((mm_density - gr_density)/gr_density) << endl;
	cerr << "Time: " << clock() << endl;

	//outfile << "Filename: " << input_file << endl;
	//outfile << "Iterations: " << iters << endl;
	//outfile << "Max load: " << w[V[0]] * eps << endl;
	//cerr << "Approximate maximum density: " << mm_density << endl;
	//cerr << "Approximate densest subgraph:" << endl;
	//outfile << m_ans.size() << endl;
	//for(int i : m_ans) 
	//{
	//	outfile << i << endl;
	//}
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
		bool * flag = new bool[n];
		memset(flag, 0, sizeof(bool) * n);
		for(int i = 0; i < t; i++) {
			flag[a[i] - 1] = true;
		}
		for(int i = 0; i < n; i++) {
			if(!flag[i]) continue;
			for(int p = idx[i]; p; p = edges[p].next) {
				int j = edges[p].y;
				x += flag[j];// Sum degrees in solution
			}
		}
		outfile << "Exact density: " << double(x)/t/2; // Rho* = average degree/2
	}
	return 0;
}
