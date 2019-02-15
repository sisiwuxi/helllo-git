import sys
import tensorflow as tf

tmp_input = [[True, False],
                     [True, False]]
where_input = tf.where(tmp_input)

sess = tf.Session()
sess.run(tf.initialize_all_variables())
print(sess.run(where_input))
