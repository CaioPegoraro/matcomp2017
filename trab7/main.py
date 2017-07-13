import numpy as np
import networkx as nx
import sys
from numpy import linalg as LA
import matplotlib.pyplot as plt

if len(sys.argv) > 1:
    arq = sys.argv[1]
else:
    print("Enter a filename")
    arq = input()
print(arq)
data = np.loadtxt(arq, delimiter=',')
data = np.delete(data, 0, 1)
mean = data.mean(axis=0, keepdims=True);
print(mean)
scaledData = data - mean
print(scaledData.mean(axis=0))
covMatrix = np.cov(scaledData,rowvar=False)
eigenValues, eigenVectors = LA.eig(covMatrix)
index1 = 0;
index2 = 0;
i = 0;
max1 = 0;
max2 = 0;
for eigVal in eigenValues:
    if eigVal > max1:
        max2 = max1
        index2 = index1
        max1 = eigVal;
        index1 = i
    elif eigVal > max2:
        max2 = eigVal
        index2 = i
    i = i+1

PCA = eigenVectors[:, [index1,index2]]
PCAt = PCA.transpose()
y = np.dot(PCAt, data.transpose())

print(y)

