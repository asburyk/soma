import matplotlib.pyplot as plt
import igraph as ig
from igraph import Graph
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Load('igraph stuff/Somap3.gml', 'gml')
#g= Graph(n=3,edges=[[0,1],[1,2],[2,0]],
#          edge_attrs={'b':[1,1,1]})

# Weight function
def weightF(v):
    return 5 - v

# Calculates weighted degree
def weighted_degree(graph):
    wd = []
    for v in graph.vs:
        sum = 0
        for e in v.incident():
            sum = sum + weightF(e['value'])
        wd = wd + [sum]
    return wd


# Prints centrality information
def print_central(cent, graph, centMeasure):
    i = 0
    max = 0
    maxI = -1
    count = 0
    while (i < graph.vcount()):
        if (max == cent[i]):
            count = count + 1
        if (max < cent[i]):
            max = cent[i]
            maxI = i
            count = 1
        i = i + 1
    print("Central vertices for " + centMeasure + ":")
    i = 0
    while (i < graph.vcount()):
        if (max == cent[i]):
            print("Label: " + g.vs[i]['label'] + ", code: " + g.vs[i]['u'] + chr(int(g.vs[i]['w'])))
        i = i + 1
    print("Centrality score: " + str(cent[maxI]))
    print("# of central vertices: " + str(count))
    print('')

# calculates weight array

weights = []
i = 0
while (i < g.ecount()):
    weights = weights + [weightF(g.es[i]['value'])]
    i = i + 1


# Centrality calculations / printing
print('')
print_central(weighted_degree(g), g, "Weighted Degree") # uses weight function, so if w(v)=5-value(v), w(2) = 3 and w(3) = 2
print_central(g.closeness(weights='value'), g, "Closeness Centrality") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3
print_central(g.betweenness(weights='value'), g, "Betweenness Centrality") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3
print_central(g.eigenvector_centrality(weights=weights), g, "Eigenvector Centrality") # uses weight function, so if w(v)=5-value(v), w(2) = 3 and w(3) = 2
print_central(g.harmonic_centrality(weights='value'), g, "Harmonic Closeness") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3


# Code that checks for K3's in this graph
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