gdb --args ./bazel-bin/tensorflow/lite/toco/toco \
--input_file=./yolov3.pb \
--output_file=./yolov3.tflite \
--input_format=TENSORFLOW_GRAPHDEF \
--output_format=TFLITE \
--input_arrays=input \
--output_arrays=TFLite_YoloV3_Detection_PostProcess,TFLite_YoloV3_Detection_PostProcess:1,TFLite_YoloV3_Detection_PostProcess:2,TFLite_YoloV3_Detection_PostProcess:3 \
--input_shapes=1,608,608,3 \
--inference_types=FLOAT \
--std_values=127.5 \
--mean_values=127.5 \
--allow_custom_ops


gdb --args ./bazel-bin/tensorflow/lite/toco/toco \
--input_file=./mobilenet.pb \
--output_file=./mobilenet.tflite \
--input_format=TENSORFLOW_GRAPHDEF \
--output_format=TFLITE \
--input_arrays=input \
--output_arrays=Mobilenetv1/Predictions/Reshape_1 \
--input_shapes=1,224,224,3 \
--inference_types=FLOAT


