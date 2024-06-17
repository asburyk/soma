import matplotlib.pyplot as plt
import igraph as ig
ig.config["plotting.backend"] = "matplotlib"
g=ig.Graph.Famous("petersen")
ig.plot(g)
plt.show()