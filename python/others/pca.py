import numpy as np
from sklearn.decomposition import PCA
import numpy.linalg as LA

X = np.random.randint(1, 10, size=(100, 10))
pca = PCA(n_components = 3)
print(X[:10,:])

pca.fit(X)
x_pca = pca.transform(X)
print(x_pca[:10,:])

print(x_pca.shape)
print(pca.explained_variance_ratio_)



#how to use svd perform PCA
mean_vec = np.mean(X, axis = 0)
cov_mat = (X - mean_vec).T.dot((X - mean_vec)) / (X.shape[0] - 1)

np.allclose(cov_mat, np.cov(X.T))
U,s,V = LA.svd(cov_mat)
print(U.shape, s.shape, V.shape)
S = np.diag(s)/np.sum(s)
print(np.diag(S[:3]))
print(np.allclose(np.diag(S[:3]), pca.explained_variance_ratio_))

