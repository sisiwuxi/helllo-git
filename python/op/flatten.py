import tensorflow as tf;
import tensorflow.contrib.slim as slim;

#array=([[[1,2,3],[4,5,6],[7,8,9]],[[10,11,12],[13,14,15],[16,17,27]],[[18,19,20],[21,22,23],[24,25,26]]])    #shape is (3,3,3)
array = tf.ones(shape=[1, 1024, 7, 7])
flatten=slim.flatten(array, scope='flat_32')

with tf.Session() as sess:
    sess.run(flatten)
    print(array)
    print(flatten)

