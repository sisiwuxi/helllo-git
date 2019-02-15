import tensorflow as tf;
import tensorflow.contrib.slim as slim;

#array = tf.ones(shape=[1, 1024, 7, 7])
array = tf.ones(shape=[1, 50176])
fc=slim.fully_connected(array, 512)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    sess.run(fc)
    print(array)
    print(fc)

