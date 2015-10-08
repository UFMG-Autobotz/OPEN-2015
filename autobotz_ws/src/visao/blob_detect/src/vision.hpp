#ifndef _BLOB_VISION_HPP_
	#define _BLOB_VISION_HPP_

//C++ includes
#include <vector>
//openCV includes
#include <opencv2/core/core.hpp>

using std::vector;
////////////  data structures  /////////////
typedef struct {
	vector< cv::Point > contour;
	cv::Vec3b avg_color;
	int diameter;
} feature;

/////////////// functions //////////////////

void reduceColors(cv::Mat& in_hsv, cv::Mat& out_hsv);
//Reduces the colors present on an image to those specified in a palette
//	
//This function uses the distance metric defined in colorDistance to
//find the nearest color to each pixel
//
//TODO: define a standard palette format and get it as a third argument

inline float colorDistance(cv::Vec3b color1, cv::Vec3b color2);
//Returns the distance between color1 and color2
//
//The larger the returned value, the more distant the two colors are 

void getEdges(cv::Mat& in_hsv, cv::Mat& out_bw);
//Uses canny filter to find borders 
//
//Returns an image where all is black, except the pixels in borders, which are white


void filterContours(vector< vector<cv::Point> >  in_contours, 
					vector< vector<cv::Point> >& out        );
//Receives a vector of contours (each contour is a collection of points)
//
//the function uses approximations with less points and criteria
//such as size and convexness to select only contours of interest
//
//Another vector with only approximations of the contours of 
//interest is returned

vector< cv::Vec3b > getAvgColorInContours(const cv::Mat image, vector< vector< cv::Point > > cont);
//Receives a list of contours (cont) that exist in the image img
//
//The average color of the points inside the each conotur is
//calculated and the colors are returned in a vector

#endif
