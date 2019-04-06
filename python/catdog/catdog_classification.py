import cv2
import os
import random
import numpy as np

import matplotlib.pyplot as plt
#import seaborn as sns

from keras.models import Sequential, Model
from keras.layers import Dense, Dropout, Activation, Flatten, Input
from keras.layers.convolutional import Conv2D, MaxPooling2D
from keras.optimizers import SGD, RMSprop, Adam
from keras.applications.vgg16 import VGG16, preprocess_input
from keras.preprocessing.image import ImageDataGenerator
from keras.preprocessing import image

from sklearn.model_selection import train_test_split
#%matplotlib inline

import tensorflow as tf
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto()
config.gpu_options.allow_growth=True
set_session(tf.Session(config=config))

#Using Tensorflow backend.


#load dataset
def read_and_process_image(data_dir, width=64, height=64, channels=3, preprocess=False):
  train_images=[data_dir+i for i in os.listdir(data_dir)]
  random.shuffle(train_images)

  def read_image(file_path, preprocess):
    img=image.load_img(file_path, target_size=(height, width))
    x=image.img_to_array(img)
    x=np.expand_dims(x,axis=0)
    if preprocess:
      x=preprocess_input(x)
    return x

  def prep_data(images, preprocess):
    count=len(images)
    data=np.ndarray((count,height,width,channels),dtype=np.float32)
  
    for i,image_file in enumerate(images):
      image=read_image(image_file, preprocess)
      data[i]=image

    return data

  def read_labels(file_path):
    labels=[]
    for i in file_path:
      label = 1 if 'dog' in i else 0
      labels.append(label)

    return labels

  X=prep_data(train_images,preprocess)
  labels=read_labels(train_images) 
  assert X.shape[0] == len(labels)
  print("Train shape: {}".format(X.shape))
  return X,labels

WIDTH=64
HEIGHT=64
CHANNELS=3
X,y=read_and_process_image('/home/sisi/work/dataset/catdog/train/',width=WIDTH,height=HEIGHT,channels=CHANNELS)

#count plot the labels
#sns.countplot(y)
#sns.plt.title('Cat vs Dog')

def show_cats_and_dogs(X, idx):
  plt.figure(figsize=(10,5),frameon=True)
  img=X[idx,:,:,::-1]
  img=img/255
  plt.imshow(img)
  plt.show()

# show three pictures
for idx in range(0,3):
  show_cats_and_dogs(X, idx)

#Converlution Netron Network
train_X, test_X, train_y, test_y = train_test_split(X,y,test_size=0.3)
#VGG double channels every time
model=Sequential()
model.add(Conv2D(32,3,input_shape=(HEIGHT,WIDTH,3),activation='relu',padding="same"))
model.add(Conv2D(32,3,activation='relu',padding="same"))
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(64,3,activation='relu',padding="same"))
model.add(Conv2D(64,3,activation='relu',padding="same"))
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(128,3,activation='relu',padding="same"))
model.add(Conv2D(128,3,activation='relu',padding="same"))
model.add(MaxPooling2D(pool_size=2))
model.add(Conv2D(256,3,activation='relu',padding="same"))
model.add(Conv2D(256,3,activation='relu',padding="same"))
model.add(MaxPooling2D(pool_size=2))
#Add more hidden layers like VGG
model.add(Flatten())
model.add(Dense(256,activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(256,activation='relu'))
model.add(Dropout(0.5))
model.add(Dense(1))
model.add(Activation('sigmoid'))

#compile model
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

#fit model
history=model.fit(train_X, train_y, validation_data=(test_X, test_y), epochs=20, batch_size=200)
#final evaluation of the model
scores=model.evaluate(test_X, test_y, verbose=0)
#print("Large CNN error: %.2f%%" % (100-scores*100))

#loss
plt.plot(history.history['loss'])
plt.plot(history.history['val_loss'])
plt.title('model loss')
plt.ylabel('loss')
plt.xlabel('epoch')
plt.legend(['train','test'],loc='upper right')
plt.show()

#accuracy
plt.plot(history.history['acc'])
plt.plot(history.history['val_acc'])
plt.title('model accuracy')
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend(['train','test'],loc='upper right')
plt.show()
