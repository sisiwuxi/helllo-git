import numpy as np
import tensorflow as tf
def nms(bounding_boxes, score, threshold):
    # If no bounding boxes, return empty list
    if len(bounding_boxes) == 0:
        return [], []
    # coordinates of bounding boxes
    start_x = bounding_boxes[:, 0]
    start_y = bounding_boxes[:, 1]
    end_x = bounding_boxes[:, 2]
    end_y = bounding_boxes[:, 3]
    # Picked bounding boxes
    picked_boxes = []
    picked_score = []
    # Compute areas of bounding boxes
    areas = (end_x - start_x + 1) * (end_y - start_y + 1)
    # Sort by confidence score of bounding boxes
    order = np.argsort(score)
    # Iterate bounding boxes
    while order.size > 0:
        # The index of largest confidence score
        index = order[-1]
        # Pick the bounding box with largest confidence score
        picked_boxes.append(bounding_boxes[index])
        picked_score.append(confidence_score[index])
        # Compute ordinates of intersection-over-union(IOU)
        x1 = np.maximum(start_x[index], start_x[order[:-1]])
        x2 = np.minimum(end_x[index], end_x[order[:-1]])
        y1 = np.maximum(start_y[index], start_y[order[:-1]])
        y2 = np.minimum(end_y[index], end_y[order[:-1]])
        # Compute areas of intersection-over-union
        w = np.maximum(0.0, x2 - x1 + 1)
        h = np.maximum(0.0, y2 - y1 + 1)
        intersection = w * h
        # Compute the ratio between intersection and union
        ratio = intersection / (areas[index] + areas[order[:-1]] - intersection)
        left = np.where(ratio < threshold)
        order = order[left]
    return picked_boxes, picked_score

bounding_boxes =np.asarray([[187, 82, 337, 317],[150, 67, 305, 282],[246, 121, 368, 304]],dtype=np.float32)
confidence_score = np.asarray([0.9, 0.75, 0.8],dtype=np.float32)
# Draw parameters
font_scale = 1
thickness = 2
# IoU threshold
threshold = 0.5
picked_boxes, picked_score = nms(bounding_boxes, confidence_score, threshold)
print(picked_boxes,picked_score)
import datetime
with tf.Session() as sess:
    for i in range(3):
        old=datetime.datetime.now()
        nms = tf.image.non_max_suppression(bounding_boxes,confidence_score,max_output_size=5,iou_threshold=threshold)
        print("cost time",(datetime.datetime.now()-old).microseconds)
        # print('face detectd', len(nms.eval()))
        for index, value in enumerate(nms.eval()):
            rect = bounding_boxes[value]
            print(rect)

