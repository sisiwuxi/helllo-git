import matplotlib.pyplot as plt 
import numpy as np

'''
#32,3,3,3
#wc=np.load("MobilenetV1_MobilenetV1_Conv2d_0_weights_quant_FakeQuantWithMinMaxVars.npy")
#wc=np.load("MobilenetV1_Conv2d_0_weights.npy")
for i in range(3):
    plt.matshow(wc[:, :, 0, i], cmap=plt.get_cmap('gray'))
    plt.title(str(i) + "th conv filter")
    plt.colorbar()
    plt.show() 
'''

'''
#1024,1,1,1024
#wc=np.load("MobilenetV1_Logits_Conv2d_1c_1x1_weights.npy")
#wc=np.load("MobilenetV1_Conv2d_13_pointwise_weights.npy")
plt.matshow(wc[:, 0, 0, :], cmap=plt.get_cmap('gray'))
plt.show() 
'''

'''
#1,3,3,1024
wc=np.load("MobilenetV1_Conv2d_13_depthwise_depthwise_weights.npy")
print(wc.shape)
plt.matshow(wc[0, :, :, 1], cmap=plt.get_cmap('gray'))
plt.show() 
'''

#'''
#64,7,7,3
wc=np.load("yolo_conv_2_weights_quant_FakeQuantWithMinMaxVars.npy")
print(wc.shape)
'''
for i in range(1):   
    index=3*i+1
    cmd = "plt%s=plt.subplot(8,3,%d)" % index
    exec(cmd)
    cmd1 = "plt%s.matshow(wc[0, :, :, 0], cmap=plt.get_cmap('gray'))" %index
    exec(cmd1)


'''
#'''
plt1=plt.subplot(8,3,1)
plt1.matshow(wc[0, :, :, 0], cmap=plt.get_cmap('gray'))
plt2=plt.subplot(8,3,2)
plt2.matshow(wc[0, :, :, 1], cmap=plt.get_cmap('gray'))
plt3=plt.subplot(8,3,3)
plt3.matshow(wc[0, :, :, 2], cmap=plt.get_cmap('gray'))

plt4=plt.subplot(8,3,4)
plt4.matshow(wc[1, :, :, 0], cmap=plt.get_cmap('gray'))
plt5=plt.subplot(8,3,5)
plt5.matshow(wc[1, :, :, 1], cmap=plt.get_cmap('gray'))
plt6=plt.subplot(8,3,6)
plt6.matshow(wc[1, :, :, 2], cmap=plt.get_cmap('gray'))

plt7=plt.subplot(8,3,7)
plt7.matshow(wc[2, :, :, 0], cmap=plt.get_cmap('gray'))
plt8=plt.subplot(8,3,8)
plt8.matshow(wc[2, :, :, 1], cmap=plt.get_cmap('gray'))
plt9=plt.subplot(8,3,9)
plt9.matshow(wc[2, :, :, 2], cmap=plt.get_cmap('gray'))

plt10=plt.subplot(8,3,10)
plt10.matshow(wc[3, :, :, 0], cmap=plt.get_cmap('gray'))
plt11=plt.subplot(8,3,11)
plt11.matshow(wc[3, :, :, 1], cmap=plt.get_cmap('gray'))
plt12=plt.subplot(8,3,12)
plt12.matshow(wc[3, :, :, 2], cmap=plt.get_cmap('gray'))

plt13=plt.subplot(8,3,13)
plt13.matshow(wc[4, :, :, 0], cmap=plt.get_cmap('gray'))
plt14=plt.subplot(8,3,14)
plt14.matshow(wc[4, :, :, 1], cmap=plt.get_cmap('gray'))
plt15=plt.subplot(8,3,15)
plt15.matshow(wc[4, :, :, 2], cmap=plt.get_cmap('gray'))

plt16=plt.subplot(8,3,16)
plt16.matshow(wc[5, :, :, 0], cmap=plt.get_cmap('gray'))
plt17=plt.subplot(8,3,17)
plt17.matshow(wc[5, :, :, 1], cmap=plt.get_cmap('gray'))
plt18=plt.subplot(8,3,18)
plt18.matshow(wc[5, :, :, 2], cmap=plt.get_cmap('gray'))

plt19=plt.subplot(8,3,19)
plt19.matshow(wc[6, :, :, 0], cmap=plt.get_cmap('gray'))
plt20=plt.subplot(8,3,20)
plt20.matshow(wc[6, :, :, 1], cmap=plt.get_cmap('gray'))
plt21=plt.subplot(8,3,21)
plt21.matshow(wc[6, :, :, 2], cmap=plt.get_cmap('gray'))

plt22=plt.subplot(8,3,22)
plt22.matshow(wc[7, :, :, 0], cmap=plt.get_cmap('gray'))
plt23=plt.subplot(8,3,23)
plt23.matshow(wc[7, :, :, 1], cmap=plt.get_cmap('gray'))
plt24=plt.subplot(8,3,24)
plt24.matshow(wc[7, :, :, 2], cmap=plt.get_cmap('gray'))
#'''
plt.show() 
#'''
