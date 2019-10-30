import matplotlib.pyplot as plt
import matplotlib.patches as patches

fig1 = plt.figure()
ax1 = fig1.add_subplot(1, aspect='equal')
ax1.add_patch(patches.Rectangle((0, 0),0.5,0.5))
plt.show()
#fig1.savefig('rect1.png', dpi=90, bbox_inches='tight')

