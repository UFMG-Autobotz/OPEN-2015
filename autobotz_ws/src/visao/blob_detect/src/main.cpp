//////////////////////////////////////////////////////////////////////////
//                                                                      //
// main file for the blob_detect node                                   //
// This ROS node is intended to aquire an image from a MS kinect and    //
// publish the position of different coloured blobs on a topic          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//local includes
#include "blob_detect_ROS.hpp"
#include "vision.hpp"
#include "settings.hpp"
//cpp
#include <iostream>
#include <vector>
//ROS
#include <ros/ros.h>
//openCV
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp> //blob detector
//needed to find out home directory
#include <unistd.h>
#include <pwd.h>

using namespace std;
using cv::Mat;
using cv::Point;

void visionCode1();
void visionCode2();
void visionCode3();
void visionCode4();
void visionCode5(Mat& img, vector< feature >& features);

//main function
int main(int argc, char** argv)
{
	//get user home directory
	char* homedir;
	if ((homedir = getenv("HOME")) == NULL)
	{
	    homedir = getpwuid(getuid())->pw_dir;
	}

	//read program settings from files
	settingsServer.updatePaletteFromFile(std::string(homedir) + "/open-2015/autobotz_ws/config/palette.conf");

	//init ROS
	initROS(argc, argv);

	//create a Rate object to control the execution rate
	//of our main loop
	ros::Rate loop_rate(50);


	int c = 1;
	unsigned long avg = 0;

	while(ros::ok())
	{
		Mat img = getHSVImg();
		vector< feature > F;
		visionCode5(img, F);
		
		//publish to topics
		pubFeatures(F);

		//spin ROS
		ros::spinOnce();

		//wait
		loop_rate.sleep();

		//if enabled, auto-resize the image to keep frame rate
		if(settingsServer.MAIN_autoresize)
		{
			unsigned long minDiff = 10*1000*1000;
			unsigned long delta   = loop_rate.cycleTime().nsec - loop_rate.expectedCycleTime().nsec;

			if(delta > minDiff)  //too slow
			{
				settingsServer.MAIN_resize_factor = settingsServer.MAIN_resize_factor - 5.0/img.rows;
				if(settingsServer.MAIN_resize_factor < settingsServer.MAIN_min_resize_factor)
					settingsServer.MAIN_resize_factor = settingsServer.MAIN_min_resize_factor;
			}
			if(delta < -minDiff)  //fast enough
			{
				settingsServer.MAIN_resize_factor = settingsServer.MAIN_resize_factor + 5.0/img.rows;
				if(settingsServer.MAIN_resize_factor > settingsServer.MAIN_max_resize_factor)
					settingsServer.MAIN_resize_factor = settingsServer.MAIN_max_resize_factor;
			}
		}

		//report loop rate
		avg = (c*avg + loop_rate.cycleTime().nsec)/(c+1);
		c++;
		ROS_INFO("Period: cur: %ims,   avg: %ims,   ideal: %ims,   f: %f\n", 
		           loop_rate.cycleTime().nsec/1000000, int(avg/1000000), loop_rate.expectedCycleTime().nsec/1000000, settingsServer.MAIN_resize_factor);
	}

	//deallocate and close any relevant stuff
	freeROS();

	return 0;
}

