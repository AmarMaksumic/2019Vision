import numpy as np
import cv2


def split_hsv(img):

    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    hue ,saturation ,value = cv2.split(hsv)
    cv2.imshow('Hue Channel',hue)
    cv2.imshow('Saturation Channel',saturation)
    cv2.imshow('Value Channel', value)


def find_corners(img):

    corners = cv2.goodFeaturesToTrack(img,6,0.06,25)
    corners = np.float32(corners)
    return corners

    # for item in corners:
    #     x,y = item[0]
    #     cv2.circle(img,(x,y),10,255,-1)


def find_convex_hull(img):
    hull = cv2.convexHull(img)
    contour = hull
    print hull
