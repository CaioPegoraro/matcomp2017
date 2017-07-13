import numpy as np
import networkx as nx
import sys
from numpy import linalg as LA

if len(sys.argv) > 1:
    arq = sys.argv[1]
else:
    print("Enter a filename")
    arq = input()
print(arq)
data = np.loadtxt(arq, delimiter=',')
mean = data.mean(axis=0, keepdims=True);
print(mean)
scaledData = data - mean
print(scaledData.mean(axis=0))
covMatrix = np.cov(scaledData)
eigenValues, eigenVectors = LA.eig(covMatrix)
print("teste")

