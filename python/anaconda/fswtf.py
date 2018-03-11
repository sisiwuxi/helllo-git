#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sun Mar 11 17:02:36 2018

@author: sisiwuxi
"""
"""a simple multiplication program"""
import tensorflow as tf

a=tf.constant(10)
b=tf.constant(32)
sess = tf.Session()
print sess.run(a+b)

a = tf.placeholder("float")
b = tf.placeholder("float")
y = tf.multiply(a, b)
sess = tf.Session()
print sess.run(y, feed_dict={a: 3, b: 3})


"""
# Set up a linear classifier.
classifier = tf.estimator.LinearClassifier()

# Train the model on some example data.
classifier.train(input_fn=train_input_fn, steps=2000)

# Use it to predict.
predictions = classifier.predict(input_fn=predict_input_fn)
"""

"""Linear Regression"""
import numpy as np
 
num_points = 100
vectors_set = []
for i in xrange(num_points):
         x1= np.random.normal(0.0, 0.55)
         y1= x1 * 0.1 + 0.3 + np.random.normal(0.0, 0.03)
         vectors_set.append([x1, y1])
 
x_data = [v[0] for v in vectors_set]
y_data = [v[1] for v in vectors_set]

import matplotlib.pyplot as plt
 
plt.plot(x_data, y_data, 'ro', label='Original data')
plt.legend()
plt.show()

"""Cost function and gradient descent algorithm"""
W = tf.Variable(tf.random_uniform([1], -1.0, 1.0))
b = tf.Variable(tf.zeros([1]))
y = W * x_data + b
loss = tf.reduce_mean(tf.square(y - y_data))
optimizer = tf.train.GradientDescentOptimizer(0.5)
train = optimizer.minimize(loss)
init = tf.initialize_all_variables()
sess = tf.Session()
sess.run(init)
for step in xrange(8):
   sess.run(train)
   print step, sess.run(W), sess.run(b), sess.run(loss)
   #Graphic display
   plt.plot(x_data, y_data, 'ro')
   plt.plot(x_data, sess.run(W) * x_data + sess.run(b))
   plt.xlabel('x')
   plt.xlim(-2,2)
   plt.ylim(0.1,0.6)
   plt.ylabel('y')
   plt.legend()
   plt.show()