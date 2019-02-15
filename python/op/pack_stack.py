import tensorflow as tf
import numpy as np
sess = tf.InteractiveSession()
x=tf.constant([[11,12,13],[21,22,23]])
y=tf.constant([[31,32,33],[41,42,43]])
#z=tf.pack([x,y])
z=tf.stack([x,y])
print(x)
print(x.eval())
print(y)
print(y.eval())
print(z)
print(z.eval())
sess.close()

