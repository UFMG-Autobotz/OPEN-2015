// Implements the computer vision functions for the blob_detect package
//

//local includes
#include "vision.hpp"
//openCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//cpp
#include <vector>
#include <cmath>

using cv::Mat;
using cv::Scalar;
using cv::Point;
using namespace std;


void reduceColors(cv::Mat& in, cv::Mat& out)
{
	//TODO: palette should be obtained as an argument
	//TODO: incorporate palette in settings server and pass it to reduceColors when
	//      the function is called in main()
	//define a palette 
	vector< Mat > p;
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(0, 0, 0)      ));
	//p.push_back(Mat(1, 1, CV_8UC3, Scalar(100, 100, 100))); //dark grey
	//p.push_back(Mat(1, 1, CV_8UC3, Scalar(200, 200, 200))); //light grey
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(255, 255, 255))); //white
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(200, 200, 255))); //white (bluish)
    p.push_back(Mat(1, 1, CV_8UC3, Scalar(200, 0, 0)  )); //red
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(0, 255, 0)  )); //green
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(0, 0, 255)  )); //blue
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(200, 200, 50)  )); //yellow1
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(255, 255, 50)  )); //yellow1
	p.push_back(Mat(1, 1, CV_8UC3, Scalar(255, 255, 100) )); //yellow1
	

	//convert palette to HSV
	for(int i = 0; i < p.size(); i++)  
		cv::cvtColor(p[i], p[i], CV_RGB2HSV);
	//create the actual pallete as a vector of cv::Vec3b
	vector< cv::Vec3b > palette;
	for(int i = 0; i < p.size(); i++)
		palette.push_back(p[i].clone().at<cv::Vec3b>(0,0));

	//initialize the output as a clone of the input
	out = in.clone();

	//loop through image
	for(int y = 0; y < in.rows; y++)
	{
		for(int x = 0; x < in.cols; x++)
		{
			//get current pixel color
			cv::Vec3b pixel_color = in.at<cv::Vec3b>(y,x);

			//find the index of the closest color to that pixel
			int closest_color   = 0;
			float shortest_dist = colorDistance(pixel_color, palette[0]);
			for(int i = 1; i < palette.size(); i++)
			{
				float new_dist = colorDistance(pixel_color, palette[i]);
				if(new_dist < shortest_dist)
				{
					shortest_dist = new_dist;
					closest_color = i;
				}
			}
			//max distance
			if(shortest_dist > 120)
				closest_color = 0;

			//set the color in that pixel on out to that of the closest one
			out.at<cv::Vec3b>(y,x) = palette[closest_color];
		}
	}
}

//calculate distance between two colors
inline float colorDistance(cv::Vec3b color1, cv::Vec3b color2)
{
	float d0 = color1[0] - color2[0];
	float d1 = color1[1] - color2[1];
	float d2 = color1[2] - color2[2];

	return sqrt(d0*d0 + d1*d1 + d2*d2);  //L2-norm
}

//get edges image using canny filter
void getEdges(cv::Mat& in_rgb, cv::Mat& out)
{
	out = in_rgb.clone();
	//canny parameters
	double low_threshold = 200;
	double high_threshold = 3*low_threshold;
	int kernel_size = 5;

	//convert color image to grayscale
	cv::Mat gray;
	cv::cvtColor(in_rgb, gray, CV_BGR2GRAY);

	//apply canny edge detector and write to output image
	cv::Canny(gray, out, low_threshold, high_threshold, kernel_size);  // OBS: Esses parâmetros podem ser modificados
}

//select only some kinds of contours
void filterContours(std::vector< std::vector<cv::Point> >   in  , 
					std::vector< std::vector<cv::Point> >&   out )
{
	out.clear();

	//iterate through each contour
	for (int i = 0; i < in.size(); i++)
	{
		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(in[i])) < 900)// || !cv::isContourConvex(approx))
    		continue;

		// Approximate contour with accuracy proportional
		// to the contour perimeter
		std::vector<cv::Point> approx = in[i];
		bool closed = true;
		cv::approxPolyDP(cv::Mat(in[i]), approx,
			             cv::arcLength(cv::Mat(in[i]), true) * 0.01,
			             closed);

		//cv::convexHull(approx, approx);

    	//add to output
    	out.push_back(approx);
	}
}

//find the average color inside a contour
vector< cv::Vec3b > getAvgColorInContours(const cv::Mat image, vector< vector< cv::Point > > cont)
{
	Mat mask = Mat::zeros(image.size(), CV_8UC1);
	vector<cv::Vec3b> avgs(cont.size(), 0.f); // This contains the averages of each contour

	for (size_t i = 0; i < cont.size(); ++i)  //for each contour
	{
	    cv::drawContours(mask, cont, i, cv::Scalar(i), CV_FILLED);   //draw a mask of 'color' i
	    cv::Rect roi = cv::boundingRect(cont[i]);

	    Scalar tmp = cv::mean(image(roi), mask(roi) == i);    //use the mask of color i

	    avgs[i][0] = tmp[0];
	    avgs[i][1] = tmp[1];
	    avgs[i][2] = tmp[2];
	}

	return avgs;
}
