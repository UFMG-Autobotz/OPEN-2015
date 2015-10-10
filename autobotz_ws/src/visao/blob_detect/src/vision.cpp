// Implements the computer vision functions for the blob_detect package
//

//local includes
#include "vision.hpp"
#include "settings.hpp"
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

	////////  define a palette  ////////
	palette pal;

	pal.addColor(0,   0,   0)  ;
	pal.addColor(255, 255, 255); //white
	pal.addColor(200, 200, 255); //white (bluish)
    pal.addColor(200, 0,   0);   //red
	pal.addColor(0,   255, 0);   //green
	pal.addColor(0,   0,   255); //blue
	pal.addColor(200, 200, 50)  ; //yellow1
	pal.addColor(255, 255, 50)  ; //yellow2
	pal.addColor(255, 255, 100) ; //yellow3

	int max_distance = 120;

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
			float shortest_dist = colorDistance(pixel_color, pal.getColor(0));
			for(int i = 1; i < pal.size(); i++)
			{
				float new_dist = colorDistance(pixel_color, pal.getColor(i));
				if(new_dist < shortest_dist)
				{
					shortest_dist = new_dist;
					closest_color = i;
				}
			}
			//max distance
			if(shortest_dist > max_distance)
				closest_color = 0;

			//set the color in that pixel on out to that of the closest one
			out.at<cv::Vec3b>(y,x) = pal.getColor(closest_color);
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
	double low_threshold  = settingsServer.getEdges_low_threshold;
	double high_threshold = settingsServer.getEdges_high_threshold;
	int kernel_size       = settingsServer.getEdges_kernel_size;

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

	int   min_contour_area = 500;
	float max_PA_rate = 0.2;   //perimeter/area  
	float cont_approx_error = 0.02;  //factor in determinig precision of contour approximation

	//iterate through each contour
	for (int i = 0; i < in.size(); i++)
	{
		float area      = std::fabs(cv::contourArea(in[i]));
		float perimeter = cv::arcLength(Mat(in[i]), true);

		// Skip small objects
		if (area < min_contour_area) //|| !cv::isContourConvex(approx))
    		continue;

    	// skip objects with big borders and small areas
    	if(perimeter/area > max_PA_rate)
    		continue;

		// Approximate contour with accuracy proportional
		// to the contour perimeter
		std::vector<cv::Point> approx = in[i];
		bool closed = true;
		cv::approxPolyDP(cv::Mat(in[i]), approx,
			             perimeter * cont_approx_error,
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

//extract feature info from the image and the contours
vector< feature > getFeaturesInfo(const cv::Mat& img, const vector< vector<cv::Point> > contours)
{
	vector< feature > out(contours.size());  //create an array of features with the same
	                                         //number of elements as contours

	if(img.empty() || img.rows < 1 || img.cols < 1)  //invalid input
		return out;

	for(int i = 0; i < contours.size(); i++)
	{
 		out[i].contour = contours[i];

 		//get mask of the contour area
 		Mat mask = Mat::zeros(img.size(), CV_8UC1);
 		cv::drawContours(mask, contours, i, cv::Scalar(i), CV_FILLED);   //draw a mask of 'color' i
 	    cv::Rect roi = cv::boundingRect(contours[i]);

		//get color info   -- getAvgColorInContours is not used in order to implement
		//					  stdDev calculation combined with average
	    Scalar tmpAvg; Scalar tmpSD;
//	    cv::meanStdDev(img(roi), tmpAvg, tmpSD, mask(roi) == i);    //find avg and std mean using mask of color i
	    tmpAvg = cv::mean(img(roi), mask(roi) == i);
	    tmpSD = tmpAvg;

	    out[i].avgColor   [0] = tmpAvg[0];  //assign tmp valued (Scalar) to output (Vec3b)
	    out[i].avgColor   [1] = tmpAvg[1];
	    out[i].avgColor   [2] = tmpAvg[2];
	    out[i].colorStdDev[0] = tmpSD [0];
	    out[i].colorStdDev[1] = tmpSD [1];
	    out[i].colorStdDev[2] = tmpSD [2];

	    //get geometry info
		cv::Moments m   = cv::moments(mask, true);
  		out[i].centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);

  		out[i].boundingBox = roi;
  		out[i].area = std::fabs(cv::contourArea(contours[i]));
	}

	return out;
}

void filterFeatures(vector< feature > in, vector< feature >& out,
	                const palette& pal, float maxDistance)
{
	out.clear();

	//for each feature, find the closest color and only 
	for(int i = 0; i < in.size(); i++)
	{
		//get current color
		cv::Vec3b cur_color = in[i].avgColor;

		//find the index of the closest color to the one in the feature
		int closest_color_index = getClosestColorInPalette(cur_color, pal);
		cv::Vec3b closest_color = pal.getColor(closest_color_index);

		//filter by max distance
		if(colorDistance(closest_color, cur_color) < maxDistance)
			out.push_back(in[i]);

		//only add feature to output if it is a color of interest

	}
}

//find the closest color to 'color' in a palette
//returns the index of the closest color in the palette
int getClosestColorInPalette(cv::Vec3b color, const palette& pal)
{
	//assume the first color is the closest
	int closest_color   = 0;  //index of the closest color in the palette
	float shortest_dist = colorDistance(color, pal.getColor(0));

	//test each color to see if its closer
	for(int i = 1; i < pal.size(); i++)
	{
		float new_dist = colorDistance(color, pal.getColor(i));
		if(new_dist < shortest_dist)
		{
			shortest_dist = new_dist;
			closest_color = i;
		}
	}

	return closest_color;
}

////////////// implement palette class ///////////////

void palette::addColor(int B, int G, int R)
{
	cv::Vec3b newColor(B, G, R);
	colors.push_back(newColor);
}

void palette::addColor(Scalar BGR)
{
	cv::Vec3b newColor(BGR[0], BGR[1], BGR[2]);
	colors.push_back(newColor);
}

int palette::size() const
{
	return colors.size();
}

cv::Vec3b palette::getColor(int i) const
{
	return colors[i];
}

bool palette::hasColor(cv::Vec3b c) const
{
	for(int i = 0; i < colors.size(); i++)
	{
		if(c == colors[i])
			return true;
	}

	return false;
}
