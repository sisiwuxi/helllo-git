import numpy as np
from PIL import Image, ImageDraw, ImageFont
import math

def do_nms_object_yolo(boxes, iou_thresh=0.45):
    if boxes.shape[0] == 0:
        return []
    elif boxes.shape[1] == 0:
        return []

    objectness = boxes[:,4]
    sorted_indices = np.argsort(objectness)[::-1]
    sorted_boxes = boxes[sorted_indices]
    for i in range(len(sorted_boxes)):
        if sorted_boxes[i,4] == 0:
            continue
        if i<len(sorted_boxes) - 1:
            ious = IoU(sorted_boxes[i, 0:4], sorted_boxes[(i+1)：， 0：4])
            for j in range(i+1, sourted_boxes.shape[0]):
                if ious[j-(i+1)] > iou_thresh:
                    sorted_boxes[j, 4:] = 0
    out_boxes = []
    for i in range(sorted_boxes.shape[0]):
        if sorted_boxes[i,4] > 0:
            c=np.argmax(sorted_boxes[i,5:])
            c_prob=np.max(sorted_boxes[i,5:])
            box = sorted_boxes[i, 0:4]
            out_boxes.append([box[0], box[1], box[2], box[3], c_prob, c])
    return out_boxes


def do_nms_sort_yolov(boxes, num_classes=80, iou_thresh=0.45):
    if boxes.shape[0] == 0:
        return []
    elif boxes.shape[1] == 0:
        return []
    for c in range(num_classes):
        c_probs = boxes[:, 5+c]
        c_sorted_indices = np.argsort(c_probs)[-1::-1]
        for i, sorted_index in enumerate(c_sorted_indices):
            c_prob = boxes[sorted_index, 5+c]
            if c_prob == 0:
                continue
            if i < boxes.shape[0] - 1:
                ious = IoU(boxes[sorted_index, 0:4], boxes[c_sorted_indices[i+1:], 0:4])
                for j in range(i+1, boxes.shape[0]):
                    if ious[j - (i+1)] > iou_thresh:
                        boxes[c_sorted_indices[j], 5+c] = 0
    out_boxes = []
    for b  in boxes:
        for c in range(num_classes):
            if b[5+c] > 0:
                out_boxes.append(b[0], b[1], b[2], b[3], b[5+c], c)
    return out_boxes


def do_nms_fast(boxes, iou_thresh = 0.45):
    if boxes.shape[0] == 0:
        return []
    if boxes.shape[1] == 0:
        return []
    #max_classes = np.argsort(boxes[:, 5:], axis=1)[:,-1]
    #max_probs = [boxes[, 5+max_classes[i]] for i in range(boxes.shape[0])]
    thinned_boxes = np.zeros((boxes.shape[0], 6), dtype=np.float32)
    for i in range(boxes.shape[0]):
        max_classes = np.argmax(boxes[i, 5:])
        max_probs = boxes[i, 5+max_classes]
        thinned_boxes[i, 0:4] = boxes[i, 0:4]
        thinned_boxes[i, 4] = max_probs
        thinned_boxes[i, 5] = max_classes
    sorted_indices = np.argsort(thinned_boxes[:, 4])[::-1]
    thinned_boxes = thinned_boxes[sorted_indices]
    for i in range (thinned_boxes.shape[0]):
        if thinned_boxes[i, 4] == 0:
            continue 
        if i < thinned_boxes.shape[0] -1:
            ious = IoU(thinned_boxes [i , 0:4], thinned_boxes[(i+1):, 0:4])
            for j in range(i+1, thinned_boxes.shape[0]):
                if ious[j - (i+1)] > iou_thresh :
                    thinned_boxes[j , 4] = 0
                    #if int(thinned_boxes[j,5]) == int (thinned_boxes[i,5]):
                    #    thinned_boxes[j , 4] = 0
    out_boxes = []
    for i in range(thinned_boxes.shape[0]):
        if thinned_boxes[i, 4] > 0:
            out_boxes.append(thinned_boxes[i, :])
    return out_boxes


