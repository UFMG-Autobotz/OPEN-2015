// ROS
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <stdlib.h>
#include "rs232.h"

// C++
#include <math.h>
#include <stdio.h>
#include <cstdlib>
#include<string.h>

//variaveis
int bdrate = 115200, debug = 1;
int serialPort; //porta serial de comunicação

//Salvar em um Arquivo
#include <iostream>
//#include <unistd.h>
//#include <fstream> 
//#include <string>

using namespace std;

int main(int argc, char **argv){
 
  //char texto[20]; 
  int size = 7;
  unsigned char tx[20];
  if(!argv[1]){
    cout<<"\n\nUse: rosrun nome_do_pacote arducom [port number]\nVide port number in: http://www.teuniz.net/RS-232/\nSerial ACM0 => 24,  Serial ACM1 => 25 ...\n\n";
    return -1;
  }
	 
  //tenta abrir a comunicação serial, serial == 24 ou 25 (ACM0 ou ACM1) "/dev/ttyACM0"
  serialPort = atoi(argv[1]);

  if(RS232_OpenComport(serialPort, bdrate)){
	ROS_INFO("Can not open comport\n");
	return(0);
  }
  else 
	ROS_INFO("Arduino conected!\n");

  if(argv[2]) {
	debug = 1;
  }

  ros::init(argc, argv, "IMUSerial"); 
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Float32>("/eletronica/imu/yaw", 1000);
  ros::Rate loop_rate(10);
  float yaw;

  int tam = RS232_PollComport(serialPort,tx,size);
  char* texto = reinterpret_cast<char*>(tx);
  
  while(tam>0){
     tam = RS232_PollComport(serialPort,tx,size);
     cout<<"Tam: "<<tam<<endl;
  }
  int s;

  while(ros::ok()){

    int n;
    std_msgs::Float32 msg;
    RS232_PollComport(serialPort,tx,size);
    char* texto = reinterpret_cast<char*>(tx);
    s = strlen (texto);

    if(s == 7){
	yaw = atof(texto);
        msg.data = yaw;
        chatter_pub.publish(msg);
	cout<<"Direcao: "<<yaw<<endl;
    }
    else 
	cout<<"Erro! \n"; 
    
    ros::spinOnce();
    loop_rate.sleep();
    usleep(200 * 1000);
  }

  ros::spin();
  return 0;

}
