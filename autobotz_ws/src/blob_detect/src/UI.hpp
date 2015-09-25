#ifndef _BLOB_UI_HPP_
	#define _BLOB_UI_HPP_

#include "opencv2/core/core.hpp"
#include <vector>
#include <string>

using cv::Mat;
using std::string;
using std::vector;

// funcoes que geram a interface grafica do programa

void setLabel(Mat& im, const string label, vector<cv::Point>& contour);

void createTrackbarYellow();

void drawSquares(Mat& image, const vector< vector<cv::Point> >& squares);

#endif