def do_nms_fda(boxes, iou_thresh=0.5):
    if boxes.shape[0] == 0:
        return []
    elif boxes.shape[1] == 0:
        return []
    sorted_indices = np.argsort(boxes[:,4])[::-1]
    boxes = boxes[sorted_indices]
    for i in range(boxes.shape[0]):
        if boxes[i, 4] == 0:
            continue
        if i<boxes.shape[0] - 1:
            ious = IoU(boxes[i, 0:4], boxes[(i+1):, 0:4])
            for j in range(i+1, boxes.shape[0]):
                boxes[j,4] = 0
    out_boxes = []
    for i in range(boxes.shape[0]):
        if boxes[i,4] > 0:
            out_boxes.append(boxes[i,:])
    out_boxes = np.asarray(out_boxes)
    return out_boxes


def get_network_boxes(predictions, cfg, img_size):
    if cfg.input_size == 608:
        num_images = predictions.shape[0]
        bbox = [[] for _ in range(num_images)]
        for ni in range(num_images):
            width, height = img_size
            num_boxes = predictions.shape[1]
            for nb in range(num_boxes):
                prediction = predictions[ni][nb]
                i_col_row = int(nb/cfg.num_scales)
                i_ch = nb - i_col_row * cfg.num_scales
                i_col = int(i_col_row / cfg.last_featuremap_size)
                i_row = i_col_row - i_col * cfg.last_featuremap_size
                
                bp = logistic(prediction[4])
                # if a box has too small a chance of having objects in it, it is ignored.
                if bp < cfg.conf_thresh:
                    continue
                # remaining boxes are kept.
                classes = [logistic(prediction[c+5]) for c in range(cfg.num_classes)]
                
                bx = (logistic(prediction[0]) + i_col) / cfg.last_featuremap_size
                by = (logistic(prediction[1]) + i_row) / cfg.last_featuremap_size
                bw = cfg.anchors[i_ch*2] * math.exp(predictions[2]) / cfg.last_featuremap_size
                bh = cfg.anchors[i_ch*2+1] * math.exp(predictions[3]) / cfg.last_featuremap_size
                # map to original image
                x1 = (bx - bw/2)*width
                y1 = (by - bh/2)*height
                x2 = (bx + bw/2)*width
                y2 = (by + bh/2)*height                
                classes = bp*np.array(classes)
                classes[class<cfg.conf_thresh] = 0
                b = np.zeros(5+cfg.num_classes, dtype=np.float32)
                b[0:4] = [x1,y1,x2,y2]
                b[4] = bp
                b[5:] = classes
                bbox[ni].append(b)
            bbox[ni] = np.array(bbox[ni])
            bbox[ni] = do_nms_fast(bbox[ni], iou_thresh=cfg.nms_thresh)
            
            #restrict the bounding boxes for images
            nb = len(bbox[ni])
            for i in range(nb):
                if bbox[ni][i][0] < 0:
                    bbox[ni][i][0] = 0
                if bbox[ni][i][1] < 0:
                    bbox[ni][i][1] = 0
                if bbox[ni][i][2] > width - 1:
                    bbox[ni][i][2] = width - 1
                if bbox[ni][i][3] > height - 1:
                    bbox[ni][i][3] = height - 1
        return bbox

    anchors = cfg[0]
    mask = cfg[1]
    nclass = cfg[2]
    thresh = cfg[3]
    nms_thresh = cfg[4]
    input_w = cfg[5]
    input_h = cfg[6]
    nbox = [len(m) for m in mask]
    
    N_sample = len(img_size)
    
    bbox = [[] for i in range(N_sample)]
    for n in range(N_sample):
        width, height = img_size[n]
        # Gather all boxes with prob highter than threshold
        for l in range(len(nbox)):
            prediction = predictions[l][n]
            # each scale has a different shape and thus has a different number of box predictions.
            h, w, _ = prediction.shape
            for k in range(nbox[l]):
                # for each scale, predict three boxes at all feature points.
                b_idx = k*(nclass + 5)
                for i in range(h):
                    for j in range(w):
                        c_idx = b_idx + 4
                        bp = logistic(predictions[i][j][c_idx])
                        # if a box has too small a chance of having objects in it, it is ignored.
                        if bp < thresh:
                            continue
                        # remaining boxes are kept.
                        if nclass > 1:
                            classes = [logistic(prediction[i][j][x+c_idx+1]) for x in range(nclass)]
                        else:
                            classes = [1]
                            
                        bx = (logistic(prediction[i][j][b_idx]) + j) / w
                        by = (logistic(prediction[i][j][b_idx+1]) + i) / h
                        bw = anchors[mask[l][k]][0] * math.exp(prediction[i][j][b_idx+2]) / input_w
                        bh = anchors[mask[l][k]][1] * math.exp(prediction[i][j][b_idx+3]) / input_h
                        # map to original image
                        x1 = (bx - bw/2)*width
                        y1 = (by - bh/2)*height
                        x2 = (bx + bw/2)*width
                        y2 = (by + bh/2)*height
                        
                        classes = bp*np.array(classes)
                        classes[class<thresh] = 0
                        b = np.zeros(5+nclass, dtype=np.float32)
                        b[0:4] = [x1,y1,x2,y2]
                        b[4] = bp
                        b[5:] = classes
                        bbox[n].append(b)
        bbox[n] = np.array(bbox[n])
        bbox[n] = do_nms_fast(bbox[n], iou_thresh=nms_thresh)
    
        # restrict the bounding boxes for images
        nb = len(bbox[ni])
        for i in range(nb):
            if bbox[ni][i][0] < 0:
                bbox[ni][i][0] = 0
            if bbox[ni][i][1] < 0:
                bbox[ni][i][1] = 0
            if bbox[ni][i][2] > width - 1:
                bbox[ni][i][2] = width - 1
            if bbox[ni][i][3] > height - 1:
                bbox[ni][i][3] = height - 1
    return bbox
    
    
