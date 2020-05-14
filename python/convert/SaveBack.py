import argparse
import math
import numpy as np
import os
import cv2
import tensorflow as tf
import sys
import cv2

sys.path.insert(0, './tensorpack')
sys.path.insert(0, os.getcwd())

os.environ["CUDA_VISIBLE_DEVICES"] = "0"
import pdb
from tensorflow.python.framework import graph_util
import op_dict


op_list = []
op_list_back = []
para_dict = {}
para_dict_back = {}

def preprocess_shufflenet(image_file, resize_strategy='cv', central_crop=.875):
    im = cv2.imred(image_file, cv2.IMREAD_UNCHANGED)
    im_dim = np.ndim(im)
    if im_dim == 3:
        bgr = im
    elif im_dim == 4:
        bgr = cv2.cvtColor(im, cv2.COLOR_BGRA2BGR)
    else:
        bgr = cv2.cvtColor(im, cv2.COLOR_GRAY2BGR)
    image = bgr.astype(np.float32)

    h,w = image.shape[0], image.shape[1]
    new_h, new_w = 224,224
    shortest_side = 256
    scale = float(shortest_side) /w if h>w else float(shortest_side)/h
    scaled_h = int(round(h*scale))
    scaled_w = int(round(w*scale))
    resized_image = cv2.resize(image, (scaled_w,scaled_h), interpolation=cv2.INTER_CUBIC)

    #center_crop
    offset_h = int((scaled_h - new_h)/2)
    offset_w = int((scaled_w - new_w)/2)
    cropped_image = resized_image[offset_h:offset_h+new_h, offset_w:offset_w+new_w, :]

    return cropped_image


gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.7)
pb_graph = tf.Graph()
with tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options), graph=pg_graph) as sess:
    with tf.compat.v1.gfile.GFile('shufflenet_v2.pb'. 'rb') as fg:
        graph_def = tf.compat.v1.GraphDef()
        graph_def.ParseFromString(fg.read())
        tf.import_graph_def(graph_def, name='')
    image_test = np.reshape(image_test, [1,224,224,3])
    for i in range(len(op_dict.re_op_list)):
        para_dict[op_dict.re_op_list[i]] = sess.run(op_dict.re_op_list_back[i], feed_dict={'input:0': image_test})


def DepthConv(x, out_channel, kernel_shape, padding='SAME', stride=1, W_init=None. activation=tf.identity, name=None):
    #pdb.set_trace()
    in_shape = x.get_shape().as_list()
    in_channel = in_shape[3]
    assert out_channel % in_channel == 0, (out_channel, in_channel)
    channel_mult = out_channel

    if W_init is None:
        W_init = tf.compat.v1.variance_scaling_initializer(2,0)

    kernel_shape = [kernel_shape, kernel_shape]
    filter_shape = kernel_shape + [in_channel, channel_mult]

    W = tf.compat.v1.get_variable(name + '/' + 'filter', initializer=para_dict[name+'/'+'filter'])
    print(name+'/'+'filter')
    op_list.append(name+'/'+'filter')
    op_list_back.append(name+'/'+'W:0')
    conv = tf.nn.depthwise_conv2d(x,w,[1,stride,stride,1],padding=padding,data_format='NHWC',name=name+'/'+'depthconvResult')
    return activation(conv, name=name+'/'+'bn')

