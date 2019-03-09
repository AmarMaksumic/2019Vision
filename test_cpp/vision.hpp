#ifndef VISION_HPP
#define VISION_HPP

//Include OpenCV
#include <opencv2/opencv.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef std::vector<cv::Point> contour_type;

const int CUBE_LENGTH = 13;
const double SIDE_RATIO = 1.2;
const int CAMERA_FOCAL_LENGTH = 1200;

/*
 *  Color Schemes
 */
//
const cv::Scalar MY_RED (0, 0, 255);
const cv::Scalar MY_BLUE (255, 0, 0);
const cv::Scalar MY_GREEN (0, 255, 0);
const cv::Scalar MY_PURPLE (255, 0, 255);
const cv::Scalar MY_WHITE (255,255,255);

/**
 *  RGB Threshold Levels
 */

const int RGB_RED[2] =   {0,255};
const int RGB_GREEN[2] = {142,255};
const int RGB_BLUE[2] = {0,110};

/**
 *  HSV Threshold Levels
 *
 */
const int HSV_HUE[2] = {0,38};
const int HSV_SAT[2] = {78,255};
const int HSV_VAL[2] = {64,255};

/**
 *  HSL Threshold Levels
 */

const int HSL_HUE[2] =   {150,255};
const int HSL_SAT[2] = {93,255};
const int HSL_LUM[2] =  {0,111};


/*
 *  Median Blur
https://docs.opencv.org/2.4/doc/tutorials/imgproc/gausian_median_blur_bilateral_filter/gausian_median_blur_bilateral_filter.html
*/

const int MEDIAN_BLUR_LEVEL = 5;  // must be ODD!



void calculate(const cv::Mat &bgr, cv::Mat &processedImage);


#endif //VISION_HPP

