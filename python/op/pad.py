import tensorflow as tf
 
t=tf.constant([[[1,2,3], [2,3,4],[2,1,4]],
               [[1,2,3], [2,3,4],[2,1,4]],
               [[1,2,3],  [2,3,4],[2,1,4]]])
t2=tf.constant([[1,2,3]])
print(t.get_shape)
print(t2.get_shape())
 
a=tf.pad(t,[[1,1], [2,2],[1,1]])
c=tf.pad(t2,[[1,1],[2,2]])


yolo=tf.constant([[
                 [[11,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]],
                 [[21,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]],
                 [[31,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]],
                 [[41,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]],
                 [[51,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]],
                 ]])

yolo_pad=tf.pad(yolo,[[0,0],[3,3],[3,3],[0,0]])

with tf.Session() as sess:
    yolo,yolo_pad=sess.run([yolo,yolo_pad])
    print(yolo.shape)
    print(yolo)
    print(yolo_pad.shape)
    print(yolo_pad)
""" 
with tf.Session() as sess:
    a,c=sess.run([a,c])
    print(t)
    print(t.shape)
    print(a)
    print(a.shape)
    print(t2)
    print(t2.shape)
    print(c)
    print(c.shape)
"""
