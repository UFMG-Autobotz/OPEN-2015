//////////////////////////////////////////////////////
//
// main file, part of the blobDetect node
// This ROS node is intended to aquire an image from a MS kinect and
// publish the position of blobs of a specified color range on a topic
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
#include <ros/ros.h>	
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/Point.h>
#include <sstream>
#include <stdio.h>
#include <stdint.h> 
#include <stdlib.h>
#include <string.h> 
#include <vector>
#include <X11/keysym.h>

#include "UI.hpp"
#include "CV_pipeline.hpp"

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
const char* ROS_topic_rect_pos = "/teste";

Mat binary, image_rgb, image_rgb_previous, image_merge, image_yellow, image_red; 

//////////////////////  function declarations  /////////////////////

void imageCallback(const sensor_msgs::ImageConstPtr& image);

/////////////////////////////////////////////// main ////////////////////////////////////////////////////////

geometry_msgs::Point msgYellowBlockPosition;  //TODO: mover essa variável para dentro do main

int main(int argc, char **argv){

	// init ROS stuff
	ros::init(argc, argv, "image_processor");
    ros::NodeHandle nh;

	image_transport::Publisher pub;
	ros::Publisher blockPub = nh.advertise<geometry_msgs::Point>(ROS_topic_rect_pos, 10);

	image_transport::ImageTransport img_trans(nh); 
    image_transport::Subscriber sub = img_trans.subscribe(ROS_topic_image_rgb, 1, imageCallback); //********

	ros::Rate loop_rate(30);

	//TODO: codigo mais versatil para configurar o callback

	// main loop

	while (ros::ok())
	{
		//publish the yellow block position
		blockPub.publish(msgYellowBlockPosition);

		ros::spinOnce();
		loop_rate.sleep();
	}

    pub = img_trans.advertise("rgb/image_processed", 1);

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
	findYellowBlocks(image_rgb, squaresYellow, RefPointYellow, RefPointYellowDepth);
	drawSquares(image_yellow, squaresYellow);
	//draw();

	//update yellow block position message to be published to ROS
	msgYellowBlockPosition.x = RefPointYellow.x;
	msgYellowBlockPosition.y = RefPointYellow.y;

	ROS_INFO("RefPointYellow: %i, %i", RefPointYellow.x, RefPointYellow.y);

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
	namedWindow("Image Yellow"); 
	imshow("Image Yellow", image_yellow); 
	//namedWindow("Image Red"); 
	//imshow("Image Red", image_red); 
	// findShapes(image_rgb);
	cv::waitKey(3);
}

