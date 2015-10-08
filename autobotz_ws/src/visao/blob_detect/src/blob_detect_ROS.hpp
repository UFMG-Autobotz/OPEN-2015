// handels ROS initialization and listening or publishing to
// topics

#ifndef _BLOB_DETECT_ROS_HPP_
	#define _BLOB_DETECT_ROS_HPP_

//ROS includes
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

//openCV includes
#include <opencv2/core/core.hpp>

typedef struct {
 		ros::NodeHandle* nh;
 		image_transport::Subscriber color_sub;  //subscriber for the color image topic

 		cv::Mat lastFrame;        //stores the last frame received from ROS
 		unsigned long frameCnt;   //counter that is incremented each time a frame is received
} T_ROSvars;

//there is a global instance of the struct that stores the ROS info
extern T_ROSvars ROSvars;

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

////// callbacks
void rgbImgCallback(const sensor_msgs::ImageConstPtr& image);
//callback for the kinect color image

#endif