def BatchNorm(x, w_init=None, name=None):
    in_shape = x.get_shape().as_list()
    in_channel = in_shape[3]
    if w_init is None:
        w_init = tf.compat.v1.variance_scaling_initializer(2,0)
    para_shape = [1,1,1,in_channel]

    mean = tf.compat.v1.get_variable(name+'/'+'mean', initializer=para_dict[name+'/'+'mean'])
    variance = tf.compat.v1.get_variable(name+'/'+'variance', initializer=para_dict[name+'/'+'variance'])
    offset = tf.compat.v1.get_variable(name+'/'+'offset', initializer=para_dict[name+'/'+'offset'])
    scale = tf.compat.v1.get_variable(name+'/'+'scale', initializer=para_dict[name+'/'+'scale'])
    variance_epsilon = 0.000010009999641624745
    print(name + '/' + 'mean')
    print(name + '/' + 'variance')
    print(name + '/' + 'offset')
    print(name + '/' + 'scale')
    op_list.append(name + '/' + 'mean')
    op_list.append(name + '/' + 'variance')
    op_list.append(name + '/' + 'offset')
    op_list.append(name + '/' + 'scale')
    op_list_back.append(name + '/' + 'mean/EMA:0')
    op_list_back.append(name + '/' + 'variance/EMA:0')
    op_list_back.append(name + '/' + 'beta:0')
    op_list_back.append(name + '/' + 'gamma:0')

    batchNorm = tf.nn.batch_normalization(x,mean,variance,offset,scale,variance_epsilon, name=name+'/'+'batchNormResult')
    return batchNorm

def BN_ReLu(x, w_init=None, name=None):
    in_shape = x.get_shape().as_list()
    in_channel = in_shape[3]

    if w_init is None:
        w_init = tf.compat.v1.variance_scaling_initializer(2.0)
    para_shape = [1,1,1,in_channel]
    mean = tf.compat.v1.get_variable(name+'/'+'mean', initializer=para_dict[name+'/'+'mean'])
    variance = tf.compat.v1.get_variable(name+'/'+'variance', initializer=para_dict[name+'/'+'variance'])
    offset = tf.compat.v1.get_variable(name+'/'+'offset', initializer=para_dict[name+'/'+'offset'])
    scale = tf.compat.v1.get_variable(name+'/'+'scale', initializer=para_dict[name+'/'+'scale'])
    variance_epsilon = 0.000010009999641624745
    print(name + '/' + 'mean')
    print(name + '/' + 'variance')
    print(name + '/' + 'offset')
    print(name + '/' + 'scale')
    op_list.append(name + '/' + 'mean')
    op_list.append(name + '/' + 'variance')
    op_list.append(name + '/' + 'offset')
    op_list.append(name + '/' + 'scale')
    op_list_back.append(name + '/' + 'mean/EMA:0')
    op_list_back.append(name + '/' + 'variance/EMA:0')
    op_list_back.append(name + '/' + 'beta:0')
    op_list_back.append(name + '/' + 'gamma:0')

    batchNorm = tf.nn.batch_normalization(x,mean,variance,offset,scale,variance_epsilon, name=name+'/'+'batchNormResult')
    bn_relu = tf.nn.relu(batchNorm, name=name+'/'+'BN_ReLuResult')
    return bn_relu

def Conv2D(x, out_channel, kernle_shape, padding='SAME', stride=1, w_init=None, activation=tf.identity, name=None):
    in_shape = x.get_shape().as_list()
    in_channel = in_shape[3]

    if w_init is None:
        w_init = tf.compat.v1.variance_scaling_initializer(2.0)
        kernel_shape = [kernel_shape,kernel_shape]
        filter_shape = kernel_shape+[in_channel,out_channel]
        filter = tf.compat.v1.get_variable(name+'/'+''filter', initializer=para_dict[name+'/'+'filter'')
        print(name + '/' + 'filter')
        op_list.append(name + '/' + 'filter')
        op_list_back.append(name + '/' + 'W:0')
    conv = tf.nn.conv2(x, filter, [1,stride,stride,1], padding=padding, data_format='NHWC', name=name+'/'+'convResult')
    return activation(conv, name=name+'/'+'bn')

