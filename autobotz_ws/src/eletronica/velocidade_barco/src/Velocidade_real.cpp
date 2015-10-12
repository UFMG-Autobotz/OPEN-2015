#include "ros/ros.h"
#include "rosserial_arduino/ultrassom.h"
#include "std_msgs/Float32.h"
#include "geometry_msgs/Pose2D.h"
#include<iostream>
using namespace std;
#define VELGRANDE 300

int cont = 0;
float orientacao;
float velX, velY;
float velocidadeX, velocidadeY;
rosserial_arduino::ultrassom velocidade;
rosserial_arduino::ultrassom msg0;

void chatterCallbackYaw(const std_msgs::Float32& msg)
{
  orientacao = msg.data;
  std::cout<<"Orientacao(RAW): "<<orientacao<<endl;
  if (orientacao<0)
  		orientacao = 360 + orientacao;
  std::cout<<"Orientacao: "<<orientacao<<endl;
}

void chatterCallback(const rosserial_arduino::ultrassom& msg)
{
  
  if (cont == 0){
	  msg0.dir = msg.dir;
	  msg0.esq = msg.esq;
	  msg0.frente = msg.frente;
	  msg0.tras = msg.tras;
	  cont ++;
  }
  else{
	  velocidade.dir = msg.dir - msg0.dir;
	  msg0.dir = msg.dir;

	  velocidade.esq = msg.esq - msg0.esq;
	  msg0.esq = msg.esq;

	  velocidade.frente = msg.frente - msg0.frente;
	  msg0.frente = msg.frente;

	  velocidade.tras = msg.tras - msg0.tras;
	  msg0.tras = msg.tras;
  } 
  std::cout<<"Dist - Dir: "<<msg.dir<<" Esq: "<<msg.esq<<" F: "<<msg.frente<<" T: "<<msg.tras<<" "<<endl;
  std::cout<<"Vel - [Dir: "<<velocidade.dir<<"] [Esq: "<<velocidade.esq<<"] [F: "<<velocidade.frente<<"] [T: "<<velocidade.tras<<"]"<<endl;
}

void getVelocidade(){
//Considerando que na aproximacao a velocidade é negativa
  velX = (velocidade.dir - velocidade.esq)/2; 
  velY = (velocidade.frente - velocidade.tras)/2; 
  
  velocidadeX = sin(orientacao) * velY + cos(orientacao) * velX;
  velocidadeY = sin(orientacao) * velX + cos(orientacao) * velY;  
  std::cout<<"function - Vel - [ X: "<<velocidadeX<<" ]    [ Y: "<<velocidadeY<<" ]  [ Orient.: "<<orientacao<<" ]"<<endl;

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::NodeHandle nh;
  ros::Subscriber sub = n.subscribe("/eletronica/ultrassom/", 1000, chatterCallback);
  ros::Subscriber subYaw = n.subscribe("/eletronica/IMU/yaw/", 1000, chatterCallbackYaw);
  ros::Publisher chatter_pub = nh.advertise<geometry_msgs::Pose2D>("/eletronica/ultrassom/velocidade", 1000);

  ros::Rate loop_rate(10);

  geometry_msgs::Pose2D vel;
  getVelocidade();

  while (ros::ok())
  {
  ros::spin();

  getVelocidade();

  if(velocidadeX < VELGRANDE || velocidadeY < VELGRANDE){
    vel.x = velocidadeX;
    vel.y = velocidadeY;
  }
  else{
  	vel.x = 0.0;
    vel.y = 0.0;
  }
  std::cout<<"Vel - [ X: "<<vel.x<<" ]    [ Y: "<<vel.y<<" ]  [ Orient.: "<<orientacao<<" ]"<<endl;

  chatter_pub.publish(vel);
  ros::spinOnce();
  loop_rate.sleep();
  }

  return 0;
}
