#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include<iostream>
using namespace std;

//std_msgs::Float32 ultrassomR, ultrassomL, ultrassomB;
std_msgs::Float32 ultrassomF;

/*
void CallbackR(const std_msgs::Float32& msg)
{
  ultrassomR.data = msg.data;
  if(ultrassomR.data>= 0.00 && ultrassomR.data <=2.50)
	ultrassomR.data =  msg.data*1.38;
  
  else if(ultrassomR.data>2.50 && ultrassomR.data <=3.20)
	ultrassomR.data =  msg.data*1.42;

  else if(ultrassomR.data>3.20 && ultrassomR.data <=7.50)
	ultrassomR.data =  msg.data*1.35;

  else if(ultrassomR.data> 7.50 && ultrassomR.data <= 14.00)
	ultrassomR.data =  msg.data*1.33;

  else if(ultrassomR.data> 14.00  && ultrassomR.data <= 29.50)
	ultrassomR.data =  msg.data*1.27;

  else if(ultrassomR.data> 29.50 && ultrassomR.data <=39.00)
	ultrassomR.data =  msg.data*1.25;

  else if(ultrassomR.data> 39.00 && ultrassomR.data <=200.00)
	ultrassomR.data =  msg.data*1.19;
 
}

void CallbackL(const std_msgs::Float32& msg){
ultrassomL.data = msg.data;
}*/
void CallbackF(const std_msgs::Float32& msg){
  ultrassomF.data = msg.data;
  if(ultrassomF.data>= 0.00 && ultrassomF.data <=1.70)
	ultrassomF.data =  msg.data*1.40;
  
  else if(ultrassomF.data>1.70 && ultrassomF.data <=2.80)
	ultrassomF.data =  msg.data*1.48;//42

  else if(ultrassomF.data>2.80 && ultrassomF.data <=3.20)
	ultrassomF.data =  msg.data*1.52;//42

  else if(ultrassomF.data>3.20 && ultrassomF.data <=7.50)
	ultrassomF.data =  msg.data*1.35;//35

  else if(ultrassomF.data> 7.50 && ultrassomF.data <= 13.00)
	ultrassomF.data =  msg.data*1.31;

  else if(ultrassomF.data> 13.00  && ultrassomF.data <= 23.50)
	ultrassomF.data =  msg.data*1.24;

  else if(ultrassomF.data> 23.50 && ultrassomF.data <=39.00)
	ultrassomF.data =  msg.data*1.16;//25

  else if(ultrassomF.data> 39.00 && ultrassomF.data <=200.00)
	ultrassomF.data =  msg.data*1.14;//19
}
/*
void CallbackB(const std_msgs::Float32& msg){
ultrassomB.data = msg.data;
}
*/
int main(int argc, char **argv)
{
  ros::init(argc, argv, "calibra_ultrassom");
  ros::NodeHandle n;

  //ros::Publisher chatterR = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/R", 1000);
  //ros::Publisher chatterL = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/L", 1000);
  ros::Publisher chatterF = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/F", 1000);
  //ros::Publisher chatterB = n.advertise<std_msgs::Float32>("/eletronica/ultrassom/B", 1000);

//  ros::Subscriber subR = n.subscribe("/eletronica/ultrassom/raw/R", 1000, CallbackR);
//  ros::Subscriber subL = n.subscribe("/eletronica/ultrassom/raw/L", 1000, CallbackL);
  ros::Subscriber subF = n.subscribe("/eletronica/ultrassom/raw/F", 1000, CallbackF);
//  ros::Subscriber subB = n.subscribe("/eletronica/ultrassom/raw/B", 1000, CallbackB);

  ros::Rate loop_rate(4);
  
  //std_msgs::Float32 msgR, msgL, msgB;
  std_msgs::Float32 msgF;
  while (ros::ok())
  {
    msgF.data = ultrassomF.data;
//    msgL.data = ultrassomL.data;
//    msgF.data = ultrassomF.data;
//    msgB.data = ultrassomB.data;
    cout<<"Frente: "<<msgF.data<<endl;
//    cout<<"Dir: "<<msgR.data<<"  Esq: "<<msgL.data<<"  Frente: "<<msgF.data<<"  Tras: "<<msgB.data<<endl;
//    chatterR.publish(msgR);
//    chatterL.publish(msgL);
    chatterF.publish(msgF);
//    chatterB.publish(msgB);
    ros::spinOnce();
    loop_rate.sleep();
  }

  ros::spin();
  return 0;
}