////////possible CV pipelines
void visionCode1()
{
	//Pipe:
	//rgb -> color clipping -> canny -> find contours -> filter contours

	Mat edges_img;         //black and white edge map
	Mat color_reduced; //image with colors clipped to a given palette (HSV format)
	
	// get color image from callback
	Mat img = getHSVImg();
	Mat tmp(img.size(), img.type());

	//reduce number of colors
	palette pal;  //define a custom palette
	int max_distance = 120;

	pal.addColor(cv::Scalar(0,   0,   0  ), max_distance);
	pal.addColor(cv::Scalar(255, 255, 255), max_distance);  //white
	pal.addColor(cv::Scalar(200, 200, 255), max_distance);  //white (bluish)
    pal.addColor(cv::Scalar(200, 0,   0  ), max_distance);  //red
	pal.addColor(cv::Scalar(0,   255, 0  ), max_distance);  //green
	pal.addColor(cv::Scalar(0,   0,   255), max_distance);  //blue
	pal.addColor(cv::Scalar(200, 200, 50 ), max_distance);  //yellow1
	pal.addColor(cv::Scalar(255, 255, 50 ), max_distance);  //yellow2
	pal.addColor(cv::Scalar(255, 255, 100), max_distance);  //yellow3

	reduceColors(img, color_reduced, pal);
	 	//DEBUG
		//cvtColor(img, tmp, CV_HSV2BGR);
		cv::namedWindow("color reduced"); cv::imshow("color reduced", color_reduced); cv::waitKey(1);

	// apply filters and canny to find image with "highlighted" borders
	getEdges(img, edges_img);
		//DEBUG
		cv::namedWindow("edge map"); cv::imshow("edge map", edges_img);

	// find contours
	std::vector<std::vector<cv::Point> > contours;  //stores the contours
	cv::findContours(edges_img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//filter out some types of contours (eg. small, non-convex)
	filterContours(contours, contours);
	
	//color contours according to avg color
	vector< cv::Vec3b > colors = getAvgColorInContours(img, contours);
		tmp = cv::Scalar(0, 0, 0);
		for(int i = 0; i < contours.size(); i++)
		{
			drawContours(tmp, contours, i, cv::Scalar(colors[i]), CV_FILLED);
		}
		cv::namedWindow("avg colors"); cv::imshow("avg colors", tmp);

	//alternative color reduction function that uses uniform colors intervals
	//(not sure if works)
	//
	// void color_reduce(cv::Mat &input, cv::Mat &output, size_t div)
	// {
	//     if(input.data != output.data){
	//         output.create(input.size(), input.type());
	//     }

	//     uchar buffer[256];
	//     for(size_t i = 0; i != 256; ++i){
	//         buffer[i] = i / div * div + div / 2;
	//     }
	//     cv::Mat table(1, 256, CV_8U, buffer, sizeof(buffer));
	//     cv::LUT(input, table, output);
	// }
}

void visionCode2()
{
	//Pipe:
	//rgb -> canny -> find contours -> filter contours

	Mat edges_img;         //black and white edge map
	Mat color_reduced; //image with colors clipped to a given palette (HSV format)

	// get color image from callback
	Mat img = getHSVImg();
	Mat tmp(img.size(), img.type());
	

	// apply filters and canny to find image with "highlighted" borders
	getEdges(img, edges_img);
		//DEBUG
		cv::namedWindow("edge map"); cv::imshow("edge map", edges_img); cv::waitKey(1);

	// find contours
	std::vector<std::vector<cv::Point> > contours;  //stores the contours
	cv::findContours(edges_img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//filter out some types of contours (eg. small, non-convex)
	filterContours(contours, contours);
		// DEBUG
		//tmp = cv::Scalar(0,0,0);	
		//drawContours(tmp, contours, -1, cv::Scalar(255,200,200), 1, 8);
		//cv::namedWindow("contours"); cv::imshow("contours", tmp);

	//color contours according to avg color
	vector< cv::Vec3b > colors = getAvgColorInContours(img, contours);
	tmp = cv::Scalar(0, 0, 0);
	for(int i = 0; i < contours.size(); i++)
		drawContours(tmp, contours, i, cv::Scalar(colors[i]), CV_FILLED);
	cv::namedWindow("avg colors"); cv::imshow("avg colors", tmp);
}

void visionCode3()
{
	//this one just uses openCV's simpleBlobDetector

	Mat img = getHSVImg();

	// Set up the detector with default parameters.
	cv::SimpleBlobDetector::Params params;
	params.filterByArea = 1;
 	params.minArea = 1;
 	params.maxArea = 10000;

 	params.minThreshold = 100;
 	params.thresholdStep = 50;
 	params.maxThreshold = 1000;

	cv::SimpleBlobDetector detector(params);

	//convert image to BGR
	//cv::cvtColor(img, img, CV_HSV2BGR);
	// Detect blobs.
	vector<cv::KeyPoint> keypoints;
	detector.detect(img, keypoints);
	 
	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	Mat im_with_keypoints;
	cv::drawKeypoints(img, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	 
	// Show blobs
	cv::namedWindow("keypoints");
	cv::imshow("keypoints", im_with_keypoints );
	cv::waitKey(1);
}

void visionCode4()
{
	//Pipe:
	//img -> erode/dilate -> canny -> erode edge map -> canny -> find contours
	//-> filter contours -> get blob information
	//
	//possible improvements: adjust brightness or quantize colors before first canny

	Mat edges_img;         //black and white edge map

	// get color image from callback
	Mat img = getHSVImg();
	Mat tmp(img.size(), img.type());  //this matrix is for debugging

	//erode and dilate image
	Mat erodeK  = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)); 	//Create kernels	
	Mat dilateK = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8)); 

	cv::erode (img, img, erodeK );   //apply operations
	cv::dilate(img, img, dilateK);
	

	//find edges map (bw image with "highlighted" borders)
	getEdges(img, edges_img);

	//dilate edge map to close open contours where there is a small gap on the border
	erodeK = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,1));
	cv::dilate (edges_img, edges_img, dilateK );

	//find edges on the new map
	cvtColor(edges_img, edges_img, CV_GRAY2BGR);
	getEdges(edges_img, edges_img);

		//DEBUG
		cv::namedWindow("edge map"); cv::imshow("edge map", edges_img); cv::waitKey(1);

	// find contours
	std::vector<std::vector<cv::Point> > contours;  //stores the contours
	cv::findContours(edges_img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//filter out some types of contours (eg. small, non-convex)
	filterContours(contours, contours);
		// DEBUG
		//tmp = cv::Scalar(0,0,0);	
		//drawContours(tmp, contours, -1, cv::Scalar(255,200,200), 1, 8);
		//cv::namedWindow("contours"); cv::imshow("contours", tmp);

	//color contours according to avg color
	vector< cv::Vec3b > colors = getAvgColorInContours(img, contours);
	tmp = cv::Scalar(0, 0, 0);
	for(int i = 0; i < contours.size(); i++)
		drawContours(tmp, contours, i, cv::Scalar(colors[i]), CV_FILLED);
	cv::namedWindow("avg colors"); cv::imshow("avg colors", tmp);	 
}

