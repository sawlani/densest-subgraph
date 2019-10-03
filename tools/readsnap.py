import networkx as nx
import os

for file in os.listdir(os.getcwd()):
    filename = os.path.splitext(file)[0]
    ext = os.path.splitext(file)[1]
    print(file)
    if (ext == '.snap'):
        f=open(filename + ".txt", "w+")
        G = nx.read_edgelist(file)
        mapping=dict(zip(G.nodes(),range(G.number_of_nodes())))
        G=nx.relabel_nodes(G,mapping)
        f.write(str(G.number_of_nodes())+" "+str(G.number_of_edges())+"\n")
        for e in G.edges:
            f.write(str(int(e[0])+1)+" "+str(int(e[1])+1)+"\n")

        f.close()