/*

= {2,4,6,8}; // Echo Pin 
 = {3,5,7,9}; // Trigger Pin
 int echoPin[5];
int trigPin[5];


 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.

roscore
rosrun rosserial_python serial_node.py /dev/ttyACM0
rostopic echo /eletronica/ultrassom/D

 */

#include <ros.h>
#include <std_msgs/Float32.h>

ros::NodeHandle  nh;

std_msgs::Float32 distD;
std_msgs::Float32 distE;
std_msgs::Float32 distF;
std_msgs::Float32 distT;

ros::Publisher chatterD("/eletronica/ultrassom/R", &distD);
ros::Publisher chatterE("/eletronica/ultrassom/L", &distE);
ros::Publisher chatterF("/eletronica/ultrassom/F", &distF);
ros::Publisher chatterT("/eletronica/ultrassom/B", &distT);

int echoPin[4];  // Echo Pin
int trigPin[4];  // Trigger Pin
#define LEDPin 13 // Onboard LED


int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed

void setup() {  
  //setup ROS publisher
  
  nh.initNode();
  
  nh.advertise(chatterD);
  nh.advertise(chatterE);
  nh.advertise(chatterF);
  nh.advertise(chatterT);
  
  for(int i=0; i<4; i++){ 
    echoPin[i] = (i+1)*2; 
    trigPin[i] = (i*2) + 3; 
  }

  //setup pins
  for(int i=0; i<4; i++){ 
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }

  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}

void getDistance(float* duration, float* distanceCm, int num)
{
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 digitalWrite(trigPin[num], LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin[num], HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin[num], LOW);
 *duration = pulseIn(echoPin[num], HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 *distanceCm = *duration/58.2;
 
 if (*distanceCm >= maximumRange || *distanceCm <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 *distanceCm = -1;
 digitalWrite(LEDPin, HIGH);
 }
 else
 //turn LED OFF to indicate successful reading.
 digitalWrite(LEDPin, LOW);
}


void loop()
{
 //sensor reads distance
 float duration, distance;
 int num_Ultrassonico=0;
 
 for(num_Ultrassonico=0; num_Ultrassonico<4; num_Ultrassonico++){ 
 
 getDistance(&duration, &distance,num_Ultrassonico);
 
 if(num_Ultrassonico == 0){ 
   distD.data = distance;
   chatterD.publish( &distD );
 }
 if(num_Ultrassonico == 1){ 
   distE.data = distance;
   chatterE.publish( &distE );
 }
 if(num_Ultrassonico == 2){ 
   distF.data = distance;
   chatterF.publish( &distF );
 }
 if(num_Ultrassonico == 3){ 
   distT.data = distance;
   chatterT.publish( &distT );
 }
 
 } 


 nh.spinOnce();

 //Delay before next reading.
 delay(100);
}
