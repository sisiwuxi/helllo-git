import tensorflow as tf

fq_input = tf.placeholder(tf.float32)
fq_output = tf.fake_quant_with_min_max_vars(fq_input, min=-0.498046875, max=0.498046875, num_bits=8)#结果完全正确
with tf.Session() as sess:
  print(sess.run(fq_output, feed_dict = {fq_input:[0.2309581 ,  0.02781899, -0.25252128,  0.08246478,  0.08959707,  0.11362964,  0.06973048, -0.13781932, -0.04602775, -0.28280422,  0.25792092,  -0.12966314]}))
