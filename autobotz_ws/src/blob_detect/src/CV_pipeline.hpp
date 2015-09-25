#ifndef _CV_PIPELINE_HPP_
	#define _CV_PIPELINE_HPP_

#include <vector>
#include "opencv2/core/core.hpp"

using namespace std;
using cv::Mat;
using cv::Point;

////////////////   main vision functions    //////////////////

void filter(Mat in_rgb, Mat out);
// applies erosion and dilation filter to each channel of the HSV image
//
// takes a rgb image as input and returns the output on the
// 'out' matrix


void contorno(Mat in_rgb);
// does: ???
//
// takes a rgb image as input

void findYellowBlocks(const Mat& image, vector< vector<Point> >& squares,
	                  cv::Point& RefPointYellow, cv::Point& RefPointYellowDepth );
//find yellow blocks in the image

void findShapes(Mat image_rgb);
//Search for polygons (triengles, quadrangles and other) in the input image
//also outputs stuff to the screen

///////////////  some auxiliary functions  ///////////////////////

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
	
void findContours(Mat image_rgb);

/////////////////    Data structures   /////////////////////

typedef union U_FloatParse{
	    float float_data;
	    unsigned char byte_data[4];
}U_FloatConvert;

#endif
