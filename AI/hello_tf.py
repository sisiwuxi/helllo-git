import tensorflow as tf
a=tf.constant(1,name="input_a")
b=tf.constant(2,name="input_a")

c=tf.multiply(a,b,name="mul_c")
d=tf.multiply(a,b,name="mul_d")

e=tf.add(c,d,name="add_e")

print(e)

sess=tf.Session()
sess.run(e)

writer=tf.summary.FileWriter('./my_graph', sess.graph)

writer.close()
sess.close()
