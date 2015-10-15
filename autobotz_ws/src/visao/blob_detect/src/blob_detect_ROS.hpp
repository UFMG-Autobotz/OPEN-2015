// handels ROS initialization and listening or publishing to
// topics

#ifndef _BLOB_DETECT_ROS_HPP_
	#define _BLOB_DETECT_ROS_HPP_

//cpp
#include <vector>
//ROS includes
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
//openCV includes
#include <opencv2/core/core.hpp>
//include vision.hpp to use the 'feature' data type
#include "vision.hpp"
//custom messages to tell ROS about features
#include "blob_detect/feature.h"     //data about a single feature
#include "blob_detect/featureVec.h" //message containing a list of features

/////////////////////////// data structures ///////////////////

typedef struct {
 		ros::NodeHandle* nh;
 		image_transport::Subscriber color_sub;     //subscriber for kinects's color image topic
 		ros::Publisher              features_pub;  //publishes vectors of features

 		cv::Mat lastFrame;        //stores the last frame received from ROS
 		unsigned long frameCnt;   //counter that is incremented each time a frame is received
} T_ROSvars;

//there is a global instance of the struct that stores the ROS info
extern T_ROSvars ROSvars;

//////////////////////// functions ///////////////////////////

void initROS(int argc, char** argv);
//function to init ROS and register topics and callbacks
void freeROS();
//frees some ROS resources

cv::Mat getHSVImg(int* refCnt = NULL);
//get the last color frame from kinect as an openCV Mat in HSV format
//
//The callback keeps track of how many frames have been received
//one may optionally pass an int pointer to the function to get the
//count associated with the image being returned

void pubFeatures(std::vector< feature > F);
//Receives a vector of features (struct defined in vision.hpp)
//and publish to the appropriate topic

//////////////////////    callbacks    ////////////////////////

void rgbImgCallback(const sensor_msgs::ImageConstPtr& image);
//callback for the kinect color image

#endif
