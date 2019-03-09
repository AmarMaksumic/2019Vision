#ifndef VISION_HPP
#define VISION_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <utility>
#include "helper.hpp"

using namespace std;

struct VisionResultsPackage {
	i64 timestamp;
	cv::Point centerPoint;
	float distance, angle;
	bool valid;
};

typedef std::vector<cv::Point> contour_type;

const int RES_X = 320, RES_Y = 25;
const int MIN_RED = 229, MAX_RED = 255;
const int MIN_GREEN = 240, MAX_GREEN = 255;
const int MIN_BLUE = 234, MAX_BLUE = 255;
const int MIN_HUE = 0, MAX_HUE = 94;
const int MIN_SAT = 0, MAX_SAT = 38;
const int MIN_VAL = 233, MAX_VAL = 255;





const double
MIN_AREA = 0.000000000000000000000000001, MAX_AREA = 1000000,
MIN_WIDTH = 0, MAX_WIDTH = 100000, //rectangle width
IN_HEIGHT = 0, MAX_HEIGHT = 100000, //rectangle height
MIN_RECT_RAT = 1.5, MAX_RECT_RAT = 8, //rect height / rect width
MIN_AREA_RAT = 0.85, MAX_AREA_RAT = 100; //cvxhull area / contour area

/**
 * Processes the raw image provided in order to determine interesting values
 * from the image. The OpenCV image pipeline (thresholding, etc) is performed on
 * processedImage so this may be sent back to the driver station laptop for 
 * debugging purposes. Results package is returned in a struct.
 * @param bgr raw image to do processing on
 * @param processedImage results of OpenCV image pipeline
 * @return results of vision processing (e.g location of target, timestamp)
 */ 
VisionResultsPackage calculate(const cv::Mat &bgr, cv::Mat &processedImage);
void drawOnImage (cv::Mat &img, VisionResultsPackage info);

const int LINE_LENGTH = 11.3;
const int CAMERA_FOCAL_LENGTH = 1200;
const double SIDE_RATIO = 1.2;

/*
 *  Color Schemes
 */
const cv::Scalar MY_RED (0, 0, 255);
const cv::Scalar MY_BLUE (255, 0, 0);
const cv::Scalar MY_GREEN (0, 255, 0);
const cv::Scalar MY_PURPLE (255, 0, 255);
const cv::Scalar MY_WHITE (255,255,255);
/**
 *  RGB Threshold Levels
 */
const int RGB_RED[2] = {229, 255};
const int RGB_GREEN[2] = {240, 255};
const int RGB_BLUE[2] = {234, 255};
// // // /**

// // //  *  HSV Threshold Levels
// // //  // // //  */
const int HSV_HUE[2] = {0, 94};
const int HSV_SAT[2] = {0, 38};
const int HSV_VAL[2] = {233, 255};
// // // /**
// // //  *  HSL Threshold Levels
// // //  */
// // // const int HSL_HUE[2] =   {150,255};
// // // const int HSL_SAT[`2] = {93,255};
const int HSL_LUM[2] =  {0,255};

const int MEDIAN_BLUR_LEVEL = 5;  // must be ODD!

#endif
