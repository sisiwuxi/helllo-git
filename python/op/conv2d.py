import tensorflow as tf 
import tensorflow.contrib.slim as slim


x1 = tf.ones(shape=[1, 454, 454, 3]) 
#w = tf.fill([5, 5, 3, 64], 1)
w = tf.fill([64, 7, 7, 3], 1)
# print("rank is", tf.rank(x1))

#y1 = tf.nn.conv2d(x1, w, strides=[1, 1, 1, 1], padding='SAME')
#y2 = slim.conv2d(x1, 64, [5, 5], weights_initializer=tf.ones_initializer, padding='SAME')
y2 = slim.conv2d(x1, 64, 7, 2, weights_initializer=tf.ones_initializer, padding='VALID', scope='conv_2')

with tf.Session() as sess: 
    sess.run(tf.global_variables_initializer()) 
    #y1_value,y2_value,x1_value=sess.run([y1,y2,x1])
    y2_value,x1_value=sess.run([y2,x1])
    print("shapes ", y2_value.shape)
    #print(y1_value==y2_value)
    #print(y1_value)
    print(y2_value)

