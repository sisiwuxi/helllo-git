import tensorflow as tf

tensor = tf.constant([1, 2, 3, 4, 5, 6, 7,8])
with tf.Session() as sess:
  sess.run(tf.initialize_all_variables())
  print(tensor)
  print(sess.run(tensor))
  '''
  [1 2 3 4 5 6 7 8]
  '''
  tensorReshape = tf.reshape(tensor,[2,4])
  print(tensorReshape)
  print(sess.run(tensorReshape))
  '''
  [[1 2 3 4]
   [5 6 7 8]]
  '''
  tensorReshape = tf.reshape(tensor,[1,2,4])
  print(tensorReshape)
  print(sess.run(tensorReshape))
  '''
  [[[1 2 3 4]
    [5 6 7 8]]]
  '''
  tensorReshape = tf.reshape(tensor,[-1,2,2])
  print(tensorReshape)
  print(sess.run(tensorReshape))
  '''
  [[[1 2]
    [3 4]]
 
   [[5 6]
    [7 8]]]
  '''
