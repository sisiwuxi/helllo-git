import numpy as np
test=np.load('/home/sisi/work/AI/tflite/MobilenetV1_Logits_Conv2d_1c_1x1_weights_quant_FakeQuantWithMinMaxVars.npy',encoding = "latin1")  #加载文件
doc = open('/home/sisi/work/AI/tflite/MobilenetV1_Logits_Conv2d_1c_1x1_weights_quant_FakeQuantWithMinMaxVars.txt', 'a')  #打开一个存储文件，并依次写入
np.set_printoptions(threshold=np.NaN) #禁用NumPy自动省略行为并强制打印整个数组
print(test, file=doc)  #将打印内容写入文件中