void visionCode5(Mat& img, vector< feature >& features)
{
	//Pipe:
	//img -> erode/dilate -> canny -> erode edge map -> canny -> find contours
	//-> filter contours -> get blob information
	//
	//possible improvements: adjust brightness or quantize colors before first canny

	Mat edges_img;         //black and white edge map

	// get color image from callback
	Mat tmp(img.size(), img.type());  //this matrix is for debugging

	//erode and dilate image
	Mat erodeK  = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)); 	//Create kernels	
	Mat dilateK = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8)); 

	cv::erode (img, img, erodeK );   //apply operations
	cv::dilate(img, img, dilateK);
	

	//find edges map (bw image with "highlighted" borders)
	getEdges(img, edges_img);

	//dilate edge map to close open contours where there is a small gap on the border
	erodeK = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,1));
	cv::dilate (edges_img, edges_img, dilateK );

	//find edges on the new map
	cvtColor(edges_img, edges_img, CV_GRAY2BGR);
	getEdges(edges_img, edges_img);

		//DEBUG
		cv::namedWindow("edge map"); cv::imshow("edge map", edges_img); cv::waitKey(1);

	// find contours
	std::vector<std::vector<cv::Point> > contours;  //stores the contours
	cv::findContours(edges_img.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//filter out some types of contours (eg. small, non-convex)
	filterContours(contours, contours);
		// DEBUG
		//tmp = cv::Scalar(0,0,0);	
		//drawContours(tmp, contours, -1, cv::Scalar(255,200,200), 1, 8);
		//cv::namedWindow("contours"); cv::imshow("contours", tmp);

	//extract features information
	features = getFeaturesInfo(img, contours);

		tmp = cv::Scalar(0, 0, 0);
		for(int i = 0; i < features.size(); i++)
		{
			vector< vector< cv::Point > > tmpCont;
			tmpCont.push_back(features[i].contour);
			drawContours(tmp, tmpCont, -1, cv::Scalar(features[i].avgColor), CV_FILLED);
		}
		cv::namedWindow("features - pre filter"); cv::imshow("features - pre filter", tmp);	

	//filter only relevant features
	filterFeatures(features, features, settingsServer.targetPalette);

		tmp = cv::Scalar(0, 0, 0);
		for(int i = 0; i < features.size(); i++)
		{
			vector< vector< cv::Point > > tmpCont;
			tmpCont.push_back(features[i].contour);
			drawContours(tmp, tmpCont, -1, cv::Scalar(features[i].avgColor), CV_FILLED);
		}
		cv::namedWindow("features - post filter"); cv::imshow("features - post filter", tmp);	

		cv::waitKey(1); //needed to make imshow work
}

//idea: apply std deviation filter to HSV image before trying 
//      to find borders
//see here: https://stackoverflow.com/questions/11456565/opencv-mean-sd-filter
