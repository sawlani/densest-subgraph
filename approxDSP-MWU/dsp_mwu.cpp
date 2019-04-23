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

int main(int argc, char** argv) {

	int iters = atoi(argv[1]);
	double eps = 1.0/iters;
	string input_file = argv[2];
	
	string output_file;

	if (argc >= 4)
	{
		output_file = argv[3];
	}
	else
	{
		output_file = "results/results_MWU.txt";
	}
	
	freopen (input_file.c_str(), "r", stdin); //input file
	outfile.open(output_file.c_str(), ios_base::app); //output file
	
	cin >> n >> m;
	vector<int> adj_e[n];
	unordered_set<int> adj_v[n];
	int e[m][2];
	
	w.reserve(n);
	deg.reserve(n);

	bool feasible = true;

	for (int i = 0; i < n; i++) 
		w[i] = 0; //initial vertex weights=0, i.e., no self loops at the start
	for (int i = 0; i < m; i++) {
		int p, q;
		cin >> p >> q;
		p -= 1;
		q -= 1;
		e[i][0] = p;
		e[i][1] = q;
		adj_v[p].insert(q);
		adj_v[q].insert(p);
		adj_e[p].insert(i);
		adj_e[q].insert(i);
	}

	double D = n; //change this
	vector<double> y(m,1); //Initialize probability (dual) vector. Note that we are not scaling it to sum to 1.
	int q=2;
	vector<double> z(2*m); //Primal vector

	for (int tt = 0; tt < iters && feasible; tt++) {
		
		///////////////////////////// INNER PROBLEM
		for (int v=0; v<n; v++)
		{
			z.clear();
			E = adj_e[v];
			double D_temp = D;
			double C = 0;
			sort(E.begin(),E.end(), [&](int i,int j){return y[i]>y[j];} );
			for (j=0; D_temp > q; j++)
			{
				int edge = E[j];
				if (e[edge][0] == v)
				{
					z[2*edge] == q;
					z_avg[2*edge] += (double)q/iters;
				}
				else
				{
					z[2*edge + 1] == q;
					z_avg[2*edge + 1] += (double)q/iters;
				}
				C += y[edge]*q;
				D_temp -= q;
			}
			int edge = E[j];
			if (e[edge][0] == v)
			{
				z[2*edge] == D_temp;
				z_avg[2*edge] += (double)D_temp/T;
			}
			else
			{
				z[2*edge + 1] == D_temp;
				z_avg[2*edge + 1] += (double)D_temp/T;
			}
			C += y[edge]*D_temp;
		}
		/////////////////////////////

		double sum_y = 0;
		for (int edge=0; edge < m; edge++)
		{
			sum_y += y[edge];
		}
		if (C < sum_y)
		{
			feasible = false;
		}
		else
		{
			for (int j=0; j<m; j++)
			{
				y[j] = y[j]*(1- 0.25*eps*(z[2*j]+z[2*j+1]))
			}
		}

	}



	return 0;
}