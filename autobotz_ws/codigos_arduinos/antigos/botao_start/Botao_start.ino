//Rodar o no do arduino
//rosrun rosserial_python serial_node.py /dev/ttyACM0

#include <ros.h>
#include <std_msgs/Bool.h>

ros::NodeHandle  nh;
std_msgs::Bool msg;

ros::Publisher chatter("/eletronica/start", &msg);

int BotaoPin = 8; 
 
void setup()
{
  pinMode(BotaoPin, INPUT); 
  nh.initNode();
  nh.advertise(chatter);
}

void loop()
{
  msg.data= digitalRead(BotaoPin);
  chatter.publish( &msg );
  nh.spinOnce();
  delay(100);
}

