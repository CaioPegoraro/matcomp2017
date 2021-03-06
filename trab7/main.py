import numpy as np
import sys
from numpy import linalg as LA
import matplotlib.pyplot as plt

if len(sys.argv) > 1:
    arq = sys.argv[1]
else:
    print("Enter a filename")
    arq = input()
print(arq)
# load file
data = np.loadtxt(arq, delimiter=',')
# delete classification column
data = np.delete(data, 0, 1)
# find mean
mean = data.mean(axis=0, keepdims=True);
print(mean)
# scale data
# 1) Centralizar dados:
scaledData = data - mean
print(scaledData.mean(axis=0))
# 2) Computar a matriz de covari
covMatrix = np.cov(scaledData, rowvar=False)
# 3) Obter autovalores e autovetores de
eigenValues, eigenVectors = LA.eig(covMatrix)
index1 = 0;
index2 = 0;
i = 0;
max1 = 0;
max2 = 0;
# 4) Selecionar os d autovetores associados aos d maiores autovalores

for eigVal in eigenValues:
    if eigVal > max1:
        max2 = max1
        index2 = index1
        max1 = eigVal;
        index1 = i
    elif eigVal > max2:
        max2 = eigVal
        index2 = i
    i = i + 1

# 5) Definir W PCA=
PCA = eigenVectors[:, [index1, index2]]
PCAt = PCA.transpose()
# 6) Projetar dados:  (autovetores nas linhas de W TPCA evetor coluna)
y = np.dot(PCAt, data.transpose())
plt.plot(y.transpose(), 'ro')
plt.show()