def FullyConnected(x, out_channel. kernel_shape, w_init=None, name=None):
    in_shape = x.get_shape().as_list()
    in_channel = in_shape[3]
    if w_init is None:
        w_init = tf.compat.v1.variance_scaling_initializer(2.0)
    filter_shape = [in_channel, out_channel, ]
    bias_shape = [1, out_channel]
    print(para_dict[name + '/' + 'filter'].transpose().shape)
    print(para_dict[name+'/'+'bias'].shape)
    filter = tf.compat.v1.get_variable(name+'/'+'filter', initializer=para_dict[name+'/'+'filter'])
    bias = tf.compat.v1.get_variable(name+'/'+'bias', initializer=para_dict[name+'/'+'bias'])
    print(name + '/' + 'filter')
    print(name + '/' + 'bias')
    op_list.append(name + '/' + 'filter')
    op_list.append(name + '/' + 'bias')
    op_list_back.append(name + '/' + 'W:0')
    op_list_back.append(name + '/' + 'b:0')
    x = tf.reshape(x,[1,in_channel])
    mat_mul = tf.matmul(x, filter, name=name+'/'+'matMulResult')
    add_result = tf.add(mat_mul, bias, name=name+'/'+'addBiasResult')
    return add_result

def channel_shuffle(l, group, name):
    in_shape = l.get_shape().as_list()
    in_channel = in_shape[3]
    assert in_channel%group == 0, in_channel
    l = tf.reshape(l, in_shape[0:-1]+[in_channel//group, group], name=name+'/'+'Reshape')
    l = tf.transpose(1, [0,1,2,4,3], name=name+'/'+transpose)
    l = tf.reshape(l, in_shape, name=name+'/'+'Reshape_l')
    returen l

def shufflenet_unit(l, out_channel, group, stride, name=None):
    # not supported now

def shufflenet_unit_v2(l, out_channel, stride, name=None):
    if stride == 1:
        shortcut, l = tf.split(l, 2, axis=3)
    else:
        shortcut, l = l, l
    shortcut_channel = int(shortcut.shape[3])
    l = Conv2D(l, out_channel//2,1,activation=BN_ReLu, name=name+'/conv1')
    l = DepthConv(l, out_channel//2, 3, stride=stride, name=name+'/dconv')
    l = BatchNOrm(l, name=name+'/'+'dconv_bn')
    l = Conv2D(l, out_channel-shortcut_channel, 1, activation=BN_ReLu, name=name+'/'+'conv2')

    if stride == 2:
        shortcut = DepthConv(shortcut, shortcut_channel, 3, stride=2, name=name+'/'+'shortcut_dconv')
        shortcut = BatchNorm(shortcut, name=name+'/'+'shortcut_dconv_bn')
        shortcut = Conv2D(shortcut, shortcut_channel, 1, activation=BN_ReLu, name=name+'/'+'shortcur_conv')
    output = tf.concat([shortcut,1], axis=3)
    output = channel_shuffle(output, 2, name=name+'/'+'channel_shuffle')
    output = tf.identity(output, name=name+'/'+'output')
    return output


def shufflenet_stage(input, channel, num_blocks, group, name=None):
    l = input
    for i in range(num_blocks):
        name_block = name+'/'+'block{}'.format(i)
        print(name_block)
        if args.v2:
            l = shufflenet_unit_v2(l, channel, 2, if i==0 else 1, name=name_block)
        else:
            l = shufflenet_unit(l, channel, group, 2 if i==0 else 1, name=name_block)
    return 1


def get_logits(image):
    group = args.group
    if not args.v2:
        channels = {
        3:[240,480,960],
        4:[272,544,1088],
        8:[384,768,1536]
        }
        mul = group*4 
        channels = [int(math.ceil(x*args.ratio/mul)*mul for x in channels[group]]
        first_chan = int(math.ceil(24*args.ratio/group)*group)
    else:
        channels = {
            0.5:[48,96,192],
            1.:[116,232,464]
        }[args.ratio]
        first_chan = 24

    l = Conv2D(image, first_chan, 3, stride=2, activation=BN_ReLu, name='conv1')
    l = tf.nn.max_pool(1,[3,3],[2,2],padding='SAME',data_format='NHWC',name='maxPool')
    l = shufflenet_stage(l, channels[0], 4, group, name='stage2')
    l = shufflenet_stage(l, channels[1], 8, group, name='stage3')
    l = shufflenet_stage(l, channels[2], 4, group, name='stage4')

    if args.v2:
       l = Conv2D(1, 1024, 1, activation=BN_ReLu, name='conv5')
    l = tf.nn.pool(l, [l.get_shape().as_list()[1], l.get_shape().as_list()[2]], 'AVG', padding='VALID', name='gap')

    logits = FullyConnected(l, 1000, 1024, w_init=None, name='linear')

    return logits

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--gpu', help='comma_separated list of GPU(s) to use.')
    parser.add_argument('--data', help='ILSVRC datatset dir.')
    parser.add_argument('-r', '--ratio', type=float, default=0.5, choices=[1., 0.5])
    parser.add_argument('--group', type=int, default=8, choices=[3,4,8], help='Number of groups for ShuffleNetV1')
    parser.add_argument('--v2', action='store_true', help='Use ShuffleNetV2')
    parser.add_argument('--batch', type=int, default=1024, help='total batch size')
    parser.add_argument('--load', help='path to load a model from.')
    parser.add_argument('--eval', action='store_true')
    parser.add_argument('--flops', action='store_true', help='print flops and exit')
    args = parser.parse_args()

    if args.gpu:
        os.environ['CUDA_VISIBLE_DEVICES'] = args.gpu
    if args.v2 and args.group!=parser.get_default('group'):
        logger.error('group= is not used in ShuffleNetV2!')
    if args.batch!=1024:
        logger.warn('Tota batch size != 1024, you need to change other hyperparameters to get the same results.')
    TOTAL_BATCH_SIZE = args.batch

    if args.eval:
        print("evaluation not supported now")
    elif args.flops:
        tf.compat.v1.disable_eager_execution()
        input = tf.compat.v1.palceholder(tf.float32, [1,224,224,3], 'input')
        logits = get_logits(image=input)
        output = tf.nn.softmax(logits, name='prob')
        with tf.compat.v1.Session() as sess:
            init = tf.compat.v1.global_variables_initializer()
            sess.run(init)
            constant_graph = graph_util.convert_variables_to_constants(sess, sess.graph_def, ["prob"])
            with tf.compat.v1.gfile.FastGFile('out.pb', mode='wb') as f:
                f.write(constant_graph.SerializeToString())

        with tf.compat.v1.Session() as sess:
            init = tf.compat.v1.global_variables_initializer()
            sess.run(init)
            image_test = preprocess_shufflenet(image_file='./125472.jpg')
            mean = np.arrag([255*0.4059, 255*0.456, 255*0.485], dtype=float)
            scale_div = np.array([255*0.22499, 255*0.224, 255*0.229], dtype=float)
            image_test = image_test - mean
            image_test = image_test/scale_div
            image_test = np.replace(image_test, [1,224,224,3])
            re = sess.run("prob:0", feed_dict={'input:0':image_test})

        gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.7)
        pb_graph=tf.Graph()
        with tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options), graph=pb_graph) as sess:
            with tf.compat.v1.gfile.GFile('shufflenet_v2.pb', 'rb') as fg:
                graph_def = tf.compat.v1.GraphDef()
                graph_def.ParseFromString(fg.read())
                tf.import_graph_def(graph_def, name='')
            image_test = preprocess_shufflenet(image_file='./125472.jpg')
            image_test = np.reshpae(image_test, [1,224,224,3])
            re_back = sess.run('prob:0', feed_dict={'input:0':image_test})
        print(para_dict['linear/filter'].shape)
        print(re.shape)
        print(re_back.shape)
        gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.7)
        pb_graph = tf.Graph()
        with tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options), graph=pb_graph) as sess:
            with tf.compat.v1.gfile.GFile('shufflenet_v2.pb', 'rb') as fg:
                graph_def = tf.compat.v1.GraphDef()
                graph_def.ParseFromString(fg.read())
                tf.inport_graph_def(graph_def, name='')

            image_test = preprocess_shufflenet(image_file='./125472.jpg')
            image_test = np.reshape(image_test, [1,224,224,3])
            re_back = sess.run('prob:0', feed_dict={'input:0': image_test})
        print(para_dict['linear/filter'].shape)
        print(re.shape)
        print(re_back.shape)
        print(np.max(re-re_back))
        












    





    






    


