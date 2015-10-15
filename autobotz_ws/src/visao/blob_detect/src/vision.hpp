#ifndef _BLOB_VISION_HPP_
	#define _BLOB_VISION_HPP_

//C++ includes
#include <vector>
#include <string>
//openCV includes
#include <opencv2/core/core.hpp>

using std::vector;
using std::string;
////////////  data structures  /////////////

//structure to store information about image features
typedef struct feature {
	vector< cv::Point > contour;

	cv::Point centroid;
	cv::Rect  boundingBox;
	int       area;

	cv::Vec3b avgColor;
	cv::Vec3b colorStdDev;  //std deviation by channel
	string    colorName;    //color name given by clipping the average color to those
	                        //specified in settingsServer.targetPalette
} feature;

//
class palette
{
	public:
	//add colors to the palette
	void addColor(cv::Scalar BGR, float maxDistance);   //A color added with this version of addColor will have an empty string as name
	void addColor(cv::Scalar BGR, float maxDistance, string name); 

	int       size() const;                    //return number of elements in the palette
	cv::Vec3b getColor(int i) const;       //return the 'i'th color in the palette
	float     getMaxDistance(int i) const; //return the maxDistance associated with the i'th color in the palette
	string    getName(int i) const;

	bool hasColor(cv::Vec3b BGR) const;  //returns true if the given color is present in the palette

	private:
	vector< cv::Vec3b >  colors;
	vector< int >        maxDistances;
	vector< string >     names;
	//each color in the palette has a name and a neighbouring region defined by maxDistances[i]
	//some functions clip colors in this neighbour region to the color in the palette
};

/////////////// functions //////////////////

void reduceColors(cv::Mat& in_hsv, cv::Mat& out_hsv, const palette pal);
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
//The function excludes contours that are too small or have too long a border
//compared to its area.
//Each valid contour is then approximated with less points
//
//Another vector with only approximations of the contours of 
//interest is returned

vector< cv::Vec3b > getAvgColorInContours(const cv::Mat image, vector< vector< cv::Point > > cont);
//Receives a list of contours (cont) that exist in the image img
//
//The average color of the points inside the each conotur is
//calculated and the colors are returned in a vector

vector< feature > getFeaturesInfo(const cv::Mat& img, const vector< vector<cv::Point> > contours);
//Takes a list of contours that are associated with the image 'img'
//
//Each contour defines a feature in the image. Information about
//the features are extracted, assembled in the 'feature' data type
//and a vector of features is returned
//
//this function sets all the fields of the contour struct, except colorName

void filterFeatures(vector< feature > in, vector< feature >& out,
					const palette& relevantColors);
//Filters only features relevant to the problem being solved based
//on information produced by getFeaturesInfo, such as average color
//
//Uniting close features and rejecting feature loss during one or two
//frames should futurely be implemented in this function as well

int getClosestColorInPalette(cv::Vec3b color, const palette& pal);
//Using colorDistance() as metric, this function finds the closest
//color to 'color' in the palette and returns the index to that color

#endif
