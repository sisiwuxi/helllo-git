import matplotlib.pyplot as plt
x = [10,16,33,30,62,59,116,156,373]
y = [13,30,23,61,45,119,90,198,326]
plt.xlim(xmax=608,xmin=0)
plt.ylim(ymax=608,ymin=0)
plt.plot(x,y,'ro')
for a,b in zip(x,y):
    plt.text(a,b,(a,b),ha='center',va='bottom',fontsize=10)
ax=plt.gca()
ax.xaxis.set_ticks_position('top')
ax.invert_yaxis()
plt.show()
plt.savefig('anchors.png')
