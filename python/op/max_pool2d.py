import tensorflow as tf 
import tensorflow.contrib.slim as slim


x1 = tf.ones(shape=[1, 224, 224, 64]) 

y1 = slim.max_pool2d(x1, 2, padding='SAME', scope='pool_1')

with tf.Session() as sess: 
    sess.run(tf.global_variables_initializer()) 
    y1_value,x1_value=sess.run([y1,x1])
    print("shapes ", y1_value.shape)
    print(y1_value)
