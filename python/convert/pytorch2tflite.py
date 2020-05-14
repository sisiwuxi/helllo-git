# pytorch2keras
# keras2tflite

import tensorflow as tf
from tensorflow import keras
import numpy as np

import torch
from torchvision import models
import torch.nn as nn
# import torch.nn.functional as F
from torch.autograd import Variable
from collections import OrderedDict
from torch.nn import init
import math


def conv_bn(inp, oup, stride):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 3, stride, 1, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True)
    )


def conv_1x1_bn(inp, oup):
    return nn.Sequential(
        nn.Conv2d(inp, oup, 1, 1, 0, bias=False),
        nn.BatchNorm2d(oup),
        nn.ReLU(inplace=True)
    )

def channel_shuffle(x, groups):
    batchsize, num_channels, height, width = x.data.size()

    channels_per_group = num_channels // groups
    
    # reshape
    x = x.view(batchsize, groups, 
        channels_per_group, height, width)

    x = torch.transpose(x, 1, 2).contiguous()

    # flatten
    x = x.view(batchsize, -1, height, width)

    return x
    
class InvertedResidual(nn.Module):
    def __init__(self, inp, oup, stride, benchmodel):
        super(InvertedResidual, self).__init__()
        self.benchmodel = benchmodel
        self.stride = stride
        assert stride in [1, 2]

        oup_inc = oup//2
        
        if self.benchmodel == 1:
            #assert inp == oup_inc
            self.banch2 = nn.Sequential(
                # pw
                nn.Conv2d(oup_inc, oup_inc, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup_inc),
                nn.ReLU(inplace=True),
                # dw
                nn.Conv2d(oup_inc, oup_inc, 3, stride, 1, groups=oup_inc, bias=False),
                nn.BatchNorm2d(oup_inc),
                # pw-linear
                nn.Conv2d(oup_inc, oup_inc, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup_inc),
                nn.ReLU(inplace=True),
            )                
        else:                  
            self.banch1 = nn.Sequential(
                # dw
                nn.Conv2d(inp, inp, 3, stride, 1, groups=inp, bias=False),
                nn.BatchNorm2d(inp),
                # pw-linear
                nn.Conv2d(inp, oup_inc, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup_inc),
                nn.ReLU(inplace=True),
            )        
    
            self.banch2 = nn.Sequential(
                # pw
                nn.Conv2d(inp, oup_inc, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup_inc),
                nn.ReLU(inplace=True),
                # dw
                nn.Conv2d(oup_inc, oup_inc, 3, stride, 1, groups=oup_inc, bias=False),
                nn.BatchNorm2d(oup_inc),
                # pw-linear
                nn.Conv2d(oup_inc, oup_inc, 1, 1, 0, bias=False),
                nn.BatchNorm2d(oup_inc),
                nn.ReLU(inplace=True),
            )
          
    @staticmethod
    def _concat(x, out):
        # concatenate along channel axis
        return torch.cat((x, out), 1)        

    def forward(self, x):
        if 1==self.benchmodel:
            x1 = x[:, :(x.shape[1]//2), :, :]
            x2 = x[:, (x.shape[1]//2):, :, :]
            out = self._concat(x1, self.banch2(x2))
        elif 2==self.benchmodel:
            out = self._concat(self.banch1(x), self.banch2(x))

        return channel_shuffle(out, 2)


class ShuffleNetV2(nn.Module):
    def __init__(self, n_class=1000, input_size=224, width_mult=1.):
        super(ShuffleNetV2, self).__init__()
        
        assert input_size % 32 == 0
        
        self.stage_repeats = [4, 8, 4]
        # index 0 is invalid and should never be called.
        # only used for indexing convenience.
        if width_mult == 0.5:
            self.stage_out_channels = [-1, 24, 48, 96, 192, 1024]
        elif width_mult == 1.0:
            self.stage_out_channels = [-1, 24, 116, 232, 464, 1024]
        elif width_mult == 1.5:
            self.stage_out_channels = [-1, 24, 176, 352, 704, 1024]
        elif width_mult == 2.0:
            self.stage_out_channels = [-1, 24, 224, 488, 976, 2048]
        else:
            raise ValueError(
                """{} groups is not supported for
                       1x1 Grouped Convolutions""".format(num_groups))

        # building first layer
        input_channel = self.stage_out_channels[1]
        self.conv1 = conv_bn(3, input_channel, 2)    
        self.maxpool = nn.MaxPool2d(kernel_size=3, stride=2, padding=1)
        
        self.features = []
        # building inverted residual blocks
        for idxstage in range(len(self.stage_repeats)):
            numrepeat = self.stage_repeats[idxstage]
            output_channel = self.stage_out_channels[idxstage+2]
            for i in range(numrepeat):
                if i == 0:
                #inp, oup, stride, benchmodel):
                    self.features.append(InvertedResidual(input_channel, output_channel, 2, 2))
                else:
                    self.features.append(InvertedResidual(input_channel, output_channel, 1, 1))
                input_channel = output_channel
                
                
        # make it nn.Sequential
        self.features = nn.Sequential(*self.features)

        # building last several layers
        self.conv_last      = conv_1x1_bn(input_channel, self.stage_out_channels[-1])
        self.globalpool = nn.Sequential(nn.AvgPool2d(int(input_size/32)))              
    
        # building classifier
        self.classifier = nn.Sequential(nn.Linear(self.stage_out_channels[-1], n_class))

    def forward(self, x):
        x = self.conv1(x)
        x = self.maxpool(x)
        x = self.features(x)
        x = self.conv_last(x)
        x = self.globalpool(x)
        x = x.view(-1, self.stage_out_channels[-1])
        x = self.classifier(x)
        return x


#shufflenetv2_x0.5_60.646_81.696.pth.txt
def KerasShuffleNetV2_05(input_shape=(224, 224, 3)):
    image_input = keras.layers.Input(shape=input_shape)
    # conv1
    network = keras.layers.Conv2D(24, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(image_input)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=False)(network)
    network = keras.layers.Activation("relu")(network)
    #maxpool
    network = keras.layers.MaxPool2D(pool_size=(3, 3), strides=(2, 2))(network)

    # featuers.0.banch1
    network = keras.layers.Conv2D(24, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.0.banch2
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(24, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.1.banch2
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(24, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.2.banch2
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(24, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.3.banch2
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(24, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(24, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.4.banch1
    network = keras.layers.Conv2D(48, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.4.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.5.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.6.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.7.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.8.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.9.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.10.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.11.banch2
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(48, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(48, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.12.banch1
    network = keras.layers.Conv2D(96, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.12.banch2
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(96, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.13.banch2
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(96, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.14.banch2
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(96, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.15.banch2
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(96, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(96, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # conv_last
    network = keras.layers.Conv2D(1024, (1, 1), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=False)(network)
    network = keras.layers.Activation("relu")(network)
    # globalpool
    network = keras.layers.AveragePooling2D(pool_size=(7,7), strides=7, padding="same")(network)
    #classifier
    network = keras.layers.Dense(1000, input_shape=(1024,))(network) 

    model = keras.Model(inputs=image_input, outputs=network)

    return model


#shufflenetv2_x1_69.402_88.374.pth.tar
def KerasShuffleNetV2_10(input_shape=(224, 224, 3)):
    image_input = keras.layers.Input(shape=input_shape)
    # conv1
    network = keras.layers.Conv2D(24, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(image_input)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=False)(network)
    network = keras.layers.Activation("relu")(network)
    #maxpool
    network = keras.layers.MaxPool2D(pool_size=(3, 3), strides=(2, 2))(network)

    # featuers.0.banch1
    network = keras.layers.Conv2D(24, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.0.banch2
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(58, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.1.banch2
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(58, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.2.banch2
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(58, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.3.banch2
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(58, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(58, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.4.banch1
    network = keras.layers.Conv2D(116, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.4.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.5.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.6.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.7.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.8.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.9.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.10.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.11.banch2
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(116, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(116, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.12.banch1
    network = keras.layers.Conv2D(232, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Activation("relu")(network)
    # featuers.12.banch2
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(232, (3, 3), strides=(2, 2), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.13.banch2
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(232, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.14.banch2
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(232, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # featuers.15.banch2
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    network = keras.layers.Conv2D(232, (3, 3), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)   
    network = keras.layers.Conv2D(232, (1, 1), strides=(1, 1), use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=True)(network)
    network = keras.layers.Activation("relu")(network)
    # conv_last
    network = keras.layers.Conv2D(1024, (1, 1), strides=(1, 1), padding="same", use_bias="FALSE")(network)
    network = keras.layers.BatchNormalization(axis=3, trainable=False, fused=False)(network)
    network = keras.layers.Activation("relu")(network)
    # globalpool
    network = keras.layers.AveragePooling2D(pool_size=(7,7), strides=7, padding="same")(network)
    #classifier
    network = keras.layers.Dense(1000, input_shape=(1024,))(network) 

    model = keras.Model(inputs=image_input, outputs=network)

    return model



class PytorchToKeras(object):
    def __init__(self, pModel, kModel):
        super(PytorchToKeras, self)
        self.__source_layers = []
        self.__target_layers = []
        self.pModel = pModel
        self.kModel = kModel
        tf.keras.backend.set_learning_phase(0)

    def __retrieve_k_layers(self):
        for i, layer in enumerate(self.kModel.layers):
            if len(layer.weights) > 0:
                self.__target_layers.append(i)

    def __retrieve_p_layers(self, input_size):

        input = torch.randn(input_size)
        input = Variable(input.unsqueeze(0))
        hooks = []

        def add_hooks(module):

            def hook(module, input, output):
                if hasattr(module, "weight"):
                    # print(module)
                    self.__source_layers.append(module)

            if not isinstance(module, nn.ModuleList) and not isinstance(module, nn.Sequential) and module != self.pModel:
                hooks.append(module.register_forward_hook(hook))

        self.pModel.apply(add_hooks)

        self.pModel(input)
        for hook in hooks:
            hook.remove()

    def convert(self, input_size):
        self.__retrieve_k_layers()
        self.__retrieve_p_layers(input_size)

        for i, (source_layer, target_layer) in enumerate(zip(self.__source_layers, self.__target_layers)):
            print(source_layer)
            weight_size = len(source_layer.weight.data.size())
            transpose_dims = []
            for i in range(weight_size):
                transpose_dims.append(weight_size - i - 1)
                """
            if isinstance(source_layer, nn.Conv2d):
                transpose_dims = [2,3,1,0]
                self.kModel.layers[target_layer].set_weights([source_layer.weight.data.numpy(
                ).transpose(transpose_dims), source_layer.bias.data.numpy()])
            elif isinstance(source_layer, nn.BatchNorm2d):
                self.kModel.layers[target_layer].set_weights([source_layer.weight.data.numpy(), source_layer.bias.data.numpy(),
                source_layer.running_mean.data.numpy(), source_layer.running_var.data.numpy()])
               """

    def save_model(self, output_file):
        self.kModel.save(output_file)

    def save_weights(self, output_file):
        self.kModel.save_weights(output_file, save_format='h5')


pytorch_model = ShuffleNetV2()
#pytorch_model.load_state_dict(torch.load('./shufflenetv2_x0.5_60.646_81.696.pth.tar'))
pytorch_model.load_state_dict(torch.load('./shufflenetv2_x1_69.402_88.374.pth.tar'))


torch.save(pytorch_model, 'pt.pth')

#Load the pretrained model
pytorch_model = torch.load('pt.pth')

#keras_model = KerasShuffleNetV2_05(input_shape=(224, 224, 3))
keras_model = KerasShuffleNetV2_10(input_shape=(224, 224, 3))

# #Time to transfer weights
converter = PytorchToKeras(pytorch_model, keras_model)

converter.convert((3, 224, 224))

# #Save the converted keras model for later use
#converter.save_weights("keras_weights.h5")
converter.save_model("keras.h5")


# convert keras model to tflite model
#converter = tf.contrib.lite.TocoConverter.from_keras_model_file("keras.h5")
converter = tf.lite.TocoConverter.from_keras_model_file("keras.h5")

tflite_model = converter.convert()
open("tf.tflite", "wb").write(tflite_model)

