#include "ros/ros.h"
#include "rosserial_arduino/ultrassom.h"
#include<iostream>
using namespace std;
int cont = 0;
rosserial_arduino::ultrassom velocidade;
rosserial_arduino::ultrassom msg0;

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

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/eletronica/ultrassom/", 1000, chatterCallback);
  ros::spin();

  return 0;
}
