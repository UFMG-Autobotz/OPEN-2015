// Implements the palette data structure

//local includes
#include "palette.hpp"
//openCV
#include <opencv2/core/core.hpp>
//cpp
#include <vector>
#include <string>

#include <iostream>

using cv::Mat;
using cv::Scalar;
using namespace std;

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
