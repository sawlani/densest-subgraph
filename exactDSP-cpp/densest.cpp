extern "C" {
#include "hi_pr.h"
}

//#include <bits/stdc++.h>
#include <iostream> 
#include <vector> 
#include <cmath> 

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

int main(int argc, char **argv) {
//THIS NUMBER DETERMINES HOW MANY DECIMALS WE COMPUTE THINGS TO
//  HI_PR ONLY TAKES INTEGER CAPACITIES
//    SO WE CAN'T PUT FRACTIONALS, INSTEAD, JUST MULTIPLY EVERYTHING BY 100 OR 1000...
    int ACCURACY = 1000;
 
    int n, m;
    int k = 2; ///EDGES CASE!
    scanf("%d%d", &n, &m);

    int *cliques = new int[m * k];
    for (int i = 0; i < m * k; ++i) {
        cin >> cliques[i]; cliques[i]--;
printf("%d\n", cliques[i]);
    }

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
    
    vector<long> subg;
    node *j;
    while (l < r) {
        int c = (l + r + 1) / 2;
        cerr << "[" << l << ", " << r << "] " << c << endl;
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
    
    cerr << "Density is " << l << endl;

    cerr << "subgraph:\n";
    for (vector<long>::iterator v = subg.begin(); v != subg.end(); ++v) {
        cerr << (*v + 1) << " ";
//        cout << *v << " ";
    }
    cerr << endl;
    cout << endl;

    /*
    node nodes[3];
    arc arcs[2];
    cType cap[2];
    nodes[0].first = arcs + 0;
    nodes[1].first = arcs + 1;
    arcs[0].head = nodes + 1;
    arcs[1].head = nodes + 0;
    arcs[0].rev = arcs + 1;
    arcs[1].rev = arcs + 0;
    cap[0] = 100;
    cap[1] = 0;*/
}
