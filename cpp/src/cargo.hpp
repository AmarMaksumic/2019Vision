#ifndef CARGO_HPP
#define CARGO_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include "opencv-code/cap_gstreamer.cpp"
//#include "opencv-code/cap_gstreamer.hpp"
//#include "opencv-code/cvconfig.h"
//#include "opencv-code/precomp.hpp"
#include <vector>
#include <iostream>
#include <utility>
#include "helper.hpp"

using namespace std;

struct VisionResultsPackageCargo {
	i64 timestamp;
	cv::Point centerPoint;
	float distance, angle;
	bool valid;
};

typedef std::vector<cv::Point> contour_type;

const int CAR_RES_X = 320, CAR_RES_Y = 240;
const int CAR_MIN_RED = 71, CAR_MAX_RED = 255;
const int CAR_MIN_GREEN = 0, CAR_MAX_GREEN = 184;
const int CAR_MIN_BLUE = 0, CAR_MAX_BLUE = 49;
const int CAR_MIN_HUE = 8, CAR_MAX_HUE = 23;
const int CAR_MIN_SAT = 118, CAR_MAX_SAT = 255;
const int CAR_MIN_VAL = 45, CAR_MAX_VAL = 255;

const double
CAR_MIN_AREA = 0.001, CAR_MAX_AREA = 1000000,
CAR_MIN_WIDTH = 0, CAR_MAX_WIDTH = 100000, //rectangle width
CAR_MIN_HEIGHT = 0, CAR_MAX_HEIGHT = 100000, //rectangle height
CAR_MIN_RECT_RAT = 1.5, CAR_MAX_RECT_RAT = 8, //rect height / rect width
CAR_MIN_AREA_RAT = 0.85, CAR_MAX_AREA_RAT = 100; //cvxhull area / contour area

/**
 * Processes the raw image provided in order to determine interesting values
 * from the image. The OpenCV image pipeline (thresholding, etc) is performed on
 * processedImage so this may be sent back to the driver station laptop for
 * debugging purposes. Results package is returned in a struct.
 * @param bgr raw image to do processing on
 * @param processedImage results of OpenCV image pipeline
 * @return results of vision processing (e.g location of target, timestamp)
 */
VisionResultsPackageCargo calculateCargo(const cv::Mat &bgr, cv::Mat &processedImage);


void drawOnImage (cv::Mat &img, VisionResultsPackageCargo info);

const int CAR_LINE_LENGTH = 13;
const int CAR_CAMERA_FOCAL_LENGTH = 1200;
const double CAR_SIDE_RATIO = 1.2;

/*
 *  Color Schemes
 */
const cv::Scalar CAR_MY_RED (0, 0, 255);
const cv::Scalar CAR_MY_BLUE (255, 0, 0);
const cv::Scalar CAR_MY_GREEN (0, 255, 0);
const cv::Scalar CAR_MY_PURPLE (255, 0, 255);
const cv::Scalar CAR_MY_WHITE (255,255,255);
/**
 *  RGB Threshold Levels
 */
const int CAR_RGB_RED[2] = {71, 255};
const int CAR_RGB_GREEN[2] = {0, 184};
const int CAR_RGB_BLUE[2] = {0, 49};
/**
 *  HSV Threshold Levels
 *
 */
const int CAR_HSV_HUE[2] = {8, 23};
const int CAR_HSV_SAT[2] = {188, 255};
const int CAR_HSV_VAL[2] = {45, 255};
/**
 *  HSL Threshold Levels
 */
//const int HSL_HUE[2] =   {150,255};
//const int HSL_SAT[2] = {93,255};
const int CAR_HSL_LUM[2] =  {0,111};

const int CAR_MEDIAN_BLUR_LEVEL = 5;  // must be ODD!

#endif