def get_fda_boxes(boxes, scores, cfg):
    num_boxes = boxes.shape[1]
    probs = 1./(1.+np.exp(scores[:,0] - scores[:,1]))
    width, height = 1280, 720
    # Gather all boxes with prob higher than threshold
    bboxes = []
    for n in range(num_boxes):
        if probs[n] < 0.05:
            continue
        box = boxes[:,n]
        x = cfg['priors'][0][n] + box[0]*cfg['variances'][0] * cfg['priors'][2][n]
        y = cfg['priors'][1][n] + box[1]*cfg['variances'][0] * cfg['priors'][3][n]
        w = cfg['priors'][2][n] * np.exp(box[2]) * cfg['variances'][1]
        h = cfg['priors'][3][n] * np.exp(box[3]) * cfg['variances'][1]
        xmin = x-w/2.
        ymin = y-h/2.
        xmax = xmin + w
        ymax = ymin + h
        bbox = [xmin * width, ymin * height, xmax * width, ymax * height, probs[n]]
        bboxes.append(bbox)
    bboxes = np.array(bboxes)
    bboxes = do_nms_fda(bboxes, iou_thresh=0.3)
    for i in range(len(bboxes)):
        if bboxes[i, 0] < 0:
            bboxes[i, 0] = 0
        if bboxes[i, 1] < 0:
            bboxes[i, 1] = 0
        if bboxes[i, 2] > width - 1:
            bboxes[i, 2] = width - 1
        if bboxes[i, 3] > height - 1:
            bboxes[i, 3] = height - 1
    bboxes[:, 2] = bboxes[:, 2] - bboxes[:, 0]
    bboxes[:, 3] = bboxes[:, 3] - bboxes[:, 1]
    return np.array(bboxes)
        
