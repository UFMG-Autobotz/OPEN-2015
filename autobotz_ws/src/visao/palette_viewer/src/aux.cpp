//implement functions for the palette_viewer

//
#include "aux.hpp"
#include "palette.hpp"
//cpp
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
//open CV
#include <opencv2/core/core.hpp>

using std::string;
using std::cerr; using std::endl;

int updatePaletteFromFile(string fileName, palette& P)
{
	if(fileName.empty())
	{
		cerr << "Got empty string for palette filename!" << endl;
		return -1;
	}

	//open file
	std::ifstream file;
	file.open(fileName.c_str());
	if(!file.good() || !file.is_open())
	{
		cerr << "Error opening palette file!" << endl;
		return -2;
	}

	P.clear();

	//read every line
	int i = 0;
	while(file.good() && !file.eof())
	{
		i++;

		//get line from file
		string line;
		if(!getline(file, line))    //skip parsing if read fails
			continue; 
		if(line.empty())
			continue;

		//get info from line
		int b, g, r, ret;
		float maxDist;
		string name;
		ret = parsePaletteLine(line, b, g, r, maxDist, name);
	
		if(ret != 0)
			cerr << "settingsServer: could not parse line " << i << " of palette file" << endl;
		else
			//add color to palette
			P.addColor(cv::Scalar(b,g,r), maxDist, name);
	}

	//close file
	file.close();

	return 0;
}

//parse a line from the palette file
int parsePaletteLine(std::string L, int& B, int& G, int& R,
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

//draw a palette to an openCV image
void drawPalette(cv::Mat& canvas, const palette& P, int b)
{
	canvas = cv::Scalar(0,0,0);

	if(canvas.rows < 256 || canvas.cols < 256)    //canvas must have at least 256 positions
	{
		cerr << "can't draw palette in a matrix smaller then 256x256" << endl;
		return;
	}

	//draw the palette
	for(int i = 0; i < P.size(); i++ )  //for each color in the palette
	{
		cv::Vec3b curColor = P.getColor(i);
		int       curDist  = P.getMaxDistance(i);

		//assuming that we are using the L2-norm as distance metric
		//maxDistance for a given color describes a sphere around that point
		//of the rgb space
		//
		//md² = (dr)² + (dg)² + (db)²
		//where dx is the distance between the two colors in that channel.
		//making  dr = dg = 0, we get db = md and db = -md
		//
		//thus, we can get a range of valid blue values:
		int minB = curColor[2] - curDist;
		int maxB = curColor[2] + curDist; 

		//only draw if b is within the range for this color
		if( minB < b && b < maxB)
		{			
			//since the b value is already determined, we have now a circle
			//on the RG plane whose radius depends on db, va distance between
			//b and the palette's color b value
			//
			//radius² = dr² + dg² = md² - db²
			int radius = sqrt( pow(curDist      , 2)
			                 - pow(curColor[2]-b, 2) );

			//lets get a min/max value for r in this circle
			int minR = curColor[0] - radius;
			int maxR = curColor[0] + radius; 

			//two fors make a circle around the color
			for(int r = minR; r < maxR; r++)
			{


				//now b and r are chosen. This leaves a line in the g direction
				//lets find the limiting points that keep this line inside the
				//color sphere
				//
				//dg² = md² - db² - dg²
				int dg = sqrt( pow(curDist      , 2)
				              -pow(curColor[2]-b, 2)
				              -pow(curColor[0]-r, 2) );
				
				int minG = curColor[1] - dg;
				int maxG = curColor[1] + dg;
				
				for(int g = minG; g < maxG; g++)
				{
					//draw the color associated with this position in
					// //the RGB cube
					// cerr << "r:" << r << endl;
					// cerr << "g:" << g << endl;
					
					cv::Vec3b newColor(r, g, b);
					//only draw if (r,g) is a valid coordinate in the matrix
					if(r >= 0 && g >= 0 && r < canvas.rows && g < canvas.cols)
						canvas.at<cv::Vec3b>(cv::Point(r, g)) = newColor;
				}
			}
		}
	}
}
