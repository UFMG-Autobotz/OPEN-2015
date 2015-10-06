//////////////////////////////////////////////////////
//
// main file, part of the suqare_detect node
// This ROS node is intended to aquire an image from a MS kinect and
// publish the position of yellow rectangles on a topic
//
//////////////////////////////////////////////////////

#include <cmath>
#include <cv.h> 
#include <cv_bridge/cv_bridge.h>
#include <cxcore.h>
#include <highgui.h>
#include <image_transport/image_transport.h>	
#include <iostream> 
#include <opencv/cv.h>
#include "opencv/cvaux.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>	
#include <opencv2/imgproc/imgproc.hpp>	
#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <string.h> 
#include <vector>
#include <X11/keysym.h>

#include <ros/ros.h>	
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/Point.h>

#include "UI.hpp"
#include "CV_pipeline.hpp"

#include "square_detect/squareCenters.h"  //custom message

using namespace cv;
using namespace std;

////////////////////////////////////   variaveis globais    //////////////////////////////////

// Declaração e inicialização dos valores HSV do Trackbar para os Blocos Amarelos
int H_yellow_min = 0, H_yellow_max = 152, 
    S_yellow_min = 185, S_yellow_max = 255,
    V_yellow_min = 118, V_yellow_max = 238; 



int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";
const char* ROS_topic_image_rgb = "/camera/rgb/image_raw";
const char* ROS_topic_rect_pos = "/visao/squares";

Mat binary, image_rgb, image_rgb_previous, image_merge, image_yellow, image_red; 

vector <cv::Point> blockCenters;  //stores the center of all those rectangles

//////////////////////  function declarations  /////////////////////

void imageCallback(const sensor_msgs::ImageConstPtr& image);

/////////////////////////////////////////////// main ////////////////////////////////////////////////////////


int main(int argc, char **argv){

	// init ROS stuff
	ros::init(argc, argv, "image_processor");
    ros::NodeHandle nh;

	ros::Publisher centersPub = nh.advertise<square_detect::squareCenters>(ROS_topic_rect_pos, 10);
	square_detect::squareCenters centers_msg;

	image_transport::ImageTransport img_trans(nh); 
    image_transport::Subscriber sub = img_trans.subscribe(ROS_topic_image_rgb, 1, imageCallback); //********

	image_transport::Publisher pub;
    pub = img_trans.advertise("rgb/image_processed", 1);


	//TODO: funcao para configurar os callbacks

	int loopcount = 0;
	ros::Rate loop_rate(10);

	// main loop
	while (ros::ok())
	{
		//publish the yellow block position
		centers_msg.centers.clear();
		for(int i=0; i < blockCenters.size(); i++)
		{
			geometry_msgs::Point P;
			P.x = blockCenters[i].x;
			P.y = blockCenters[i].y;

			centers_msg.centers.push_back(P);    //set the array field of the publish message to be
			                                                    //equal to the global center positions array
		}
		
		if(loopcount % 2 == 0)
			centersPub.publish(centers_msg);

		ros::spinOnce();
		loop_rate.sleep();
	}

   	ROS_INFO("tutorialROSOpenCV::main.cpp::No error.");

	return 0;
}



/////////////////////////////////////////////     image callback     ////////////////////////////////////////////////////

void imageCallback(const sensor_msgs::ImageConstPtr& image)
{  //This function is called everytime a new image is published

	vector<vector<Point> > squaresYellow; // Vetor de pontos para os Blocos Amarelos

	Point RefPointYellow;
	Point RefPointYellowDepth;

	int aux = 0,  // Variável que conta quantas imagens foram capturadas da câmera 
	    depth = 0;


	cv_bridge::CvImageConstPtr cv_ptr;
	try{
		cv_ptr = cv_bridge::toCvShare(image, "bgr8");
	} 
	catch(cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	if(aux != 0)  // Caso não seja a primeira imagem pegada da câmera, copia-se a imagem prévia
		image_rgb_previous = image_rgb.clone();
	image_rgb = cv_ptr->image;
	if(aux == 0)  // Caso seja a primeira imagem pegada da câmera, não há imagem prévia. Logo, se copia a imagem atual
		image_rgb_previous = image_rgb.clone();
	//opticalFlow(image_rgb_previous, image_rgb);

	Mat image_filtered;

	createTrackbarYellow(); 
	//creatTrackbarRed();
	filter(image_rgb, image_filtered);
	contorno(image_rgb);
	findYellowBlocks(image_rgb, squaresYellow);
	drawSquares(image_yellow, squaresYellow);
	findShapes(image_rgb);
	
	blockCenters.clear();   //blockCenters is a global variable

	//get square centers to be sent on ROS
	for(int i = 0; i < squaresYellow.size(); i++)
	{
		blockCenters.push_back(cv::Point(0,0));

		if(squaresYellow[i].size() != 4)
			ROS_INFO("Got a square from findYellowBlocks that is not 4-sided...");
		else
		{
			for(int j = 0; j < 4; j++)
				blockCenters[i] += squaresYellow[i][j];  //add the coordinates of an vertex

			blockCenters[i].x = blockCenters[i].x/4;  //take the arithmetic mean
			blockCenters[i].y = blockCenters[i].y/4;

			//update ROS message

			//draw a circle on the center of the block
			cv::circle(image_yellow, blockCenters[i], 5, Scalar(255,0,0), 5);
		}
	}

	namedWindow("Image Yellow"); 
	imshow("Image Yellow", image_yellow); 

	//depth stuff
		//int depth = ReadDepthData(RefPointYellowDepth.y, RefPointYellowDepth.x, image); // Width = 640, Height = 480		
		
		// depth += ReadDepthData(260, 160, image); // Width = 640, Height = 480
		// aux++;	
		// if(aux%20 == 0){       
		// 	ROS_INFO("Depth: %d", depth/20);
		// 	aux = 0;
		// 	depth = 0;
		// }	
	

	//findRedBlocks(image_rgb, squaresRed);
	//drawSquares(image_red, squaresRed);
	//namedWindow("Image Red"); 
	//imshow("Image Red", image_red); 
	// findShapes(image_rgb);
	cv::waitKey(3);
}

