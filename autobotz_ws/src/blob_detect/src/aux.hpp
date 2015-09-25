#ifndef _AUX_HPP_
	#define _AUX_HPP_

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

using namespace cv;
using namespace std;

////////////////    vision functions    //////////////////

void filter();
// applies erosion and dilation filter to each channel of the HSV image

void contorno();
// finds the contours of an image, generating the black and white contour image

void findYellowBlocks(const Mat& image, vector< vector<Point> >& squares);
//find yellow blocks in the image

void findContours(Mat image_rgb);
//TODO: mudar nome para nao conflitar com cv::findcontours


/////////////////////////////////////////////////////////////////////////////
//////////////////////     auxiliar functions    ////////////////////////////
/////////////////////////////////////////////////////////////////////////////


void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	
void findContours(Mat image_rgb);

void createTrackbarYellow();

void drawSquares(Mat& image, const vector<vector<Point> >& squares);


///////////////////////////////////// declaracoes ///////////////////////////////////////////

typedef union U_FloatParse{
	    float float_data;
	    unsigned char byte_data[4];
}U_FloatConvert;

#endif
