import tensorflow.examples.tutorials.mnist.input_data as input_data
import numpy as np
import matplotlib.pyplot as plt
import pylab
import tensorflow as tf 
from scipy import signal
import cv2

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

x=tf.placeholder(tf.float32,[None,784])#input
w=tf.Variable(tf.zeros([784,10]))#weights
b=tf.Variable(tf.zeros([10]))#unlinear
y=tf.matmul(x,w)+b#predict value

y_t=tf.placeholder(tf.float32,[None,10])#realy value
cross_entropy=tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y_t,logits=y))

train_step=tf.train.GradientDescentOptimizer(0.5).minimize(cross_entropy)
sess=tf.InteractiveSession()
tf.global_variables_initializer().run()

for _ in range(10):
    batch_xs,batch_ys=mnist.train.next_batch(100)
    sess.run(train_step,feed_dict={x:batch_xs,y_t:batch_ys})

correct_prediction=tf.equal(tf.argmax(y,1), tf.argmax(y_t,1))
accuracy=tf.reduce_mean(tf.cast(correct_prediction,tf.float32))

#========================Step2.mnist test==========================#

for i in range(0,len(mnist.test.images)):
    result=sess.run(correct_prediction,feed_dict={x:np.array([mnist.test.images[i]]), y_t:np.array([mnist.test.labels[i]])})
    if not result:
        print(sess.run(y,  feed_dict={x:np.array([mnist.test.images[i]]),y_t:np.array([mnist.test.labels[i]])}))
        #[[ 1.4489825 -3.6879177  2.046319  -7.4227304  2.7487266  2.8074431  8.348517  -6.484418   1.4153743 -1.2202989]]
        print(sess.run(y_t,feed_dict={x:np.array([mnist.test.images[i]]),y_t:np.array([mnist.test.labels[i]])}))
        #[[0. 0. 0. 0. 0. 1. 0. 0. 0. 0.]]
        pylab_show(mnist.test.images[i])
        break
#========================Step3.mnist test final result==========================#

print(sess.run(accuracy,feed_dict={x:mnist.test.images,y_t:mnist.test.labels}))


