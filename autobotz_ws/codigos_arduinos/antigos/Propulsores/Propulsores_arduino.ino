/*
 * rosserial Subscriber Example
 * Blinks an LED on callback
 */

#include <ros.h>
#include <std_msgs/Int32.h>

//variaveis da ponte H
int motor_left[] = {6, 5};
int motor_right[] = {10, 9};
int velocidadeR, velocidadeL;
int direcaoR, direcaoL;
int auxiliarR, auxiliarL;
#define TRAS 0
#define FRENTE 1

ros::NodeHandle nh;


void messageR( const std_msgs::Int32& msg){
  auxiliarR = msg.data;
  if (auxiliarR < 0){
    velocidadeR = (-1) * auxiliarR;
    direcaoR = TRAS;
  }
  else{
    velocidadeR = auxiliarR;
    direcaoR = FRENTE;
  }
}

void messageL( const std_msgs::Int32& msg){
 auxiliarL = msg.data;
  if (auxiliarL < 0){
    velocidadeL = (-1) * auxiliarL;
    direcaoL = TRAS;
  }
  else{
    velocidadeL = auxiliarL;
    direcaoL = FRENTE;
  }
}

ros::Subscriber<std_msgs::Int32> subR("/eletronica/propulsor/R", &messageR );
ros::Subscriber<std_msgs::Int32> subL("/eletronica/propulsor/L", &messageL );

void setup()
{
  pinMode(13, OUTPUT);
  nh.initNode();
  nh.subscribe(subR);
  nh.subscribe(subL);
  int i;
  for(i = 0; i < 2; i++){
    pinMode(motor_left[i], OUTPUT);
    analogWrite(motor_left[i], 0);
    pinMode(motor_right[i], OUTPUT);
    analogWrite(motor_right[i], 0);
  }
}

void loop()
{
  digitalWrite(motor_left[1], direcaoL);  
  digitalWrite(motor_right[1], direcaoR);
  analogWrite(motor_left[0],velocidadeL);
  analogWrite(motor_right[0],velocidadeR);
  
  nh.spinOnce();
  delay(1);
}
