import numpy as np
import matplotlib.pyplot as plt
#%matplotlib inline
from sklearn.linear_model import LinearRegression

x = np.linspace(1,10,20)
print(x)
a = np.random.randint(1,10,20)
print(a)
y = x*4 + a
print(y)
plt.plot(x,y,'o')
ply.show()

'''
model = LinearRegression()
model.fit(x.reshape(20,1), y.reshape(20,1))
model.predict(6)
model.coef_
model.intercept_
#y = kx+b
'''
