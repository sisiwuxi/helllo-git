import tensorflow as tf;
import numpy as np;
 
A = np.array([[1,2,3],[4,5,6]])
x = tf.transpose(A, [1,0])
 
B = np.array([[[1,2,3],[4,5,6]]])
y = tf.transpose(B, [2,1,0])
with tf.Session() as sess:
	print(A)
	print(sess.run(x))
	print(B)
	print(sess.run(y))
