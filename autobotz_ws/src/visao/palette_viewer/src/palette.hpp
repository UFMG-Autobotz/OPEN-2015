//Palette data structure
//
//A palette is an organized collection of colors

#ifndef _PALETTE_HPP_
	#define _PALETTE_HPP_

//cpp
#include <vector>
#include <string>
//cv
#include <opencv2/core/core.hpp>

using std::vector;
using std::string;

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

	void clear();  //clears the palette

	private:
	vector< cv::Vec3b >  colors;
	vector< int >        maxDistances;
	vector< string >     names;
	//each color i in the palette has a name and a neighbouring region defined by maxDistances[i]
};

#endif
