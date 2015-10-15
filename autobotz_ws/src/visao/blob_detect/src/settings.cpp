//implement the settings server

//
#include "settings.hpp"
//cpp
#include <string>
#include <fstream>
#include <sstream>
//open CV
#include <opencv2/core/core.hpp>
//ROS
#include <ros/ros.h>  //needed for ROS_ERROR

using std::string;

//actually create the global settings server object
settingsServer_T settingsServer;

//class constructor
settingsServer_T::settingsServer_T()
{
	/////////////  set some hard-coded default values  /////////////

	//used on the getEdges function 
	getEdges_low_threshold  = 100;
	getEdges_high_threshold = 3*getEdges_low_threshold;
	getEdges_kernel_size    = 5;

	//palette to which colors are clipped before sending to ROS
	float maxDistance = 100;

	targetPalette.addColor(cv::Scalar(0  , 0,   200), maxDistance, "red");  
	targetPalette.addColor(cv::Scalar(50 , 200, 200), maxDistance, "yellow");  
	targetPalette.addColor(cv::Scalar(50 , 255, 255), maxDistance, "yellow");  
	targetPalette.addColor(cv::Scalar(100, 255, 255), maxDistance, "yellow2");

	///////////////// read custom values from some file ////////////
	//TODO, use updateFromFile here
	//
	//Also, load targetPalette from a file
}

//update class variables from file
void settingsServer_T::updateSettingsFromFile(string fileName)
{
	//TODO: implement
}

void settingsServer_T::updatePaletteFromFile(string fileName)
{
	//open file
	std::ifstream file;
	file.open(fileName.c_str());
	if(!file.good())
	{
		ROS_ERROR("settingsServer: Error opening palette file!\n");
		return;
	}

	//read every line
	int i = 0;
	while(file.good() && !file.eof())
	{
		i++;

		//get line from file
		std::string line;
		if(!getline(file, line))
			continue;   //skip parsing if read fails
		if(line.empty())
			continue;

		//get info from line
		int b, g, r, ret;
		float maxDist;
		std::string name;
		ret = parsePaletteLine(line, b, g, r, maxDist, name);
	
		if(ret != 0)
			ROS_ERROR("settingsServer: could not parse line %i of palette file", i);
		else
			//add color to palette
			targetPalette.addColor(cv::Scalar(b,g,r), maxDist, name);
		
		//Debug: uncomment following lines to see the colors added to the palette
		//
		//ROS_INFO("added color to palette: %3i, %3i, %3i", b, g, r);
	}

	//close file
	file.close();
}

//parse a line from the palette file
int settingsServer_T::parsePaletteLine(std::string L, int& B, int& G, int& R,
                                       float& maxDist, string& name)
{
	//expected line format: B G R maxDist name
	std::stringstream ss;
	ss << L;

	ss >> B; ss >> G; ss >> R;
	ss >> maxDist;
	ss >> name;

	return 0;
	//TODO: do some error checking?
}