import numpy as np
import matplotlib.pyplot as plt
import math


def constant_func(x):
	y=1
	return y

def log_arithm_func(x, a=math.e):
	y=math.log(x, a)
	return y

def nlog_arithm_func(x, a=math.e):
	y=x*math.log(x, a)
	return y

def n_square_func(x):
	y=x*x
	return y

def n_cubic_func(x):
	y=x*x*x
	return y

def n_power2_func(x):
	y=math.pow(2,x)
	return y

def n_powern_func(x):
	y=math.pow(x,x)
	return y

def n_factorial_func(x):
	if x==0 or x==1:
		return 1
	y=1
	for i in range(2,int(x+1)):
		y*=i
	return y

X=np.linspace(0.01,10,100)
y0=[constant_func(x) for x in X]
y1=[log_arithm_func(x) for x in X]
y2=X
y3=[nlog_arithm_func(x) for x in X]
y4=[n_square_func(x) for x in X]
y5=[n_cubic_func(x) for x in X]
y6=[n_power2_func(x) for x in X]
y7=[n_factorial_func(x) for x in X]
y8=[n_powern_func(x) for x in X]

plt.plot(X,y0,color='black',label='y=1')
plt.plot(X,y1,color='green',label='y=logn')
plt.plot(X,y2,color='blue',label='y=x')
plt.plot(X,y3,color='yellow',label='y=nlogn')
plt.plot(X,y4,color='orange',label='y=n^2')
plt.plot(X,y5,color='red',label='y=n^3')
plt.plot(X,y6,color='cyan',label='y=2^n')
plt.plot(X,y7,color='grey',label='y=n!')
plt.plot(X,y8,color='magenta',label='y=n^n')

plt.xlim(-1,10)
plt.ylim(-10,50)
plt.legend()
plt.savefig("time_complexity.png")