def get_yolov2_boxes(predictions, cfg , image_size):
    anchors = [[1.3221, 1.73145], [3.19275, 4.00944], [5.05587, 8.09892], [9.47112, 4.84053], [11.2364, 10.0071]]
    thresh = 0.4
    num_classes = 20
    num_images = predictions.shape[0]
 
    bboxes = [[] for _ in range (num_images)]
    for i in range(num_images):
        bbox = []
        height , width = img_size[i]
        for row in range(predictions.shape[2]):
            for col in range(predictions.shape[3]):
                for k in range(5):
                    objectness = logistic (predictions[i , k*25+4, row , col])
                    if objectness < thresh :
                        continue 
                    classes = [logistic(pr, ns[i , k*25+5+c , row , col]) for c in range (num_classes)]
                    bx = logistic (predictions[i , k*25 , row , col]) + col )/predictions .shape [3]
                    by = logistic (predictions[i , k*25 +1 , row , col]) + col )/predictions .shape [2]
                    bw = anchors [k][0] * np.exp(predictions[i , k*25 + 2 , row , col]) / 13
                    bh = anchors [k][1] * np.exp(predictions[i , k*25 + 3 , row , col]) / 13
                     # map to original image
                    x1 = (bx - bw / 2) * width
                    y1 = (by - bh / 2) * height
                    x2 = (bx + bw / 2) * width
                    y2 = (by + bh / 2) * height
                    classes = objectness * np.array (classes)
                    classes [classes < thresh] = 0
                    b = np.zeros(5 + num_classes , dtype = np.float32)
                    b[0:4] = [x1, y1, x2, y2]
                    b[4] = objectness
                    b[5:] = classes
                    bbox.append(b)
        bbox = np.array(bbox)
        bbox = do_nms_fast(bbox , iou_thresh = .45)
        # restrict the bounding boxes for images
        nb = len(bbox)
        for j in range(nb):
            if bbox[j][0] < 0:
                bbox[j][0] = 0
            if bbox[j][1] < 0:
                bbox[j][1] = 0
            if bbox[j][2] > width - 1:
                bbox[j][2] = width - 1
            if bbox[j][3] > height - 1:
                bbox[j][3] = height - 1
        bboxes[i] = bbox
    return bboxes


def get_bbox(predictions, cfg, img_size):
    anchors = cfg[0]
    mask = cfg[1]
    nclass = cfg[2]
    thresh = cfg[3]
    nms_thresh = cfg[4]
    input_w = cfg[5]
    input_h = cfg[6]
    nbox = [len(m) for m in mask]
    
    N_sample = len(img_size)
    
    bbox = [[] for i in range(N_sample)]
    for n in range(N_sample):
        width, height = img_size[n]
        # gather all boxes with prob higher than threshold
        for l in range(len(nbox)):
            predictions = predictions[l][n]
            # each scale has a different shape and thus has a different number of box predictions.
            h, w, _ = prediction.shape
            for k in range(nbox[l]):
                for each scale, predict three boxes at all feature points.
                b_idx = k*(nclass+5)
                for i in range(h):
                    for j in range(w):
                        c_idx = b_idx + 4
                        bp = logistic(prediction[i][j][c_idx])
                        # if a box has too small a chance of having objects in it, it is ignored.
                        if bp < thresh:
                            continue
                        # remaining boxes are kept
                        if nclass > 1:
                            classes = [logistic(prediction[i][j][x+c_idx+1]) for x in range(nclass)]
                        else:
                            classes = [1]
                        
                        # bx,by,bw,bh ~ [0,1]
                        bx = (logistic(prediction[i][j][b_idx]) + j) / w
                        by = (logistic(prediction[i][j][b_idx+1]) + i) / h
                        bw = anchors[mask[l][k]][0] * math.exp(prediction[i][j][b_idx+2]) / input_w
                        bh = anchors[mask[l][k]][1] * math.exp(prediction[i][j][b_idx+3]) / input_h
                        # map to original image
                        x1 = (bx - bw/2)*width
                        y1 = (by - bh/2)*height
                        x2 = (bx + bw/2)*width
                        y2 = (by + bh/2)*height
                        
                        classes = bp*np.array(classes)
                        classes[class<thresh] = 0
                        b = np.zeros(4+nclass, dtype=np.float32)
                        b[0:4] = [x1,y1,x2,y2]
                        b[4:] = classes
                        bbox[n].append(b)
        bbox[n] = np.array(bbox[n])
        bbox[n] = NMS_darknet(bbox[n], nms_thresh)
    
        # restrict the bounding boxes for images
        nb = len(bbox[n])
        for i in range(nb):
            if bbox[n][i][0] < 0:
                bbox[n][i][0] = 0
            if bbox[n][i][1] < 0:
                bbox[n][i][1] = 0
            if bbox[n][i][2] > width - 1:
                bbox[n][i][2] = width - 1
            if bbox[n][i][3] > height - 1:
                bbox[n][i][3] = height - 1
    return bbox
 
