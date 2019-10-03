import networkx as nx
import os

for file in os.listdir(os.getcwd()):
    filename = os.path.splitext(file)[0]
    ext = os.path.splitext(file)[1]
    print(ext)
    if (ext == '.gml'):
        f=open(filename + ".txt", "w+")
        G = nx.read_gml(file, label='id')
        f.write(str(G.number_of_nodes())+" "+str(G.number_of_edges())+"\n")
        for e in G.edges:
            f.write(str(e[0])+" "+str(e[1])+"\n")

        f.close()