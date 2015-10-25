//////////////////////////////////////////////////////////////////////////
//                                                                      //
// main file for the palette_viewer program.                            //
// Use it to view the content of a palette file, used in the            //                                                         //
// blob_detect node                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//local includes
#include "aux.hpp"
#include "palette.hpp"
//openCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//cpp
#include <iostream>

using namespace std;
using cv::Mat;
using cv::Point;

//variaveis globais porque sim
int blue_value;
bool canvas_needs_update;

//
void on_trackbar(int, void*)
{
	canvas_needs_update = true;
}

//main function
int main(int argc, char** argv)
{
	palette P;
	bool exit = false;

	//read palette from directory
	if(argc < 2)
	{
		cerr << "Not enough params. First param should be path to palette file..." << endl;
		return -1;
	}
	if(updatePaletteFromFile(argv[1], P) != 0)
	{
		cerr << "Could not open palette file!  Exiting..." << endl;
		return -2;
	}
	
	//stuff for the output
	Mat canvas(256, 256, CV_8UC3);	
	cv::namedWindow("palette");
	cv::namedWindow("Blue value");

	//trackbar
	blue_value = 127;
	cv::createTrackbar("bv_trackbar", "Blue value", &blue_value, 255, on_trackbar);

	canvas_needs_update = true;

	while(!exit)
	{
		//update output
		if(canvas_needs_update)
		{
			drawPalette(canvas, P, blue_value);
			canvas_needs_update = false;
		}
		cv::imshow("palette", canvas);

		//wait for keypress
		switch(char(cv::waitKey(10)))
		{
			case 'q':
			case 'Q':
				exit = true;
				break;
			case 'r':
			case 'R':
				updatePaletteFromFile(argv[1], P);
				canvas_needs_update = true;
				break;

		}	
	}

	return 0;
}

