#include "blob_detect_ROS.hpp"
//ROS imports
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
//openCV imports
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

T_ROSvars ROSvars;

// //class constructor
// ROSHandle::ROSHandle()
// {
// 	frameCnt = 0;
// 	nh = NULL;
// }

// ROSHandle::~ROSHandle()
// {
// 	if(nh != NULL)
// 		delete nh;
// }

//initialize ROS
void initROS(int argc, char** argv)
{
	//params
	char node_name[] = "blob_detect";
	char rgb_topic[] = "/camera/rgb/image_raw";

	//init ROS
	ros::init(argc, argv, node_name);
	
	ROSvars.nh        = new ros::NodeHandle;
	ROSvars.frameCnt  = 0;
	ROSvars.lastFrame = cv::Mat::zeros(100, 100, CV_8UC3);

	//subscribe to kinect's color image
	image_transport::ImageTransport img_trans(*ROSvars.nh); 
    ROSvars.color_sub = img_trans.subscribe(rgb_topic, 1, rgbImgCallback); 

    ros::start();
}

//free ROS related memory
void freeROS()
{
	if(ROSvars.nh != NULL)
		delete ROSvars.nh;
}

//return last frame in openCV friendly format
cv::Mat getHSVImg(int* refCnt)
{
	//return reference counter value
	if(refCnt != NULL)
		*refCnt = ROSvars.frameCnt;

	//return cv::Mat type image
	return ROSvars.lastFrame.clone();
}

//copy topic content to lastFrame
void rgbImgCallback(const sensor_msgs::ImageConstPtr& image)
{
	//convert ROS message to cv_bridge::CvImgConstPtr format
	cv_bridge::CvImageConstPtr cv_ptr;
	try {
		cv_ptr = cv_bridge::toCvShare(image, "bgr8");
	}
	catch(cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
	}

	//set the frame data and update counter
	ROSvars.lastFrame = cv_ptr->image;
	ROSvars.frameCnt++; //increment frame count
}