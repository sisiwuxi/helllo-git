import tensorflow as tf;
import tensorflow.contrib.slim as slim;

#array = tf.ones(shape=[1, 1024, 7, 7])
array = tf.ones(shape=[1, 4096])
dropout=slim.dropout(array, keep_prob=0.5, is_training=True)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    sess.run(dropout)
    print(array)
    print(dropout)
