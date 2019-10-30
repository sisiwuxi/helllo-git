python caffe2pb.py --model=./resnet-50.prototxt --weights=./resnet-50.caffemodel --out_dir=./

python caffe2pb.py --model=./resnet-50.prototxt --weights=./resnet-50.caffemodel --out_dir=./ --mean=103.94,116.78,123.68 --calibrate_dir=./ImageNet_train_1000/ --quantize=True --batch=64

python caffe2pb.py --model=./yolov3.prototxt --weights=./yolov3.caffemodel --out_dir=./ \
--channel_swap=True --input_scale=0.00390625 \
--calibrate_dir=./ImageNet_train_1000/ --quantize=True

gdb --args ./calibration \
-m ./yolov3_float.tflite \
-i ./1.bmp \
-l ./mscoco_label.txt \
-s min_max

gdb --args ./calibration \
-m ./yolov3_float.tflite \
-i ./1.bmp \
-l ./mscoco_label.txt \
-s qab

gdb --args ./calibration \
-m ./yolov3_float.tflite \
-o ./yolov3_fix.tflite \
-c ./Config.txt \
-d ./tensor_min_max.txt \
-q ./tensor_qab.txt \
-e ./lut.txt \
-p convert_fixed




