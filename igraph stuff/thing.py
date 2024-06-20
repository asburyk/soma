import matplotlib.pyplot as plt
import igraph as ig
from igraph import Graph
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Load('igraph stuff/Somap3.gml', 'gml')
#print(g.summary())
print(g.eigenvector_centrality(weights='value'))
#ig.plot(g)
#plt.show()