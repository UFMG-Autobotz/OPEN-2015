#include "blob_detect_ROS.hpp"
#include "settings.hpp"
//openCV imports
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//ROS imports
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
//custom ROS msgs
#include "blob_detect/featureVec.h"
#include "blob_detect/feature.h"
#include "geometry_msgs/Point.h"
#include "std_msgs/Float32.h"


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
	char feature_topic[]= "/visao/features";
	char screen_size_x_topic[] = "/visao/screenX";
	char screen_size_y_topic[] = "/visao/screenY";

	//init ROS
	ros::init(argc, argv, node_name);
	
	ROSvars.nh        = new ros::NodeHandle;
	ROSvars.frameCnt  = 0;
	ROSvars.lastFrame = cv::Mat::zeros(100, 100, CV_8UC3);

	//subscribe to kinect's color image
	image_transport::ImageTransport img_trans(*ROSvars.nh); 
    ROSvars.color_sub = img_trans.subscribe(rgb_topic, 1, rgbImgCallback); 

    const int buff_size = 5;
    //publish vector of features
    ROSvars.features_pub = ROSvars.nh->advertise<blob_detect::featureVec>(feature_topic, buff_size);
    //publish screen size 
    ROSvars.screenX_pub = ROSvars.nh->advertise<std_msgs::Float32>(screen_size_x_topic, buff_size);
    ROSvars.screenY_pub = ROSvars.nh->advertise<std_msgs::Float32>(screen_size_y_topic, buff_size);

    if(!ROSvars.features_pub || !ROSvars.screenX_pub || !ROSvars.screenY_pub)
    	ROS_ERROR("blob_detect: One of the publishers could not be initialized!");

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

//copy kinect's topic content to 'lastFrame'
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
	cv::Mat tmp = cv_ptr->image;
	float c = settingsServer.MAIN_resize_factor;
	cv::resize(tmp, tmp, cv::Size(c * tmp.cols, c * tmp.rows));
	ROSvars.lastFrame = tmp;
	ROSvars.frameCnt++; //increment frame count
}

//publish a vector of features to ROS
void pubFeatures(std::vector< feature > F, cv::Mat& screen)
{
	//create a message of type featureVec
	blob_detect::featureVec FVec_msg;

	//for each item in F, push a corresponding
	//feature message to FVec_msg
	for(int i = 0; i < F.size(); i++)
	{
		blob_detect::feature F_msg;

		//fill F_msg fields
		for(int j = 0; j < F[i].contour.size(); j++)   //copy (x,y) coordenates of every point in the contour
		{
			geometry_msgs::Point32 P;
			P.x = F[i].contour[j].x;
			P.y = F[i].contour[j].y;

			F_msg.contour.points.push_back(P);
		}
		F_msg.centroid.x = F[i].centroid.x;
		F_msg.centroid.y = F[i].centroid.y;
		F_msg.area.data  = F[i].area;
		F_msg.avgColor.b = F[i].avgColor[0];
		F_msg.avgColor.g = F[i].avgColor[1];
		F_msg.avgColor.r = F[i].avgColor[2];
		F_msg.stdDevByChannel.b = F[i].colorStdDev[0];
		F_msg.stdDevByChannel.g = F[i].colorStdDev[1];
		F_msg.stdDevByChannel.r = F[i].colorStdDev[2];
		F_msg.colorName.data = F[i].colorName;

		//add feature message to vector
		FVec_msg.features.push_back(F_msg);
	}

	//publish vector of features and screen size
	ROSvars.features_pub.publish(FVec_msg);

	std_msgs::Float32 x, y;
	x.data = screen.cols;
	y.data = screen.rows;
	ROSvars.screenX_pub.publish(x);
	ROSvars.screenY_pub.publish(y);
}
