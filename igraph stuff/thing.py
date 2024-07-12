import matplotlib.pyplot as plt
import igraph as ig
from igraph import Graph
from numpy import floor, sort
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Load('igraph stuff/Somap3copy.gml', 'gml')


mult = g.is_multiple()
count = 0
for e in mult:
    if (e == 1):
        count = count + 1
print()
print()
print(count)
print()
print()

for v in g.vs:
    v['hasMult'] = 0
i = 0
while (i < len(mult)):
    if (mult[i] == 1):
        g.vs[g.es[i].source]['hasMult'] += 1
        g.vs[g.es[i].target]['hasMult'] += 1
    i += 1

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
        if (g.vs[i]['hasMult'] != 0):
            print("(Multi-edge Vertex, multi-edges " + str(g.vs[i]['hasMult']) + ") Label: " + g.vs[i]['label'] + ", code: " + g.vs[i]['u'] + chr(int(g.vs[i]['w'])) +", centrality: " + str(cent[i]))
        i = i + 1
    print("Centrality score: " + str(cent[maxI]))
    print("# of central vertices: " + str(count))
    print('')

# calculates weight array

weights = []
i = 0
while (i < g.ecount()):
    weights = weights + [weightF(g.es[i]['value'])]
    g.es[i]['weight'] = weightF(g.es[i]['value'])
    i = i + 1


# Centrality calculations / printing
print('')
print_central(weighted_degree(g), g, "Weighted Degree") # uses weight function, so if w(v)=5-value(v), w(2) = 3 and w(3) = 2
print_central(g.closeness(weights='value'), g, "Closeness Centrality") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3
print_central(g.betweenness(weights='value'), g, "Betweenness Centrality") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3
print_central(g.eigenvector_centrality(weights=weights), g, "Eigenvector Centrality") # uses weight function, so if w(v)=5-value(v), w(2) = 3 and w(3) = 2
print_central(g.harmonic_centrality(weights='value'), g, "Harmonic Closeness") # uses value, value(2)=w(2)=2 and value(3)=w(3)=3
# using these values for weights ensure 2-swap edges are 'more important' than 3-swap edges



# Code that checks for K3's in this graph
#print(g.eigenvector_centrality(weights='value'))
#print(g.subisomorphic_vf2(g2, edge_color1=g.es['b'],edge_color2=g2.es['b']))
g2=Graph(n=5,edges=[[0,1],[1,2],[2,3],[3,4],[4,0]],
         edge_attrs={'b':[2,2,2,2,2]})
i = 0
color = []
while (i < g.ecount()):
   
   if (g.es[i]['b'] == 'WGO' or g.es[i]['b'] == 'WG' or g.es[i]['b'] == 'WO' or g.es[i]['b'] == 'GO'):
       color = color + [1]
   else:
       color = color + [0]
   i = i + 1
subs = g.get_subisomorphisms_vf2(g2, edge_color1=g.es["value"],edge_color2=g2.es['b'])
#subs = g.get_subisomorphisms_vf2(g2)
subs = sort(subs)
subs = sorted(subs, key=lambda x: (x[0], x[1], x[2], x[3], x[4]))

f = open("k5s.txt", "w")
for num in range(0,int(len(subs)/10)):
    e = subs[num*10]
    colors = [0]*200
    for i in range(0,5):
        for j in range(i+1,5):
            if (g.are_adjacent(e[i],e[j])):
                for k in range(0,int(g.es[g.get_eid(e[i],e[j])]["value"])):
                    colors[ord(g.es[g.get_eid(e[i],e[j])]["b"][k])] = 1
    sum = 0
    for n in colors:
        sum += n
    if (sum <= 3):
        f.write(str(e))
        f.write("\n")
print(len(subs))
f.close()
"""
for e in subs:
    print(e)
print(len(subs))
"""
#ig.plot(g)
#plt.show()