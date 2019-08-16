#include "helper.hpp"
#include "gst_pipeline.hpp"
#include "tape.hpp"
#include "cargo.hpp"

using namespace std;

shared_ptr<NetworkTable> myNetworkTable; //our networktable for reading/writing
string netTableAddress = "10.26.1.2";    //address of the rio

void pushToNetworkTablesTape(VisionResultsPackage info)
{
  myNetworkTable->PutNumber("CenterPoint X Tape", info.centerPoint.x);
  myNetworkTable->PutNumber("CenterPoint Y Tape", info.centerPoint.y);
  myNetworkTable->PutNumber("Distance Tape", info.distance);
  myNetworkTable->PutNumber("Angle Tape", info.angle);
  myNetworkTable->PutBoolean("Valid Tape", info.valid);
  myNetworkTable->Flush();
}

void pushToNetworkTablesCargo(VisionResultsPackageCargo info)
{
  myNetworkTable->PutNumber("CenterPoint X Cargo", info.centerPoint.x);
  myNetworkTable->PutNumber("CenterPoint Y Cargo", info.centerPoint.y);
  myNetworkTable->PutNumber("Distance Cargo", info.distance);
  myNetworkTable->PutNumber("Angle Cargo", info.angle);
  myNetworkTable->PutBoolean("Valid Cargo", info.valid);
  myNetworkTable->Flush();
}

//vision camera parameters
int
    device = 1, //bc we are using video1 in this case
    width = 320,
    height = 240,
    framerate = 30,
    mjpeg = true; //mjpeg is not better than just grabbing a raw image in this case

//driver cam parameters
int
    driver_device = 0,
    driver_width = 800,
    driver_height = 600,
    driver_framerate = 60;

//network parameters
int
    bitrate = 600000, //kbit/sec over network
    port_driver_stream = 5808,
    port_driver_thresh = 5807,
    port_stream = 5806,  //destination port for raw image
    port_thresh = 5805;  //destination port for thresholded image
string ip = "10.26.1.5"; //destination ip
string tableName = "CVResultsTable";
bool verbose = true;

int main()
{
  //initialize NetworkTables
  NetworkTable::SetClientMode();
  NetworkTable::SetDSClientEnabled(false);
  NetworkTable::SetIPAddress(llvm::StringRef(netTableAddress));
  NetworkTable::Initialize();
  if (verbose)
    printf("Initialized table\n");
  myNetworkTable = NetworkTable::GetTable(tableName);

  //open camera using CvCapture_GStreamer class
  CvCapture_GStreamer mycam;
  CvCapture_GStreamer mycam_driver;
  string read_pipeline = createReadPipelineSplit(
      device, width, height, framerate, mjpeg,
      bitrate, ip, port_stream);
  string read_driver_pipeline = createReadPipelineSplit(
      driver_device, driver_width, driver_height, driver_framerate, mjpeg, bitrate, ip, port_driver_stream);

  if (verbose)
  {
    printf("GStreamer read pipeline: %s\n", read_pipeline.c_str());
  }
  mycam.open(CV_CAP_GSTREAMER_FILE, read_pipeline.c_str());
  mycam_driver.open(CV_CAP_GSTREAMER_FILE, read_driver_pipeline.c_str());

  if (verbose)
  {
    printf("Succesfully opened camera with dimensions: %dx%d\n",
           width, height);
  }

  //open vidoe writer using CvVideoWriter_GStreamer class
  CvVideoWriter_GStreamer mywriter;
  CvVideoWriter_GStreamer mywriter_driver;
  string write_pipeline = create_write_pipeline(width, height, framerate,
                                                bitrate, ip, port_thresh);
  string write_driver_pipeline = create_write_pipeline(driver_width, driver_height, driver_framerate, bitrate, ip,
                                                       port_driver_thresh);
  if (verbose)
  {
    printf("GStreamer write pipeline: %s\n", write_pipeline.c_str());
  }
  mywriter.open(write_pipeline.c_str(),
                0, framerate, cv::Size(width, height), true);
  mywriter_driver.open(write_driver_pipeline.c_str(), 1, driver_framerate, cv::Size(driver_width, driver_height),
                       true);

  //initialize raw & processed image matrices
  cv::Mat cameraFrame, processedImage;
  cv::Mat cameraFrame_driver, processedImage_driver;

  //take each frame from the pipeline
  for (long long frame = 0;; frame++)
  {

    bool success = mycam.grabFrame();
    mycam_driver.grabFrame();
    if (verbose)
      printf("frame #%lld\n", frame);

    if (success)
    {
      IplImage *img = mycam.retrieveFrame(0); //store frame in IplImage
      cameraFrame = cv::cvarrToMat(img);      //convert IplImage to cv::Mat
      processedImage = cameraFrame;
      IplImage *imgdriver = mycam_driver.retrieveFrame(1);
      cameraFrame_driver = cv::cvarrToMat(imgdriver);
      processedImage_driver = cameraFrame_driver;

      //process the image, put the information into network tables
      VisionResultsPackage tape = calculate(cameraFrame, processedImage);
      pushToNetworkTablesTape(tape);
      VisionResultsPackageCargo cargo = calculateCargo(cameraFrame, processedImage);
      pushToNetworkTablesCargo(cargo);

      //pass the results back out
      IplImage outImage = (IplImage)processedImage;
      IplImage outImage_driver = (IplImage)processedImage_driver;
      outImage_driver.depth = 8;
      outImage_driver.nChannels = 3;

      if (verbose)
      {
        printf("Out image stats: (depth %d), (nchannels %d)\n",
               outImage_driver.depth, outImage_driver.nChannels);
      }
      mywriter.writeFrame(&outImage); //write output image over network
      mywriter_driver.writeFrame(&outImage_driver);
    }

    //delay for 10 millisecondss
    usleep(10);
  }
  mywriter.close();
  mywriter_driver.close();
  mycam.close();
  mycam_driver.close();
  return 0;
}