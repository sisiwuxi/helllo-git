import tensorflow.examples.tutorials.mnist.input_data as input_data
import numpy as np
import matplotlib.pyplot as plt
import pylab
import tensorflow as tf 
from scipy import signal
import cv2
from keras.models import Sequential
from keras.layers import Dense, Activation, Conv2D, MaxPooling2D, Flatten


def pylab_show(image):
    image_arr = np.reshape(image,(28,28))                           
    plt.imshow(image_arr)
    pylab.show()

mnist = input_data.read_data_sets("/home/sisi/work/dataset/MNIST-data", one_hot=True)#input_data read .zip and do normalization

#========================Step1.mnist info==========================#
"""
print(mnist.train.images.shape, mnist.train.labels.shape)#(55000, 784) 784=28*28 (55000, 10)
print(mnist.test.images.shape, mnist.test.labels.shape)#(10000, 784) 784=28*28 (10000, 10)

plt.figure()

image=mnist.train.images[130]#[784,]
image_arr = np.reshape(image,(28,28)) 
plt.subplot(4,2,1)
plt.imshow(image_arr)

vertical=[[1,-1]]#[1,2]
plt.subplot(4,2,3)
plt.title('vertical')
plt.imshow(vertical)

horizontal=[[1],[-1]]#[2,1]
plt.subplot(4,2,4)
plt.title('horizontal')
plt.imshow(horizontal)

conv_vertical=signal.convolve2d(image_arr, vertical)
plt.subplot(4,2,5)
plt.imshow(conv_vertical)

conv_horizontal=signal.convolve2d(image_arr, horizontal)
plt.subplot(4,2,6)
plt.imshow(conv_horizontal)

#door_image = cv2.imread('./door.png', 0)
#door_image_arr = np.reshape(door_image,(449,269)) 
door_image = cv2.imread('./6.png', cv2.IMREAD_GRAYSCALE)#cv2.IMREAD_COLOR
door_image_arr = np.reshape(door_image,(648,1152)) 

plt.subplot(4,2,2)
plt.imshow(door_image)
#plt.imshow(door_image[:,::-1])

door_conv_vertical=signal.convolve2d(door_image_arr, vertical)
plt.subplot(4,2,7)
plt.imshow(door_conv_vertical)

door_conv_horizontal=signal.convolve2d(door_image_arr, horizontal)
plt.subplot(4,2,8)
plt.imshow(door_conv_horizontal)
plt.show()
"""
#========================Step2.mnist train==========================#
"""
num_pixels=784#28*28=784
num_classes=10#0~9

model=Sequential()
model.add(Dense(num_pixels, input_dim=num_pixels, activation='relu'))
model.add(Dense(num_classes, activation='softmax'))
model.compile(loss='categorical_crossentropy',optimizer='adam',metrics=['accuracy'])

model.summary()

history=model.fit(mnist.train.images, mnist.train.labels, validation_data=(mnist.test.images, mnist.test.labels), epochs=10, batch_size=128, verbose=True)
"""
#========================Convolution Neural Network==========================#
num_classes=10#0~9

X_train=mnist.train.images.reshape((mnist.train.images.shape[0],28,28,1))
X_test=mnist.train.labels

Y_train=mnist.test.images.reshape((mnist.test.images.shape[0],28,28,1))
Y_test=mnist.test.labels

model=Sequential()
#model.add(Conv2D(32,3,input_shape=(28,28,1),activation='relu'))#kernels number=32,3 means 3*3 square
model.add(Conv2D(32,3,activation='relu'))#kernels number=32,3 means 3*3 square
model.add(Conv2D(32,3,activation='relu'))#keras can match input_shape self-adaption, input_shape is not necessary
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(64,3,activation='relu'))#After MaxPooling, featuremap decrease, so increase the kernel channel
model.add(Conv2D(64,3,activation='relu'))
model.add(MaxPooling2D(pool_size=2))
model.add(Flatten())#4->1 dimension
model.add(Dense(128,activation='relu'))
model.add(Dense(num_classes,activation='softmax'))

model.compile(loss='categorical_crossentropy',optimizer='adam',metrics=['accuracy'])

model.summary()

model.fit(X_train, X_test, validation_data=(Y_train, Y_test), epochs=10, batch_size=200)
scores=model.evaluate(X_test, Y_test, verbose=0)

