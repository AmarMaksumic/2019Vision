/*
 *   Opencv 2.4.13
 *
 */

#include "vision.hpp"

using namespace std;

int singleFrame(string imageSrc )
{
    cv::Mat image, processedImage;

    image = cv::imread(imageSrc, CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

//    processedImage = image;

    calculate(image, processedImage);

//    resize(processedImage, processedImage,
//           cv::Size(processedImage.cols/4, processedImage.rows/4));  // to half size or even smaller

    cv::namedWindow("Display Window",CV_WINDOW_AUTOSIZE);
    cv::imshow("Display Window", processedImage );                  // Show our image inside it.
    cv::waitKey(0);                                                // Wait for a keystroke in the window
    return 0;
}



int process_video(int cameraSource)
{
    //Capture stream from webcam.
    cv::VideoCapture capture(cameraSource);

    //Check if we can get the webcam stream.
    if(!capture.isOpened())
    {
        std::cout << "Could not open camera" << std::endl;
        return -1;
    }

    while (true)
    {

        //initialize raw & processed image matrices
        cv::Mat cameraFrame, processedImage;

        //Read an image from the camera.
        capture.read(cameraFrame);

        processedImage = cameraFrame;

        //process the image, put the information into network tables
        calculate(cameraFrame, processedImage);

        cv::imshow("output", processedImage);

        //Waits 50 miliseconds for key press, returns -1 if no key is pressed during that time
        if (cv::waitKey(50) >= 0)
            break;
    }

    return 0;
}


int main(void) {

//    int exitCode = singleFrame("frc_cube.jpg");
    int exitCode = process_video(0);
    std::cout << exitCode;

}
