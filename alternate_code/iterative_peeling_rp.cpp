#include <bits/stdc++.h>
using namespace std;

const int MAXV = 10000000;
const int MAXE = 10000000;
int n, m;
int e_ini[MAXE * 2];
int e_start[MAXV], e[MAXE * 2];
int temp[MAXV];

//IO NONSENSE
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

const int MAXLOAD = 10000;
short load[MAXV];
int last[MAXLOAD];
int tail;
int id[MAXE], id_prev[MAXE];

inline void LLAppend(int u) {
	int i1 = load[u] / 2;
//assert(i1 < MAXLOAD); //don't bother with load too high case
	if(i1 < MAXLOAD) {
		id_prev[tail] = last[i1];
		last[i1] = tail;
		id[tail] = u;
		tail++;
	}
//if(u == 11) cout << "!!!" << load[u] << " " << last[2] << endl;
}

pair<int, int> bleh[MAXV];

int main(int argc, char** argv) {
//parameters: #ITERS, INPUTFILENAME, OUTPUT FILE NAME
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
		output_file = "results/results_default.txt";
	}

	freopen (input_file.c_str(), "r", stdin); //input file
	outfile.open(output_file.c_str(), ios_base::app);
	n = getInt();
	m = getInt();

	memset(e_start, 0, sizeof(e_start));
	cout << "DECLARED/CLEARED INITIAL MEMORY: " << clock() << endl;
	for (int i = 0; i < m; i++) {
		int u, v;
		e_ini[i * 2] = getInt() - 1;
		e_ini[i * 2 + 1] = getInt() - 1;
		e_start[e_ini[i * 2]]++;
		e_start[e_ini[i * 2 + 1]]++;
	}
	cout << "DONE WITH IO: " << clock() << endl;
/*
//reorder vertices: sort by degree...
	for(int u = 0; u < n; ++u) {
		bleh[u] = make_pair(e_start[u], u);
	}
	sort(bleh, bleh + n);
	for(int i = 0; i < n; ++i) {
		temp[bleh[i].second] = i;
	}
	memset(e_start, 0, sizeof(e_start));
	for(int e1 = 0; e1 < m * 2; ++e1) {
		e_ini[e1] = temp[e_ini[e1]];
		e_start[e_ini[e1]]++;
	}	
*/

//radix sort all the edges so things are in increasing CSR ordering
	int s = 0;
	for(int u = 0; u < n; ++u) {
		int temp = s;
		s += e_start[u];
		e_start[u] = temp;
	}
	for(int i = 0; i < m; ++i) {
		e[e_start[e_ini[i * 2]]++] = e_ini[i * 2 + 1];
		e[e_start[e_ini[i * 2 + 1]]++] = e_ini[i * 2];
	}
	for(int u = n; u > 0; --u) {
		e_start[u] = e_start[u - 1];
	}
	e_start[0] = 0;
	memcpy(e_ini, e, e_start[n] * sizeof(int));
	memcpy(temp, e_start, (n + 1) * sizeof(int));
	for(int u = 0; u < n; ++u) {
		for(int e1 = temp[u]; e1 < temp[u + 1]; ++e1) {
			e[e_start[e_ini[e1]]++] = u;
		}
	}	
	for(int u = n; u > 0; --u) {
		e_start[u] = e_start[u - 1];
	}
	e_start[0] = 0;
	cout << "DONE WITH CREATING GRAPH: " << clock() << endl;

//for(int u = 0; u < n; ++u) for(int e1 = e_start[u]; e1 < e_start[u + 1]; ++e1) cout << u << "===" << e[e1] << endl;
	vector<int> m_ans;
	double mm_density = 0;
	for(int u = 0; u < n; ++u) {
		load[u] = 0;
	}

	for (int tt = 0; tt < iters; tt++) {
		for(int i = 0; i < MAXLOAD; ++i) {
			last[i] = -1;
		}
		tail = 0;

		for(int u = 0; u < n; ++u) {
			int degree_u = e_start[u + 1] - e_start[u];
//assert(load[u] % 2 == 0); //should be unvisited
			load[u] = load[u] + degree_u * 2 + 1;
			//last bit = 1 ---> not finished yet
			LLAppend(u);
		}

//int cou[MAXLOAD]; memset(cou, 0, sizeof(cou));
		int cur_n = n;
		int cur_m = m;
		double max_density = (double)m / n;
		int i1 = 0;
//cout << cur_n << endl;
		while(cur_n > 0) {
			while(last[i1] == -1 && i1 < MAXLOAD) {
				i1++;
			}
assert(i1 < MAXLOAD); //didn't support the case where load is too big
//cout << i1 << endl;
//if(cur_n > 0) cout << i1 << " " << cur_n << endl;
			int u = id[last[i1]];
			last[i1] = id_prev[last[i1]]; //delete u from its linked list
			if(load[u] % 2 == 1) {//not done yet
				load[u] ^= 1; //mark u as done
//cou[i1]++;
//assert(i1 == load[u] / 2);
				cur_n--;
				for(int e1 = e_start[u]; e1 < e_start[u + 1]; ++e1) {
					int v = e[e1];
					if(load[v] % 2 == 1) {
//decrement v's counter if v is not done yet
						load[v] -= 2;
						LLAppend(v);
						cur_m--;
					}
				}
//cout << "removed " << u << "    " << cur_n << " vertices and " << cur_m << " edges remain" << endl;
				max_density = max(max_density, (double)cur_m / cur_n);
			}
			while(i1 > 0 && last[i1 - 1] != -1) {
				i1--;
			}
		}
//for(int i = 0; i < MAXLOAD; ++i) if(cou[i] > 0) cout << i << ":" << cou[i] << endl;
//cout << cur_n << endl;
		if(max_density > mm_density) {
			mm_density = max_density;
		}
		cout << "ITER = " << tt << "  ";
		cout << "Max density = " << max_density << " ";
		cout << "Time = " << clock() << endl;
	}
	//outfile << "Time: " << clock() << endl;
	//outfile << "Filename: " << input_file << endl;
	//outfile << "Iterations: " << iters << endl;
	//outfile << "Max load: " << w[V[0]] * eps << endl;
	cout << "Time: " << clock() << endl;
	cout << "Approximate maximum density: " << mm_density << endl;
/*
	outfile << m_ans.size() << endl;
	for(int i : m_ans) 
	{
		outfile << i << endl;
	}
*/
	return 0;
}
