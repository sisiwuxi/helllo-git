import tensorflow as tf
from tensorflow.python.framework import graph_util

def pb2pb(input_model, output_model, verbose=Flase):
    input_graph = tf.Graph()
    gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.4)
    input_sess = tf.Session(graph=input_graph, config=tf.ConfigProto(gpu_options=gpu_options))
    #Graph of the quantized model
    q_graph = tf.Graph()
    q_sess = tf.Session(graph=q_graph, config=tf.ConfigProto(gpu_options=gpu_options))

    with input_graph.as_default():
        with tf.gfile.GFile(input_model, 'rb') as fid:
            graph_def = tf.GraphDef()
            graph_def.ParseFromString(fid.read())
            tf.import_graph_def(graph_def, name='')

    f_ops = input_graph.get_operations()
    f_op_types = []
    #quantizable_op_types = []
    for op in f_ops:
        print(op.type)
        if op.type not in f_op_types:
            f_op_types.append(op.type)

    with input_graph.as_default():
        for i, op_r in enumerate(f_ops):
            if op_r.type == 'Conv2D':
                weights0 = input_graph.get_tensor_by_name(op_r.inputs[1].name)
                weights0 = input_sess.run(weights0)
                break
    with q_graph.as_default():
        output_model_input = tf.placeholder(tf.float32, shape=[1,224,224,3], name='input')
        output_model_weights0 = tf.convert_to_tensor(weights0, name='conv1/Conv2D/ReadVariableOp')

        flag = 0
        for i, op_r in enumerate(f_ops):
            #Conv2d layers are quantized
            if flag == 0:
                if op_r.type == 'Conv2D':
                    input_q = output_model_input
                    weight_q = output_model_weights0
                    strides = op_r.get_attr('strides')
                    padding = op_r.get_attr('padding').decde('utf-8')
                    outputs = tf.nn.conv2d(input_q, weight_q, strides, padding=padding, name='conv1/Conv2D')
                    flag = 1
            else:
                variables_in = [input_tensor for input_tensor in op_r.inputs]
                variables_control = [control_tensor for control_tensor in op_r.control_inputs]
                variables_out = [output_tensor for output_tensor in op_r.outputs]
                variables = variables_in + variables_control + variables_out
                new_op = tf.contrib.copy_graph.copy_op_to_graph(op_r, q_graph, variables)


      constant_graph = graph_util.convert_variables_to_constants(q_sess, q_sess.graph_def, ['prob'])

      with tf.gfile.GFile(output_model, mode='wb') as f:
          f.write(constant_graph.SerializeToString())

def hwcn2nhwc():
    shufflenet_v2_hwcn = './shufflenet_v2_hwcn.pb'
    shufflenet_v2_nhwc = './shufflenet_v2_nhwc.pb'
    g_hwcn = tf.Graph()
    g_nhwc = tf.Graph()
    gpu_options = tf.GPUOptions(per_process_gpu_memory_fraction=0.4)
    s_hwcn = tf.Session(graph=g_hwcn, config=tf.ConfigProto(gpu_options=gpu_options))    
    s_nhwc = tf.Session(graph=g_nhwc, config=tf.ConfigProto(gpu_options=gpu_options))

    with g_hwcn.as_default():
        with tf.gfile.GFile(shufflenet_v2_hwcn, 'rb') as fid:
            graph_def = tf.GraphDef()
            graph_def.ParseFromString(fid.read())
            tf.import_graph_def(graph_def, name='')
    hwcn_ops = g_hwcn.get_operations()
    hwcn_types = []
    #quantizable_op_types = []
    for op in hwcn_ops:
        print(op.type)
        if op.type not in hwcn_types:
            hwcn_types.append(op.type)

    with g_nhwc.as_default():
        for i ,hwcn_op in enumerate(hwcn_ops):
            if hwcn_op.type == 'Conv2D':
                input_q = g_nhwc.get_tensor_name(hwcn_op.inputs[0].name)
                weight_q = g_nhwc.get_tensor_name(hwcn_op.inputs[1].name)
                weight_q = tf.transpose(weight_q, perm=[3,0,1,2], name='temp_transpose')
                strides = hwcn_op.get_attr('strides')
                padding = hwcn_op.get_attr('padding').decode('utf-8')
                # strides = [1,2,2,1]
                stride = strides[1]

                outputs = tf.nn.conv2d(input_q,weight_q,strides,padding=padding)
                outputs0 = tf.multiply(tf.reduce_sum(weight_q, [0,1,2], keepdims=True), ib_zero)
                output_name = hwcn_op.outputs[0].name
                output_name = output_name.split(':')[0]
                outputs_q = tf.multiply(tf.subtract(outputs, outputs0), scales, name=output_name)
            elif hwcn_op.type == 'DepthwiseConv2dNative':
                pass
            else:
                variables_in = [input_tensor for input_tensor in hwcn_op.inputs]
                variables_control = [control_tensor for control_tensor in hwcn_op.control_inputs]
                variables_out = [output_tensor for outputs_tensor in hwcn_op.outputs]
                variables = variables_in + variables_control + variables_out
                tf.contrib.copy_graph.copy_op_to_graph(hwcn_op, g_hwcn, variables)
    pass

if __name__ == '__main__':
    # pb2pb(shufflenet_v2, shufflenet_v2s)
    hwcn2nhwc()