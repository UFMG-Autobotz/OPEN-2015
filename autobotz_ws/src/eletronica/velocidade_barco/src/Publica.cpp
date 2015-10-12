#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "rosserial_arduino/ultrassom.h"
#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<rosserial_arduino::ultrassom>("/eletronica/ultrassom/", 1000);
  ros::Publisher chatter_YAW = n.advertise<std_msgs::Float32>("/eletronica/IMU/yaw/", 1000);

  ros::Rate loop_rate(10);

  float countD = 0.0;
  float countE = 0.0;
  float countF = 0.0;
  float countT = 0.0;
  int cont = 0;
  std_msgs::Float32 msg_yaw;

  msg_yaw.data = 0.00;

  while (ros::ok())
  {
    rosserial_arduino::ultrassom msg;

    msg.dir = countD;
    msg.esq = countE;
    msg.frente = countF;
    msg.tras = countT;
    ROS_INFO("Dir: %f , Esq: %f ,Frente: %f ,Tras: %f ", msg.dir, msg.esq, msg.frente, msg.tras);
    chatter_YAW.publish(msg_yaw);
    chatter_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();
    cont ++ ;
if(cont <200){
    countD = countD + 1.0;
    countE = countE + 2.0;
    countF = countF + 3.0;
    countT = countT + 4.0;
//    countD = 1.0;
//    countE = 2.0;
//    countF = 3.0;
//    countT = 4.0;
}
if(cont>=200 && cont<400){
    countD = countD + 10.0;
    countE = countE + 20.0;
    countF = countF + 30.0;
    countT = countT + 40.0;
//    countD = 10.0;
//    countE = 20.0;
//    countF = 30.0;
//    countT = 40.0;
}
if(cont>=400){
    countD = countD + 100.0;
    countE = countE + 200.0;
    countF = countF + 300.0;
    countT = countT + 400.0;
//    countD = 100.0;
//    countE = 200.0;
//    countF = 300.0;
//    countT = 400.0;
}

  }


  return 0;
}
