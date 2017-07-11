import numpy as np
import networkx as nx

A = np.loadtxt('matriz.txt')
G = nx.from_numpy_matrix(A)

# liste de vértices e arestas com pesos
vertices = G.nodes()
arestas = G.edges(data=True)