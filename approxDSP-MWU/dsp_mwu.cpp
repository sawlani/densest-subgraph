#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>   
#include <queue> 
#include <set>
#include <algorithm>
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
	double eps = 0.1;
	string input_file = argv[2];
	
	string output_file;
	ofstream outfile;

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
		adj_e[p].push_back(i);
		adj_e[q].push_back(i);
	}

	/*for (int i = 0; i < n; i++)
	{
		cout << endl << "Nbrs of " << i << endl;
		for (int j : adj_v[i])
		{
			cout << j << " ";
		}

		cout << endl << "Nbr edges of " << i << endl;
		for (int j : adj_e[i])
		{
			cout << j << " ";
		}
	}*/

	double D = 1.24; //change this
	vector<double> y(m,1.0); //Initialize probability (dual) vector. Note that we are not scaling it to sum to 1.
	int q=2;
	double C;
	vector<double> z(2*m,0.0); //Primal vector
	vector<double> z_avg(2*m, 0.0);

	for (int tt = 0; tt < iters && feasible; tt++) {
		
		///////////////////////////// INNER PROBLEM //////////////////////////
		fill(z.begin(), z.end(), 0);
		C = 0;
		for (int v=0; v<n; v++)
		{
			vector<int> E = adj_e[v];
			double D_temp = D;
			sort(E.begin(),E.end(), [&](int i,int j){return y[i]>y[j];} );
			int j;
			for (j=0; j < E.size() && D_temp > q; j++)
			{
				//cout << v << j << " " << E[j] << endl;
				int edge = E[j];
				if (e[edge][0] == v)
				{
					z[2*edge] = q;
					z_avg[2*edge] += (double)q/iters;
				}
				else
				{
					z[2*edge + 1] = q;
					z_avg[2*edge + 1] += (double)q/iters;
				}
				C += y[edge]*q;
				D_temp -= q;
			}
			if (j < E.size())
			{
				//cout << v << j << " " << E[j] << endl;
				int edge = E[j];
				if (e[edge][0] == v)
				{
					z[2*edge] = D_temp;
					z_avg[2*edge] += (double)D_temp/iters;
				}
				else
				{
					z[2*edge + 1] = D_temp;
					z_avg[2*edge + 1] += (double)D_temp/iters;
				}
				C += y[edge]*D_temp;
			}
			/*cout << "z values" << endl; 
			for (int j=0; j<2*m; j++)
			{
				cout << "  " << z[j];
			}*/
			
			
			
		}

		////////////////////////////////////////////////////////////////////////
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
				y[j] = y[j]*(1- 0.25*eps*(z[2*j]+z[2*j+1]));
			}
		}
		/*cout << "y values" << endl; 
		for (int j=0; j<m; j++)
		{
			cout << "  " << y[j];
		}*/
		if (feasible)
		{
			cout << "Iteration: " << tt << "Feasible for D value: " << D << endl;
		}
		else
		{
			cout << "Iteration: " << tt << "Infeasible for D value: " << D << endl;
		}
	}
	return 0;
}