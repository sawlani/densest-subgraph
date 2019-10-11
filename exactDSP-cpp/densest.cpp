extern "C" {
#include "hi_pr.h"
}

//#include <bits/stdc++.h>
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
#include <vector> 
#include <cmath> 
#include <chrono>
using namespace std;


const int INF = (int)1e9;

void add_arc(int u, int v, int capacity, node *nodes, arc *arcs, cType *cap, int *cur_arc) {
    arcs[cur_arc[u]].head = nodes + v;
    arcs[cur_arc[u]].rev = arcs + cur_arc[v];
    arcs[cur_arc[v]].head = nodes + u;
    arcs[cur_arc[v]].rev = arcs + cur_arc[u];
    cap[cur_arc[u]] = capacity;
    cap[cur_arc[v]] = 0;
    ++cur_arc[u];
    ++cur_arc[v];
}

bool nontrivial(int n_nodes, node *nodes) {
    int res = 0;
    for (int i = 0; i < n_nodes; ++i) {
        if (nodes[i].d < n_nodes) {
            ++res;
        }
    }
    return res > 1;
}

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

int main(int argc, char **argv) {
//THIS NUMBER DETERMINES HOW MANY DECIMALS WE COMPUTE THINGS TO
//  HI_PR ONLY TAKES INTEGER CAPACITIES
//    SO WE CAN'T PUT FRACTIONALS, INSTEAD, JUST MULTIPLY EVERYTHING BY 100 OR 1000...
    auto startio = chrono::steady_clock::now();

    int ACCURACY = atoi(argv[1]);
 
    /*string input_file = argv[2];
    string output_file;
    ofstream outfile;
    if (argc >= 4)
	{
		output_file = argv[3];
	}
	else
	{
		output_file = "results/results_exact.txt";
	}
	//string st1 ("tests/"+ss);
	//string st2 ("tests/"+ss+".a");
	
    freopen (input_file.c_str(), "r", stdin); //input file
    //freopen (output_file.c_str(), "a", stdout); //output file
    outfile.open(output_file.c_str(), ios_base::app);
    */
    
    int n, m;
    int k = 2; ///EDGES CASE!
    //cin >> n >> m;
    n = getInt(); m = getInt();


    int *cliques = new int[m * k];
    for (int i = 0; i < m * k; ++i) {
      //cin >> cliques[i];
      cliques[i]=getInt();
      cliques[i]--;
    }

    auto endio = chrono::steady_clock::now();
    
    cout << "Time for reading input: " << chrono::duration_cast<chrono::milliseconds>(endio - startio).count() << " ms" << endl;
   
    auto start = chrono::steady_clock::now();
    
    int source = n + m;
    int sink = n + m + 1;
    int n_nodes = n + m + 2;
    int *deg = new int[n_nodes];
    deg[source] = n;
    deg[sink] = m;
    for (int i = 0; i < n; ++i) {
        deg[i] = 1;
    }
    for (int i = 0; i < m * k; ++i) {
        ++deg[cliques[i]];
    }
    for (int i = 0; i < m; ++i) {
        deg[n + i] = k + 1;
    }
    for (int i = 1; i < n_nodes; ++i) {
        deg[i] += deg[i - 1];
    }

    int n_arcs = deg[n_nodes - 1];

    int *cur_arc = new int[n_nodes];

    node *nodes_ = new node[n_nodes + 1];
    arc *arcs = new arc[n_arcs];
    cType *cap  = new cType[n_arcs];

    for (int i = 0; i < n_nodes; ++i) {
        cur_arc[i] = (i == 0) ? 0 : deg[i - 1];
        nodes_[i].first = arcs + cur_arc[i];
    }

    for (int i = 0; i < n; ++i) {
        add_arc(source, i, 0, nodes_, arcs, cap, cur_arc);
    }
    for (int i = 0; i < m; ++i) {
        add_arc(n + i, sink, ACCURACY, nodes_, arcs, cap, cur_arc);
    }
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < k; ++j) {
            add_arc(cliques[k * i + j], n + i, INF, nodes_, arcs, cap, cur_arc);
        }
    }

    // upper and lower bound on density
    int l = 0, r = m * ACCURACY;
    //    cerr << "start" << endl;
    vector<long> subg;
    node *j;
    while (l < r) {
        int c = (l + r + 1) / 2;
        //cerr << "[" << l << ", " << r << "] " << c << endl;
        for (int i = 0; i < n; ++i) {
            cap[nodes_[source].first - arcs + i] = c;
        }
        min_cut(n_nodes, n_arcs / 2, nodes_, arcs, cap, nodes_ + source, nodes_ + sink, 0);
        if (nontrivial(n_nodes, nodes_)) {
            l = c;
            subg.clear();
            forAllNodes(j)
                if (j->d < n_nodes && nNode(j) < n)
                    subg.push_back(nNode(j));

	    /*
            cerr << endl << "subgraph:\n";
            for (vector<long>::iterator v = subg.begin(); v != subg.end(); ++v) {
            cerr << *v << " ";
            }
            cerr << endl;
	    */

        } else {
            r = c - 1;
        }

    }
    
    auto end = chrono::steady_clock::now();
    
    cout << "Time for finding solution: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    cerr << "Density is " << (double)l/ACCURACY << endl;

    string output_file;
    ofstream outfile;
    
    if (argc >= 3)
    {
        output_file = argv[2];
    }
    else
    {
        output_file = "soln.tmp";
    }

    outfile.open(output_file.c_str());
    for (int i : subg)
    {
        outfile << i+1 << endl;
    }

    return 0;
}
