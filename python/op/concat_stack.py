import tensorflow as tf

a = tf.constant([[1,2,3],[4,5,6]]) # shape (2,3)
b = tf.constant([[7,8,9],[10,11,12]]) # shape (2,3)

ab_concat_1 = tf.concat([a,b], axis=0) # shape(4,3)
#[[1,2,3],[4,5,6],[7,8,9],[10,11,12]]
ab_concat_2 = tf.concat([a,b], axis=1) # shape(2,6)
#[[1,2,3,7,8,9],[4,5,6,10,11,12]]
ab_stack_1 = tf.stack([a,b], axis=0) # shape (2,2,3)
#[[[1,2,3],[4,5,6]],[[7,8,9],[10,11,12]]]
ab_stack_2 = tf.stack([a,b], axis=2) # shape (2,3,2)
#[[[1,4],[2,5],[3,6]],[[7,10],[8,11],[9,12]]]

ab_unstack = tf.unstack(ab_stack_1, axis=0) 
# unstack:0 shape (2,3) unstack:1 shape (2,3)

