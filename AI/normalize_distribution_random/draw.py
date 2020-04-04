import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pdb

arr_gauss = []
with open("./gauss.txt") as fdata:
    for line in fdata.readlines():
        lineArr = line.strip().split(',')
        for index in range(8):
            arr_gauss.append(float(lineArr[index]))
arr_laplace = []
with open("./laplace.txt") as fdata:
    for line in fdata.readlines():
        lineArr = line.strip().split(',')
        for index in range(8):
            arr_laplace.append(float(lineArr[index]))

#pdb.set_trace()
#n,bins,patches = plt.hist(arr , bins = 256, normed = 1, facecolor = 'green' , alpha = 0.75)
plt.subplot(2,1,1)
print("# max = {} in {} ".format(np.max(arr_gauss), np.argmax(arr_gauss)+1))
print("# min = {} in {} ".format(np.min(arr_gauss), np.argmin(arr_gauss)+1))
n,bins,patches = plt.hist(arr_gauss , bins = 256, facecolor = 'green' , alpha = 0.75)
plt.title('guass')

plt.subplot(2,1,2)
print("# max = {} in {} ".format(np.max(arr_laplace), np.argmax(arr_laplace)+1))
print("# min = {} in {} ".format(np.min(arr_laplace), np.argmin(arr_laplace)+1))
n,bins,patches = plt.hist(arr_laplace , bins = 256, facecolor = 'red' , alpha = 0.75)
plt.title('laplace')

#plt.show()
plt.savefig("gauss_laplace.png")
