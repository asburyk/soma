import matplotlib.pyplot as plt
import igraph as ig
from igraph import Graph
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Load('igraph stuff/Somap3.gml', 'gml')
g2= Graph(n=3,edges=[[0,1],[1,2],[2,0]],
          edge_attrs={'b':[1,1,1]})
#print(g.summary())
#print(g.eigenvector_centrality(weights='value'))
#print(g.subisomorphic_vf2(g2, edge_color1=g.es['b'],edge_color2=g2.es['b']))
i = 0
color = []
while (i < g.ecount()):
    if (g.es[i]['b'] == 'LRB' or g.es[i]['b'] == 'LR' or g.es[i]['b'] == 'LB' or g.es[i]['b'] == 'LB'):
        color = color + [1]
    else:
        color = color + [0]
    i = i + 1
print(g.subisomorphic_vf2(g2, edge_color1=color,edge_color2=g2.es['b']))
#ig.plot(g)
#plt.show()