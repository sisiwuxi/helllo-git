import sys
import tensorflow as tf
print(sys.version)

a = tf.Variable([1.0,1.3,2.1,3.41,4.51])
b = tf.cast(a>3,dtype=tf.bool)
c = tf.cast(a>3,dtype=tf.int8)
e = tf.cast(a<2,dtype=tf.float32)
d = tf.cast(a,dtype=tf.int8)

sess = tf.Session()
sess.run(tf.initialize_all_variables())
print(sess.run(a))
print(sess.run(b))
print(sess.run(c))
print(sess.run(e))
print(sess.run(d))