def logistic(x):
    return 1./(1. + math.exp(-x))

def IoU(box1, box2):
    wh = np.maximum(np.minimum(box1[2:], box[:,2:]) - np.maximum(box1[0:2], box2[:, 0:2]) + 1, 0)
    intersect = wh[:,0] * wh[:, 1]
    area1 = (box1[3] - box1[1] +1) * (box1[2] - box1[0] + 1)
    area2 = (box2[:, 3] - box2[:, 1] + 1) * (box2[:,2] - box2[:,0] + 1)
    union = area1 + area2 - intersect
    if union.any() == 0:
        print(union)
    return intersect / union


def NMS_darknet(bbox, thresh = 0.5):
    N = len(bbox)
    if N == 0:
        return []
    out_boxes = []
    
    nclass = bbox.shape[1] - 4
    for i in range(N):
        ious = IoU(bbox[i, 0:4], bbox[:, 0:4])
        idx = np.where(ious >= thresh)[0]
        max_prob = np.amax(bbox[idx, 4:], axis=0)
        ismax = ((bbox[i, 4:] == max_prob) * (max_prob > 0))
        for c in range(nclass):
            if ismax[c]:
                b = bbox[i]
                out_boxes.append([b[0], b[1], b[2], b[3], b[4+c], c])
                bbox[idx, 4+c] = 0
    return out_boxes


def draw(bbox, img_name, nclass, label_file=None, out_dir='./'):
    im = Image.open(img_name)
    np.random.seed(nclass)
    bbox_color = []
    for i in range(nclass):
        c = np.random.randint(256, size=[3])
        bbox_color.append((c[0], c[1], c[2], 255))        
    if (label_file is not None):
        names = []
        with open(label_file) as f:
            for i in range(nclass):
                line = f.readline().strip("\r\n")
                names.append(line)
    else:
        name = None
    draw = ImageDraw.Draw(im)
    drawboxes(bbox, draw, names, bbox_color=bbox_color, text_bgcolor=bbox_color)
    
    img_name = img_name.split('/')[-1]
    img_name = img_name.split('.')[0]
    im.save(out_dir + '/' + img_name + '.png', "PNG")

def drawboxes(bbox, draw, names=None, bbox_color=[(255,255,0,255)], text_color=(0,0,0,255), text_bgcolor=[(255,255,0,255)]):
    font = ImageFont.load_default()
    n_color = len(bbox_color)
    for b in bbox:
        color_idex = 0 if n_color == 1 else b[5]
        draw.rectangle(tuple(b[0:4]), outline=bbox_color[color_idx])
        if names is None:
            text_bg_rbxy = [b[0]+24, b[1]+14]
            draw_str = "%.0f%%" % (b[4]*100)
        else:
            text_bg_rbxy = [b[0]+(len(names[b[5]])+4)*6+4, b[1]+14]
            draw_str = "%s %.0f%%" % (names[b[5]], b[4]*100)            
        # fine-tune the label position
        text_xy = b[0:2]
        text_xy[0] += 2
        text_xy[1] += 2
        # draw the label background
        bg_xy = b[0:2]
        bg_xy.extend(text_bg_rbxy)
        draw.rectangle(tuple(bg_xy), text_bgcolor[color_idx])
        draw.text(tuple(text_xy), draw_str, text_color, font)
        
        
