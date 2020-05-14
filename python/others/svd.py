import numpy as np
import numpy.linalg as LA

A = np.random.randn(9, 6)
print("==================A================")
print(A)
#print(A.T)
#LA = np.linalg.norm(A)
#print(LA)

"""
full_matrices: bool, optional
True(default), u and v have the shapes (M,M) and (N,N), respectively.
Otherwise, the shapes are (M,K) and (K,N), respectively, where K=min(M,N)
"""
#U, s, V = LA.svd(A, full_matrices=True)
U, s, V = LA.svd(A, full_matrices=False)
print(U.shape, s.shape, V.shape)

print("==================U================")
print(U)
print("==================s================")
print(s)
S=np.diag(s)
print("==================S================")
print(S)
print("==================V================")
print(V)

A_svd = U.dot(S.dot(V))
print("==================A_svd================")
print(A_svd)

print(np.allclose(A, A_svd))


