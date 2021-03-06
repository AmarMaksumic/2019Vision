#include "tape.hpp"
#include <cmath>

using namespace std;

bool isContourValid(int camera_height, int camera_width, contour_type &contour)
{
  cv::Rect rect = cv::boundingRect(contour);
  double width = rect.width, height = rect.height;
  double contour_side_ratio = height / width;

  if (contour_side_ratio < 1.70 || contour_side_ratio > 1.775)
  {
    return false;
  }
  return true;
}

float getAngle(cv::Mat image, cv::Point point)
{
  float a = float(abs(image.cols / 2 - point.x));
  float b = float(image.rows - point.y);
  float radians = atan(a / b);
  float angle = radians * 180 / CV_PI;
  return angle;
}

float getDistance(int width_pixel)
{
  return float(CAMERA_FOCAL_LENGTH * LINE_LENGTH) / width_pixel;
}

bool isPairValid(int height, int width)
{
  float ratio = width / height;
  if (ratio > 1.95 && ratio < 2.10)
  {
    return true;
  }
  return false;
}

VisionResultsPackage calculate(const cv::Mat &img, cv::Mat &processedImage)
{
  ui64 timeBegan = millis_since_epoch();
  VisionResultsPackage res;

  // RGB THRESHOLD
  cv::Mat rgbMat, rgbThreshold;
  cv::cvtColor(img, rgbMat, cv::COLOR_BGR2RGB);
  cv::inRange(rgbMat,
              cv::Scalar(RGB_RED[0], RGB_GREEN[0], RGB_BLUE[0]),
              cv::Scalar(RGB_RED[1], RGB_GREEN[1], RGB_BLUE[1]),
              rgbThreshold);

  // END RGB

  // START MASK
  cv::Mat rgbThresholdOutput;
  cv::bitwise_and(img, img, rgbThresholdOutput, rgbThreshold);

  // HSV THRESHOLD
  cv::Mat hslMat, hslThreshold;
  cv::cvtColor(rgbThresholdOutput, hslMat, cv::COLOR_BGR2HSL);
  cv::inRange(hslMat,
              cv::Scalar(HSL_HUE[0], HSL_SAT[0], HSL_LIT[0]),
              cv::Scalar(HSL_HUE[1], HSL_SAT[1], HSL_LIT[1]),
              hsvThreshold);

  // END HSV

  cv::Mat medianBlurMat;
  cv::medianBlur(hslThreshold, medianBlurMat, MEDIAN_BLUR_LEVEL);

  //contour detection
  vector<contour_type> contours;
  vector<cv::Vec4i> hierarchy; //throwaway, needed for function
  cv::findContours(medianBlurMat, contours, hierarchy,
                   cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  //store the convex hulls of any valid contours
  vector<contour_type> contour_hulls;
  vector<contour_type> valid_contours;

  for (int i = 0; i < (int)contours.size(); i++)
  {
    contour_type contour = contours[i];

    if (isContourValid(processedImage.rows,
                    processedImage.cols,
                    contour))
    {
      contour_type hull;
      cv::convexHull(contour, hull);
      valid_contours.push_back(contour);
      contour_hulls.push_back(hull);
    }
  }
  vector<contour_type> contour_to_loop;

  // we can choose to use the contours or its convex hull
  //contour_to_loop = contour_hulls;
  contour_to_loop = valid_contours;
  cv::Point robot_center(processedImage.cols / 2, processedImage.rows);
  cv::Point top_center(processedImage.cols / 2, 0);

  //replaced shortest_distance with shortest_angle - Andy
  float shortest_angle = INT_MAX;
  double centerXAverage;
  double centerYAverage;
  contour_type closest_contour;
  int counter = 0;
  double centers[2][2];
  double centerXOne;
  double centerXTwo;
  double centerYOne;
  double centerYTwo;

  for (size_t i = 0; i < contour_to_loop.size(); i++)
  {
    contour_type contour = contour_to_loop[i];

    if (isCubeShape(contour))
    {
      cv::drawContours(processedImage, contour_to_loop, i, MY_WHITE, 1);

      //IF CODE CRASHES CHANGE minAreaRect to boundingRect !!!!!
      cv::Rect rect = cv::minAreaRect(contour);

      // find center of mass
      cv::Moments centerMass = cv::moments(contour, true);
      double centerX = (centerMass.m10) / (centerMass.m00);
      double centerY = (centerMass.m01) / (centerMass.m00);
      cv::Point center(centerX, centerY);
      cv::drawMarker(processedImage, center, MY_PURPLE, 1);
      centers[counter][0] = centerX;
      centers[counter][1] = centerY;
      float angle;

      if (counter >= 1)
      {
        cv::Point centerOne(centers[0][0], centers[0][1]);
        cv::Point centerTwo(centers[1][0], centers[1][1]);
        centerXOne = centers[0][0];
        centerXTwo = centers[1][0];
        centerYOne = centers[0][1];
        centerYTwo = centers[1][1];
        centers[0][0] = centers[1][0];
        centers[0][1] = centers[1][1];
        centerXAverage = (centerXOne + centerXTwo) / 2;
        centerYAverage = (centerYOne + centerYTwo) / 2;
        cv::Point centerAverage(centerXAverage, centerYAverage);
        cv::line(processedImage, centerOne, centerTwo, MY_GREEN, 1);
        cv::line(processedImage, robot_center, centerAverage, MY_PURPLE, 1);
        counter = 1;

        // calculate angle, added by Andy
        angle = get_angle(processedImage, centerAverage);
      }
      else
      {
        counter++;
      }

      // draw line from robot to center mass
      cv::line(processedImage, robot_center, center, MY_PURPLE, 1);
      cv::rectangle(processedImage, rect, MY_PURPLE, 1);

      // calculate distance
      float distance = get_distance((int)sqrt(pow(centerXTwo + centerXOne, 2) + pow(centerYTwo + centerYOne, 2)));
      bool isPairValid = isPairValid(rect.height, centerXAverage);

      //replaced distance with angle - Andy
      //replaced shortest_distance with shortest_angle - Andy
      if (angle < shortest_angle && isPairValid)
      {
        cv::Point centerAverage(centerXAverage, centerYAverage);
        res.centerPoint = centerAverage;
        res.angle = angle;
        res.distance = distance;
        closest_contour = contour;
        shortest_angle = angle;
      }
    }
  }
  cv::line(processedImage, robot_center, top_center, MY_WHITE, 2);
  cv::line(processedImage, robot_center, res.centerPoint, MY_PURPLE, 1);
  res.timestamp = timeBegan;
  res.valid = (contour_to_loop.size() > 0);
  return res;
}
