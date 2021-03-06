// Implements the computer vision functions for the blob_detect package
//

#include <ros/ros.h>
//local includes
#include "vision.hpp"
#include "settings.hpp"
//openCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//cpp
#include <vector>
#include <cmath>
#include <string>

using cv::Mat;
using cv::Scalar;
using cv::Point;
using namespace std;


void reduceColors(cv::Mat& in, cv::Mat& out, const palette pal)
{
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
			if(shortest_dist > pal.getMaxDistance(closest_color))
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

	int   min_contour_area = settingsServer.FCONT_min_contour_area;
	float max_PA_rate = settingsServer.FCONT_max_PA_rate;              //perimeter/area rate  
	float cont_approx_error = settingsServer.FCONT_cont_approx_error;  //factor in determinig precision of contour approximation

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
	    cv::meanStdDev(img(roi), tmpAvg, tmpSD, mask(roi) == i);    //find avg and stddev using mask of color i

	    out[i].avgColor   [0] = tmpAvg[0];  //assign tmp valued (Scalar) to output (Vec3b)
	    out[i].avgColor   [1] = tmpAvg[1];
	    out[i].avgColor   [2] = tmpAvg[2];
	    out[i].colorStdDev[0] = tmpSD [0];
	    out[i].colorStdDev[1] = tmpSD [1];
	    out[i].colorStdDev[2] = tmpSD [2];

	    //get geometry info
		cv::Moments m   = cv::moments(mask, true);
  		out[i].centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);  //centroid is computed from image moments

  		out[i].boundingBox = roi;
  		out[i].area = std::fabs(cv::contourArea(contours[i]));
	}
	return out;
}

inline float distBetweenPoints(const cv::Point p1, const cv::Point p2)
{
	return sqrt(pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2));
}

//discard features not relevant to robot operation
//fill the "colorName" field of the selected features
//eliminates blinking between frames
enum status {IN_NEW, IN_TRACKED, IN_WAS_MISSING, TRACKED_FOUND, TRACKED_NOT_FOUND, MISSING_FOUND, MISSING_NOT_FOUND};


	
void filterFeatures(vector< feature > in, vector< feature >& out,
	                const palette& pal)
{
	out.clear();

	//Each point in a vector corresponds to the position of a feature in
	//the last frame
	vector <cv::Point> last_positions;
	const float r_max = 15;  //raio maximo para a feature ser associada ao ponto
	///////////   filter features by color  //////
	vector< feature > color_filtered;

	//find closest color and only put it in color_filtered if it is within a distance
	for(int i = 0; i < in.size(); i++)
	{
		//get current color
		cv::Vec3b cur_color = in[i].avgColor;

		//find the index of the closest color to the one in the feature
		int closest_color_index = getClosestColorInPalette(cur_color, pal);
		cv::Vec3b closest_color = pal.getColor(closest_color_index);

		//filter by max distance
		if(colorDistance(closest_color, cur_color) < pal.getMaxDistance(closest_color_index))
		{
			color_filtered.push_back(in[i]);      //only add feature to output if it is a color of interest
			color_filtered.back().colorName = pal.getName(closest_color_index);  //give a name to the feature's avg color
		}
	}

	out = color_filtered;
}

void Estabiliza(vector< feature > color_filtered, vector< feature >& f)
{
	int r_max;  
	static int primeira_vez = 1;
	static std::vector<int> frames;
	std::vector<int> posi_enc, posi_muda;
	std::vector<feature> areas;

	if (primeira_vez == 1){
		frames.clear();
		primeira_vez = 0;
	}

	int min;
	float dist;
	posi_muda.clear();

	for(int i = 0; i < f.size(); i++)	
	{	
		areas.clear();
		posi_enc.clear();
		bool found_any = false;
	
	 	for(int j = 0; j < color_filtered.size(); j++)
	 	{	
	 		dist = distBetweenPoints(f[i].centroid, color_filtered[j].centroid);
			r_max = sqrt(f[i].area);
			//ROS_INFO("%d %d %d %d", (int) f.size(), (int) color_filtered.size(), (int) dist, (int)  r_max);
			//ROS_INFO("teste1: %d %d %d", (int) f.size(), (int) color_filtered.size(), (int)  frames.size()); 		
			if(dist < r_max && color_filtered[j].area > 0.6*f[i].area && color_filtered[j].area < 1.4*f[i].area)//feature is near point
	 		{
				areas.push_back(color_filtered[j]);
				posi_enc.push_back(j);
				found_any = true;
			}
		}
		
		int min_k = 0;

		if (found_any)
			min = abs(areas[0].area-f[i].area);
	
		for(int k = 1; k < areas.size(); k++)
	 	{			
			if(abs(areas[k].area-f[i].area) < min) 
	 		{
				min = abs(areas[k].area-f[i].area);
				min_k = k;	 		
	 		}
	 	}

		if (found_any)
		{
			posi_muda.push_back(posi_enc[min_k]);
			f[i] = areas[min_k];	 		
			frames[i] = 0;	
			found_any = false; 		
		}
		else
			frames[i] += 1;
	
		//ROS_INFO("teste1: %d %d %d", (int) areas.size(), (int) posi_enc.size(), (int)  posi_muda.size()); 
	}
	
	for(int i = 0; i < color_filtered.size(); i++)	
	{
	 	bool new_bloc = true;

	 	for(int j = 0; j < posi_muda.size(); j++)
	 	{	
	 		if(posi_muda[j] == i)
			{
	 			new_bloc = false;
				break;
			}
	 	}

		if (new_bloc)
		{
			f.push_back(color_filtered[i]);
			frames.push_back(0);
		}
	}

	for (int i = 0; i < frames.size(); i++)
	{
		if (frames[i] > 6)
		{
			f.erase(f.begin() + i);
			frames.erase(frames.begin() + i);
		}
	}
	//ROS_INFO("%d %d %d %d", (int) f.size(), (int) color_filtered.size(), (int) dist, (int)  r_max);
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

void palette::addColor(Scalar BGR, float maxDist)
{
	//add color
	cv::Vec3b newColor(BGR[0], BGR[1], BGR[2]);
	colors.push_back(newColor);

	//add distance
	maxDistances.push_back(maxDist);

	//use empty string as name
	names.push_back(std::string(""));
}

void palette::addColor(Scalar BGR, float maxDist, string name)
{
	//add color
	cv::Vec3b newColor(BGR[0], BGR[1], BGR[2]);
	colors.push_back(newColor);

	//add distance
	maxDistances.push_back(maxDist);

	//add name
	names.push_back(name);
}

int palette::size() const
{
	return colors.size();
}

cv::Vec3b palette::getColor(int i) const
{
	return colors[i];
}

float palette::getMaxDistance(int i) const
{
	return maxDistances[i];
}

string palette::getName(int i) const
{
	return names[i];
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

void palette::clear()
{
	colors.clear();
	maxDistances.clear();
	names.clear();
}
