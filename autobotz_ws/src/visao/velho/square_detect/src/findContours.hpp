#include <ros/ros.h>	
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>	
#include <sensor_msgs/image_encodings.h>	
#include <opencv2/imgproc/imgproc.hpp>	
#include <opencv2/highgui/highgui.hpp>	
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>  
#include <string.h> 
#include <iostream> 
#include <sensor_msgs/Image.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <X11/keysym.h>
#include <cv.h> 
#include <highgui.h>
#include <cxcore.h>
#include "opencv/cvaux.h"
#include "opencv/highgui.h"
#include "opencv/cxcore.h"
#include "opencv2/core/core.hpp"
#include <sstream>
//#include "stitcher/Num.h"

using namespace cv;
using namespace std;

void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	
void findContours(Mat image_rgb);

