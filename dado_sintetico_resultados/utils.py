import cv2
import numpy as np

def get_threshold_length(opening_transform):
    # Filter opening transform to remove background
    foreground = opening_transform[opening_transform > 1]
    # Get position peaks from histogram
    bins = int(foreground.max())
    Y,X = np.histogram(foreground, bins)
    peak_indices = np.sort(np.argpartition(Y, -2)[-2:])
    
    #Get minimum value between peaks
    threshold_index = Y[peak_indices[0]:peak_indices[1]].argmin() + Y[:peak_indices[0]].size
    return X[threshold_index]

def get_segmentation_result(op_transform_filename, t= -1):
    opening_transform = cv2.imread(op_transform_filename, cv2.IMREAD_UNCHANGED)

    #Get threshold length value
    if t < 0:
        L = get_threshold_length(opening_transform)
    else:
        L = t
        
    #Get fractures by thresholding val > L
    fractures = (opening_transform > L)*255
    #Get fractures by thresholding 1 < val < L
    vugs = ((opening_transform < L) & (opening_transform > 1))*255
    
    return opening_transform, fractures, vugs, L