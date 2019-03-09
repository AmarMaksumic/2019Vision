#include "cargo.hpp"
#include <cmath>
using namespace std;

//double width, height;
//double area_ratio;

bool isValidAreaCargo(int camera_height, int camera_width, contour_type &contour){

    cv::Rect rect = cv::boundingRect(contour);

    double width = rect.width;
    double height = rect.height;
    double totalArea = ( camera_height * camera_width );
    
    double contour_side_ratio = height/width;
    if (height > width) {
        contour_side_ratio = height/width;
    }

    if( (contour_side_ratio > 1.05) && (contour_side_ratio < 0.95))
    {
        return false;
    }

    //calculate relevant ratios & values
    double area_ratio = width * height/totalArea * 100;

    //std::cout << area_ratio << std::endl;
    //std::cout << contour_side_ratio << std::endl;
    

    if( area_ratio < 1 ){
        return false;
    }
    return true;
}

bool isCubeShapeCargo (contour_type &contour) {
    return true;
}

float get_angleCargo( cv::Mat image, cv::Point point)
{
    float a = float(abs(image.cols/2 - point.x));
    float b = float(image.rows - point.y);

    float radians = atan(a/b);
    float angle = radians * 180 / CV_PI;
    return angle;
}

float get_distanceCargo( int width_pixel ){
    return float(CAR_CAMERA_FOCAL_LENGTH * CAR_LINE_LENGTH) / width_pixel;
}

VisionResultsPackageCargo calculateCargo(const cv::Mat &img, cv::Mat &processedImage) {
    ui64 timeBegan = millis_since_epoch();

    VisionResultsPackageCargo res;


    // RGB THRESHOLD
    cv::Mat rgbMat, rgbThreshold;

    cv::cvtColor(img, rgbMat, cv::COLOR_BGR2RGB);
    cv::inRange(rgbMat,
                cv::Scalar(CAR_RGB_RED[0], CAR_RGB_GREEN[0], CAR_RGB_BLUE[0]),
                cv::Scalar(CAR_RGB_RED[1], CAR_RGB_GREEN[1], CAR_RGB_BLUE[1]),
                rgbThreshold);

    // END RGB


    // START MASK

    cv::Mat rgbThresholdOutput;

    cv::bitwise_and(img, img, rgbThresholdOutput, rgbThreshold);
//
    //cv::imshow("RGB", rgbThresholdOutput);

    // HSV THRESHOLD
    cv::Mat hsvMat, hsvThreshold;

    cv::cvtColor(rgbThresholdOutput, hsvMat, cv::COLOR_BGR2HSV);
    cv::inRange(hsvMat,
                cv::Scalar(CAR_HSV_HUE[0], CAR_HSV_SAT[0], CAR_HSV_VAL[0]),
                cv::Scalar(CAR_HSV_HUE[1], CAR_HSV_SAT[1], CAR_HSV_VAL[1]),
                hsvThreshold);

    // END HSV

    //cv::imshow("HSV", hsvThreshold);

    cv::Mat medianBlurMat;

    cv::medianBlur(hsvThreshold, medianBlurMat, CAR_MEDIAN_BLUR_LEVEL);

    //cv::imshow("BLUR", medianBlurMat);
    //contour detection
    vector<contour_type> contours;
    vector<cv::Vec4i> hierarchy; //throwaway, needed for function
    cv::findContours(medianBlurMat, contours, hierarchy,
                     cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//

    //store the convex hulls of any valid contours
    vector<contour_type> contour_hulls;
    vector<contour_type> valid_contours;
    for (int i = 0; i < (int) contours.size(); i++) {
        contour_type contour = contours[i];
        if (isValidAreaCargo( processedImage.rows,
                         processedImage.cols,
                         contour)) {
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

    cv::Point robot_center( processedImage.cols / 2, processedImage.rows);
    cv::Point top_center( processedImage.cols / 2, 0 );

    //changed shortest_distance to shortest_angle
    float shortest_distance = INT_MAX;
    contour_type closest_contour;
    for (size_t i = 0; i < contour_to_loop.size(); i++)
    {
        contour_type contour = contour_to_loop[i];
        if (isCubeShapeCargo(contour)) 
	{
            cv::drawContours(processedImage, contour_to_loop, i, CAR_MY_WHITE, 1);

            cv::Rect rect = cv::boundingRect(contour);

            // find center of mass
            cv::Moments centerMass = cv::moments(contour, true);
            double centerX = (centerMass.m10) / (centerMass.m00);
            double centerY = (centerMass.m01) / (centerMass.m00);
            cv::Point center(centerX, centerY);
            cv::drawMarker(processedImage, center, CAR_MY_PURPLE, 1);

            float angle = get_angleCargo(processedImage, center);
            /*if (counter >= 1)
            {
                cv::Point centerOne(centers[0][0], centers [0][1]);
                cv::Point centerTwo (centers [1][0], centers [1][1]);
                centerXOne = centers[0][0];
                centerXTwo = centers[1][0];
                centerYOne = centers [0][1];
                centerYTwo = centers[1][1];
                centerXAverage = (centerXOne + centerXTwo)/2;
                centerYAverage = (centerYOne + centerYTwo)/2;

                cv::Point centerAverage(centerXAverage,centerYAverage);
                
                //cout << centerAverage;
                cv::line (processedImage, centerOne, centerTwo, CAR_MY_GREEN, 1);
                cv::line(processedImage, robot_center, centerAverage, CAR_MY_PURPLE, 1);
                //res.angle = get_angleCargo(processedImage, centerAverage);
                //res.centerPoint = centerAverage;
                
                //cout << res.centerPoint;

                counter = 0;

               angle = get_angleCargo(processedImage, centerAverage);
            }
            else
            {
                counter++;
            }*/

            // draw line from robot to center mass
            cv::line(processedImage, robot_center, center, CAR_MY_BLUE, 1);

            cv::rectangle(processedImage, rect, CAR_MY_WHITE, 1);

            // calculate distance
            float distance = get_distanceCargo((int)sqrt(pow(centerX,2) + pow(centerY,2)));

        //changed distance to angle, shortest_distance to shortest_angle
	    if(distance < shortest_distance){
              res.centerPoint = center;
	      res.distance = distance;
	      res.angle = angle;
	      closest_contour = contour;
	      shortest_distance = distance;
	    }
        }
    }

    //if( shortest_distance < INT_MAX ){
      //cv::Rect rect = cv::boundingRect(closest_contour);
      //cv::rectangle(processedImage, rect, MY_GREEN, 2);
    //}

    cv::line(processedImage, robot_center, top_center, CAR_MY_WHITE, 2);


	
	res.timestamp = timeBegan;
	res.valid = true;	
	return res;

}


