import networkx as nx
#import time
import sys
import os

filename = sys.argv[1]

with open(filename, 'r') as fin:
	data = fin.read().splitlines(True)
with open(filename+".mod", 'w+') as fout:
	fout.writelines(data[1:])

result_folder = "results_exact/"
result_filename = result_folder + "results.txt"
G = nx.read_edgelist(filename+".mod")
nx.set_edge_attributes(G, 1, 'capacity')
G = nx.DiGraph(G)
n = G.number_of_nodes()
print("n:")
print(n)
m = G.number_of_edges()//2
print("m:")
print(m)
possibleD = []
for i in range(1,m+1):
	for j in range(2,n+1):
		possibleD.append(i/j)

possibleD = sorted(possibleD)
#print(possibleD)
minD = 0
maxD = len(possibleD)-1
while maxD - minD > 0:
	g_index=(minD+maxD+1)//2
	g = possibleD[g_index]
	#print("g:")
	#print(g)
	H = G.copy()
	H.add_node('s')
	H.add_node('t')
	for v in G.nodes():
		H.add_edge('s',v, capacity = m)
		H.add_edge(v,'t', capacity = m+2*g-G.in_degree(v))

	soln = nx.minimum_cut(H,'s','t', capacity = 'capacity')
	cut = soln[1][0]
	#print(soln)
	if cut == {'s'}:
		maxD = g_index-1
	else:
		minD = g_index
		V = cut
		bestg = g
	#print(maxD)
	#print(minD)
os.remove(filename+".mod")
f = open(result_filename,"a+") 
f.write(filename)
f.write("\n")
f.write(str(V - {'s'}))
f.write("\n")
f.write(str(bestg))

#start_time = time.time()
#end_time = time.time()
#print("Seconds:" + str(end_time - start_time))

