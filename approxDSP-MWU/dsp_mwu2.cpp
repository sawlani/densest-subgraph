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
	double eps = atof(argv[2]);
	double rho = 0.0; //Initialization of rho values
	string input_file = argv[3];
	
	string output_file;
	ofstream outfile;

	if (argc >= 5)
	{
		output_file = argv[4];
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
	
	// Calculate rho, ie max degree
	for (int v = 0; v < n; v++)
	{
		if(adj_e[v].size() > rho)
		{
			rho = (double)adj_e[v].size(); //choose max degree vertex as value for rho
		}
	}
	cout << "Value of rho = " << rho << endl;
	cout << "Value of eps = " << eps << endl;


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
	vector<double> x(n,1.0); //Initialize dual vector of the size of vertices. We are not scaling it to sum to 1.
	//int q=2;
	double C;
	vector<double> z(2*m,0.0); //Primal vector
	//vector<double> z_avg(2*m, 0.0);
	vector<double> grad(n, 0.0);
	vector<double> z_avg(2*m, 0.0);
	vector<double> grad_avg(n, 0.0);

	for (int tt = 0; tt < iters && feasible; tt++) {
		
		///////////////////////////// INNER PROBLEM //////////////////////////
		//Here we computed z_ev. Now we want to compute inner problem as an iteration over all edges in m x 2
		//matrix, denoted by variable 'e'. Then we also compute gradient estimate Bz \in \Rbb^n as we go along to
		//save running time.
		fill(z.begin(), z.end(), 0);
		fill(grad.begin(), grad.end(), 0);
		C = 0;
		fill(grad_avg.begin(), grad_avg.end(), 0);
		for (int l=0; l<m; l++)
		{
			if (x[e[l][0]] <= x[e[l][1]])
			{
				z[2*l] = 1;
				z_avg[2*l] = (double)1/(tt+1) + ((double)tt/(tt+1))*z_avg[2*l];
				z_avg[2*l+1] = ((double)tt/(tt+1))*z_avg[2*l+1];
				C += x[e[l][0]];
				grad[e[l][0]] += 1.0;
			}
			else
			{
				z[2*l+1] = 1;
				z_avg[2*l+1] = (double)1/(tt+1) + ((double)tt/(tt+1))*z_avg[2*l+1];
				z_avg[2*l] = ((double)tt/(tt+1))*z_avg[2*l];
				C += x[e[l][1]];
				grad[e[l][1]] += 1.0;
			}
			grad_avg[e[l][0]] += z_avg[2*l];
			grad_avg[e[l][1]] += z_avg[2*l+1];
			/*cout << "Values of avg z: ";
			for (int t = 0; t < z_avg.size(); t++)
			{
				cout << z_avg[t] << " ";
			}
			cout << endl;*/
			/*vector<int> E = adj_e[v];
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
			}*/
			/*cout << "z values" << endl; 
			for (int j=0; j<2*m; j++)
			{
				cout << "  " << z[j];
			}*/
			
			
			
		}

		//////////////////////////////////////////////////////////////////////// 
		/*double sum_x = 0;
		for (int v=0; v < m; v++)
		{
			sum_x += x[v];
		}
		double avg_den = C/sum_x;
		cout << "Average density estimate for iteration "<<tt <<" = " << avg_den << endl;*/
		double den_est = *max_element(grad_avg.begin(), grad_avg.end());
		cout << "Density estimate for iteration "<<tt <<" = " << den_est << endl;
		/*if (C < sum_y)
		{
			feasible = false;
		}*/
		for (int v=0; v<n; v++)
		{
			x[v] = x[v]*(1 +  0.25*eps/rho *grad[v]);
		}
		/*cout << "y values" << endl; 
		for (int j=0; j<m; j++)
		{
			cout << "  " << y[j];
		}
		if (feasible)
		{
			cout << "Iteration: " << tt << "Feasible for D value: " << D << endl;
		}
		else
		{
			cout << "Iteration: " << tt << "Infeasible for D value: " << D << endl;
		}*/
	}
	return 0;
}
