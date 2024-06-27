import matplotlib.pyplot as plt
import igraph as ig
from igraph import Graph
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Load('igraph stuff/Somap3.gml', 'gml')
#g2= Graph(n=3,edges=[[0,1],[1,2],[2,0]],
#          edge_attrs={'b':[1,1,1]})
def weighted_degree():
    wd = []
    for v in g.vs:
        sum = 0
        for e in v.incident():
            sum = sum + e['value']
        wd = wd + [sum]
    return wd   
weight = []
i = 0
while (i < g.ecount()):
    if (g.es[i]['value'] == 2):
        weight = weight + [2] # 2-swap weights
    else:
        weight = weight + [1] # 3-swap weights
    i = i + 1
#print(g.summary())
centrality = g.betweenness(weights=weight)
#centrality = weighted_degree()
print(centrality)
i = 0
max = 0
maxI = -1
count = 0
while (i < g.vcount()):
    if (max == centrality[i]):
        count = count + 1
    if (max < centrality[i]):
        max = centrality[i]
        maxI = i
        count = 1
    i = i + 1
print('Central vertex: ' + g.vs[maxI]['label'] + ', centrality: ' + str(centrality[maxI]))
print('# of central vertices: ' + str(count))
#print(g.eigenvector_centrality(weights='value'))
#print(g.subisomorphic_vf2(g2, edge_color1=g.es['b'],edge_color2=g2.es['b']))
#i = 0
#color = []
#while (i < g.ecount()):
#    if (g.es[i]['b'] == 'LRB' or g.es[i]['b'] == 'LR' or g.es[i]['b'] == 'LB' or g.es[i]['b'] == 'RB'):
#        color = color + [1]
#    else:
#        color = color + [0]
#    i = i + 1
#print(g.subisomorphic_vf2(g2, edge_color1=color,edge_color2=g2.es['b']))
#ig.plot(g)
#plt.show()