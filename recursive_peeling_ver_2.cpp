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
Node * lists;

__inline void linklists (int x, int y) {
	if(y == 0) return;
	lists[x].next = y;
	lists[y].prev = x;
};
int * nxt, * prv, *itr;
__inline void linknodes (int x, int y) {
	if(y == -1) return;
	nxt[x] = y;
	prv[y] = x;
};
__inline void eraselist(int x) {
	lists[lists[x].prev].next = lists[x].next;
	if(lists[x].next != 0) lists[lists[x].next].prev = lists[x].prev;
};
__inline void erasenode (int x) {
	if(prv[x] == -1) {
		lists[itr[x]].idx = nxt[x];
	}
	if(prv[x] != -1) nxt[prv[x]] = nxt[x];
	if(nxt[x] != -1) prv[nxt[x]] = prv[x];

};
int l = 0;
Edge * edges;
int * idx;
__inline void build(int x, int y) {
		edges[++l].next = idx[x];
		edges[l].y = y;
		idx[x] = l;
};
	
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
	idx = new int[n];
	memset(idx, 0, sizeof(int) * n);
	int * init_deg = new int[n];
	memset(init_deg, 0, sizeof(int) * n);
	l = 0;
	//int LIM = 2 * m;

	lists = new Node[n + 2 * m + 10];
	int n_list = 0;
	itr = new int[n];
	int * deg = new int[n], * w = new int[n], * pos = new int [n];
	memset(deg, 0, sizeof(int) * n);
	memset(w, 0, sizeof(int) * n);//initial vertex weights=0, i.e., no self loops at the start 
	memset(pos, 0, sizeof(int) * n);
	prv = new int[n]; nxt = new int[n];
	//cout << clock() << endl;
	//outfile << clock() << endl;
	for (int i = 0; i < m; i++) {
		int p, q;
		p = getInt();
		q = getInt();
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
	vector<int> m_ans;
	double mm_density = 0;
	double gr_density = 0;
	int sizeofcnt = 2 * iters * n + 10;
	int * cnt = new int[sizeofcnt],  * o = new int[n], * nwo = new int[n];
	for (int tt = 0; tt < iters; tt++) {
		//cout << tt << endl;
		memset(cnt, 0, sizeof(int) * sizeofcnt);
		//cout << clock() << endl;
		for (int i = 0; i < n; i++) {
			nxt[i] = prv[i] = -1;
			pos[i] = 0;
			//cout << w[i] << ' ' << init_deg[i] << endl;
			deg[i] = w[i] + init_deg[i]; //degree for this iteration is "vertex weight" + actual degree
			o[i] = i;
			cnt[deg[i]]++;
		}
		{
			for(int i = 1; i < sizeofcnt; i++) cnt[i] = cnt[i - 1] + cnt[i];
			for(int i = 0; i < n; i++) o[--cnt[deg[i]]] = i;		
			/*memset(cnt, 0, sizeof(int) * sizeofcnt);
			for(int i = 0; i < n; i++) {
				cnt[deg[i] / (n * 2)]++;
			}
			for(int i = 1; i < sizeofcnt; i++) cnt[i] = cnt[i - 1] + cnt[i];
			for(int i = n - 1; i >= 0; i--) nwo[--cnt[deg[o[i]] / (n * 2)]] = o[i];
			for(int i = 0; i < n; i++) o[i] = nwo[i];*/
		}
		n_list = 0;
		//cout << clock() << '?' << endl;

		for(int i = 0; i < n; i++) {
			int v = o[i];

			if(n_list == 0 || lists[n_list].deg != deg[v]) {
				++n_list;
				lists[n_list].clear();
				linklists(n_list - 1, n_list);
				lists[n_list].deg = deg[v];
			}
			//printf("%d %d %d %d %d\n", v, lists[n_list].idx, deg[v], w[v], init_deg[v]);
			linknodes(v, lists[n_list].idx);
			lists[n_list].idx = v;
			itr[v] = n_list;
		}
		//for(int i = 0; i < n; i++) printf("prv[%d] = %d\n", i, nxt[i]);
		//exit(0);
		double max_density = (double)m / n;
		int cur_m = m, cur_n = n;
		vector<int> ans;
		int max_size = 0;
		while(lists[0].next) {
			//cout << "??" << endl;
			int i = lists[0].next;
			int k = lists[i].idx;
			//cout << "k = " << k << ' ' << i << ' ' << lists[i].deg << '!' << deg[k] << ' ' << nxt[k] << endl;
			if(nxt[k] == -1) {
				eraselist(i);
			}else {
				erasenode(k);
			}
			pos[k] = -1;
			w[k] = deg[k]; //increment vertex weight for the next iteration (self loops)
			cur_n -= 1;
			ans.push_back(k);
			for (int p = idx[k]; p; p = edges[p].next) { //decrement degrees of k's neighbors
				int j = edges[p].y;
				if(pos[j] == -1) continue;
				cur_m -= 1;
				//cout << k << "->" << j << ' ' << deg[j] << ' ' << pos[j] << endl;
				//cout << "?" << ordered_deg[deg[j]].size()<< endl;
				int i = itr[j];
				erasenode(j);
				int i1 = lists[i].prev;
				//cout << j << ' ' << deg[j] << ' ' << i << 'j' << lists[i].deg << endl;
				//cout << i1 << ' ' << lists[i1].deg << endl;
				if(lists[i].idx == -1) eraselist(i);
				deg[j]--;				
				prv[j] = nxt[j] = -1;
				if(i1 == 0 || lists[i1].deg != deg[j]) {
					++n_list;
					lists[n_list].clear();
					itr[j] = n_list;
					int i2 = lists[i1].next;
					lists[n_list].deg = deg[j];
					lists[n_list].idx = j;
					linklists(i1, n_list);
					if(i2) linklists(n_list, i2);
				}else {
					linknodes(j, lists[i1].idx);
					lists[i1].idx = j;
					itr[j] = i1;
				}
				//cout << itr[j]->first << ' ' << deg[j] << ' ' << itr[j]->second[pos[j]] << ' ' << j << endl;
			}
			if(cur_n == 0) continue;
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
		if (tt==0) gr_density = mm_density;
		outfile <<  float(clock()-begin) /  CLOCKS_PER_SEC  << ", " << std::setprecision(12) << mm_density << endl;
		//		printf("Max density = %.12f (iteration %d)\n", mm_density, tt);
		//		outfile << "Time: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
	
	}
	cerr << "Total time: " << float(clock()-begin) /  CLOCKS_PER_SEC << endl;
	cerr << "Improvement over greedy: " << float((mm_density - gr_density)*100/gr_density) << "%" << endl;
		outfile << "Improvement over greedy: " << float((mm_density - gr_density)/gr_density) << endl;
	//outfile << "Time: " << clock() << endl;

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
