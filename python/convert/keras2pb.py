#"""
# -*- coding: utf-8 -*-
from tensorflow import keras
#from keras.models import load_model
import collections
from collections import defaultdict
import numpy as np
import sys
import tensorflow as tf
import os
import os.path as osp
#from keras import backend as K

def freeze_session(session, keep_var_names=None, output_names=None, clear_devices=True):
    from tensorflow.python.framework.graph_util import convert_variables_to_constants
    graph = session.graph
    with graph.as_default():
        freeze_var_names = list(set(v.op.name for v in tf.global_variables()).difference(keep_var_names or []))
        output_names = output_names or []
        output_names += [v.op.name for v in tf.global_variables()]
        input_graph_def = graph.as_graph_def()
        if clear_devices:
            for node in input_graph_def.node:
                node.device = ""
    frozen_graph = convert_variables_to_constants(session, input_graph_def, output_names, freeze_var_names)
    return frozen_graph


tf.keras.backend.set_learning_phase(0)
#net_model = load_model('./keras.h5')
net_model = tf.keras.models.load_model('./keras.h5')

print('input is :', net_model.input.name)
print ('output is:', net_model.output.name)

sess = tf.keras.backend.get_session()

frozen_graph = freeze_session(tf.keras.backend.get_session(), output_names=[net_model.output.op.name])
from tensorflow.python.framework import graph_io
graph_io.write_graph(frozen_graph, './', 'tf.pb', as_text=True)
print('saved the constant graph (ready for inference) at: ', './tf.pb')
#"""
